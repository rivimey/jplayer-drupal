<?php
// $Id$

/**
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for jplayer.tpl.php when doing a single item.
 */
function template_preprocess_jplayer_formatter_jplayer_single(&$vars) {
  jplayer_add();

  $vars['mode'] = 'single';
  $vars['item'] = array(
    'url' => file_create_url($vars['element']['#item']['filepath']),
    'label' => !empty($vars['element']['#item']['data']['description']) ? $vars['element']['#item']['data']['description'] : $vars['element']['#item']['filename'],
  );
  $vars['player_id'] = 'jplayer-' . $vars['element']['#node']->nid . '-' . str_replace('_', '-', $vars['element']['#field_name']) . '-' . $vars['element']['#item']['#delta'];
}

/**
 * Preprocess function for jplayer.tpl.php when using a playlist.
 */
function template_preprocess_jplayer_formatter_jplayer_playlist(&$vars) {
  jplayer_add();

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
