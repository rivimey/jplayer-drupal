/**
 * @file
 * Drupal behaviors for jPlayer.
 */

(function ($) {
  Drupal.behaviors.jPlayer = {
    attach: function(context, settings) {
      $('.jp-jplayer', context).each(function() {
        var player = this;
        var playerId = $(this).attr('id');
        var playerSettings = Drupal.settings.jplayerInstances[playerId];
        
        // Initialize the player.
        $(player).jPlayer({
          ready: function() {
            $(this).jPlayer("setMedia", playerSettings.files);
            
            $.jPlayer.timeFormat.showHour = playerSettings.showHour;
            $.jPlayer.timeFormat.showMin = playerSettings.showMin;
            $.jPlayer.timeFormat.showSec = playerSettings.showSec;
            
            $.jPlayer.timeFormat.padHour = playerSettings.padHour;
            $.jPlayer.timeFormat.padMin = playerSettings.padMin;
            $.jPlayer.timeFormat.padSec = playerSettings.padSec;
            
            $.jPlayer.timeFormat.sepHour = playerSettings.sepHour;
            $.jPlayer.timeFormat.sepMin = playerSettings.sepMin;
            $.jPlayer.timeFormat.sepSec = playerSettings.sepSec;
            
            // Add bindings
            $(player).bind($.jPlayer.event.play, function() {
              $(this).jPlayer("pauseOthers");
            });
            
            // Repeat?
            if (playerSettings.repeat == true) {
              $(player).bind($.jPlayer.event.ended, function() {
                $(this).jPlayer("play");
              });
            }
            
            // Autoplay?
            if (playerSettings.autoplay == true) {
              $(this).jPlayer("play");
            }
          },
          swfPath: Drupal.settings.jPlayer.swfPath,
          cssSelectorAncestor: '#'+playerId+'_interface',
          solution: playerSettings.solution,
          supplied: playerSettings.supplied,
          preload: playerSettings.preload,
          volume: playerSettings.volume,
          muted: playerSettings.muted
        });
      });
    }
  };
})(jQuery);