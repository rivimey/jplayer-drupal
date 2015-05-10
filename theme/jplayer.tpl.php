<?php
/**
 * @file
 * Provide the HTML output for a video jPlayer interface.
 */

$templates = _jplayer_get_skin_property($skin, 'templates');
$location = _jplayer_get_skin_property($skin, 'location');
$css = _jplayer_get_skin_property($skin, 'css');
$cssfile = $css[0];
$css = $location . '/' . $cssfile;
$tplfile = $templates[$type][0];
$tpl = $location . '/' . $tplfile;

if (!empty($cssfile) && file_exists($css)) {
  drupal_add_css($css);
}
if (!empty($tplfile) && file_exists($tpl)) {
  include $tpl;
  print drupal_render($dynamic);
}
