<?php
/**
 * @file
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for a single item.
 */
function template_preprocess_jplayer_single(&$vars) {
  $vars['mode'] = 'single';
  
  $video_extensions = array('m4v', 'mp4', 'ogv', 'webmv');
  $audio_extensions = array('mp3', 'm4a', 'oga', 'webma', 'wav');
  $poster_extensions = array('jpg', 'jpeg', 'png', 'gif');
  
  $poster = array();
  $videos = array();
  $audio = array();
  
  $files = array();
  
  // Look through all the files provided and see what we have
  foreach($vars['items'] as $delta => $item) {
    // Get file URL
    if (!isset($item['url'])) {
      $item['url'] = file_create_url($item['uri']);
    }
    
    // Get file extension
    if (!isset($item['ext'])) {
      $fileinfo = pathinfo($item['url']);
      $item['ext'] = $fileinfo['extension'];
    }
    
    // Get file label
    if (!isset($item['label'])) {
      if (empty($item['description'])) {
        $item['label'] = $item['filename'];
      }
      else {
        $item['label'] = $item['description'];
      }
    }
    
    // Add file into correct group
    if (in_array($item['ext'], $video_extensions)) {
      $videos[] = $item;
    }
    else if (in_array($item['ext'], $audio_extensions)) {
      $audio[] = $item;
    }
    else if (in_array($item['ext'], $poster_extensions)) {
      $poster[] = $item;
    }
  }
  
  // What player are we making? Video is preferred.
  if (empty($videos)) {
    $vars['type'] = 'audio';
    foreach($audio as $file) {
      $files[$file['ext']] = $file['url'];
      $vars['label'] = $file['label'];
    }
  }
  else {
    $vars['type'] = 'video';
    foreach($videos as $file) {
      $files[$file['ext']] = $file['url'];
      $vars['label'] = $file['label'];
    }
  }
  
  // Add poster file if available
  if (isset($poster[0])) {
    $files['poster'] = $poster[0]['url'];
  }
  
  // Add player settings
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $files,
        'solution' => $vars['settings']['solution'],
        'supplied' => implode(',', array_keys($files)),
        'preload' => $vars['settings']['preload'],
        'volume' => $vars['settings']['volume'] / 100,
        'muted' => (boolean)$vars['settings']['muted'],
        'autoplay' => (boolean)$vars['settings']['autoplay'],
        'repeat' => (boolean)$vars['settings']['repeat'],
        'backgroundColor' => $vars['settings']['backgroundColor'],
      ),
    ),
  );
  drupal_add_js($player, 'setting');
}

/**
 * Preprocess function for a playlist.
 */
function template_preprocess_jplayer_playlist(&$vars) {
  $vars['mode'] = 'playlist';
  
  $video_extensions = array('m4v', 'ogv', 'webmv');
  $audio_extensions = array('mp3', 'm4a', 'oga', 'webma', 'wav');
  $poster_extensions = array('jpg', 'jpeg', 'png', 'gif');
  
  $poster = array();
  $videos = array();
  $audio = array();
  
  $files = array();
  $supplied = array();
  
  // Look through all the files provided and see what we have
  foreach($vars['items'] as $delta => $item) {
    // Get file URL
    if (!isset($item['url'])) {
      $item['url'] = file_create_url($item['uri']);
    }
    
    // Get file extension
    if (!isset($item['ext'])) {
      $fileinfo = pathinfo($item['url']);
      $item['ext'] = $fileinfo['extension'];
    }
    
    // Get file label
    if (!isset($item['label'])) {
      if (empty($item['description'])) {
        $item['label'] = $item['filename'];
      }
      else {
        $item['label'] = $item['description'];
      }
    }
    
    // Add file into correct group
    if (in_array($item['ext'], $video_extensions)) {
      $videos[] = $item;
    }
    else if (in_array($item['ext'], $audio_extensions)) {
      $audio[] = $item;
    }
    else if (in_array($item['ext'], $poster_extensions)) {
      $poster[] = $item;
    }
  }
  
  $num = 0;
  
  if (!empty($audio)) {
    $vars['type'] = 'audio';
    foreach($audio as $file) {
      $files[] = array($file['ext'] => $file['url']);
      $supplied[] = $file['ext'];
      $playlist[] = '<a href="#" id="' . $vars['player_id'] . '_item_' . $num . '" tabindex="1">' . $file['label'] . '</a>';
      $num++;
    }
  }
  
  if (!empty($videos)) {
    $vars['type'] = 'video';
    foreach($videos as $file) {
      if (isset($poster[0])) {
        $files[] = array($file['ext'] => $file['url'], 'poster' => $poster[0]['url']);
      }
      else {
        $files[] = array($file['ext'] => $file['url']);
      }
      $supplied[] = $file['ext'];
      $playlist[] = l($file['label'], $file['url'], array('attributes' => array('id' => $vars['player_id'] . '_item_' . $num, 'tabindex' => 1)));
      $num++;
    }
  }
  
  $vars['playlist'] = theme(
    'jplayer_item_list',
    array(
      'items' => $playlist,
    )
  );
  
  // Add player settings
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $files,
        'solution' => $vars['settings']['solution'],
        'supplied' => implode(',', $supplied),
        'preload' => $vars['settings']['preload'],
        'volume' => $vars['settings']['volume'] / 100,
        'muted' => (boolean)$vars['settings']['muted'],
        'autoplay' => (boolean)$vars['settings']['autoplay'],
        'repeat' => $vars['settings']['repeat'],
        'backgroundColor' => $vars['settings']['backgroundColor'],
      ),
    ),
  );
  drupal_add_js($player, 'setting');
}

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
        $data .= theme_item_list(array('items' => $children, 'title' => NULL, 'type' => $type, 'attributes' => $attributes));
      }
      if ($i == 0) {
        $attributes['class'][] = 'first jp-playlist-first';
      }
      if ($i == $num_items - 1) {
        $attributes['class'][] = 'last jp-playlist-last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }
  return $output;
}