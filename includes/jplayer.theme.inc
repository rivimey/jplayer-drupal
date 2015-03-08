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
 */
function template_preprocess_jplayer(&$vars) {
  // Determine a unique player ID.
  $ids = entity_extract_ids($vars['entity_type'], $vars['entity']);
  $vars['player_id'] = _jplayer_check_id('jplayer-' . $vars['entity_type'] . '-' . $ids[0] . '-' . str_replace('_', '-', $vars['field_name']));

  $vars['mode'] = $vars['settings']['mode'];

  // Get the field info so we can figure out what type it is.
  $field_info = field_info_field($vars['field_name']);
  $player = jplayer_sort_files($vars['items'], $vars['player_id'], $vars['mode'], $field_info['type']);
  $vars['playlist'] = theme('jplayer_item_list', array('items' => $player['playlist']));
  $vars['type'] = $player['type'];

  // Add player settings.
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $player['files'],
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

  $vars['dynamic'] = _jplayer_js_attach();

}

/**
 * Modified version of theme_item_list().
 *
 * @param array $variables
 *   Array of variables.
 *
 * @return string
 *   Formatted string.
 */
function theme_jplayer_item_list($variables) {
  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];

  $output = '';
  if (isset($title)) {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $data = '';
    foreach ($items as $i => $item) {
      $attributes = array();
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
            $attributes[$key] = $value;
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
          'attributes' => $attributes,
        ));
      }
      if ($i == 0) {
        $attributes['class'][] = 'first jp-playlist-first';
      }
      if ($i == $num_items - 1) {
        $attributes['class'][] = 'last jp-playlist-last';
      }
      $attributes['oncontextmenu'] = "return false;";
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
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
 * @param string $type
 *   Either 'single' or 'playlist'.
 * @param string $item_type
 *   The field type for the files.
 *
 * @return array
 *   a settings array of data being passed to jPlayer instance
 */
function jplayer_sort_files($raw_files = array(), $player_id = 0, $type = 'single', $item_type = 'file') {
  $video_extensions = array('m4v', 'mp4', 'ogv', 'webmv');
  $audio_extensions = array('mp3', 'm4a', 'oga', 'webma', 'wav');
  $poster_extensions = array('jpg', 'jpeg', 'png', 'gif');

  $poster = NULL;
  $videos = FALSE;
  $audio = FALSE;
  $media = array();

  $files = array();
  $extensions = array();
  $playlist = array();

  // Look through all the files provided and see what we have.
  foreach ($raw_files as $delta => $item) {
    // Get file URL.
    if (!isset($item['url'])) {
      switch ($item_type) {
        case 'file':
          $item['url'] = file_create_url($item['uri']);
          break;

        case 'text':
          $item['url'] = $item['safe_value'];
          break;
      }
    }

    // Get file extension from pathinfo or link_field title-attribute
    if (!isset($item['ext'])) {
      $fileinfo = pathinfo($item['url']);
      // If the path does not have an extension, no extension element will be
      // returned (see http://php.net/manual/en/function.pathinfo.php)
      if (isset($item['attributes']['title']) && !isset($fileinfo['extension'])) {
        $item['ext'] = $item['attributes']['title'];
      }
      elseif (isset($item['attributes']['title']) && !in_array($fileinfo['extension'], $audio_extensions) && !in_array($fileinfo['extension'], $video_extensions) && !in_array($fileinfo['extension'], $poster_extensions)) {
        $item['ext'] = $item['attributes']['title'];
      }
      else {
        $item['ext'] = $fileinfo['extension'];
      }
    }

    // Get file label.
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

    // Add file into correct group.
    if (!isset($item['type'])) {
      $ext = strtolower($item['ext']);
      if (in_array($ext, $video_extensions)) {
        $videos = TRUE;
        $item['type'] = 'video';
      }
      elseif (in_array($ext, $audio_extensions)) {
        $audio = TRUE;
        $item['type'] = 'audio';
      }
      elseif (in_array($ext, $poster_extensions)) {
        $poster = $item['url'];
        $item['type'] = 'poster';
      }
    }

    $media[] = $item;
  }

  $num = 0;
  $player_type = '';
  foreach ($media as $file) {
    $player_type = 'audio';
    if ($videos == TRUE && $type != 'playlist') {
      if ($file['type'] == 'audio') {
        unset($file);
      }
      $player_type = 'video jp-video-360p';
    }
    elseif ($videos == TRUE && $type == 'playlist') {
      $player_type = 'video jp-video-360p';
    }

    $options = array(
      'fragment' => '',
      'attributes' => array(
        'id' => $player_id . '_item_' . $num,
        'tabindex' => 1,
        'onclick' => 'return(false);',
      ),
    );
    if (isset($file) && $file['type'] == 'audio') {
      $files[][$file['ext']] = $file['url'];
      $extensions[] = $file['ext'];
      $playlist[] = l($file['label'], $file['url'], $options);
      $num++;
    }
    elseif (isset($file) && $file['type'] == 'video') {
      $files[][$file['ext']] = $file['url'];
      if ($poster != NULL) {
        $files[]['poster'] = $poster;
      }
      $extensions[] = $file['ext'];
      $playlist[] = l($file['label'], $file['url'], $options);
      $num++;
    }
  }

  if ($type == 'single') {
    $item = array_shift($playlist);
    $playlist = array();
    $playlist[] = $item;
  }

  return array(
    'files' => $files,
    'extensions' => implode(',', $extensions),
    'playlist' => $playlist,
    'type' => $player_type,
  );
}

/**
 * Preprocess function for jplayer.tpl.php when displaying a view as a playlist.
 *
 * @param array $vars
 *   Variable array.
 */
function template_preprocess_jplayer_view_playlist(&$vars) {
  $view = $vars['view'];
  $vars['settings'] = $view->style_plugin->options;
  $vars['mode'] = 'playlist';
  $vars['player_id'] = _jplayer_check_id('jplayer-view-' . str_replace('_', '-', check_plain($view->name)));

  $player = jplayer_sort_files($vars['items'], $vars['player_id'], $vars['mode']);

  $vars['playlist'] = theme('jplayer_item_list', array('items' => $player['playlist']));
  $vars['type'] = $player['type'];

  // Add player settings.
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $player['files'],
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

  $vars['dynamic'] = _jplayer_js_attach();
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
