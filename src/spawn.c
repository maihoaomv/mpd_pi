// spawn.c

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
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ASYNCHRONOUS SPAWNS THE PLAYVIEW DIALOG
void spawnPlayviewDialog(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
	g_autoptr(GError) error = NULL;
	gint atLine = 0;

	if(mpd_piVars->playviewDialogFile == NULL)
	{
		gchar *tmpStr = g_strdup("Playview dialog helper file not specified in conf file");
		messageDialog (tmpStr, __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);
		g_free(tmpStr);
		return;
	}

    g_autofree gchar *playviewMsg = NULL;


// SET THE ARGUGMENTS TO BE PASSED TO THE PLAYVIEW DIALOG
    GString *spawnStr = g_string_new (NULL);

    g_string_append_printf(spawnStr, "%s ", mpd_piVars->playviewDialogFile);
    g_string_append_printf(spawnStr, "--host=%s ", mpd_piVars->mpdHostname);
    g_string_append_printf(spawnStr, "--port=%d ", mpd_piVars->mpdPortnumber);
    g_string_append_printf(spawnStr, "--timeout=%d ", mpd_piVars->mpdTimeout);
    g_string_append_printf(spawnStr, "--width=%d ", mpd_piVars->playviewWidth);
    g_string_append_printf(spawnStr, "--height=%d ", mpd_piVars->playviewHeight);
    g_string_append_printf(spawnStr, "--title=\"%s\" ", "Select MPD Playlist");
    // SAVE TO SENT FONT TO DIALOG
    g_string_append_printf(spawnStr, " --font=\"%s \" ", mpd_piVars->windowFont);
// playview  [ --host ]  [ --port ]  [ --timeout ]  [ --width ]  [ --height ]  [ --font ]  [ --title ]
//~ printf("Line %d -- %s\n", __LINE__, spawnStr->str);
    g_spawn_command_line_async (spawnStr->str,  &error);
	atLine = __LINE__ - 1 ;
	if(error)
	{
		messageDialog (error->message, atLine, __FILE__, __func__, NOTIFY_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);
		exit(0);
	}
    g_string_free(spawnStr, TRUE);
} // End of spawnPlayviewDialog

// ASYNCHRONOUS SPAWNS THE DATAVIEW DIALOG
void spawnDataviewDialog(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
	g_autoptr(GError) error = NULL;
	gint atLine = 0;

	if(mpd_piVars->dataviewDialogFile == NULL)
	{
		gchar *tmpStr = g_strdup("Dataview dialog helper file not specified in conf file");
		messageDialog (tmpStr, __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);
		g_free(tmpStr);
		return;
	}

    g_autofree gchar *dataviewMsg = NULL;

    /*
     DATAVIEW COMMAND OPTIONS
     Usage: 	dataview  [ --host ]  [ --port ]  [ --timeout ]  [ --width ]  [ --height ]  [ --font ]  [ --title ]
     Example: 	dataview --host="localhost" --port=6600 --timeout=30000 --width=1200 --height=800 --font="Bold 20px Sans" --title="Display MPD Playlist"
    */

// SET THE ARGUGMENTS TO BE PASSED TO THE DATAVIEW DIALOG
    GString *spawnStr = g_string_new (NULL);

    g_string_append_printf(spawnStr, "%s ", mpd_piVars->dataviewDialogFile);
    g_string_append_printf(spawnStr, "--host=%s ", mpd_piVars->mpdHostname);
    g_string_append_printf(spawnStr, "--port=%d ", mpd_piVars->mpdPortnumber);
    g_string_append_printf(spawnStr, "--timeout=%d ", mpd_piVars->mpdTimeout);
    g_string_append_printf(spawnStr, "--width=%d ", mpd_piVars->playviewWidth);
    g_string_append_printf(spawnStr, "--height=%d ", mpd_piVars->playviewHeight);
    g_string_append_printf(spawnStr, "--title=\"%s\" ", "Select MPD Database");
    // SAVE TO SENT FONT TO DIALOG
    g_string_append_printf(spawnStr, " --font=\"%s \" ", mpd_piVars->windowFont);

    g_spawn_command_line_async (spawnStr->str,  &error);
	atLine = __LINE__ - 1 ;
	if(error)
	{
		messageDialog (error->message, atLine, __FILE__, __func__, NOTIFY_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);
		exit(0);
	}
    g_string_free(spawnStr, TRUE);
} // End of spawnDataviewDialog

// THIS FUNCTION SPAWNS THE simpleMsgDialog HELPER TO DISPLAY ERRORS, NOTIFY AND QUESTION MESSAGES TO USER
int messageDialog(gchar *msg, gint line, gchar *file, const gchar *function, gint mode, gint timeout, gboolean sync, app_variables *mpd_piVars)
{   //	linenum;   printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if(mpd_piVars->windowFont == NULL) mpd_piVars->windowFont = strdup("");

    if(g_access(mpd_piVars->msgDialogFile, F_OK) == -1)
    {
        printDebugMsg(msg, line, __FILE__, __func__, mpd_piVars);
        bareMinimumErrMsgDialog (msg, line, file, function);
        exit(0);;
    }

    g_autoptr(GError) error = NULL;
    int exit_status;
    gint exitCode = 1234;
    g_autofree gchar *errMsg = NULL;
    g_autofree gchar *textStr = NULL;
    g_autofree gchar *dialogTitle = NULL;
    g_autofree gchar *dialogMode = NULL;
    gint atLine;
	gchar *DebugFile = NULL;
    switch(mode)
    {
    case 	ERROR_MSG_MODE:
        DebugFile = g_strdup_printf("%s/%s.debug",g_get_tmp_dir (), g_get_prgname ());

        errMsg = g_strdup_printf("%s\n\nLine -- %d --\nFile -- %s --\nFunc -- %s --\n\nDebug File -- %s --\nExecutable File -- %s --\nConfig File -- %s --", msg, line, file, function, DebugFile, g_get_prgname (), mpd_piVars->cfgFile);

        g_free(DebugFile);

        dialogMode = g_strdup("--error");
        if(sync)  // SYNC
            dialogTitle = g_strdup("Spawn Simple Error Msg Dialog - SYNC");
        if(!sync)  // ASYNC
            dialogTitle = g_strdup("Spawn Simple Error Msg Dialog - ASYNC");

        // IF THIS IS AN ERROR MSG DIALOG, THEN SEND MSG TO THE DEBUG FILE ALSO.
        // SEND ZERO OR A NEGATIVE LINE NUMBER TO SUPRESS THE
        // LINE, FILE AND FUNCTION THAT WOULD NORMALY BE PRINTED
        printDebugMsg(errMsg, 0, __FILE__, __func__, mpd_piVars);
        //~ printDebugMsg(errMsg, line, __FILE__, __func__, mpd_piVars);

        break;

    case 	NOTIFY_MSG_MODE:
        errMsg = g_strdup(msg);
        dialogMode = g_strdup("--notify");
        if(sync)
            dialogTitle = g_strdup("Spawn Simple Notify Msg Dialog - SYNC");
        if(!sync)  // ASYNC
            dialogTitle = g_strdup("Spawn Simple Notify Msg Dialog - ASYNC");
        break;

    case 	INFORMATION_MSG_MODE:
        errMsg = g_strdup(msg);
        dialogMode = g_strdup("--info");
        if(sync)
            dialogTitle = g_strdup("Spawn Simple Information Msg Dialog - SYNC");
        if(!sync)  // ASYNC
            dialogTitle = g_strdup("Spawn Simple Information Msg Dialog - ASYNC");
        break;

    case 	QUESTION_MSG_MODE:
        errMsg = g_strdup(msg);
        dialogMode = g_strdup("--question");
        if(sync)
            dialogTitle = g_strdup("Spawn Simple Question Msg Dialog - SYNC");
        if(!sync)  // ASYNC
            dialogTitle = g_strdup("Spawn Simple Question Msg Dialog - ASYNC");
        break;

    // SPECIAL CASE ASYNCHRONOUS ERROR DIALOG FOR MPD CONNECTION THAT RETURNS A CHILD PID
    case	CHILD_ERR_MSG_MODE:
        dialogTitle = g_strdup("Child Error Message - ASYNC");
        goto childSpecialCaseErrMsg;
        break;
    }

// SET THE ARGUGMENTS TO BE PASSED TO THE SIMPLE MSG DIALOG
    GString *spawnStr = g_string_new (NULL);

    g_string_append_printf(spawnStr, "%s ", mpd_piVars->msgDialogFile);
    g_string_append_printf(spawnStr, " %s ", dialogMode);
    g_string_append_printf(spawnStr, " --timeout=%d ", timeout);
    g_string_append_printf(spawnStr, " --title=\"%s\"  ", dialogTitle);
    g_string_append_printf(spawnStr, " --msg=\"%s \" ", errMsg);

    g_string_append_printf(spawnStr, " --font=\"%s \" ", mpd_piVars->windowFont	);

    //****************************************** SIMPLE MSG DIALOG  ******************************************
    // OPEN DIALOG AS ASYNCHRONOUS
    if(!sync)  // ASYNC
    {
        g_spawn_command_line_async (spawnStr->str,  &error);
        atLine = __LINE__  - 1;
        g_string_free(spawnStr, TRUE);
        return 1234;  //  ASYNC - DON'T CARE ABOUT RETURN CODES.
    }

    // OPEN DIALOG AS SYNCHRONOUS
    if(sync)   // SYNC
    {
        g_spawn_command_line_sync (spawnStr->str, NULL, NULL, &exit_status, &error);
        atLine = __LINE__  - 1;
        g_string_free(spawnStr, TRUE);
    }
    if(error)	goto weGotAnErr;

    // OTHERWISE GET DIALOG RETURN CODE
	// FOR LINUXMINT COMPILE THIS FUNCTION
    g_spawn_check_wait_status (exit_status, &error);
	atLine = __LINE__  - 1;

	if(error->code > 128)	 exitCode = error->code - 256;
	else	 exitCode = error->code;

	// FOR RASPBERRY PI COMPILE THESE TO LINES AND COMMENT OUT 2 LINES AB0VE
	//~ g_spawn_check_exit_status (exit_status, &error);
	//~ atLine = __LINE__  - 1;
	if(error)
	{
		if(error->code == SIMPLE_MSG_DIALOG_CSS_FAILED)
		{
			bareMinimumErrMsgDialog ("SIMPLE MSG DIALOG CSS has Failed", atLine, __FILE__, __func__)	;
		}
		if(error->code == SIMPLE_MSG_DIALOG_GLADE_FAILED) {
			bareMinimumErrMsgDialog ("SIMPLE MSG DIALOG GLADE has Failed", atLine, __FILE__, __func__)	;
		}
	}


    /*
    	* SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!  SAVE !!
    	* https:*www.geeksforgeeks.org/exit-codes-in-c-c-with-examples/
    	* NOTE: (exit_status / 256) - 256 -- THIS IS THE CALCULATION FOR EXIT CODES LESS THAN ZERO
    	* NOTE: (exit_status % 256) -- THIS IS THE CALCULATION FOR EXIT CODES EQUAL TO AND GREATER THAN ZERO
    	*
    	* THESE ARE THE EXIT CODES RETURNED BY THE DIALOG
    	* GTK_RESPONSE_DELETE_EVENT = -4
    	* GTK_RESPONSE_OK = -5
    	* GTK_RESPONSE_CLOSE = -7
    	* GTK_RESPONSE_YES = -8
    	* GTK_RESPONSE_NO = -9
    	* GTK_RESPONSE_NONE = 0
    	* BUT WILL NEED TO CONVERT THE SPAWN RETRUNED EXIT STATUS TO THE DIALOG RESPONSE CODE
    */
//~ linenum;
    return  exitCode;

    //*************************************** SPECIAL CASE CHILD MODE ***************************************
    /*
    	*  THIS  ASYNCHRONOUS ERROR MESSAGE SPAWN IS A SPECIAL CASE THAT DOES NOT BLOCK songview
    	*  SENDING THE PARENT'S PID (--pid) TO THE DIALOG ALLOWS TO KILL THIS APP FROM THE DIALOG
    	*
    	*  THIS FUNCTION ALLOWS TO ABORT THIS APP FROM THE ERR MSG DIALOG IF "EXIT" IS CLICKED
    	*  OR JUST CLOSES THE ERR MSG DIALOG IF "OK" IS CLICKED.
    	*  IT ALSO ALLOWS FOR AN AUTO-TIMEOUT TO AUTOMATICIALLY CLOSE THE ERR MSG DIALOG (--timeout option) AFTER X NUMBER OF SECONDS
    	* NOTE: --timeout 0   MEANS NO TIMEOUT
    */

childSpecialCaseErrMsg:
    ;

    GPid child_pid;

    gchar **args;
    args = g_new0 (char *, 7);
    args[0] = g_strdup_printf("%s", mpd_piVars->msgDialogFile);//simple dialog filename
    args[1] = g_strdup_printf ( "--msg=%s ",  msg); // the message to the dialog
    args[2] = g_strdup_printf ( " --pid=%d ", getpid()); // parent PID
    args[3] = g_strdup_printf("-T %s", dialogTitle); // error dialog
    args[4] = g_strdup ("--error"); // error dialog
    //~ args[5] = g_strdup_printf ("--font=%s", boldStr	); // error dialog
    args[5] = g_strdup_printf ("--font=%s", mpd_piVars->windowFont	); // error dialog
    args[6] = NULL;

//~ printf("ARGV 0 = %s\n", args[0]);
//~ printf("ARGV 1 = %s\n", args[1]);
//~ printf("ARGV 2 = %s\n", args[2]);
//~ printf("ARGV 3 = %s\n", args[3]);
//~ printf("ARGV 4 = %s\n", args[4]);
//~ printf("ARGV 5 = %s\n", args[5]);
//~ printf("ARGV 6 = %s\n", args[6]);
//~ printf("ARGV 7 = %s\n", args[7]);

    g_spawn_async (NULL, args, NULL,	G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL,	&child_pid, &error);
    atLine = __LINE__  - 1;
    g_strfreev (args);
    if (error) goto weGotAnErr;
//~ printf("Line %d -- child_pid %d\n", __LINE__, child_pid);
    return child_pid;

    // IF THE SPAWN FAILS, WE CAN'T USE simpleMsgDialog TO POP UP AN ERROR REPORT -USE BARE MINIUMN MSG DIALOG INSTEAD
weGotAnErr:
    ;
    g_fprintf(stderr, "Spawning child failed: %s at line %d\n", error->message, atLine);
    printDebugMsg(error->message, atLine, __FILE__, __func__, mpd_piVars);
    bareMinimumErrMsgDialog (error->message, atLine, __FILE__, __func__)	;
    //~ g_error_free(error);
    exit(0);

}  // End of messageDialog
