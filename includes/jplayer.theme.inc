<?php

/**
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Themes single file.
 */
function theme_jplayer_formatter_single($element) {
  if ($element['#item']['fid']) {
    jplayer_add();
    return theme('jplayer_single', $element);
  }
  return '';
}

/**
 * Preprocess function for jplayer.tpl.php when doing a single item.
 */
function template_preprocess_jplayer_single(&$vars) {
  $vars['mode'] = 'single';
  $vars['item'] = array(
    'url' => file_create_url($vars['element']['#item']['filepath']),
    'label' => !empty($vars['element']['#item']['data']['description']) ? $vars['element']['#item']['data']['description'] : $vars['element']['#item']['filename'],
  );
  $vars['player_id'] = 'jplayer-' . $vars['element']['#node']->nid . '-' . str_replace('_', '-', $vars['element']['#field_name']) . '-' . $vars['element']['#item']['#delta'];

}

/**
 * Themes multiple items as playlist.
 */
function theme_jplayer_formatter_playlist($element) {
  if ($element[0]['#item']['fid']) {
    jplayer_add();
    return theme('jplayer_playlist', $element);
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
  if (!$vars['element']['#item']['fid'])
    return;
    
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
