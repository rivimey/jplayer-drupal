
The jPlayer module provides a wrapper around the jPlayer JavaScript library.
This library provides an HTML5-based player, that uses a Flash fallback for
browsers that do not support it. This module provides a default presentation
for the player, as well as integration with CCK file fields. This makes it
possible to easily convert the display of any file field into an audio player.

This player will only work with MP3 files, so please be sure to restrict the
file upload extensions on your file fields to only allow the mp3 extension.

jPlayer module was written by Nate Haug.

This Module Made by Robots: http://www.lullabot.com

Dependencies
------------

* CCK (Content module)
* FileField

Install
-------

1) Copy the jplayer folder to the modules folder in your installation. Usually
   this is sites/all/modules.

2) Download jPlayer from http://www.happyworm.com/jquery/jplayer/download.htm.
   The downloaded directory only contains 2 files, jplayer.jquery.js and
   Jplayer.swf. Place these two files in sites/all/libraries/jplayer.

3) In your Drupal site, enable the module under Administer -> Site building ->
   Modules (/admin/build/modules).

4) Add or configure a FileField under Administer -> Content management ->
   Content types -> [type] -> Manage Fields
   (admin/content/node-type/[type]/fields). Restrict the upload extension to
   only allow the mp3 extension. It's also a good idea to enable the Description
   option, as this can be used to label your files when they are displayed in
   the playlist.

5) Configure the display of your FileField to use "jPlayer player". If your want
   to have a multi-file field display as a playlist of songs, you may use the
   "jPlayer multi-file playlist" formatter.

6) Create a piece of content with the configured field. Give the file a
   description that will be used as the file label in the playlist.

Support
-------

Many issues you may have may be caused by jPlayer library itself, rather than
the Drupal module. Check with the jPlayer support pages for issues with Flash
warnings or the player behaving oddly:
http://www.happyworm.com/jquery/jplayer/support.htm
http://www.happyworm.com/jquery/jplayer/latest/developer-guide.htm#jPlayer-compatibility

If the problem is with the jPlayer Drupal module, please file a support request
at http://drupal.org/project/issues/jplayer.
