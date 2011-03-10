/**
 * @file
 * Drupal behaviors for jPlayer.
 */

(function ($) {
  Drupal.behaviors.jPlayer = {
    attach: function(context, settings) {
      // Set time format settings
      $.jPlayer.timeFormat.showHour = Drupal.settings.jPlayer.showHour;
      $.jPlayer.timeFormat.showMin = Drupal.settings.jPlayer.showMin;
      $.jPlayer.timeFormat.showSec = Drupal.settings.jPlayer.showSec;
      
      $.jPlayer.timeFormat.padHour = Drupal.settings.jPlayer.padHour;
      $.jPlayer.timeFormat.padMin = Drupal.settings.jPlayer.padMin;
      $.jPlayer.timeFormat.padSec = Drupal.settings.jPlayer.padSec;
      
      $.jPlayer.timeFormat.sepHour = Drupal.settings.jPlayer.sepHour;
      $.jPlayer.timeFormat.sepMin = Drupal.settings.jPlayer.sepMin;
      $.jPlayer.timeFormat.sepSec = Drupal.settings.jPlayer.sepSec;
      
      $('.jp-jplayer', context).each(function() {
        var player = this;
        var playerId = $(this).attr('id');
        var playerSettings = Drupal.settings.jplayerInstances[playerId];
        
        // Initialize the player.
        $(player).jPlayer({
          ready: function() {
            $(this).jPlayer("setMedia", playerSettings.files);
            
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