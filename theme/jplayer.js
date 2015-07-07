/**
 * @file
 * Drupal behaviors for jPlayer.
 */

(function ($) {

  Drupal.jPlayer = Drupal.jPlayer || {};

  Drupal.behaviors.jPlayer = {
    attach: function (context, settings) {
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

      // INITIALISE
      // jp-jplayer class must be on a child of the wrapper div with id
      $('.jp-jplayer:not(.jp-jplayer-processed)', context).each(function () {
        var type,
          playerSettings,
          wrapper,
          playerId,
          player,
          jpReadyFn;

        $(this).addClass('jp-jplayer-processed');
        player = this;
        wrapper = player.parentNode;
        playerId = $(wrapper).attr('id');
        playerSettings = Drupal.settings.jplayerInstances[playerId];
        // audio or video
        type = $(wrapper).attr('data-type');
        player.playerType = type;
        // single or playlist
        mode = $(wrapper).attr('data-mode');
        player.playerMode = mode;

        switch (mode) {
          case 'jp-type-single':
            jpReadyFn = function () {
              $(this).jPlayer("setMedia", playerSettings.files[0]);
              console.log("setMedia: " + playerId);

              // Make sure we pause other players on play
              $(player).bind($.jPlayer.event.play, function () {
                $(this).jPlayer("pauseOthers");
              });

              Drupal.attachBehaviors(wrapper);

              // Repeat?
              if (playerSettings.repeat != 'none') {
                $(player).bind($.jPlayer.event.ended, function () {
                  $(this).jPlayer("play");
                });
              }

              // Autoplay?
              if (playerSettings.autoplay == true) {
                $(this).jPlayer("play");
              }
            };

            // Initialise single player
            $(player).jPlayer({
              ready: jpReadyFn,
              swfPath: Drupal.settings.jPlayer.swfPath,
              cssSelectorAncestor: '#' + playerId,
              solution: playerSettings.solution,
              supplied: playerSettings.supplied,
              preload: playerSettings.preload,
              volume: playerSettings.volume,
              muted: playerSettings.muted
            });
            break;

          case 'jp-type-playlist':

            jpReadyFn = function () {
              Drupal.jPlayer.setFiles(wrapper, player, 0, playerSettings.autoplay);

              // Pause other players on play
              $(player).bind($.jPlayer.event.play, function () {
                $(this).jPlayer("pauseOthers");
              });

              // Add playlist selection
              // TODO: make this a $playlist object for easier user below.
              $('#' + playerId).find('a').click(function () {
                var index = $(this).attr('id').split('_')[2];
                Drupal.jPlayer.setFiles(wrapper, player, index, true);
                $(this).blur();
                return false;
              });

              Drupal.attachBehaviors(wrapper);
              if (playerSettings.continuous == 1) {
                $(player).bind($.jPlayer.event.ended, function (e) {
                  // TODO: Combine all ended event in one location.
                  if (!$('li:last', $('#' + playerId)).hasClass('jp-playlist-current')) {
                    Drupal.jPlayer.next(wrapper, player);
                    $(this).jPlayer("play");
                  }
                  else if ($('li:last', $('#' + playerId)).hasClass('jp-playlist-current')) {
                    // We are at the end of the playlist, so move to the first
                    // track but stop playing if repeat is disabled.
                    Drupal.jPlayer.next(wrapper, player);
                    if (playerSettings.repeat == 'none') {
                      $(this).jPlayer("pause");
                    }
                  }
                });
              }

              // Repeat a single track?
              if (playerSettings.repeat == 'single') {
                $(player).bind($.jPlayer.event.ended, function () {
                  $(this).jPlayer("play");
                });
              }
            };

            // Initialise playlist player
            $(player).jPlayer({
              ready: jpReadyFn,
              swfPath: Drupal.settings.jPlayer.swfPath,
              cssSelectorAncestor: '#' + playerId,
              solution: playerSettings.solution,
              supplied: playerSettings.supplied,
              preload: playerSettings.preload,
              volume: playerSettings.volume,
              muted: playerSettings.muted
            });

            // Next
            $(wrapper).find('.jp-next').click(function () {
              $(this).blur();
              Drupal.jPlayer.next(wrapper, player);
              return false;
            });

            // Previous
            $(wrapper).find('.jp-previous').click(function () {
              $(this).blur();
              Drupal.jPlayer.previous(wrapper, player);
              return false;
            });
            break;

          default:
            break;
        }
      });
    }
  };

  Drupal.jPlayer.setFiles = function (wrapper, player, index, play) {
    var playerId = $(wrapper).attr('id');
    var playerSettings = Drupal.settings.jplayerInstances[playerId];

    console.log("setFiles: " + playerId);
    $(wrapper).find('.jp-playlist-current').removeClass('jp-playlist-current');
    var plId = $('#' + playerId + '_item_' + index);
    plId.parent().addClass('jp-playlist-current');
    plId.addClass('jp-playlist-current');

    $(player).jPlayer("setMedia", playerSettings.files[index]);

    if (play == true) {
      $(player).jPlayer('play');
    }
  };

  Drupal.jPlayer.next = function (wrapper, player) {
    var playerId = $(wrapper).attr('id');
    var playerSettings = Drupal.settings.jplayerInstances[playerId];

    var current = Number($(wrapper).find('a.jp-playlist-current').attr('id').split('_')[2]);
    var index = (current + 1 < playerSettings.files.length) ? current + 1 : 0;

    Drupal.jPlayer.setFiles(wrapper, player, index, true);
  };

  Drupal.jPlayer.previous = function (wrapper, player) {
    var playerId = $(wrapper).attr('id');
    var playerSettings = Drupal.settings.jplayerInstances[playerId];

    var current = Number($(wrapper).find('a.jp-playlist-current').attr('id').split('_')[2]);
    var index = (current - 1 >= 0) ? current - 1 : playerSettings.files.length - 1;

    Drupal.jPlayer.setFiles(wrapper, player, index, true);
  };

})(jQuery);

