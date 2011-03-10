<?php
/**
 * @file
 * Provide the HTML output for a jPlayer interface.
 */
?>

<div class="jp-<?php print $type; if($type == 'video') print ' jp-video-360p'; ?>">
  <div class="jp-type-single">
    <div id="<?php print $player_id; ?>" class="jp-jplayer"></div>
    <div id="<?php print $player_id; ?>_interface" class="jp-interface">
      <?php if ($type == 'video'): ?>
      <div class="jp-video-play"></div>
      <?php endif; ?>
      <ul class="jp-controls">
        <li><a href="#" class="jp-play" tabindex="1">play</a></li>
        <li><a href="#" class="jp-pause" tabindex="1">pause</a></li>
        <li><a href="#" class="jp-stop" tabindex="1">stop</a></li>
        <li><a href="#" class="jp-mute" tabindex="1">mute</a></li>
        <li><a href="#" class="jp-unmute" tabindex="1">unmute</a></li>
      </ul>
      
      <div class="jp-progress">
        <div class="jp-seek-bar">
          <div class="jp-play-bar"></div>
        </div>
      </div>
      
      <div class="jp-volume-bar">
        <div class="jp-volume-bar-value"></div>
      </div>
      
      <div class="jp-current-time"></div>
      <div class="jp-duration"></div>
    </div>
    
    <div id="<?php print $player_id; ?>_playlist" class="jp-playlist">
      <ul>
        <?php if ($mode == 'playlist'): ?>
          <?php foreach ($items as $number => $item): ?>
            <li<?php print $item['class'] ? ' class="' . $item['class'] . '"' : '' ?>><a href="<?php print $item['url']; ?>" title="<?php print check_plain($item['label']); ?>"><?php print check_plain($item['label']); ?></a></li>
          <?php endforeach; ?>
        <?php else: ?>
          <li><?php print check_plain($label); ?></li>
        <?php endif; ?>
      </ul>
    </div>
  </div>
</div>