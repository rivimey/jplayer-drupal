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
  'title' => t("jPlayer Pink Flag Skin"),
  'description' => t('The jplayer skin pink.flag.'),
  'skin definition' => 'jplayerskin_pink_flag_info',
);

/**
 *
 */
function jplayerskin_pink_flag_info(array $plugin) {
  $skin = array(
    'pinkflag' => array(
      'label' => t('Pink Flag'),
      'description' => t('Standard jPlayer skin, a pink and purple square arrangement.'),
      'location' => $plugin['path'] . '/pink.flag',
      'templates' => array(
        'audio' => array(
          'jplayer.pink.flag.audio.playlist.tpl.php',
        ),
        'video' => array(
          'jplayer.pink.flag.video.playlist.tpl.php',
        ),
      ),
      // Array of css files to be included.
      'css' => array(
        'css/jplayer.pink.flag.min.css',
      ),
    ),
  );
  return $skin;
}
