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
  'title' => t("Blue monday"),
  'description' => t('The jplayer skin blue.monday.'),
  'skin definition' => 'jplayerskin_blue_monday_info',
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function jplayerskin_blue_monday_info() {
  return array(
    'bluemonday' => array(
      'label' => t('Blue Monday'),
      'description' => t('Standard jPlayer skin, a blue and grey rectangular layout.'),
      'location' => $path . '/theme/skin/blue.monday',
      // Template html by player mode.
      'templates' => array(
        'audio' => array(
          'jplayer.blue.monday.audio.playlist.tpl.php',
        ),
        'video' => array(
          'jplayer.blue.monday.video.playlist.tpl.php',
        ),
      ),
      // Array of css files to be included.
      'css' => array(
        'css/jplayer.blue.monday.min.css',
      ),
      // this theme copes with all varieties of player in one theme call.
      'theme' => 'jplayer_bluemonday',
    ),
  );
}
