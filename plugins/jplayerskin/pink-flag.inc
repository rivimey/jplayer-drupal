<?php
/**
 * @file
 *
 * Plugin to provide a term context
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Blue moon"),
  'description' => t('The jplayer skin blue.moon.'),
  'context' => '',
  'edit form' => '',
  'defaults' => array(
    'vid' => '',
    'tid' => '',
  ),
  'keyword' => '',
  'context name' => '',
  'convert' => '',
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function jplayerskin_bluemoon_info($empty, $data = NULL, $conf = FALSE) {
}

function jplayerskin_bluemoon_settings_form($form, &$form_state) {
  return $form;
}

/**
 * Validate.
 */
function jplayerskin_bluemoon_settings_form_validate($form, &$form_state) {
}

function jplayerskin_bluemoon_term_settings_form_submit($form, &$form_state) {
}

/**
 * Convert a context into a string.
 */
function jplayerskin_bluemoon__convert($context, $type) {
}
