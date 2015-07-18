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
 *
 */
function jplayerskin_blue_flag_info() {
  $skin = array(
    'blueflag' => array(
      'label' => t('Blue Flag'),
      'description' => t('Modified Pink Flag jPlayer skin, a blue and torquise square arrangement.'),
      'location' => $path . '/theme/skin/blue.flag',
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
      // this theme uses three varieties of theme to cover the modes.
      'theme' => array(
        'audio-single' => 'jplayer_blueflag_audio_single',
        'audio-playlist' => 'jplayer_blueflag_audio_playlist',
        'audio-stream' => 'jplayer_blueflag_audio_single',
        'video-single' => 'jplayer_blueflag',
        'video-playlist' => 'jplayer_blueflag',
      ),
    )
  );
  return $skin;
}
