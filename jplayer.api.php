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



/**
 * Return information about the plugin skin.
 *
 * The information array is:
 *
 * 'label' - Shortname of this skin. Used in admin forms.
 * 'description' - Long descriptive name of the skin.
 * 'location' - The website-root relative location of files named.
 * 'templates' - array( - template definitions of the skin.
 *   'audio' => array()  - an array of audio player templates
 *   'video' => array()  - an array of video player templates
 * )
 * 'css' - array() - an array of css files to add to the output page.
 * 'theme' - array( - theme functions, as an alternative to using template files.
 *   'audio' => array()  - an array of audio player functions
 *   'video' => array()  - an array of video player functions
 * )
 *
 * For both template and theme keys, the skin can provide functionality
 * in several ways:
 *
 * 1. A single file or a single function:
 *    'templates' => 'template_for_video_and_audio_theming'
 *    'theme' => 'function_for_video_and_audio_theming'
 *
 * 2. A file or a function split by player type:
 *    'templates' => array(
 *      'audio' => 'template_for_audio_theming',
 *      'video' => 'template_for_video_theming',
 *    )
 *    'theme' => array(
 *      'audio' => 'theme_for_audio_theming',
 *      'video' => 'theme_for_video_theming',
 *    )
 *
 * 3. A file or a function split by player mode:
 *    'templates' => array(
 *      'single' => 'template_for_single_track_theming',
 *      'playlist' => 'template_for_playlist_theming',
 *    )
 *    'theme' => ... likewise ...
 *
 * 4. A file or a function split by player type and player mode:
 *    'templates' => array(
 *      'audio' => array(
 *        'single' => 'template_for_single_track_audio_theming'),
 *        'playlist' => 'template_for_playlist_audio_theming'
 *      ),
 *      'video' => array(
 *        'single' => 'template_for_single_track_video_theming'),
 *        'playlist' => 'template_for_playlist_video_theming'
 *      ),
 *    )
 *    'theme' => ... likewise ...
 *
 * As an option:
 *    array('tpl_or_themefn')
 * is also supported, but only the first item in the array is used.
 *
 * NOTE: the template and theme keys are mutually exclusive. You must choose
 * one or the other. Should both exist, templates are given priority.
 *
 * This is a minimalist skin definition, where one template can always cope
 * with the player render, and no additional css files are required (e.g. the
 * templaye includes the css, or it is inherited from the site pages):
 *
 *  $skin = array(
 *    'SKIN' => array(
 *      'label' => t('Green pyramid'),
 *      'location' => $plugin['path'] . '/SKIN',
 *      'templates' => 'mymodule.SKIN.tpl.php',
 *    ),
 *
 * @param array $plugin
 *   The plugin definition array from ctools_get_plugin(). Useful
 *   for the plugin path.
 *
 * @return array
 *   The skin information array in the format described here.
 */
function mymodule_SKIN_info(array $plugin) {
  $skin = array(
    'SKIN' => array(
      'label' => t('Green pyramid'),
      'description' => t('jPlayer triangular skin, green.'),
      'location' => $plugin['path'] . '/SKIN',
      'templates' => array(
        'audio' => array(
          'mymodule.SKIN.audio.playlist.tpl.php',
        ),
        'video' => array(
          'mymodule.SKIN.video.playlist.tpl.php',
        ),
      ),
      // Array of css files to be included.
      'css' => array(
        'css/mymodule.SKIN.min.css',
      ),
      // NOT CURRENTLY SUPPORTED
      'theme' => array(
        'audio' => array(
          'single' => 'mymodule_SKIN_audio_single',
          'playlist' => 'mymodule_SKIN_audio_playlist',
          'stream' => 'mymodule_SKIN_audio_single',
        ),
        'video' => array(
          'single' => 'mymodule_SKIN',
          'playlist' => 'mymodule_SKIN',
        ),
      ),
    )
  );
  return $skin;
}
