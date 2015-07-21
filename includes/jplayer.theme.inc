<?php
/**
 * @file
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Implements theme_HOOK().
 *
 * Wrapper theme code for jplayer.
 *
 * @param array $variables
 *   Array of theme variables.
 *
 * @return string
 *   The renderable array for a jplayer instance.
 */
function theme_jplayer_field($variables) {
  $element = $variables['element'];
  $ids = entity_extract_ids($element['#entity_type'], $element['#entity']);

  $player_id = 'jplayer-' .
    drupal_html_class($element['#entity_type']) . '-' .
    drupal_html_class($ids[0]) . '-' .
    drupal_html_class($element['#field_name']);

  $element['#player_id'] = _jplayer_check_id($player_id);

  // Get the field info so we can tell files from text.
  $field_info = field_info_field($element['#field_name']);
  $player = jplayer_sort_files('field',
                               $element['#items'],
                               $element['#settings']['mode'],
                               $field_info['type']);

  if ($player['duplicate']) {
    watchdog('jplayer', 'Duplicate file extension(s) found in media list: @items',
             array('@items' => $element['#items']), WATCHDOG_WARNING);
  }
  $output = array();

  // Add player settings.
  $playerInstance = array(
    'jplayerInstances' => array(
      $element['#player_id'] => array(
        'files' => $player['files'],
        'mode' => $element['#settings']['mode'],
        'skin' => $element['#settings']['skin'],
        'solution' => $element['#settings']['solution'],
        'supplied' => $player['supplied'],
        'preload' => $element['#settings']['preload'],
        'volume' => $element['#settings']['volume'] / 100,
        'muted' => (boolean) $element['#settings']['muted'],
        'autoplay' => (boolean) $element['#settings']['autoplay'],
        'repeat' => $element['#settings']['repeat'],
        'backgroundColor' => $element['#settings']['backgroundColor'],
        'continuous' => $element['#settings']['continuous'],
      ),
    ),
  );

  $output['instance']['#attached']['js'][] = array(
    'data' => $playerInstance,
    'type' => 'setting',
  );

  $commonfiles = _jplayer_js_attach($element['skin']);
  if (!empty($commonfiles)) {
    $output['commonfiles'] = $commonfiles;
  }

  $item_variables = array(
    'items' => $player['playlist'],
    'player_id' => $element['#player_id'],
    'skin' => $element['#settings']['skin'],
    'mode' => $element['#settings']['mode'],
  );
  $item_variables['playlist'] = theme('jplayer_item_list', $item_variables);

  $templ_file = NULL;
  $theme_func = NULL;
  $skin = $element['#settings']['skin'];

  $location = _jplayer_get_skin_property($skin, 'location');
  $templates = _jplayer_get_skin_property($skin, 'templates');
  $themes = _jplayer_get_skin_property($skin, 'theme');
  if (!empty($templates)) {
    $templ_file = _jplayer_select_output($templates, $player['type'], $element['#settings']['mode']);

    // Find and read the html for the skin.
    $templ_path = $location . '/' . $templ_file;
    if (!empty($templ_file) && file_exists($templ_path)) {
      $content = theme_render_template($templ_path, $item_variables);
      $output['skin'] = array(
        '#type' => 'markup',
        '#markup' => $content,
      );
    }
  }
  elseif (!empty($themes)) {
    $theme_func = _jplayer_select_output($themes, $player['type'], $element['#settings']['mode']);

//    TODO Fix this: Use theme function...
//    $content = $theme_func($item_variables);
//    $output['skin'] = array(
//      '#type' => 'markup',
//      '#markup' => $content,
//    );
  }

  $cssfiles = _jplayer_get_skin_property($skin, 'css');
  // Attach the CSS files required by the skin. Normally, just use 1 file!
  $css_opts = array(
    'type' => 'file',
    'group' => CSS_DEFAULT,
  );
  $css_attached = array();
  foreach ($cssfiles as $css_file) {
    $css_path = $location . '/' . $css_file;
    if (!empty($css_file) && file_exists($css_path)) {
      $css_attached[$css_path] = $css_opts;
    }
  }
  if (!empty($css_attached)) {
    $output['skinfiles'] = array(
      '#attached' => array(
        // CSS of skin.
        'css' => $css_attached,
      ),
    );
  }

  return drupal_render($output);
}

/**
 * Implements theme_HOOK().
 *
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
 * @param string $context
 *   How to treat multiple files:
 *     'field' -> multiple files are alternate formats of one item.
 *     'list' -> multiple files are separate items.
 *   This indicates the data source.
 * @param array $raw_files
 *   An array of files to be included.
 * @param string $player_mode
 *   Either 'single' or 'playlist'. This value comes from the field settings.
 * @param string $item_type
 *   The field type for the files.
 *
 * @return array
 *   a settings array of data being passed to jPlayer instance
 */
function jplayer_sort_files($context, $raw_files, $player_mode, $item_type) {

  $seen = array();

  // Look through all the files provided and see what we have.
  $media = array();
  foreach ($raw_files as $item) {

    // Fill in any missing information.
    $item['url'] = _jplayer_get_item_url($item_type, $item);
    $item['label'] = _jplayer_get_field_label($item_type, $item);

    // Categorise the item.
    $item = _jplayer_categorise_items($item, $seen);

    $media[] = $item;
  }

  // Use seen_type to determine audio or video player.
  $player_type = _jplayer_get_player_type($seen);

  $result = _jplayer_make_playlist($media, $player_mode, $player_type);

  $result['supplied'] = implode(', ', array_keys($seen['exts']));

  return $result;
}

/**
 * Construct a playlist from a list of files.
 *
 * A playlist here is a group of arrays describing the files. Each file in the
 * list has an entry for the name of the file, a formatted html link to the
 * file, and the file extension (proxy for file type).
 *
 * @param string $player_mode
 *   Either 'single' or 'playlist', defining whether multiple files are displayed.
 * @param array $media
 *   The files to be included in the playlist, an array of file-description arrays.
 * @param string $player_type
 *   The selected player type: 'audio' or 'video'.
 *
 * @return array
 *   An array of 3 values:
 *   'files': an array, itself keyed by extension, of the url of each file.
 *   'duplicate': true if more than one url mapped to an extension.
 *   'type': the value of $player_type.
 */
function _jplayer_make_playlist($media, $player_mode, $player_type) {

  $duplicate = FALSE;
  $files = array();

  if ($player_mode == 'single') {
    // In single mode, all files are in the first playlist slot.
    foreach ($media as $file) {
      $ext = $file['ext'];
      if (isset($files[0][$ext])) {
        $duplicate = TRUE;
      }
      $files[0][$ext] = $file['url'];
    }
  }
  else {
    // In playlist mode, files are separated into multiple slots.
    $num = 0;
    foreach ($media as $file) {
      $ext = $file['ext'];
      $files[$num][$ext] = $file['url'];
      $num++;
    }
  }

  $result = array(
    'files' => $files,
    'type' => $player_type,
    'duplicate' => $duplicate,
  );

  return $result;
}

/**
 * Return the type of player (audio, video) we will be creating.
 *
 * @param $seen
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
function _jplayer_get_player_type($seen) {
  $player_type = NULL;

  // if there are videos, we need a video player
  if ($seen['type']['video']) {
    $player_type = 'video';
  }
  // no videos, so we just want an audio player
  elseif ($seen['type']['audio']) {
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
 * @param $seen
 *   Array recording the types of item seen over multiple calls. Keys:
 *    - audio: set TRUE if audio files are present in the list
 *    - video: set TRUE if audio files are present in the list
 *    - poster_url: set to the file[url] if an image is found in the list.
 *
 * @return array
 *   The returned item, with 'ext' and 'type' set if needed.
 */
function _jplayer_categorise_items($item, &$seen) {
  $extensions = array(
    'video' => array('m4v', 'mp4', 'ogv', 'ogm', 'webmv'),
    'audio' => array('mp3', 'm4a', 'oga', 'webma', 'wav', 'ogg'),
    'poster' => array('jpg', 'jpeg', 'png', 'gif'),
  );

  // Let other modules adjust the known file types.
  drupal_alter('jplayer_filetypes', $extensions);

  // Get file extension from pathinfo or link_field title-attribute
  if (empty($item['ext'])) {
    // If the path does not have an extension, no extension element will be
    // returned (see http://php.net/manual/en/function.pathinfo.php)
    $fileinfo = pathinfo($item['uri']);

    // If there is no extension but a title attribute, and that is a known
    // extension, then use that.
    if (!empty($item['attributes']['title']) &&
      (in_array($item['attributes']['title'], $extensions['audio']) ||
        in_array($item['attributes']['title'], $extensions['video']) ||
        in_array($item['attributes']['title'], $extensions['poster']))
    ) {
      $item['ext'] = $item['attributes']['title'];
    }
    elseif (!empty($fileinfo['extension'])) {
      $item['ext'] = $fileinfo['extension'];
    }
  }

  // We can't proceed without a file type...
  if (!empty($item['ext'])) {
    // Collect the types seen for use in a 'supplied' object.

    // Assign the file into one of the groups.
    if (!isset($item['type'])) {
      $ext = strtolower($item['ext']);
      if (in_array($ext, $extensions['video'])) {
        $seen['exts'][$ext] = 1;
        $seen['type']['video'] = TRUE;
        $item['type'] = 'video';
      }
      elseif (in_array($ext, $extensions['audio'])) {
        $seen['exts'][$ext] = 1;
        $seen['type']['audio'] = TRUE;
        $item['type'] = 'audio';
      }
      elseif (in_array($ext, $extensions['poster'])) {
        // poster is used iff a video player is used, and we need
        // the file url.
        $seen['exts'][$ext] = 1;
        $seen['type']['poster_url'] = $item['url'];
        $item['type'] = 'poster';
      }
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
  if (!isset($player_ids[$id])) {
    $player_ids[$id] = 1;
  }
  $id = $id . '-' . $player_ids[$id]++;

  return $id;
}

/**
 * Return the correct theme function or template file name, given the
 * array provided by the skin and the player type and mode.
 *
 * For both template and theme keys, the skin can provide functionality
 * in several ways:
 *
 * 1. A single file or a single function:
 *    'templates' => 'template_for_video_and_audio_theming'
 *    'theme' => 'function_for_video_and_audio_theming'
 *
 * 2. A file or a function split by player type:
 *    'templates' => array(
 *      'audio' => 'template_for_audio_theming',
 *      'video' => 'template_for_video_theming',
 *    )
 *    'theme' => array(
 *      'audio' => 'theme_for_audio_theming',
 *      'video' => 'theme_for_video_theming',
 *    )
 *
 * 3. A file or a function split by player type and player mode:
 *    'templates' => array(
 *      'audio' => array(
 *        'single' => 'template_for_single_track_audio_theming'),
 *        'playlist' => 'template_for_playlist_audio_theming'
 *      ),
 *      'video' => array(
 *        'single' => 'template_for_single_track_video_theming'),
 *        'playlist' => 'template_for_playlist_video_theming'
 *      ),
 *    )
 *    'theme' => ... likewise ...
 *
 * @param string $type
 *   The mode of the player -- either 'audio' or 'video'
 * @param string $mode
 *   The type of player -- 'single' or 'playlist' or (audio only) 'stream'
 * @param array $items
 *   Either the 'templates' or the 'themes' array from the skin definition.
 *
 * @return string
 *   The theme or template name.
 */
function _jplayer_select_output($items, $type, $mode) {
  if (is_string($items)) {
    // if it's a function, then it should be a theme function.
    if (is_executable($items)) {
      return $items;
    }
    // otherwise lets assume it's a file.
    return $items;
  }
  elseif (is_array($items)) {
    if (isset($items[$type])) {
      return _jplayer_select_output($items[$type], $type, $mode);
    }
    if (isset($items[$mode])) {
      return _jplayer_select_output($items[$mode], $type, $mode);
    }
    if (isset($items[0])) {
      return $items[0];
    }
  }
  return NULL;
}

