<?php
/**
 * Implements hook_jplayer_add_js().
 *
 * Enables a module to include additional javascript to the output of the
 * jplayer field formatting. This example is adapted from that used by the
 * jplayer_protect module, and shows additional javascript settings and an
 * additional script file bring added.
 *
 * While this function's intended purpose is to attach JS files, the returned
 * array is a drupal renderable array that is available in the template
 * variables for the player.
 *
 * @return array
 *   A recursive associative array, the outermost level of which is keyed by
 *   the module name. Inner levels are drupal renderable arrays.
 *
 */
function hook_jplayer_add_js() {
  if (!variable_get('module_enabled', FALSE)) {
    return array();
  }

  $settings = array(
    'jPlayer' => array(
      'mymodule' => variable_get('jplayer_mymodule', FALSE),
    ),
  );
  drupal_add_js($settings, array('type' => 'setting'));

  $path = drupal_get_path('module', 'mymodule');

  $result = array(
    'jplayer_mymodule' => array(
      '#attached' => array(
        'js' => array(
          $path . '/jplayer-mymodule.js' => array(
            'type' => 'file',
            'scope' => 'footer',
            'group' => JS_DEFAULT,
          ),
        ),
      ),
    ),
  );
  return $result;
}
