<?php

/**
 * @file
 * Administrative pages for the jPlayer module.
 */

/**
 * Menu callback; Provides the jPlayer settings form.
 */
function jplayer_settings_form() {
  $form = array();

  $form['jplayer_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('jPlayer file directory'),
    '#default_value' => variable_get('jplayer_directory', 'sites/all/libraries/jplayer'),
    '#description' => t('Specify the path that contains the jPlayer library. The jplayer.player.min.js file should be in the root of this directory.'),
  );

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('jPlayer options'),
    '#collapsible' => FALSE,
  );

  $form['options']['jplayer_autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-play files on page load'),
    '#description' => t('Use caution when combining this option with multiple players on the same page.'),
    '#default_value' => variable_get('jplayer_autoplay', ''),
  );

  $form = system_settings_form($form);
  $form['#validate'][] = 'jplayer_settings_form_validate';
  $form['#submit'][] = 'jplayer_settings_form_submit';
  return $form;
}

/**
 * Validation function to validate the jplayer_settings_form() form.
 */
function jplayer_settings_form_validate($form, &$form_state) {
  $form_state['jplayer_version'] = jplayer_get_version($form_state['values']['jplayer_directory']);
  if (!$form_state['jplayer_version']) {
    form_error($form['jplayer_directory'], t('The directory specified does not seem to contain the jPlayer library. Check to make sure that the jquery.player.min.js file is located within this directory.'));
  }
}

/**
 * Submit handler for the jplayer_settings_form() form.
 */
function jplayer_settings_form_submit($form, &$form_state) {
  drupal_set_message(t('The jPlayer library (version @version) successfully found in the %directory directory.', array('@version' => $form_state['jplayer_version'], '%directory' => $form_state['values']['jplayer_directory'])));
}
