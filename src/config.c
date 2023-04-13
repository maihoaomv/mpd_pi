// *** config.c ***

/*
*  MIT License
*
*  Copyright (c) 2023 maihoaomv
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*/

#include "mpd_pi.h"
extern gint globalVerbose;
// SET CONFIG FILE CONFIGURATION -MOSTLY SETS WIDGETS AND VARIABLES FROM KEY FILE (.conf) DATA
void setConfigVariables(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if((globalVerbose == 3) || (globalVerbose > 9)) {
        linenum;
        printf( "Line %d -- SET CONFIG FILE CONFIGURATION\n", __LINE__);
    }


	g_object_set (mpd_piVars->w_window_main, "default-width", mpd_piVars->windowWidth, NULL);
	g_object_set (mpd_piVars->w_window_main, "default-height", mpd_piVars->windowHeight, NULL);

    if(g_ascii_strcasecmp(mpd_piVars->GuiStartUp, "FULLSCREEN") == 0)  // IF FULLSCREEN IS SET
        gtk_window_fullscreen(GTK_WINDOW(mpd_piVars->w_window_main));

    if(g_ascii_strcasecmp(mpd_piVars->GuiStartUp, "MAXIMIZED") == 0)  // IF MAXIMIZED IS SET
        gtk_window_maximize(GTK_WINDOW(mpd_piVars->w_window_main));

    g_free(mpd_piVars->GuiStartUp);

    // OTHERWISE SCREEN SIZE WILL DEFAULT TO NORMAL
    // End of SET STARTUP SCREEN SIZE

    //***** SHOW OR HIDE THE PLAYER CONTROLS
    if(mpd_piVars->ShowPlayerControls)
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
    else     // if FALSE
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));

    //***** SHOW OR HIDE THE STATUS DISPLAY
    // NOTE: WILL TOGGLE WITH ALTERNATE STATUS DISPLAY
    if(mpd_piVars->ShowProgressDisplay)
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
    else  // if FALSE
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ***** SET WINDOW TITLE
    // IF NO WINDOW TITLE IN THE CONFIG FILE THEN US PROGRAM NAME AND HOSTNAME, OTHERWISE TITLE AND HOSTNAME
    g_autofree gchar *titleBarString = NULL;
	g_autofree gchar *addProgName = NULL;
	g_autofree gchar *addHostName = NULL;

		if(g_str_has_prefix (mpd_piVars->WindowTitle, "+"))
		{
			mpd_piVars->WindowTitle[0] = ' ';
			addProgName = g_strdup_printf("< %s >", g_get_prgname());
		}

		if(g_str_has_suffix (mpd_piVars->WindowTitle, "+"))
		{
			mpd_piVars->WindowTitle[strlen(mpd_piVars->WindowTitle) -1] = ' ';
			addHostName = g_strdup_printf("< %s >", mpd_piVars->mpdHostname);
		}

		g_strstrip( mpd_piVars->WindowTitle );

		titleBarString = g_strdup_printf("%s  %s  %s", (addProgName) ? addProgName : ""  ,  mpd_piVars->WindowTitle ,  (addHostName) ? addHostName : ""  );

    gtk_window_set_title (GTK_WINDOW (mpd_piVars->w_window_main), titleBarString);

    g_free(mpd_piVars->WindowTitle);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ****** ICONS and PIXBUF
    //https://www.codeproject.com/Questions/1220779/Gtk-gtk-CRITICAL-gtk-image-set-from-pixbuf-asserti
    GdkPixbuf *pixbuf = NULL;

    //************************************* SET ICONS **************************************//
    // THE WINDOW TITLEBAR ICON
	// THIS ALLOWS MPD_PI TO PICK 1 OF 3 POSSIBLE ICONS FOR THE TITLEBAR ICON

	// IF A ICON FILE IS SPECIFIED IN THE mpd_pi.conf FILE, THEN THIS IS THE DEFAULT ICON TO USE IRREGARDLESS OF RESOURCE OR STOCK ICONS
	if (mpd_piVars->titleBarIconFile != NULL)
		pixbuf = gdk_pixbuf_new_from_file (mpd_piVars->titleBarIconFile, NULL);
	// IF ICON FILE NOT SPECIFIED, THEN SET  THE RESOURCE ICON IF IT EXIST
	else
	{
		if(g_resources_get_info (TITLEBAR_ICON_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL, NULL, NULL) == TRUE)
			pixbuf = gdk_pixbuf_new_from_resource (TITLEBAR_ICON_RESOURCE, NULL);

	// IF RESOURCE ICON DOES NOT EXIST, THEN SET THE STOCK ICON, IF IT EXIST
		else
		{
			pixbuf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (),/* gnome-mime-audio*/ /*audio-x-generic*/  "audio-x-generic"  , 48 , GTK_ICON_LOOKUP_USE_BUILTIN, NULL);
		}
	}
		gtk_window_set_icon (GTK_WINDOW(mpd_piVars->w_window_main), pixbuf);
		g_free(mpd_piVars->titleBarIconFile);
		g_clear_object(&pixbuf);

    //************************************* SET BUTTON ICONS **************************************//
    // PLAY PLAYER BUTTON ICON
	setButtonImage("gtk-media-play", PLAY_ICON_RESOURCE, mpd_piVars->playIconFile, mpd_piVars->w_btnPlay, mpd_piVars);

    // PAUSE PLAYER BUTTON ICON
	//~ setButtonImage("||", PAUSE_ICON_RESOURCE, mpd_piVars->pauseIconFile, mpd_piVars->w_btnPause, mpd_piVars);
	setButtonImage("gtk-media-pause", PAUSE_ICON_RESOURCE, mpd_piVars->pauseIconFile, mpd_piVars->w_btnPause, mpd_piVars);

    // STOP PLAYER BUTTON ICON
	setButtonImage("gtk-media-stop", STOP_ICON_RESOURCE, mpd_piVars->stopIconFile, mpd_piVars->w_btnStop, mpd_piVars);

    // REWIND PLAYER BUTTON ICON
	setButtonImage("gtk-media-rewind", REWIND_ICON_RESOURCE, mpd_piVars->rewindIconFile, mpd_piVars->w_btnRewind, mpd_piVars);

    // NEXT PLAYER BUTTON ICON
	setButtonImage("gtk-media-next", NEXT_ICON_RESOURCE, mpd_piVars->nextIconFile, mpd_piVars->w_btnNext, mpd_piVars);

    // FORWARD PLAYER BUTTON ICON
	setButtonImage("gtk-media-forward", FORWARD_ICON_RESOURCE, mpd_piVars->forwardIconFile, mpd_piVars->w_btnForward, mpd_piVars);

    // PREVIOUS PLAYER BUTTON ICON
	setButtonImage("gtk-media-previous", PREVIOUS_ICON_RESOURCE, mpd_piVars->previousIconFile, mpd_piVars->w_btnPrevious, mpd_piVars);

    // End of SET THE ICONS

    // INITIALIZE OTHER NON-WIDGET TRACKVIEW VARIABLES
    mpd_piVars->volumeStatus = 0;
    mpd_piVars->elaspedTimeStatus = 0;
    mpd_piVars->totalTimeStatus = 0;
    mpd_piVars->queueLengthStatus = 0;
    mpd_piVars->mpdPlayerStateStatus = MPD_STATE_UNKNOWN;

    // SONG POSITION IS ZERO BASED (SONG 1 IS 0), NEED TO ADD +1 THE GET CORRECT POSITION
    mpd_piVars->currentSongPos = 0;
    mpd_piVars->prevSongPos = 0;

    // SET THE MODE
    if(mpd_piVars->showViewMode == SHOW_TRACKVIEW_MODE)
    {
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxSongview)); // HIDE THE SONGVIEW
		gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_TrackviewMode");
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxTrackview)); // SHOW THE TRACKVIEW
    }

    if(mpd_piVars->showViewMode == SHOW_SONGVIEW_MODE)
    {
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxTrackview)); // SHOW THE TRACKVIEW
		gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_SongviewMode");
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxSongview)); // HIDE THE SONGVIEW
    }
	// TURN OFF SEARCH (CTRL+F) FOR TREEVIEW
	gtk_tree_view_set_search_column(mpd_piVars->w_trackviewTreeView, -1);

}

// LOAD THE EXTERNAL GLADE FILE AND PROCESS WIDGET VARIABLES
void initilazeGladeBuilder(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if((globalVerbose == 3) || (globalVerbose > 9)) {
        linenum;
        printf( "Line %d -- LOAD THE GLADE FILE\n", __LINE__);
    }
    GError *error = NULL;

    // ****** INITALIZE THE GLADE VARIABLES ******
    GtkBuilder      *builder;

    if(mpd_piVars->gladeFile == NULL)
    {
        mpd_piVars->gladeFile = g_strdup(GLADE_RESOURCE);
        builder = gtk_builder_new_from_resource (mpd_piVars->gladeFile);
        if((globalVerbose == 1) || (globalVerbose > 9)) {
            linenum;
            printf(COLOR_BOLD  COLOR_BLUE "Line %d -- INTERNAL RESOURCE GLADE FILE\n" COLOR_OFF, __LINE__);
        }
    }
    else
    {
        builder = gtk_builder_new_from_file(mpd_piVars->gladeFile);
        if((globalVerbose == 1) || (globalVerbose > 9)) {
            linenum;
            printf(COLOR_BOLD  COLOR_BLUE "Line %d -- EXTERNAL GLADE FILE\n" COLOR_OFF, __LINE__);
        }
    }
    if((globalVerbose == 1) || (globalVerbose > 9)) {
		//~ linenum;
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- GLADE FILE %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->gladeFile);
    }

    if(error) {
        errMsgMacro (error->message);
        g_error_free(error);
        exit(0); // HAS TO BE EXIT HERE. gtk_main_quit CAUSES SEG FAULT
    }

    //******* INITILAZE THE WIDGETS *********
    if((globalVerbose == 3) || (globalVerbose > 9)) {
        linenum;
        printf( "Line %d -- INITILAZE THE WIDGETS\n", __LINE__);
    }
    // WINDOWS
    mpd_piVars->w_window_main = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
    //~ mpd_piVars->w_helpLblScrollWin    = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "helpLblScrollWin"));

    // BOXES
    mpd_piVars->w_box_0 = GTK_BOX(gtk_builder_get_object(builder, "box_0"));
    mpd_piVars->w_boxButtons = GTK_BOX(gtk_builder_get_object(builder, "boxButtons"));
    mpd_piVars->w_boxFlipHorzVertBP = GTK_BOX(gtk_builder_get_object(builder, "boxFlipHorzVertBP"));

    mpd_piVars->w_boxProgressBars = GTK_BOX (gtk_builder_get_object (builder, "boxProgressBars"));
    mpd_piVars->w_boxTrackview = GTK_BOX(gtk_builder_get_object(builder, "boxTrackview"));
    mpd_piVars->w_btnBoxPlayerControls = GTK_BOX(gtk_builder_get_object(builder, "btnBoxPlayerControls"));

    mpd_piVars->w_boxSongview = GTK_BOX(gtk_builder_get_object(builder, "boxSongview"));

    // EVENT BOXES
    mpd_piVars->w_eventBoxProgressBarVolume = GTK_EVENT_BOX(gtk_builder_get_object(builder, "eventBoxProgressBarVolume"));
    mpd_piVars->w_eventBoxProgressBarTime = GTK_EVENT_BOX(gtk_builder_get_object(builder, "eventBoxProgressBarTime"));
    mpd_piVars->w_eventBoxProgressBarQueue = GTK_EVENT_BOX(gtk_builder_get_object(builder, "eventBoxProgressBarQueue"));

    // BUTTONS
    mpd_piVars->w_btnNext = GTK_BUTTON(gtk_builder_get_object(builder, "btnNext"));
    mpd_piVars->w_btnForward = GTK_BUTTON(gtk_builder_get_object(builder, "btnForward"));
    mpd_piVars->w_btnPause = GTK_BUTTON(gtk_builder_get_object(builder, "btnPause"));
    mpd_piVars->w_btnPlay = GTK_BUTTON(gtk_builder_get_object(builder, "btnPlay"));
    mpd_piVars->w_btnPrevious = GTK_BUTTON(gtk_builder_get_object(builder, "btnPrevious"));
    mpd_piVars->w_btnStop = GTK_BUTTON(gtk_builder_get_object(builder, "btnStop"));
    mpd_piVars->w_btnRewind = GTK_BUTTON(gtk_builder_get_object(builder, "btnRewind"));

    // TREEVIEW
    mpd_piVars->w_trackviewListStore = GTK_LIST_STORE (gtk_builder_get_object (builder, "trackviewListStore"));
    mpd_piVars->w_trackViewModelSort = GTK_TREE_MODEL_SORT (gtk_builder_get_object (builder, "trackViewModelSort"));
    mpd_piVars->w_trackviewTreeView = GTK_TREE_VIEW (gtk_builder_get_object (builder, "trackviewTreeView"));

    // PROGRESS BARS
    mpd_piVars->w_progressBarVolume = GTK_PROGRESS_BAR (gtk_builder_get_object (builder, "progressBarVolume"));
    mpd_piVars->w_progressBarTime = GTK_PROGRESS_BAR (gtk_builder_get_object (builder, "progressBarTime"));
    mpd_piVars->w_progressBarQueue = GTK_PROGRESS_BAR (gtk_builder_get_object (builder, "progressBarQueue"));

    // SONGVIEW
    mpd_piVars->w_lblSongName = GTK_LABEL(gtk_builder_get_object(builder, "lblSongName"));
    mpd_piVars->w_lblSongTitle = GTK_LABEL(gtk_builder_get_object(builder, "lblSongTitle"));
    mpd_piVars->w_lblSongArtist = GTK_LABEL(gtk_builder_get_object(builder, "lblSongArtist"));
    mpd_piVars->w_lblSongAlbumArtist = GTK_LABEL(gtk_builder_get_object(builder, "lblSongAlbumArtist"));
    mpd_piVars->w_lblSongAlbum = GTK_LABEL(gtk_builder_get_object(builder, "lblSongAlbum"));
    mpd_piVars->w_lblSongDiskTrack = GTK_LABEL(gtk_builder_get_object(builder, "lblSongDiskTrack"));
    mpd_piVars->w_lblSongURI = GTK_LABEL(gtk_builder_get_object(builder, "lblSongURI"));

    mpd_piVars->w_box_for_SongviewLabel = GTK_BOX(gtk_builder_get_object(builder, "box_for_SongviewLabel"));

    // MAKE SURE BOX_0 IS IN VERTICAL MODE -THIS WILL NOT WORK IF BOX 0 IS IN HORIZONTAL MODE
    g_object_set (mpd_piVars->w_box_0, "orientation", GTK_ORIENTATION_VERTICAL,  NULL);

    // ENSURE BOX_0 WIDGETS ARE IN THE CORRECT ORDER IN RELATION TO GTK_PACK_START
    gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxSongview), 0);
    gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxTrackview), 1);
    gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP), 2); // BUTTONS AND PROGRESS BARS

    //******* End of INITILAZE THE WIDGETS *********

    // CONNECT SIGNALS
    gtk_builder_connect_signals(builder, mpd_piVars);
    //~ g_clear_object(&builder);
    g_object_unref(builder);

    //******* End of INITILAZE THE WIDGETS *********
} // End of initilazeGladeBuilder

// THIS FUNCTION LOADS THE CSS VARIABLES CONTAINED IN
// THE INTERNAL CSS RESOURCE OR EXTERNAL CSS FILE
// THIS FUNCTION SHOULD BE LOADED PRIOR TO LOADING THE
// CCS VARIABLES (loadConfigVariablesToProvider) CONTAINED IN THE CONFIG FILE.
// THESE ARE INTENDED TO BE THE BASIC DEFAULT SETTINGS FOR THE WIDGETS THAT
// NORMALLY WILL NOT CHANGE.

void loadCssProvider(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if((globalVerbose == 3) || (globalVerbose > 9)) {
        linenum;
        printf( "Line %d -- LOAD THE CSS FILE\n", __LINE__);
    }
    g_autoptr(GError) error = NULL;

    GtkCssProvider *provider = gtk_css_provider_new ();
    GdkDisplay *display = gdk_display_get_default ();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), \
            GTK_STYLE_PROVIDER_PRIORITY_USER);
    gint atKeyLine;
    // LOAD THE CSS PROVIDER FILE FROM RESOURCE
//~ linenum;	printf("mpd_piVars->cssFile %s\n", mpd_piVars->cssFile); exitatline;
    if(mpd_piVars->cssFile == NULL)
    {
        mpd_piVars->cssFile = g_strdup(CSS_RESOURCE);
        gtk_css_provider_load_from_resource (provider, mpd_piVars->cssFile);
        if((globalVerbose == 1) || (globalVerbose > 9))  {
            linenum;
            printf(COLOR_BOLD  COLOR_BLUE "Line %d -- INTERNAL RESOURCE CSS FILE\n" COLOR_OFF, __LINE__);
        }
    }
    // OTHERWISE LOAD THE CSS FROM FILE
    else
    {
        gtk_css_provider_load_from_path(provider, mpd_piVars->cssFile, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto cssFileError;

        if((globalVerbose == 1) || (globalVerbose > 9)) 	{
            linenum;
            printf(COLOR_BOLD  COLOR_BLUE "Line %d -- EXTERNAL CSS FILE\n" COLOR_OFF, __LINE__);
        }
    }
    if((globalVerbose == 1) || (globalVerbose > 9)) {
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- CSS FILE %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->cssFile);
    }

    if(error)
    {
        errMsgMacro(error->message);
        g_error_free(error);
        exit(0);
    }
    // g_clear_object (&provider);
    g_object_unref (provider);

    return;

cssFileError:
    ;

    messageDialog (g_markup_escape_text (error->message, -1), atKeyLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);


    exit(0);
}

// THIS FUNCTION ALLOWS MPD_PI TO PICK 1 OF 3 POSSIBLE ICONS FOR THE PLAYER CONTROL BUTTIONS
void setButtonImage(gchar *imageName, gchar *resourcePath, gchar *iconFile, GtkButton *buttonName, app_variables *mpd_piVars)
{  //	linenum;

    GdkPixbuf *pixbuf = NULL;
	GtkWidget  *img;
// IF A ICON FILE IS SPECIFIED IN THE mpd_pi.conf FILE, THEN THIS IS THE DEFAULT ICON TO USE IRREGARDLESS OF RESOURCE OR STOCK ICONS
	if (iconFile != NULL)
	{
		pixbuf = gdk_pixbuf_new_from_file_at_scale (iconFile, mpd_piVars->playerIconSize, mpd_piVars->playerIconSize, TRUE, NULL);
		gtk_button_set_image(GTK_BUTTON(buttonName), GTK_WIDGET(gtk_image_new_from_pixbuf (pixbuf)));
		g_free(iconFile);
		g_clear_object(&pixbuf);
		return;
	}

// IF ICON FILE NOT SPECIFIED, THEN SET  THE RESOURCE ICON IF IT EXIST
	if(g_resources_get_info (resourcePath, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL, NULL, NULL) == TRUE)
		img = gtk_image_new_from_resource (resourcePath);
// IF RESOURCE ICON DOES NOT EXIST, THEN SET THE STOCK ICON, IF IT EXIST
	else 		img = gtk_image_new_from_icon_name(imageName, GTK_ICON_SIZE_INVALID );

		gtk_image_set_pixel_size (GTK_IMAGE (img), mpd_piVars->playerIconSize);
		gtk_button_set_image( (GtkButton*)buttonName, img );

}
