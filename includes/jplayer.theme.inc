<?php
/**
 * @file
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for single item.
 */
function template_preprocess_jplayer_single(&$vars) {
  $settings = $vars['display']['settings'];
  $vars['player_id'] = 'jplayer-' . $vars['object']->nid . '-' . str_replace('_', '-', $vars['instance']['field_name']);
  
  $video_extensions = array('m4v', 'mp4', 'ogv', 'webmv');
  $audio_extensions = array('mp3', 'm4a', 'oga', 'webma', 'wav');
  $poster_extensions = array('jpg', 'jpeg', 'png', 'gif');
  
  // @todo fix?
  $vars['mode'] = 'single';
  
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
  
  // Build player js settings
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => $files,
        'solution' => $settings['solution'],
        'supplied' => implode(',', array_keys($files)),
        'preload' => $settings['preload'],
        'volume' => $settings['volume'] / 100,
        'muted' => jplayer_true_false_convert($settings['muted']),
        'autoplay' => jplayer_true_false_convert($settings['autoplay']),
        'repeat' => jplayer_true_false_convert($settings['repeat']),
        'backgroundColor' => $settings['backgroundColor'],
      ),
    ),
  );
  drupal_add_js($player, 'setting');
}

/**
 * Themes multiple items as playlist.
 */
function theme_jplayer_formatter_playlist($variables) {
  $element = $variables['element'];
  if ($element[0]['#item']['fid']) {
    jplayer_add();
    return theme('jplayer_playlist', array('element' => $element));
  }
  return '';
}

/**
 * Preprocess function for jplayer.tpl.php when using a playlist.
 */
function template_preprocess_jplayer_playlist(&$vars) {
  $vars['mode'] = 'playlist';
  $vars['items'] = array();
  $children = element_children($vars['element']);
  $count = count($children);
  $number = 0;
  foreach ($children as $key) {
    $classes = array();
    if ($number == 0) {
      $classes[] = 'first';
      $classes[] = 'jplayer_playlist_item_first';
    }
    if ($number == $count - 1) {
      $classes[] = 'last';
      $classes[] = 'jplayer_playlist_item_last';
    }
    $vars['items'][] = array(
      'class' => implode(' ', $classes),
      'url' => file_create_url($vars['element'][$key]['#item']['filepath']),
      'label' => !empty($vars['element'][$key]['#item']['data']['description']) ? $vars['element'][$key]['#item']['data']['description'] : $vars['element'][$key]['#item']['filename'],
    );
    $number++;
  }
  $vars['player_id'] = 'jplayer-' . $vars['element']['#node']->nid . '-' . str_replace('_', '-', $vars['element']['#field_name']);
}

/**
 * Preprocess function for jplayer.tpl.php when displaying a view as a playlist.
 */
function template_preprocess_jplayer_view_playlist(&$vars) {
  if (!$vars['element']['#item']['fid']) {
    return;
  }

  jplayer_add();

  $count = count($vars['items']);
  foreach ($vars['items'] as $number => $item) {
    $classes = array();
    if ($number == 0) {
      $classes[] = 'first';
      $classes[] = 'jplayer_playlist_item_first';
    }
    if ($number == $count - 1) {
      $classes[] = 'last';
      $classes[] = 'jplayer_playlist_item_last';
    }
    $vars['items'][$number]['class'] = implode(' ', $classes);
  }

  $vars['mode'] = 'playlist';
  $vars['player_id'] = 'jplayer-view-' . str_replace('_', '-', $vars['view']->name);
}
