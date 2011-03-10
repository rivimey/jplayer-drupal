<?php

/**
 * Theme and preprocess functions for the jPlayer module.
 */

/**
 * Preprocess function for jplayer.tpl.php when doing a single item.
 */
function template_preprocess_jplayer_single(&$vars) {
  $settings = $vars['display']['settings'];
  
  $vars['mode'] = 'single';
  $vars['item'] += array(
    'url' => file_create_url($vars['item']['uri']),
    'label' => !empty($vars['item']['description']) ? $vars['item']['description'] : $vars['item']['filename'],
  );
  $vars['player_id'] = 'jplayer-' . $vars['object']->nid . '-' . str_replace('_', '-', $vars['instance']['field_name']) . '-' . $vars['delta'];
  
  // Build player js settings
  $url = pathinfo($vars['item']['uri']);
  $player = array(
    'jplayerInstances' => array(
      $vars['player_id'] => array(
        'files' => array(
          $url['extension'] => $vars['item']['url'],
        ),
        'solution' => $settings['solution'],
        'supplied' => $url['extension'],
        'preload' => $settings['preload'],
        'volume' => $settings['volume'] / 100,
        'muted' => _true_false_convert($settings['muted']),
        'autoplay' => _true_false_convert($settings['autoplay']),
        'repeat' => _true_false_convert($settings['repeat']),
        'backgroundColor' => $settings['backgroundColor'],
        'showHour' => _true_false_convert($settings['showHour']),
        'showMin' => _true_false_convert($settings['showMin']),
        'showSec' => _true_false_convert($settings['showSec']),
        'padHour' => _true_false_convert($settings['padHour']),
        'padMin' => _true_false_convert($settings['padMin']),
        'padSec' => _true_false_convert($settings['padSec']),
        'sepHour' => $settings['sepHour'],
        'sepMin' => $settings['sepMin'],
        'sepSec' => $settings['sepSec'],
      ),
    ),
  );
  
  drupal_add_js($player, 'setting');
}

function _true_false_convert($value) {
  if ($value == 0) {
    return FALSE;
  }
  else {
    return TRUE;
  }
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
