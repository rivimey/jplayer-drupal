CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Upgrading
 * Known issues
 * Support
 * Sponsorship


INTRODUCTION
------------

WARNING: Currently very unstable and not intended for use on production sites.

The jPlayer module provides a wrapper around the jPlayer JavaScript library.
This library provides an HTML5-based player, that uses a Flash fallback for
browsers that do not support it. This module provides a default presentation
for the player, as well as integration with Fields. This makes it possible
to easily convert the display of any file field into an audio or video player.

This player will eventually work with MP3, M4A and M4V files, so please be sure
to restrict the file upload extensions on your file fields to only allow these
extensions.


INSTALLATION
------------

 1. Drop the jplayer folder into the modules directory (/sites/all/modules/).

 2. Download jPlayer from http://www.jplayer.org/download/. The downloaded
    directory only contains 2 files, jquery.jplayer.min.js and
    Jplayer.swf. Place these two files in sites/all/libraries/jplayer.

 3. In your Drupal site, enable the module under Administration -> Modules
    (/admin/modules).

**INCOMPLETE**


UPGRADING
---------

Since the jPlayer module is purely a formatter most of the upgrade work required
will be in upgrading from old CCK File Fields into new core File Fields in Drupal
7. Apart from that you may need to re-select your formatters after upgrading to
this new version.


KNOWN ISSUES
------------

 1. Views support doesn't work.

 2. Playlist formatter doesn't work.

 3. Video support doesn't work.

 4. Doesn't support multiple file types in a single player instance e.g. m4a and ogg.


SUPPORT
-------

Many issues you may have may be caused by jPlayer library itself, rather than
the Drupal module. Check with the jPlayer support pages for issues with Flash
warnings or the player behaving oddly:

http://www.happyworm.com/jquery/jplayer/support.htm
http://www.happyworm.com/jquery/jplayer/latest/developer-guide.htm#jPlayer-compatibility

If the problem is with the jPlayer Drupal module, please file a support request
at http://drupal.org/project/issues/jplayer.


SPONSORSHIP
-----------

The D7 branch of this module is currently sponsored by Holy Trinity Brompton
(http://www.htb.org.uk), and Cultivate4 (http://www.cultivatefour.com).

Original D6 module was written by Nate Haug and was maintained by Blazej Owczarczyk