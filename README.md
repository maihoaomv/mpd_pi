# mpd_pi
Client for music player daemon on a raspberry pi

My main goal for this project is configurability.

GMPC is an MPD client project that, in my opinion, is the best of all the various MPD clients out there. When I built the raspberry music server I incorporated a 7 inch 1366x768 monitor into the design. I wanted to use GMPC as a full screen display, but because of the small size of the monitors text display, the text was nearly unreadable. I wasn’t successful in figuring out how to configure GMPC for a font size that I could read comfortably. 

I decided instead to write my own client that would allow me to easily configure the font size, style, colors and so forth for use on the 7 inch screen.

Mpd_pi is not elegant, or sophisticated, it’s Plain Jane. It allows for an easy to see, easy to use, and hopefully intuitive (mouse/keyboard) interface for a small display that can be configured to your liking. It works really well on my laptop if I want to quickly change the track or playlist.

Mpd_pi is not feature complete, and has programming errors and possible even memory leaks. I am not a professional programmer. I am retired and I do this project as a hobby. I’ve struggled with valgrind trying to get as many of the memory leaks fixed that I could.

Beware it has errors!

Note:	Configuration will require some knowledge of GLADE - user interface designer for GTK+ and GNOME and CSS styling and layout for GTK+.
