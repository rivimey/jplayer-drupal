<?php
/**
 * @file
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for a player.
 *
 * @param array $vars
 *   Preprocess variables.
 * @throws \Exception
 *   Calls code that calls theme(), which can throw an Exception().
 */
function template_preprocess_jplayer(&$vars) {
  // Determine a unique player ID.
  $ids = entity_extract_ids($vars['entity_type'], $vars['entity']);

  // TODO: should use drupal's drupal_html_class here instead?
  $css_field_name = str_replace('_', '-', $vars['field_name']);
  $player_id = 'jplayer-' . $vars['entity_type'] . '-' . $ids[0] . '-' . $css_field_name;
  $vars['player_id'] = _jplayer_check_id($player_id);

  // Get the field info so we can figure out what type it is.
  $field_info = field_info_field($vars['field_name']);
  $player = jplayer_sort_files($vars['items'], $vars['player_id'], $vars['mode'], $field_info['type']);

  _jplayer_preprocess_template_common($player, $vars);
}

/**
 * Preprocess function for jplayer.tpl.php when displaying a view.
 *
 * @param array $vars
 *   Variable array.
 * @throws \Exception
 *   Calls code that calls theme(), which can throw an Exception().
 */
function template_preprocess_jplayer_view_playlist(&$vars) {
  $view = $vars['view'];
  $vars['settings'] = $view->style_plugin->options;

  // Code originally set playlist like this, which seems odd:
  // $vars['mode'] = 'playlist';

  // TODO: should use drupal's drupal_html_class here instead?
  $player_id = 'jplayer-view-' . str_replace('_', '-', check_plain($view->name));
  $vars['player_id'] = _jplayer_check_id($player_id);

  $player = jplayer_sort_files($vars['items'], $vars['player_id'], $vars['settings']['mode']);

  _jplayer_preprocess_template_common($player, $vars);
}

/**
 * The backend of the jplayer preprocess field hooks.
 *
 * Call theme on the playlist, set up the javascript settings needed by the
 * player, and attach the jplayer javascript and CSS files.
 *
 * @param $player
 *   The player array created by jplayer_sort_files().
 * @param $vars
 *   Preprocess hook theme variables.
 * @throws \Exception
 *   Calls theme(), which can throw an Exception().
 */
function _jplayer_preprocess_template_common($player, &$vars) {
  $vars['mode'] = $vars['settings']['mode'];
  $vars['skin'] = $vars['settings']['skin'];
  $vars['type'] = $player['type'];

  $variables = array(
    'items' => $player['playlist'],
    'skin' => $vars['skin'],
    'mode' => $vars['mode'],
  );
  $vars['playlist'] = theme('jplayer_item_list', $variables);

  // Add player settings.
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $player['files'],
        'mode' => $vars['settings']['mode'],
        'skin' => $vars['settings']['skin'],
        'solution' => $vars['settings']['solution'],
        'supplied' => $player['extensions'],
        'preload' => $vars['settings']['preload'],
        'volume' => $vars['settings']['volume'] / 100,
        'muted' => (boolean) $vars['settings']['muted'],
        'autoplay' => (boolean) $vars['settings']['autoplay'],
        'repeat' => $vars['settings']['repeat'],
        'backgroundColor' => $vars['settings']['backgroundColor'],
        'continuous' => $vars['settings']['continuous'],
      ),
    ),
  );
  drupal_add_js($player, 'setting');

  $vars['dynamic'] = _jplayer_js_attach($vars['skin']);
}

/**
 * Modified version of theme_item_list().
 *
 * Render an item_list without an enclosing div class item-list, with additional
 * classes for jPlayer and with attributes using JS to disable the context menu.
 *
 * Other Differences from D7.36 theme_item_list():
 *
 * If title is set to an 'empty' value, this code can output title <h3></h3>.
 * This code also differs from theme_item_list in its handling of $i, as
 * in the core code $i is an integer variable, while here it is the array key.
 *
 * @rivimey has modified this code to clarify the two 'attributes' arrays in
 * use: outside the loop they are for the list, then within the loop for each
 * item.
 *
 * TODO the empty-title and $i issues seem like bugs in this code.
 *
 * @param array $variables
 *   Array of theme variables.
 *
 * @return string
 *   The items formatted as an HTML list.
 */
function theme_jplayer_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $list_attributes = $variables['attributes'];

  $output = '';
  if (isset($title)) {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($list_attributes) . '>';
    $num_items = count($items);
    $data = '';
    foreach ($items as $i => $item) {
      $item_attributes = array();
      $children = array();
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $item_attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        $data .= theme_item_list(array(
          'items' => $children,
          'title' => NULL,
          'type' => $type,
          'attributes' => $item_attributes,
        ));
      }
      if ($i == 0) {
        $item_attributes['class'][] = 'first jp-playlist-first';
      }
      if ($i == $num_items - 1) {
        $item_attributes['class'][] = 'last jp-playlist-last';
      }
      $item_attributes['oncontextmenu'] = "return false;";
      $output .= '<li' . drupal_attributes($item_attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  return $output;
}

/**
 * Filters an array of files into groups ready for jPlayer.
 *
 * @param array $raw_files
 *   An array of files to be included.
 * @param int $player_id
 *   A unique id for the player.
 * @param string $player_mode
 *   Either 'single' or 'playlist'. In single mode, at most one file is playable.
 * @param string $item_type
 *   The field type for the files.
 *
 * @return array
 *   a settings array of data being passed to jPlayer instance
 */
function jplayer_sort_files($raw_files = array(), $player_id = 0, $player_mode = 'single', $item_type = 'file') {

  $seen_type = array(
    'audio' => FALSE,
    'video' => FALSE,
    'poster_url' => NULL,
  );

  // Look through all the files provided and see what we have.
  $media = array();
  foreach ($raw_files as $delta => $item) {

    // Fill in any missing information.
    $item['url'] = _jplayer_get_item_url($item_type, $item);
    $item['label'] = _jplayer_get_field_label($item_type, $item);

    // Categorise the item.
    $item = _jplayer_group_items_by_type($item, $seen_type);

    $media[] = $item;
  }

  $player_type = _jplayer_get_player_type($seen_type);

  $result = _jplayer_make_playlist($player_id, $media, $player_mode, $player_type, $seen_type['poster_url']);

  // Enforce 'single' players by returning only the first item.
  if ($player_mode == 'single') {
    $item = array_shift($result['playlist']);
    $result['playlist'] = array();
    $result['playlist'][] = $item;
  }

  return $result;
}

/**
 * Construct a playlist from a list of files.
 *
 * A playlist here is a group of arrays describing the files. Each file in the
 * list has an entry for the name of the file, a formatted html link to the
 * file, and the file extension (proxy for file type).
 *
 * @param string $player_id
 *   The player ID, a CSS value to differentiate multiple players on a page.
 * @param string $player_mode
 *   Either 'single' or 'playlist', defining whether multiple files are displayed.
 * @param array $media
 *   The files to be included in the playlist, an array of file-description arrays.
 * @param string $player_type
 *   The selected player type: audio or video.
 * @param string $poster_url
 *   The url of a poster image, or NULL if none seen. Used if a video player is
 *   selected.
 *
 * @return array
 *   An array of 4 values:
 *   'files': an array, itself keyed by extension, of the url of each file.
 *   'extensions': a comma-separated string of all the in-use file extensions.
 *   'playlist': an array, one per file, of l() links for the files.
 *   'type': the value of $player_type.
 */
function _jplayer_make_playlist($player_id, $media, $player_mode, $player_type, $poster_url) {
  $options = array(
    'fragment' => '',
    'attributes' => array(
      'id' => 'player_id_item_0', // placeholder
      'tabindex' => 1,
      'onclick' => 'return(false);',
    ),
  );

  $num = 0;
  $files = array();
  $extensions = array();
  $playlist = array();
  foreach ($media as $file) {
    // Give video files priority for 'single' mode.
    if ($file['type'] == 'audio' && $player_type == 'video' && $player_mode == 'single') {
      continue; // ignore this file...
    }

    // Combine player id and file index to make a unique id.
    $options['attributes']['id'] = $player_id . '_item_' . $num;

    switch ($file['type']) {
      case 'audio':
        $files[][$file['ext']] = $file['url'];
        $extensions[] = $file['ext'];
        $playlist[] = l($file['label'], $file['url'], $options);
        $num++;
        break;

      case 'video':
        $files[][$file['ext']] = $file['url'];
        if ($poster_url['poster_url']) {
          $files[]['poster'] = $poster_url;
        }
        $extensions[] = $file['ext'];
        $playlist[] = l($file['label'], $file['url'], $options);
        $num++;
        break;
    }
  }

  $result = array(
    'files' => $files,
    'extensions' => implode(',', $extensions),
    'playlist' => $playlist,
    'type' => $player_type,
  );

  return $result;
}

/**
 * Return the type of player (audio, video) we will be creating.
 *
 * @param $seen_type
 *   An array recording which file types have been seen in the item list. Keys:
 *    - audio: set TRUE if audio files are present in the list
 *    - video: set TRUE if audio files are present in the list
 *   The key poster_url may be present but is ignored.
 *
 * @return null|string
 *   The string 'video' if any video file seen (even if there are also
 *   audio files), otherwise 'audio' if audio files are present, otherwise
 *   Null if no recognised media files present.
 */
function _jplayer_get_player_type($seen_type) {
  $player_type = NULL;

  // if there are videos, we need a video player
  if ($seen_type['video']) {
    // Original player has:
    //   $player_type = 'video jp-video-360p';
    // TODO: where does 360p come from and why is the type not a single word?

    $player_type = 'video';
  }
  // no videos, so we just want an audio player
  elseif ($seen_type['audio']) {
    $player_type = 'audio';
  }

  return $player_type;
}

/**
 * Return the item's URL.
 *
 * If there is a url in the item, use that, otherwise calculate a url from
 * the available information.
 *
 * @param string $item_type
 *   The type of item, for example file, text, link_field.
 * @param array $item
 *   The item for which the url is wanted.
 *
 * @return string
 *   The URL.
 */
function _jplayer_get_item_url($item_type, $item) {
  if (isset($item['url'])) {
    $item_url = $item['url'];
  }
  else {
    switch ($item_type) {
      case 'file':
        $item_url = file_create_url($item['uri']);
        break;

      case 'text':
        $item_url = $item['safe_value'];
        break;
    }
  }
  return $item_url;
}

/**
 * Identify and classify the type of item into audio, video or poster (image).
 *
 * The item extension and path info is used to identify the nature of the file
 * provided and so help determine the type of player to use. $seen_item is used
 * to record whether items of the three types have been encountered over
 * multiple calls.
 *
 * This call will not override pre-existing 'ext' or 'type' definitions for the
 * item.
 *
 * @param $item
 *   The item being inspected.
 * @param $seen_type
 *   Array recording the types of item seen over multiple calls. Keys:
 *    - audio: set TRUE if audio files are present in the list
 *    - video: set TRUE if audio files are present in the list
 *    - poster_url: set to the file[url] if an image is found in the list.
 *
 * @return array
 *   The returned item, with 'ext' and 'type' set if needed.
 */
function _jplayer_group_items_by_type($item, &$seen_type) {
  $video_extensions = array('m4v', 'mp4', 'ogv', 'webmv');
  $audio_extensions = array('mp3', 'm4a', 'oga', 'webma', 'wav');
  $poster_extensions = array('jpg', 'jpeg', 'png', 'gif');

  // TODO: Include HOOK here to enable the extensions lists to be modified?

  // Get file extension from pathinfo or link_field title-attribute
  if (!isset($item['ext'])) {
    $fileinfo = pathinfo($item['url']);
    // If the path does not have an extension, no extension element will be
    // returned (see http://php.net/manual/en/function.pathinfo.php)
    if (isset($item['attributes']['title']) && !isset($fileinfo['extension'])) {
      $item['ext'] = $item['attributes']['title'];
    }
    elseif (isset($item['attributes']['title']) &&
      !in_array($fileinfo['extension'], $audio_extensions) &&
      !in_array($fileinfo['extension'], $video_extensions) &&
      !in_array($fileinfo['extension'], $poster_extensions)) {

      $item['ext'] = $item['attributes']['title'];
    }
    else {
      $item['ext'] = $fileinfo['extension'];
    }
  }

  // Assign the file into one of the groups.
  if (!isset($item['type'])) {
    $ext = strtolower($item['ext']);
    if (in_array($ext, $video_extensions)) {
      $seen_type['video'] = TRUE;
      $item['type'] = 'video';
    }
    elseif (in_array($ext, $audio_extensions)) {
      $seen_type['audio'] = TRUE;
      $item['type'] = 'audio';
    }
    elseif (in_array($ext, $poster_extensions)) {
      // poster is used iff a video player is used, and we need
      // the file url.
      $seen_type['poster_url'] = $item['url'];
      $item['type'] = 'poster';
    }
  }
  return $item;
}

/**
 * Get a label for the item.
 *
 * - use the item label if set;
 * - otherwise use the description if set;
 * - otherwise use {filename, URL basename or title} depending on field type.
 *
 * @param string $item_type
 *   The type of item, for example file, text, link_field.
 * @param array $item
 *   The item for which the label is wanted.
 *
 * @return string
 *   the chosen label.
 */
function _jplayer_get_field_label($item_type, $item) {
  if (!isset($item['label'])) {
    if (empty($item['description'])) {
      switch ($item_type) {
        case 'file':
          $item['label'] = $item['filename'];
          break;

        case 'text':
          $fileinfo = pathinfo($item['url']);
          $item['label'] = urldecode($fileinfo['basename']);
          break;

        case 'link_field':
          $item['label'] = $item['title'];
          break;
      }
    }
    else {
      $item['label'] = $item['description'];
    }
  }
  return $item['label'];
}

/**
 * Return a unique ID for a jPlayer.
 *
 * This allows multiple embedded jPlayers to point to an identical file on the
 * same page.
 *
 * @param string $id
 *   The ID to check for uniqueness.
 *
 * @return string
 *   A modified ID if the ID was not unique, or the same ID passed in if it was
 *   unique.
 */
function _jplayer_check_id($id) {
  // We keep track of player IDs generated per request. This ensures that if a
  // player pointing to the same field is shown multiple times on a page, that
  // each player gets a unique ID. This is especially a problem when jPlayers
  // are embedded in hidden content such as View rendered with Quicktabs.
  static $player_ids = array();

  // Add the request time, so if the same player is inserted multiple times
  // AJAX all players are functional.
  $id = $id . '-' . $_SERVER['REQUEST_TIME'];

  // Store a count of the number of times a unique ID is used, and make it
  // unique if needed.
  if (isset($player_ids[$id])) {
    $id = $id . '-' . $player_ids[$id]++;
  }
  else {
    $player_ids[$id] = 0;
  }

  return $id;
}
