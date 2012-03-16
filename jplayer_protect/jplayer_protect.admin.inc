<?php

/**
 * Page callback for the jPlayer protection statistics page. This page is
 * useful in determining if a browser is misbehaving and blocking legitimiate
 * file accesses, or if a user is trying to download a protected file.
 */
function jplayer_protection_statistics() {
  if (!variable_get('jplayer_protected', FALSE)) {
    drupal_set_message(t('<a href="@jplayer-settings">jPlayer content protection</a> is not currently enabled.', array('@jplayer-settings' => url('admin/settings/jplayer', array('query' => drupal_get_destination())))));
  }

  $output = '<p>' . t('This table shows the 50 top users who have been denied access to direct downloads of jPlayer files.') . '</p>';

  // TODO: convert to DBTNG.
  $result = db_query("SELECT COUNT(1) as total, uid as user, MAX(timestamp) as timestamp FROM {jplayer_denied} GROUP BY uid ORDER BY total DESC, timestamp DESC LIMIT 50;");
  $rows = array();
  foreach ($result as $denied) {
    $denied = (array)$denied;
    // Format data from the query.
    $uid = $denied['user'];
    $denied['user'] = theme('username', array('account' => user_load($denied['user'])));
    $denied['timestamp'] = format_date($denied['timestamp']);

    // Find the top-denied file for the user.
    // TODO: convert to DBTNG.
    $top_file = db_query("SELECT COUNT(fid) as fid_count, fid FROM {jplayer_denied} WHERE uid = :uid GROUP BY fid ORDER BY fid_count DESC LIMIT 1", array(':uid' => $uid))->fetchObject();
    $top_file = file_load($top_file->fid);
    $denied['file'] = str_replace($GLOBALS['base_url'], '', file_create_url($top_file->uri));

    // Find the top hostname for the user.
    // TODO: convert to DBTNG.
    $top_hostname = db_query("SELECT COUNT(hostname) as hostname_count, hostname FROM {jplayer_denied} WHERE uid = :uid GROUP BY hostname ORDER BY hostname_count DESC LIMIT 1", array(':uid' => $uid))->fetchObject();
    $denied['hostname'] = $top_hostname->hostname;

    $rows[] = $denied;
  }

  $header = array(
    t('Accesses denied'),
    t('User'),
    t('Last denied'),
    t('Top Denied File'),
    t('Top Hostname'),
  );

  if (!empty($rows)) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= '<p>' . t('There have been no files that have been denied access within the last week.') . '</p>';
  }
  return $output;
}

