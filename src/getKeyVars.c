// *** getKeyVars.c ***

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

// THIS FUNCTION READS THE CONFIG FILE, AND INITIALIZES THE trackview STRUCT VARIABLES

#include "mpd_pi.h"
extern gint globalVerbose;

// LOAD THE KEYFILE FROM KEYFILE.CONF OR RETURN.
void getKeyVars(app_variables *mpd_piVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) keyFile = g_key_file_new ();
    g_autoptr(GString) cssCfgStr = g_string_new (NULL);

    // DOES CONFIG FILE EXIST?
    //~ gint atLine = __LINE__ + 1;
    //~ if(g_file_test (mpd_piVars->cfgFile, G_FILE_TEST_IS_REGULAR) == 0)
    //~ {
        //~ g_set_error (&error, 0, 0, "Key file\n\n%s\n\nnot found.", mpd_piVars->cfgFile);
        //~ printDebugMsg(error->message, atLine, __FILE__, __func__, mpd_piVars);
        //~ bareMinimumErrMsgDialog (error->message, atLine, __FILE__, __func__);
        //~ exit(0);
    //~ }

    // THESE VARIABLES ARE FOR USE WITH THE GET KEY STRINGS TO SIMPLIFY THINGS
    g_autofree gchar *rtn_centerTrackEnable;

    g_autofree gchar *rtn_playIconFile;
    g_autofree gchar *rtn_pauseIconFile;
    g_autofree gchar *rtn_stopIconFile;
    g_autofree gchar *rtn_nextIconFile;
    g_autofree gchar *rtn_forwardIconFile;
    g_autofree gchar *rtn_previousIconFile;
    g_autofree gchar *rtn_rewindIconFile;
    g_autofree gchar *rtn_titleBarIconFile;

    g_autofree gchar *rtn_windowFont;
    g_autofree gchar *rtn_playviewDialog;
    g_autofree gchar *rtn_dataviewDialog;
    g_autofree gchar *rtn_showView;
    g_autofree gchar *rtn_songview_bg_File;
    g_autofree gchar *rtn_trackview_bg_File;

	gint atKeyLine;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ linenum;
//~ printf("\n\ncfgFile %s -- userDefaultCfgFile %s\n\n",   mpd_piVars->cfgFile, mpd_piVars->userDefaultCfgFile);

	// IF RESOURCE FILE
	if(0 == g_strcmp0(mpd_piVars->cfgFile, CONFIG_RESOURCE))
	{
		GBytes *gbytes = g_resources_lookup_data (CONFIG_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
		atKeyLine =  __LINE__ - 1;
		g_key_file_load_from_bytes (keyFile, gbytes, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;

		g_bytes_unref (gbytes); // FREE THE BYTES ARRAY
	}
	// IF CONFIG FILE
	else
	{
		// IF FILE EXIST THEN OPEN THE DEFAULT CONFIG KEY FILE
		g_key_file_load_from_file(keyFile, mpd_piVars->cfgFile, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;
	}

		if(error)  	goto keyFileError;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ********************************* FILE NAMES ************************************************************//
	// PLAYVIEW DIALOG > IS NOT < A REQUIRED RUNTIME HELPER FILE
    // READ PALYVIEW DIALOG FILE NAME FROM KEY FILE -THIS KEY WILL FAIL IF EMPTY KEY STRING FOUND
    rtn_playviewDialog  = g_key_file_get_string(keyFile, "Helper Files", "PlayviewDialog",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
	if (strlen(rtn_playviewDialog) != 0)
		mpd_piVars->playviewDialogFile = setFileName("playviewDialog", rtn_playviewDialog, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_playviewDialog %s\n" COLOR_OFF, __LINE__, rtn_playviewDialog);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->playviewDialogFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->playviewDialogFile);

    }

// ********************************* FILE NAMES ************************************************************//
	// DATAVIEW DIALOG > IS NOT < A REQUIRED RUNTIME HELPER FILE
    // READ DATAVIEW DIALOG FILE NAME FROM KEY FILE -THIS KEY WILL FAIL IF EMPTY KEY STRING FOUND
    rtn_dataviewDialog  = g_key_file_get_string(keyFile, "Helper Files", "DataviewDialog",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
	if (strlen(rtn_dataviewDialog) != 0)
		mpd_piVars->dataviewDialogFile = setFileName("rtn_dataviewDialog", rtn_dataviewDialog, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_dataviewDialog %s\n" COLOR_OFF, __LINE__, rtn_dataviewDialog);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->dataviewDialogFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->dataviewDialogFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ PLAY ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_playIconFile  = g_key_file_get_string (keyFile, "Icons", "PlayIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_playIconFile) != 0)
        mpd_piVars->playIconFile = setFileName("PlayIconFile", rtn_playIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_playIconFile %s\n" COLOR_OFF, __LINE__, rtn_playIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->playIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->playIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ PAUSE ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_pauseIconFile = g_key_file_get_string (keyFile, "Icons", "PauseIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_pauseIconFile) != 0)
        mpd_piVars->pauseIconFile = setFileName("PauseIconFile", rtn_pauseIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_pauseIconFile %s\n" COLOR_OFF, __LINE__, rtn_pauseIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->pauseIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->pauseIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ STOP ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_stopIconFile  = g_key_file_get_string (keyFile, "Icons", "StopIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_stopIconFile) != 0)
        mpd_piVars->stopIconFile = setFileName("StopIconFile", rtn_stopIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_stopIconFile %s\n" COLOR_OFF, __LINE__, rtn_stopIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->stopIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->stopIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ NEXT ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_nextIconFile   = g_key_file_get_string (keyFile, "Icons", "NextIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_nextIconFile) != 0)
        mpd_piVars->nextIconFile = setFileName("NextIconFile", rtn_nextIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_nextIconFile %s\n" COLOR_OFF, __LINE__, rtn_nextIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->nextIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->nextIconFile);
    }

// ********************************* FILE NAMES ************************************************************//
//~ linenum;	  printf(COLOR_BOLD  COLOR_GREEN "Line %d -- mpd_piVars->forwardIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->forwardIconFile);
    // READ FORWARD ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_forwardIconFile  = g_key_file_get_string (keyFile, "Icons", "ForwardIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
//~ linenum;	  printf(COLOR_BOLD  COLOR_GREEN "Line %d -- mpd_piVars->forwardIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->forwardIconFile);
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_forwardIconFile) != 0)
        mpd_piVars->forwardIconFile = setFileName("ForwardIconFile", rtn_forwardIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_forwardIconFilee %s\n" COLOR_OFF, __LINE__, rtn_forwardIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->forwardIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->forwardIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ PREVIOUS ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_previousIconFile   = g_key_file_get_string (keyFile, "Icons", "PreviousIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_previousIconFile) != 0)
        mpd_piVars->previousIconFile = setFileName("PreviousIconFile", rtn_previousIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_previousIconFile %s\n" COLOR_OFF, __LINE__, rtn_previousIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->previousIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->previousIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ REWIND ICON FILENAME FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_rewindIconFile  = g_key_file_get_string (keyFile, "Icons", "RewindIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;
    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_rewindIconFile) != 0)
        mpd_piVars->rewindIconFile = setFileName("RewindIconFile", rtn_rewindIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_rewindIconFile %s\n" COLOR_OFF, __LINE__, rtn_rewindIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->rewindIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->rewindIconFile);
    }

// ********************************* FILE NAMES ************************************************************//

    // READ THE TITLE BAR ICON FROM THE KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_titleBarIconFile = g_key_file_get_string(keyFile, "Icons", "TitleBarIconFile", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_titleBarIconFile) != 0)
        mpd_piVars->titleBarIconFile = setFileName("TitleBarIconFile", rtn_titleBarIconFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_titleBarIconFile %s\n" COLOR_OFF, __LINE__, rtn_titleBarIconFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->titleBarIconFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->titleBarIconFile);
    }

// ********************************* END OF FILE NAMES ************************************************************//

    // READ TRACK SKIP FWD TIME FROM THE KEY FILE
    mpd_piVars->trackSkipTime = g_key_file_get_integer(keyFile, "Trackview Config", "TrackSkipTime", &error);
    atKeyLine =  __LINE__ - 1;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->trackSkipTime %d\n\n", __LINE__, mpd_piVars->trackSkipTime);
    }
    if(error)  	goto keyFileError;
//**************************************************************************************************
    // READ THE TRACKVIEW HI LITE COLOR FROM THE KEY FILE
    mpd_piVars->trackviewHiLiteColor  = g_key_file_get_string(keyFile, "Trackview Config", "TrackviewHiLiteColor",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->trackviewHiLiteColor %s\n\n", __LINE__, mpd_piVars->trackviewHiLiteColor);
    }

//**************************************************************************************************
    // READ THE PLAYER ICON WIDTH AND HEIGHT FROM THE KEY FILE
    mpd_piVars->playerIconSize     = g_key_file_get_integer(keyFile, "Icons", "PlayerIconSize", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->playerIconSize %d\n\n", __LINE__, mpd_piVars->playerIconSize);
    }

//**************************************************************************************************
    // READ THE WINDOW TITLE FROM THE KEY FILE
    mpd_piVars->WindowTitle = g_key_file_get_string(keyFile, "Main Window Config", "WindowTitle", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->WindowTitle %s\n\n", __LINE__, mpd_piVars->WindowTitle);
    }

//**************************************************************************************************
    // READ THE GUI SCREEN SIZE AT START UP FROM THE KEY FILE
    // START UP GUI IN NORMAL, MAXIMIZED or FULLSCREEN. NOTE: SIZE KEYWORD HAS SPECIAL MEANING
    mpd_piVars->GuiStartUp = g_key_file_get_string(keyFile, "Main Window Config", "GuiStartUp", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->GuiStartUp %s\n\n", __LINE__, mpd_piVars->GuiStartUp);
    }

//**************************************************************************************************
    // WINDOW HEIHGT / WIDTH
    // READ THE WINDOW  HEIGHT FROM THE KEY FILE
    mpd_piVars->windowHeight= g_key_file_get_integer(keyFile, "Main Window Config", "WindowHeight", &error);
    atKeyLine =  __LINE__ - 1;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->windowHeight %d\n\n", __LINE__, mpd_piVars->windowHeight);
    }

    if(error)  	goto keyFileError;
//**************************************************************************************************
    // READ THE WINDOW WIDTH FROM THE KEY FILE
    mpd_piVars->windowWidth = g_key_file_get_integer(keyFile, "Main Window Config", "WindowWidth", &error);
    atKeyLine =  __LINE__ - 1;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- mpd_piVars->windowWidth %d\n\n", __LINE__, mpd_piVars->windowWidth);
    }
    if(error)  	goto keyFileError;
//**************************************************************************************************`
    //READ THE CENTER TRACK PARAMETER FROM THE KEY FILE
    rtn_centerTrackEnable = g_key_file_get_string(keyFile, "Trackview Config", "CenterTrackEnable",   &error);
    atKeyLine =  __LINE__ - 1;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- rtn_centerTrackEnable %s\n", __LINE__, rtn_centerTrackEnable);
    }

    if(g_ascii_strcasecmp(rtn_centerTrackEnable, "TRUE") == 0)
        mpd_piVars->centerTrackEnable =  1;

    else if(g_ascii_strcasecmp(rtn_centerTrackEnable, "FALSE") == 0)
        mpd_piVars->centerTrackEnable =  0;

    else
    {   // IF THE KEY WORD IS WRONG THEN WE GOT AN ERROR
        error = g_error_new (1, 0, "centerTrackEnable keyword -- %s -- is incorrect!\nShould be TRUE or FALSE.", rtn_centerTrackEnable);
        goto keyFileError;
    }

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        printf("Line %d -- mpd_piVars->centerTrackEnable %s\n\n", __LINE__, (mpd_piVars->centerTrackEnable) ? "TRUE" : "FALSE");
    }

    if(error)  	goto keyFileError;

//**************************************************************************************************
    // IF mpdHostname IS NULL THEN THERE WAS NO COMMAND LINE OPTION OTHERWISE READ MPD HOSTNAME FROM KEY FILE
    if(mpd_piVars->mpdHostname == NULL)
    {
        mpd_piVars->mpdHostname   = g_key_file_get_string(keyFile,  "Mpd Host Settings", "MpdHostname", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;

        if(strlen(mpd_piVars->mpdHostname) == 0)
            g_set_error (&error, 0, 0, "\"MpdHostname\" Key word not found.\nString has length of zero");

        if(error)  goto keyFileError;

        if((globalVerbose == 2) || (globalVerbose > 9)) {
            linenum;
            printf("Line %d -- mpd_piVars->mpdHostname %s\n\n", __LINE__, mpd_piVars->mpdHostname);
        }
    }
    //****************************************************************************************
    // READ PORT NUMBER FROM KEY FILE
    if(mpd_piVars->mpdPortnumber < 0)	// IF mpdPortnumber IS NEGATIVE THEN THERE WAS NO COMMAND LINE OPTION
    {
        mpd_piVars->mpdPortnumber = g_key_file_get_integer(keyFile, "Mpd Host Settings", "MpdPortnumber", &error);
        atKeyLine =  __LINE__ - 1;
        if((globalVerbose == 2) || (globalVerbose > 9)) {
            linenum;
            printf("Line %d -- mpd_piVars->mpdPortnumber %d\n\n", __LINE__, mpd_piVars->mpdPortnumber);
        }
        if(error)  	goto keyFileError;
    }
//*****************************************************************************************************
    // READ MPD TIMEOUT FROM KEY FILE
    if(mpd_piVars->mpdTimeout < 0)	// IF mpdTimeout IS NEGATIVE THEN THERE WAS NO COMMAND LINE OPTION
    {
        mpd_piVars->mpdTimeout    = g_key_file_get_integer(keyFile, "Mpd Host Settings", "MpdTimeout", &error);
        atKeyLine =  __LINE__ - 1;
        if((globalVerbose == 2) || (globalVerbose > 9)) {
            linenum;
            printf("Line %d -- mpd_piVars->mpdTimeout %d\n\n", __LINE__, mpd_piVars->mpdTimeout);
        }
        if(error)  	goto keyFileError;
    }

//*****************************************************************************************************
    // WHICH VIEW MODE TO SHOW AT STARTUP? SONGVIEW or TRACKVIEW?
    rtn_showView = g_key_file_get_string(keyFile, "View Mode", "ShowViewMode",   &error);
    atKeyLine =  __LINE__ - 1;
    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- rtn_showView %s\n", __LINE__, rtn_showView);
    }

    if(error)  	goto keyFileError;

    if(g_ascii_strcasecmp (rtn_showView,  "songview") == 0)		mpd_piVars->showViewMode = SHOW_SONGVIEW_MODE;

    else if(g_ascii_strcasecmp (rtn_showView,  "trackview") == 0)		mpd_piVars->showViewMode = SHOW_TRACKVIEW_MODE;

    else // BAD KEYWORD IN KEY FILE
    {
        atKeyLine =  __LINE__;
        error = g_error_new (1, 0, "Config file keyword -- \"%s\" -- returned string -- \"%s\" -- \nShould return SONGVIEW or TRACKVIEW.", "showView", rtn_showView);
        goto keyFileError;
    }

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        printf("Line %d -- mpd_piVars->showViewMode %d\n\n", __LINE__, mpd_piVars->showViewMode  );
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SET THE MAIN WINDOW FONT STRING
    rtn_windowFont = g_key_file_get_string(keyFile, "Set CSS Variables", "WindowFontString",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if(strlen(rtn_windowFont))
    {
        g_string_append_printf(cssCfgStr, "window {font:%s;}\n", rtn_windowFont);
        mpd_piVars->windowFont = g_strdup(rtn_windowFont);
    }
    if((globalVerbose == 2) || (globalVerbose > 9))
    {
        linenum;
        printf("Line %d -- WindowFontString %s\n\n", __LINE__, rtn_windowFont);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SET SONGVIEW BACKGROUND IMAGE (ACTULLY THE CSS_scrollSongview CSS VARIABLE)
    rtn_songview_bg_File = g_key_file_get_string(keyFile, "Set CSS Variables", "songviewBackgroundFile",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;


    if(strlen(rtn_songview_bg_File))
    {
        gchar *backgroundFile = setFileName("songviewBackgroundFile", rtn_songview_bg_File, atKeyLine, __func__, __FILE__, mpd_piVars);

        g_string_append_printf(cssCfgStr, "#CSS_scrollSongview { background-image: url(\"%s\"); }\n",  backgroundFile);

        g_free(backgroundFile);
    }

    if((globalVerbose == 2) || (globalVerbose > 9))
    {
        linenum;
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- CSS VARIABLES FROM THE CONFIG FILE \n%s\n\n" COLOR_OFF, __LINE__, cssCfgStr->str);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SET SONGVIEW BACKGROUND IMAGE (ACTULLY THE CSS_scrollSongview CSS VARIABLE)
    rtn_trackview_bg_File = g_key_file_get_string(keyFile, "Set CSS Variables", "trackviewBackgroundFile",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;


    if(strlen(rtn_trackview_bg_File))
    {
        gchar *backgroundFile = setFileName("trackviewBackgroundFile", rtn_trackview_bg_File, atKeyLine, __func__, __FILE__, mpd_piVars);

        g_string_append_printf(cssCfgStr, "#CSS_boxTrackview { background-image: url(\"%s\"); }\n",  backgroundFile);

        g_free(backgroundFile);
    }

    if((globalVerbose == 2) || (globalVerbose > 9))
    {
        linenum;
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- CSS VARIABLES FROM THE CONFIG FILE \n%s\n\n" COLOR_OFF, __LINE__, cssCfgStr->str);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SET CSS VARIABLES FROM THE CONFIG FILE IN ADDITION TO THE 2 ABOVE
// THESE VARIABLES WILL BE IGNORED IF VARIABLE ZERO IS NOT "START", EMPTY STRING, I.E "ignore"
    gchar *rtn_CSS_KeyString = g_key_file_get_string (keyFile, "Set CSS Variables", "CSS_Variable_0", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    if(g_ascii_strcasecmp(rtn_CSS_KeyString, "START") != 0)
        goto noProcessCSS;

    gchar *keyStr = NULL;
    gint x = 1;
    g_free(rtn_CSS_KeyString);

    while(1)
    {
        keyStr = g_strdup_printf("CSS_Variable_%d", x);
        // GET THE NEXT VARIABLE IN THE SEQUENCE
        rtn_CSS_KeyString = g_key_file_get_string (keyFile, "Set CSS Variables", keyStr, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;

        if(g_ascii_strcasecmp(rtn_CSS_KeyString, "END") == 0)
        {
            g_free(rtn_CSS_KeyString);
            g_free(keyStr);
            goto noProcessCSS;
        }

        if(strlen(rtn_CSS_KeyString) != 0)
            g_string_append_printf(cssCfgStr, "%s\n", rtn_CSS_KeyString);
        g_free(keyStr);
        g_free(rtn_CSS_KeyString);

        x++;
    }

noProcessCSS:
    ;
    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- CSS VARIABLES FROM THE CONFIG FILE \n%s\n\n" COLOR_OFF, __LINE__, cssCfgStr->str);
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************************************************

    GtkCssProvider *configCssProvider = gtk_css_provider_new ();
    GdkDisplay *display = gdk_display_get_default ();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (configCssProvider), \
            GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_data (configCssProvider, cssCfgStr->str, -1, &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    g_object_unref(configCssProvider);

    // *********** END OF GET KEYS ***********

    if (error) {
        g_error_free(error);
    }

    //~ g_key_file_free (keyFile);
//~ g_bytes_unref (gbytes); // FREE THE BYTES ARRAY
    return;


keyFileError:
    ;
linenum;
	/*gint response = */ messageDialog (g_markup_escape_text (error->message, -1), atKeyLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);

		bareMinimumErrMsgDialog (error->message, atKeyLine, __FILE__, __func__)	;

    exit(0);

}  // End of getKeyVars

// RE-ORDER  THE WIDGETS FROM THE CONFIG FILE
void reorderWidgets(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
// SAVE !! SAVE !! SAVE !! SAVE !! SAVE !! 	JUST IN CASE I CHANGE MY MIND AGAIN
    g_autofree gchar *rtn_orienationVertHorz = NULL;
    g_autofree gchar *rtn_controlPosition = NULL;
    g_autofree gchar *rtn_controlSplit = NULL;

    g_autofree gchar *rtn_ShowPlayerControls = NULL;
    g_autofree gchar *rtn_ShowProgressDisplay = NULL;

    gint rtn_widgetOrdering = 0;

    gint atKeyLine;

    g_autoptr(GError) error = NULL;

    // OPEN THE KEY FILE
    g_autoptr(GKeyFile) keyFile = g_key_file_new();

    //~ g_key_file_load_from_file(keyFile, mpd_piVars->cfgFile, G_KEY_FILE_NONE, &error);
    //~ atKeyLine =  __LINE__ - 1;
    //~ if(error)  	goto reorderError;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IF RESOURCE FILE
	if(0 == g_strcmp0(mpd_piVars->cfgFile, CONFIG_RESOURCE))
	{
		GBytes *gbytes = g_resources_lookup_data (CONFIG_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
		atKeyLine =  __LINE__ - 1;
		g_key_file_load_from_bytes (keyFile, gbytes, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;

		g_bytes_unref (gbytes); // FREE THE BYTES ARRAY
	}
	// IF CONFIG FILE
	else
	{
		// IF FILE EXIST THEB OPEN THE DEFAULT CONFIG KEY FILE
		g_key_file_load_from_file(keyFile, mpd_piVars->cfgFile, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;
	}

	if(error)  	goto reorderError;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***********************************************************

    // SHOW OR HIDE PLAYER CONTOLS
    rtn_ShowPlayerControls = g_key_file_get_string(keyFile, "Set Widgets", "ShowPlayerControls", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto reorderError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- rtn_ShowPlayerControls %s\n\n", __LINE__, rtn_ShowPlayerControls);
    }

    if(g_ascii_strcasecmp (rtn_ShowPlayerControls,  "yes") == 0)
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));

    else if(g_ascii_strcasecmp (rtn_ShowPlayerControls,  "no") == 0)
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));

    else
    {   // IF THE KEY WORD IS WRONG THEN WE GOT AN ERROR
        error = g_error_new (1, 0, "ShowPlayerControls keyword -- %s -- is incorrect!\nShould be YES or NO.", rtn_ShowPlayerControls);
        goto reorderError;
    }
//**********************************************************

    // SHOW OR HIDE PROGRESS BARS DISPLAY
    rtn_ShowProgressDisplay = g_key_file_get_string(keyFile, "Set Widgets", "ShowProgressDisplay", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto reorderError;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- rtn_ShowProgressDisplay %s\n\n", __LINE__, rtn_ShowProgressDisplay);
    }

    if(g_ascii_strcasecmp (rtn_ShowProgressDisplay,  "yes") == 0)
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxProgressBars));

    else if(g_ascii_strcasecmp (rtn_ShowProgressDisplay,  "no") == 0)
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxProgressBars));

    else
    {   // IF THE KEY WORD IS WRONG THEN WE GOT AN ERROR
        error = g_error_new (1, 0, "ShowProgressDisplay keyword -- %s -- is incorrect!\nShould be YES or NO.", rtn_ShowProgressDisplay);
        goto reorderError;
    }
//***********************************************************

    rtn_widgetOrdering  = g_key_file_get_integer(keyFile, "Set Widgets", "WidgetReOrdering", &error);
    atKeyLine =  __LINE__ - 1;

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf("Line %d -- rtn_widgetOrdering %d\n\n", __LINE__, rtn_widgetOrdering);
    }

    if((rtn_widgetOrdering > 10) || (rtn_widgetOrdering < 0))
        g_set_error (&error, 0, 0, "Key file variable -- %s  --\nReturned %d. Parameter out of range. Must be an integer between 0 and 10", "widgetReOrdering", rtn_widgetOrdering);

    if(error)  	goto reorderError;

    mpd_piVars->flipMode = rtn_widgetOrdering;

    if(rtn_widgetOrdering)	// IF ZERO THEN USE THE GLADE UI WIDGET DEFAULTS
        setTheWidgets(rtn_widgetOrdering, mpd_piVars);  // OTHERWISE SET THE WIDGETS PER THE CONFIG FILE

//**********************************************************

    // IF EVERYTHIG IS OK, THEN RETURN
    return;

    // OTHERWISE WE GOT AN ERROR
reorderError:
    ;

    messageDialog (g_markup_escape_text (error->message, -1), atKeyLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);
    exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// THIS FUNCTION WILL ALLOW FOR HARD COPY FILENAMES, LINKED FILENAMES IN THE CWD AND FULL PATHNAMES
gchar * setFileName(gchar *keyName, gchar *fileVariable, gint atLine, const gchar *atFunc, gchar *atFile, app_variables *mpd_piVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    gchar *filename = NULL;
    gchar *isLink = g_file_read_link (fileVariable, NULL);

    if(isLink)  // IS IT A LINK TO A FILE?
        filename = g_strdup(isLink);  // IF SO THEN COPY FILENAME INTO VAR

    // IF FILENAME IS ABSOLUTE PATH
    else if(g_path_is_absolute (fileVariable))
        filename = g_strdup(fileVariable);

    else // OTHERWISE ADD CWD TO FILENAME AND SEE WHAT HAPPENS.
        filename = g_strdup_printf("%s/%s", mpd_piVars->cwdPath, fileVariable);

    g_free(isLink);

    // TEST THE FILE. DOES THE FILE EXIST?
    if(g_file_test (filename, G_FILE_TEST_IS_REGULAR) == 0)
    {
        g_autofree gchar *str = NULL;
        str = g_strdup_printf("Key file variable -- %s -- \nFilename -- %s --\ndoes not exist.\n", keyName,  filename);

        messageDialog (str, atLine, atFile, atFunc, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);

        g_free(filename);
        exit(0);
    }

    return filename;
} // End of setFileName

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setTheWidgets(gint rtn_widgetOrdering, app_variables *mpd_piVars)
{
    // T/S = TRACKVIEW / SONGVIEW
    // P = PROGRESS BARS
    // B = PLAYER BUTTONS

    GList *listBoxFlip;  // ARE THE BUTTON AND PROGRESS BOXES ATTACHED TO THE FLIP BOX?
    listBoxFlip = gtk_container_get_children (GTK_CONTAINER (mpd_piVars->w_boxFlipHorzVertBP));

    switch( rtn_widgetOrdering )
    {
    case 1:
    case 2:
    case 3:
    case 4:  // SET THE FLIP BOX TO THE HORIZONTAL MODE
        gtk_orientable_set_orientation (GTK_ORIENTABLE(mpd_piVars->w_boxFlipHorzVertBP),GTK_ORIENTATION_HORIZONTAL);
        break;

    case 5:
    case 6:
    case 7:
    case 8:  // SET THE FLIP BOX TO THE VERTICAL MODE
        gtk_orientable_set_orientation (GTK_ORIENTABLE(mpd_piVars->w_boxFlipHorzVertBP),GTK_ORIENTATION_VERTICAL);
        break;

    case 9:   // POSITION THE SONG / TRACK VIEW IN THE MIDDLE BETWEEN THE BUTTON AND PROGRESS BOXES
    case 10:
        if(listBoxFlip == 0) break;  // IF THE FLIP BOX IS EMPTY, NO NEED TO CONTINUE

        // OTHERWISE MOVE THE BUTTON AND PROGRESS BOXES TO BOX_0
        g_object_ref(mpd_piVars->w_boxProgressBars);  // ADD A REF SO BOXES DOWN VANISH
        g_object_ref(mpd_piVars->w_boxButtons);
        // DETACH BOXES
        gtk_container_remove(GTK_CONTAINER(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars));
        gtk_container_remove(GTK_CONTAINER(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons));
        // ATTACH TO BOX_0
        gtk_container_add_with_properties (GTK_CONTAINER(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxProgressBars), "pack-type", GTK_PACK_START, NULL);

        // ATTACH TO BOX_0
        gtk_container_add_with_properties (GTK_CONTAINER(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxButtons), "pack-type", GTK_PACK_START, NULL);

        // DELETE THE REFERENCES
        g_object_unref(mpd_piVars->w_boxProgressBars); //
        g_object_unref(mpd_piVars->w_boxButtons);
        break;
    }

    switch( rtn_widgetOrdering )
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:

        if(listBoxFlip) break; // IF THE BUTTON AND PROGRESS BOXES ARE NOT ATTACHED TO BOX_0 NO NEED TO CONTIUNE

        // OTHERWISE REMOVE BUTTON AND PROGRESS BOXES FROM BOX_0 AND RE-ATTACH T0 THE FlipHorzVertBP BOX
        g_object_ref(mpd_piVars->w_boxProgressBars);
        g_object_ref(mpd_piVars->w_boxButtons);

        gtk_container_remove(GTK_CONTAINER(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxProgressBars));
        gtk_container_remove(GTK_CONTAINER(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxButtons));

        gtk_container_add_with_properties (GTK_CONTAINER(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), "pack-type", GTK_PACK_START, NULL);

        gtk_container_add_with_properties (GTK_CONTAINER(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), "pack-type", GTK_PACK_START, NULL);

        g_object_unref(mpd_piVars->w_boxProgressBars);
        g_object_unref(mpd_piVars->w_boxButtons);
        break;
    }

    switch (rtn_widgetOrdering) // SET THE WIDGETS IN THE FLIP BOX
    {
    // 1 THRU 8 POSITION THE BUTTON AND PROGRESS BOXES INSIDE THE FLIP BOX
    case 1:  // B - P OVER T/S
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	 0 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 0);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 1);
        break;

    case 2: // P - B OVER T/S
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	 0 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 1);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 0);
        break;

    case 3: // T/S OVER B - P
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	 2 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 0);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 1);
        break;

    case 4: // T/S OVER P - B
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	  2 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 1);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 0);
        break;

    case 5: // B OVER P OVER T/S
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	  0 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 0);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 1);
        break;

    case 6: // P OVER B OVER T/S
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	 0 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 1);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 0);
        break;

    case 7: // T/S OVER B OVER P
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	  2 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 0);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 1);
        break;

    case 8: // T/S OVER P OVER B
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxFlipHorzVertBP),	  2 );
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxButtons), 1);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_boxFlipHorzVertBP), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 0);
        break;

    // 9 - REORDER THE BUTTON AND PROGRESS BOXES IN B0X_0. BUTTON ON TOP, PROGRESS ON BOTTOM
    case 9: // B OVER T/S OVER P
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxButtons), 0);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 3);
        break;

    // 10 - REORDER THE BUTTON AND PROGRESS BOXES IN B0X_0. PROGRESS ON TOP, BUTTON ON BOTTOM
    case 10: // P OVER T/S OVER B
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxButtons), 3);
        gtk_box_reorder_child (GTK_BOX(mpd_piVars->w_box_0), GTK_WIDGET(mpd_piVars->w_boxProgressBars), 0);
        break;
    }

    g_list_free (listBoxFlip); // FREE THE GLIST

    return;
}

//***************************************************************************************************************
// GET THE ERROR DIALOG, CSS AND GLADE FILENAMES FROM THE CONFIG FILE
void getExternalFilenames(app_variables *mpd_piVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) keyFile = g_key_file_new ();

    g_autofree gchar *rtn_msgDialogFile ;
    g_autofree gchar *rtn_cssFile;
    g_autofree gchar *rtn_gladeFile;
    g_autofree gchar *rtn_helpFile;

    gint atKeyLine;
    // DOES THE CONFIG FILE EXIST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ linenum;
//~ printf("\n\ncfgFile %s -- userDefaultCfgFile %s\n\n",   mpd_piVars->cfgFile, mpd_piVars->userDefaultCfgFile);

	// IF RESOURCE FILE
	if(0 == g_strcmp0(mpd_piVars->cfgFile, CONFIG_RESOURCE))
	{
		GBytes *gbytes = g_resources_lookup_data (CONFIG_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
		atKeyLine =  __LINE__ - 1;
		g_key_file_load_from_bytes (keyFile, gbytes, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;

		g_bytes_unref (gbytes); // FREE THE BYTES ARRAY
	}
	// IF CONFIG FILE
	else
	{
		// IF FILE EXIST THEB OPEN THE DEFAULT CONFIG KEY FILE
		g_key_file_load_from_file(keyFile, mpd_piVars->cfgFile, G_KEY_FILE_NONE, &error);
		atKeyLine =  __LINE__ - 1;
	}

		if(error)  	goto keyFileError;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ********************************* FILE NAMES ************************************************************//
	// SIMPLE MSG DIALOG > IS < A REQUIRED RUNTIME HELPER FILE
    // GET SIMPLE MSG DIALOG, FIRST, FILE NAME FROM KEY FILE -THIS KEY WILL FAIL IF EMPTY KEY STRING FOUND
    rtn_msgDialogFile  = g_key_file_get_string(keyFile, "Helper Files", "MsgDialog",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRLEN IS ZERO
    if (strlen(rtn_msgDialogFile) == 0)
    {
        g_set_error (&error, 0, 0, "No filename found in key file variable -- %s --", "MsgDialog");
        goto keyFileError;
    }

//~ linenum;	printf("mpd_piVars->msgDialogFile %s\n", mpd_piVars->msgDialogFile);
	// IF THE msgDialogFile HAS A FILENAME ALREADY (WHICH IT SHOULD) THEN FREE IT
	if(mpd_piVars->msgDialogFile != NULL)
	{
		g_free(mpd_piVars->msgDialogFile);
		mpd_piVars->msgDialogFile = NULL;
	}


    // IF STRING LENGTH IS NOT ZERO
    mpd_piVars->msgDialogFile = setFileName("msgDialog", rtn_msgDialogFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_msgDialogFile %s\n" COLOR_OFF, __LINE__, rtn_msgDialogFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->msgDialogFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->msgDialogFile);
    }

//~ linenum;	printf("mpd_piVars->msgDialogFile %s\n", mpd_piVars->msgDialogFile);
//~ exitatline;


    // READ GLADE FILE NAME FROM KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_gladeFile = g_key_file_get_string(keyFile, "Config Files", "GladeFile",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_gladeFile) != 0)
        mpd_piVars->gladeFile = setFileName("GladeFile", rtn_gladeFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_gladeFile %s\n" COLOR_OFF, __LINE__, rtn_gladeFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->gladeFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->gladeFile);

    }

// ********************************* FILE NAMES ************************************************************//
    // READ CSS FILE NAME FROM KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_cssFile = g_key_file_get_string(keyFile, "Config Files", "CssFile",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_cssFile) != 0)
        mpd_piVars->cssFile = setFileName("CssFile", rtn_cssFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_cssFile %s\n" COLOR_OFF, __LINE__, rtn_cssFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->cssFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->cssFile);
    }


    // READ HELP FILE NAME FROM KEY FILE -THIS KEYWORD WILL NOT FAIL IF IT HAS AN EMPTY KEY NAME STRING
    rtn_helpFile = g_key_file_get_string(keyFile, "Config Files", "ExternalHelpFile",   &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // IF STRING LENGTH IS NOT ZERO
    if (strlen(rtn_helpFile) != 0)
        mpd_piVars->helpFile = setFileName("ExternalHelpFile", rtn_helpFile, atKeyLine, __func__, __FILE__, mpd_piVars);

    if((globalVerbose == 2) || (globalVerbose > 9)) {
        linenum;
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- rtn_helpFile %s\n" COLOR_OFF, __LINE__, rtn_helpFile);
        printf(COLOR_BOLD  COLOR_GREEN "Line %d -- KEY FILE -- mpd_piVars->helpFile %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->helpFile);
    }

		if((globalVerbose == 1) || (globalVerbose > 9)) {
		linenum;
		if(mpd_piVars->helpFile)
		{
			printf(COLOR_BOLD  COLOR_BLUE "Line %d -- EXTERNAL HELP FILE\n", __LINE__);
			printf("Line %d -- HELP FILE %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->helpFile);
		}
		else
		{
			printf(COLOR_BOLD  COLOR_BLUE "Line %d -- INTERNAL RESOURCE HELP FILE\n" , __LINE__);
			printf("Line %d -- HELP FILE %s\n\n" COLOR_OFF, __LINE__, HELP_RESOURCE);
		}
    }

    return;

keyFileError:
    ;
    messageDialog (g_markup_escape_text (error->message, -1), atKeyLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);

    exit(0);
}
