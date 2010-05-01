<?php
// $Id$

/**
 * Provide the HTML output of the jPlayer audio player.
 */
?>

<div id="<?php print $player_id; ?>" class="jplayer"<?php print $mode == 'single' ? ' rel="' . $item['url'] . '"' : ''; ?>></div>

<div class="jp-<?php print $mode; ?>-player">
  <div class="jp-interface">
    <ul class="jp-controls">
      <li><a href="#" id="<?php print $player_id; ?>-play" class="jp-play"><?php print t('play'); ?></a></li>
      <li><a href="#" id="<?php print $player_id; ?>-pause" class="jp-pause"><?php print t('pause'); ?></a></li>
      <li><a href="#" id="<?php print $player_id; ?>-stop"class="jp-stop"><?php print t('stop'); ?></a></li>

      <li><a href="#" id="<?php print $player_id; ?>-volume-min" class="jp-volume-min"><?php print t('min volume'); ?></a></li>
      <li><a href="#" id="<?php print $player_id; ?>-volume-max" class="jp-volume-max"><?php print t('max volume'); ?></a></li>
      <?php if ($mode == 'playlist'): ?>

      <li><a href="#" id="<?php print $player_id; ?>-volume-previous" class="jp-previous"><?php print t('previous'); ?></a></li>
      <li><a href="#" id="<?php print $player_id; ?>-volume-next" class="jp-next"><?php print t('next'); ?></a></li>
      <?php endif; ?>

    </ul>
    <div id="<?php print $player_id; ?>-progress" class="jp-progress">
      <div id="<?php print $player_id; ?>-load-bar" class="jp-load-bar">
        <div id="<?php print $player_id; ?>-play-bar" class="jp-play-bar"></div>
      </div>
    </div>

    <div id="<?php print $player_id; ?>-volume-bar" class="jp-volume-bar">
      <div id="<?php print $player_id; ?>-volume-bar-value" class="jp-volume-bar-value"></div>
    </div>
    <div id="<?php print $player_id; ?>-play-time" class="jp-play-time"></div>
    <div id="<?php print $player_id; ?>-total-time" class="jp-total-time"></div>
  </div>

  <div id="<?php print $player_id; ?>-playlist" class="jp-playlist">
    <ul>
      <?php if ($mode == 'playlist'): ?>
        <?php foreach ($items as $number => $item): ?>
          <li<?php print $item['class'] ? ' class="' . $item['class'] . '"' : '' ?>><a href="<?php print $item['url']; ?>"><?php print check_plain($item['label']); ?></a></li>
        <?php endforeach; ?>
      <?php else: ?>
        <li><?php print check_plain($item['label']); ?></li>
      <?php endif; ?>
    </ul>
  </div>
</div>
