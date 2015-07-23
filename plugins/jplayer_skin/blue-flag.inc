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
  'title' => t("jPlayer Blue Flag Skin"),
  'description' => t('The jplayer skin blue.flag.'),
  'skin definition' => 'jplayerskin_blue_flag_info',
);

/**
 * Return information about the blue flag skin.
 *
 * @param array $plugin
 *   The ctools plugin array for this plugin - useful for the path entry.
 * @return array
 *   An array containing a key-value machine name - info mapping.
 *
 * @see jplayer.api.php for details of the array returned here.
 */
function jplayerskin_blue_flag_info(array $plugin) {
  $skin = array(
    'blueflag' => array(
      'label' => t('Blue Flag'),
      'description' => t('Modified Pink Flag jPlayer skin, a blue and torquise square arrangement.'),
      'location' => $plugin['path'] . '/blue.flag',
      'templates' => array(
        'audio' => array(
          'jplayer.blue.flag.audio.playlist.tpl.php',
        ),
        'video' => array(
          'jplayer.blue.flag.video.playlist.tpl.php',
        ),
      ),
      // Array of css files to be included.
      'css' => array(
        'css/jplayer.blue.flag.min.css',
      ),
    )
  );
  return $skin;
}
