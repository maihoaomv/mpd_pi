/*** main.c ***/

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
gint globalUpdateTrackview = 0;
gint globalUpdateTrackCenter = 0;
gint globalUpdateSongview = 0;
gint globalVerbose = 0;

// THIS SETS THE VERSION AT COMPILE TIME
gchar * versionString(void)
{
	return g_strdup_printf("Version: %s -- %s", __DATE__, __TIME__);
}

// DECIMAL TO BINARY PRINT TO STDOUT
void DecimalToBinary(int n) {
   int binaryNumber[100];
   int i = 0;
   while (n > 0) {
      binaryNumber[i] = n % 2;
      n = n / 2;
      i++;
   }
   for (int j = i - 1; j >= 0; j--)
   printf("%d", binaryNumber[j]);
}

// BARE MINIMAL MESSAGE DIALOG INTENDED TO SHOW ERROR MESSAGES IF SIMPLE MSG DIALOG CAN'T BE FOUND
void bareMinimumErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GtkWidget *errorDialog;
    errorDialog = gtk_message_dialog_new_with_markup (NULL,  0,  GTK_MESSAGE_ERROR,  GTK_BUTTONS_CLOSE, "<span foreground='red' weight=\"bold\" size=\"larger\">%s\n\nFile %s\nFunction %s\nLine %d\n\nClick \"Close\" or press \"Escape key\" to exit</span>", msg, file, func, line);
    gtk_window_set_title (GTK_WINDOW (errorDialog), "Error");
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
}

// TERMINATE APP IF WE GET    SIGNAL SIGTERM, SIGINT, OR SIGHUP
void terminateApp(int signum)
{
    g_fprintf(stderr, "\nSIGNAL NUMBER = %d  TERMINATE APP\n", signum);
    exit(0);
}

int main(int argc, gchar *argv[])
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!
    //~ atexit(myAtExit);

    gtk_init(&argc, &argv);

    // AT THIS POINT WE SHOULD BE ABLE TO USE THE BARE MINIMUM MESSAGE DIALOG FOR REPORTING ERRORS
     //~ bareMinimumErrMsgDialog ("TEST BARE MIN MSG DIALOG", __LINE__, __FILE__, __func__);
     //~ exitatline;

    // IS THE VERBOSE or VERSION OPTION ON THE COMMAND LINE?
    if(argc > 1)
    {
        int loop = 1;
        while(loop < argc)
        {
            // IF REQUESTING VERBOSE THEN SET THE GLOBAL VERBOSE FLAG -UPPERCASE  V
            if((g_strcmp0(argv[loop], "--verbose") == 0) || (g_strcmp0(argv[loop], "-V") == 0) )
                globalVerbose++;

            // IF -V= OPTION
            if(globalVerbose == 0)
            {
                if(g_str_has_prefix(argv[loop], "-V="))
                    globalVerbose = (int) g_ascii_strtoll(argv[loop]+3, NULL, 0);
            }

            // THE RESOURCE OPTION SHOWS THE RESOURCES COMPILED INTO mpd_pi. PRINT TO STDOUT AND EXIT
            if((g_strcmp0(argv[loop], "--resources") == 0) || (g_strcmp0(argv[loop], "-R") == 0) )
            {
				listResources();
                exit(0);
            }

            // IF THE VERSION OPTION IS ON COMMAND LINE THAN PRINT VERSION STRING TO STDOUT AND EXIT
            if((g_strcmp0(argv[loop], "--version") == 0) || (g_strcmp0(argv[loop], "-v") == 0) )
            {
                fprintf(stdout, "%s\n", versionString());
                exit(0);
            }
            loop++;
            if(loop > argc) break;
        }
    }

    if(globalVerbose)		{
        linenum;
        printf("Line %4d -- VERBOSE ON, Level = %d\n", __LINE__, globalVerbose);
        printf("\n");

		linenum;
		printf ("GLIB-version %d.%d.%d\n",  glib_major_version,  glib_minor_version,  glib_micro_version );

		linenum;
		printf ("GTK-version %d.%d.%d\n",  gtk_major_version,  gtk_minor_version,  gtk_micro_version );
//~ printf(COLOR_BOLD  COLOR_BLUE "Line %d -- EXTERNAL DEFAULT CONFIG FILE\n" COLOR_OFF, __LINE__);
		linenum;
		printf("%s %s\n", g_get_prgname (), versionString());
		printf("\n");
    }

    // ****** INITILIZE TRACKVIEW VARS STRUCT ******
    app_variables *mpd_piVars = g_slice_new(app_variables);

    // SET DEBUG FILE FILENAME SO WE HAVE SOMETHING TO WRITE ERRORS TO
    mpd_piVars->debugFile = g_strdup_printf("%s/%s.debug",g_get_tmp_dir (), g_get_prgname ());

    // THE ABSOULTE WORKING DIRECTORY OF THIS EXECUTABLE
    mpd_piVars->cwdPath = NULL;

    // IS THIS EXECUTABLE A LINKED FILENAME, RELATIVE FILENAME OR AN ABSOULTE PATH?
    gchar *isLink = g_file_read_link (argv[0], NULL);
    gchar *isDir = g_path_get_dirname (argv[0]);

    if(isLink == NULL)
    {
        if(0 == g_strcmp0(isDir, "."))
            mpd_piVars->cwdPath = g_get_current_dir ();
        else mpd_piVars->cwdPath = g_strdup(isDir);
    }
    else {
        mpd_piVars->cwdPath = g_path_get_dirname(isLink);
    }
    g_free(isDir);
    g_free(isLink);

	// ATTEMPT TO FIND THE MSG DIALOG LOCALLY TO USE TEMPORARILY
	// UNTIL WE CAN GET THE REAL PATH/FILENAME FROM THE CONFIG FILE
    mpd_piVars->msgDialogFile = NULL;

	gchar *helperMsgFile = g_strdup_printf("%s/%s/%s",    mpd_piVars->cwdPath, HELPER_FILES_DIR, MSG_DIALOG_FILE);
	gchar *cwdMsgFile = g_strdup_printf("%s/%s",    mpd_piVars->cwdPath, MSG_DIALOG_FILE);

	//  LOOK FIRST FOR simpleMsgDialog IN THE HELPER_FILES DIR
	if(g_access(helperMsgFile, F_OK) == 0)
		mpd_piVars->msgDialogFile = g_strdup(helperMsgFile);

	// IF simpleMsgDialog NOT FOUND IN HELPER_FILES THEN LOOK IN THE CWD
	else if(g_access(cwdMsgFile, F_OK) == 0)
		mpd_piVars->msgDialogFile = g_strdup(cwdMsgFile);

	if(mpd_piVars->msgDialogFile == NULL)
	{
		gchar *msg = g_strdup_printf("Message Dialog -- %s -- not found in either:\n%s\n or\n%s\n",  MSG_DIALOG_FILE,   cwdMsgFile, helperMsgFile);
		printDebugMsg(msg, __LINE__ - 1, __FILE__, __func__, mpd_piVars);
		g_free(msg);
	}

	g_free(helperMsgFile);
	g_free(cwdMsgFile);

	// IF THE HELPER CAN'T BE FOUND  WE JUST LEAVE  IT AS NULL AND USE THE BARE MINAMUM DIALOG FOR ERRORS
	if((globalVerbose == 3) || (globalVerbose > 9))		{
        linenum;	printf("Line %d -- TEMPORARY MESSAGE DIALOG FILE = %s\n\n", __LINE__, mpd_piVars->msgDialogFile);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    // ***** SETUP ERROR HANDLERS *****
    // SEE https://www.computerhope.com/unix/signals.htm FOR LIST IS SIGNALS
    signal(SIGSEGV, signal_callback_sigFault);   // 11 -- SEGMENTATION FAULT
    signal(SIGTERM,   terminateApp);
    signal(SIGINT,    terminateApp);
    signal(SIGHUP,    terminateApp);

    // INITLIZE VARIABLES IN THE TRACKVIEW VARS STRUCT
    mpd_piVars->windowWidth 				= 0;
    mpd_piVars->windowHeight 				= 0;
    mpd_piVars->playviewHeight 				= 1200;
    mpd_piVars->playviewWidth 				= 800;
    mpd_piVars->dataviewHeight 			= 1200;
    mpd_piVars->dataviewWidth 				= 800;
    mpd_piVars->playerIconSize 				= 64;

    mpd_piVars->trackviewHiLiteColor 		= NULL;
    mpd_piVars->WindowTitle              		= NULL;
    mpd_piVars->GuiStartUp                		= NULL;

    mpd_piVars->cfgFile 							= NULL;
    mpd_piVars->userDefaultCfgFile 		= NULL;
    mpd_piVars->gladeFile 						= NULL;
    mpd_piVars->cssFile 							= NULL;
    mpd_piVars->helpFile 							= NULL;

    mpd_piVars->playviewDialogFile 		= NULL;
    mpd_piVars->dataviewDialogFile 		= NULL;

    mpd_piVars->playIconFile               		= NULL;
    mpd_piVars->pauseIconFile            		= NULL;
    mpd_piVars->stopIconFile               	= NULL;
    mpd_piVars->nextIconFile               	= NULL;
    mpd_piVars->forwardIconFile          	= NULL;
    mpd_piVars->previousIconFile        		= NULL;
    mpd_piVars->rewindIconFile           		= NULL;
    mpd_piVars->titleBarIconFile             	= NULL;

    mpd_piVars->windowFont 					= NULL;

    // MUST  ASSIGN NULL POINTER TO mpdHostname HERE IN CASE THERE IS A COMMAND LINE OPTION FOR HOST NAME
    mpd_piVars->mpdHostname 				= NULL;  // ESPECIALLY THE THIS ONE

    // ASSIGN -1 TO PORTNUMBER AND TIMEOUT HERE IN CASE THERE IS A COMMAND LINE OPTION
    mpd_piVars->mpdPortnumber 			= -1;
    mpd_piVars->mpdTimeout 					= -1;

    //~ mpd_piVars->showViewMode =	SHOW_NONE_MODE;

    // GET USERS DEFAULT CONF PATH/FILENAME (CONFIG_FILES).
    gchar *textStr = g_ascii_strup(g_get_prgname (), -1);

    mpd_piVars->userDefaultCfgFile = g_strdup_printf("%s/%s/%s.conf", mpd_piVars->cwdPath, CONFIG_FILES_DIR, g_get_prgname ());

    g_free(textStr);

    // PROCESS THE COMMAND LINE OPTIONS
    getCmdLineOptions(argc, argv, mpd_piVars);

    // GET THE ERROR DIALOG, CSS AND GLADE FILENAMES FROM THE CONFIG FILE
    getExternalFilenames(mpd_piVars);

    // LOAD THE GLADE FILE AND PROCESS WIDGET VARIABLES
    initilazeGladeBuilder(mpd_piVars);

    // SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!
    // LOAD THE CSS FILE FROM mpd_piVars->cssFile, IF IT EXIST
    loadCssProvider(mpd_piVars);

    // READ THE KEY VARIABLE FILE AFTER READING THE CSS FILE
    // READ AND INITILIZE THE KEY VARIABLES FROM THE CONFIG FILE
    getKeyVars(mpd_piVars); // FIRST PASS THROUGH THE CONFIG FILE

    // THEN INITILAZE THE WIDGET  CONFIGURATION VARIALBLES
    setConfigVariables(mpd_piVars);

    // RE-ORDER THE WIDGETS AS INSTRUCTED IN THE CONFIG FILE
    reorderWidgets(mpd_piVars);

    // SET UP THE VARIOUS SIGNALS
    setSignals(mpd_piVars);

    // TEST THE MPD CONNECTION. THIS FUNCTION SERVES MULTIPLE PORPOISES:
    // 1. PREVENT IDLE THEAD FROM CAUSING MULTIPLE MPD CONNECTION ERRORS.
    // 2. IF WE'RE UNABLE TO MAKE A CONNECTION THERE IS NO NEED TO CONTINUE IF APP CAN'T GET A CONNECTION
    // 3. IF THE APP IS AUTO-STARTED AT O/S STARTUP, THERE MAY BE A BIT OF DELAY BETWEEN THE TIME
    //		THIS APP COMES ONLINE AND THE MPD SERVER COMES ONLINE
    // THIS FUNCTION WILL DELAY EXITING APP TO ALLOW A BIT OF TIME FOR THE SERVER TO CONNECT. APPROX. 10 SECONDS.

    struct mpd_connection *conn = NULL;
    conn = get_connection(mpd_piVars);
    if(conn) {
        mpd_connection_free(conn);    // SURPRESSES unused variable 'conn' WARNING
        conn = NULL;
    }

    // START THE MPD IDLE THREAD
    GThread *mpdIdleThread = NULL;

    // MPD QUEUE THREAD - THIS CHILD WILL WAIT FOR A MPD PLAYER EVENT OF STATE CHANGE.
    mpdIdleThread = g_thread_new ("myIdleThread",  (void *) idleThread, mpd_piVars);

    // START A MILLISECOND TIMER TO GET STATUS AND TRACK DATA (timer_handler.c)
    //~ g_timeout_add_full (G_PRIORITY_DEFAULT, 333, (GSourceFunc) showViewTimer, mpd_piVars, NULL);
    g_timeout_add_full (G_PRIORITY_DEFAULT, 250, (GSourceFunc) showViewTimer, mpd_piVars, NULL);

    // START THE MAIN GUI WINDOW LOOP
    gtk_main();

    // WHEN WE EXIT THE APP MAIN WINDOW, IT WILL COME HERE.
    // NEED TO FREE, HERE, THOSE VARIABLES THAT DON'T GET FREE'D OTHERWISE
    // AFTER QUITING MAIN (gtk_main_quit) IS A GOOD PLACE FOR THEM. THAT MEANS
    // THEY WILL BE CLEARED WHEN ESCAPE, CTRL-C AND EXIT IN POPUP MENU
    // WILL CLEAR THEM.

    // NOTE: MAYBE I SHOULD UNREF WIDGET HERE ALSO g_clear_object (&object_ptr);
    if(mpd_piVars->mpdHostname)   g_free(mpd_piVars->mpdHostname);
    if(mpd_piVars->trackviewHiLiteColor)  g_free(mpd_piVars->trackviewHiLiteColor);

    if(mpd_piVars->debugFile )  g_free(mpd_piVars->debugFile );
    //~ if(mpd_piVars->userDefaultCfgFile)  g_free(mpd_piVars->userDefaultCfgFile);
    if(mpd_piVars->cfgFile)  g_free(mpd_piVars->cfgFile);
    if(mpd_piVars->gladeFile)  g_free(mpd_piVars->gladeFile);
    if(mpd_piVars->cssFile)    g_free(mpd_piVars->cssFile);
    if(mpd_piVars->helpFile)    g_free(mpd_piVars->helpFile);

    if(mpd_piVars->cwdPath)  g_free(mpd_piVars->cwdPath);

    g_free(mpd_piVars->msgDialogFile);
    g_free(mpd_piVars->playviewDialogFile);
    g_free(mpd_piVars->dataviewDialogFile);

    g_slice_free(app_variables, mpd_piVars);

    g_thread_unref (mpdIdleThread);

    exit(0);
} // *** End of main ***

// ****** COMMAND LINE OPTIONS ******
void getCmdLineOptions(int argc, gchar **argv, app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    g_autofree gchar *textStr = NULL;

    GError *error = NULL;

    // ****** GET THE COMMAND LINE OPTIONS ******
    //~ static gint cmdLn_version = 0;  // no argument required for version. use int instead of char
    static gchar *cmdLn_cfgFile = NULL;  // absolute filename
    static gchar *cmdLn_hostname = NULL;
    static gint cmdLn_portnumber = 0;
    static gint cmdLn_timeout = 0;

    // ****** HELP MENU -PROCESS THE COMMAND LINE VARIABLES ******
    static GOptionEntry entries[] =
    {
        { "cfg-file", 'c', 0, G_OPTION_ARG_STRING, &cmdLn_cfgFile, "Absolute path/filename of config file.", "/home/user/mpd_pi.conf" },
        { "hostname", 'H', 0, G_OPTION_ARG_STRING, &cmdLn_hostname, "MPD server hostname or IP address.", "rpi3.local" },
        { "port", 'P', 0, G_OPTION_ARG_INT, &cmdLn_portnumber, "MPD server port number.", "6600" },
        { "timeout", 'T', 0, G_OPTION_ARG_INT, &cmdLn_timeout, "MPD server connection timeout.", "30000" },
        { NULL }
    };

    GOptionContext *context;
    context = g_option_context_new (NULL);

	g_autoptr(GString) notes = NULL;
	notes= g_string_new("");
    g_string_append(notes, "Note:  Default hostname (as default setting in the config file) is localhost, port 0, timeout 0\n\n");
    g_string_append(notes, "Debug Options:\n");
	g_string_append_printf(notes, "  %s [ -v ]  -Print version to command line then exit\n", g_get_prgname ());
	g_string_append_printf(notes, "  %s [ -V ]  -Print verbose stats to stdout command line.\n", g_get_prgname ());
	g_string_append(notes, "  Multiple -V or -V=x  Verbose range 1 through 9. 10 or greater, show all.\n");
	g_string_append(notes, "    Level 1 - Conf Files,  Level 2 - Key Variables,  Level 3 - Set configuration,  Level 4 - Songview Variables.\n\n");
    g_string_append_printf(notes, "  %s [ -R ]  -List Resources\n", g_get_prgname ());
	g_string_append_printf(notes, "  Resources may be extracted from %s as shown in the following example:\n", g_get_prgname ());
	g_string_append_printf(notes, "    gresource  extract %s /resources/RESOURCE_NAME  > RESOURCE_NAME-extracted", g_get_prgname ());

    g_option_context_set_description (context, notes->str);

    g_option_context_add_main_entries (context, entries, NULL);

    g_option_context_set_ignore_unknown_options(context, true);

    g_option_context_parse (context, &argc, &argv, &error);
    gint atLine = __LINE__ - 1;
    if(error) {
        printDebugMsg(error->message, atLine, __FILE__, __func__, mpd_piVars);
        bareMinimumErrMsgDialog (error->message, atLine, __FILE__, __func__);
        g_error_free(error);
        exit(0);        // CAN'T USE gtk_main_quit(); HERE -WINDOW NOT INITIALIZED YET
    }
    g_option_context_free (context);

    // ****** End of GET THE COMMAND LINE OPTIONS ******

    // ***** PROCESS THE COMMAND LINE OPTIONS *****
    // IS THE CONFIG FILE TO USE ( -c , --cfg-file=  option ) FOUND ON THE COMMAND LINE?

	// IS THERE A CONFIG FILE ON THE COMMAND LINE?
    if(cmdLn_cfgFile)
    {
        // DOES COMMAND LINE FILE EXIST OR CAN'T BE READ?
        gint atLine = __LINE__ + 1;
        if(g_access(cmdLn_cfgFile, F_OK) == -1)
        {
            textStr = g_strdup_printf("Command Line Option   -c   --cfg-file\n\nConfig file\n\n-- %s --\n\nnot found.", cmdLn_cfgFile);
            printDebugMsg(textStr, atLine, __FILE__, __func__, mpd_piVars);
            bareMinimumErrMsgDialog (textStr, atLine, __FILE__, __func__);
			g_free(textStr);
            exit(0);  // CAN'T USE gtk_main_quit(); HERE -WINDOW NOT INITIALIZED YET
        }
        // IF OK THEN ASSIGN IT TO THE cfgFILE VARIABLE
        mpd_piVars->cfgFile = g_strdup(cmdLn_cfgFile);
        g_free(cmdLn_cfgFile);

        if((globalVerbose == 1)  || (globalVerbose > 9))
		{
            linenum;
            printf(COLOR_BOLD  COLOR_BLUE "Line %d -- COMMAND LINE EXTERNAL CONFIG FILE\n" COLOR_OFF, __LINE__);
        }
	}

	//  IF NO COMMAND LINE CONFIG FILE, THEN TEST THE DEFAULT CONFIG FILE,
	else	if(g_access(mpd_piVars->userDefaultCfgFile, F_OK) == 0)
	{
		mpd_piVars->cfgFile = g_strdup(mpd_piVars->userDefaultCfgFile);

		if((globalVerbose == 1)  || (globalVerbose > 9))
		{
			linenum;
			printf(COLOR_BOLD  COLOR_BLUE "Line %d -- EXTERNAL DEFAULT CONFIG FILE\n" COLOR_OFF, __LINE__);
		}
	}
	else // OTHERWISE WE OPEN THE INTERNAL RESOURCE CONFIG FILE
	{
		mpd_piVars->cfgFile = g_strdup(CONFIG_RESOURCE); // ASSIGN RESOURCE TO 	cfgFile
		if((globalVerbose == 1)  || (globalVerbose > 9))
		{
			linenum;
			printf(COLOR_BOLD  COLOR_BLUE "Line %d -- INTERNAL RESOURCE CONFIG FILE\n" COLOR_OFF, __LINE__);
		}
				g_spawn_command_line_async ("./simpleMsgDialog -n -t 5 \"Default Cfg File not found\ndefaulting to INTERNAL RESOURCE config file.\"", NULL);
	}
	g_free(mpd_piVars->userDefaultCfgFile);

    if((globalVerbose == 1) || (globalVerbose > 9)) {
		//~ linenum;
        printf(COLOR_BOLD  COLOR_BLUE "Line %d -- CONFIG FILE %s\n\n" COLOR_OFF, __LINE__, mpd_piVars->cfgFile);
    }

    // IF COMMAND LINE VARIABLES FOR THE MPD SERVER, ARE SPECIFIED ON
    // THE COMMAND LINE VARIABLES THEN OVERRIDE THOSE FROM THE CFG FILE
    // MPD HOSTNAME
    if(cmdLn_hostname) {
        g_free(mpd_piVars->mpdHostname);
        mpd_piVars->mpdHostname = NULL;
        mpd_piVars->mpdHostname = g_strdup(cmdLn_hostname);
        g_free(cmdLn_hostname);
    }

    // MPD PORTNUMBER
    if(cmdLn_portnumber < 0)
        mpd_piVars->mpdPortnumber = cmdLn_portnumber;

    // MPD TIMEOUT
    if(cmdLn_timeout < 0)
        mpd_piVars->mpdTimeout = cmdLn_timeout;
}

void idleThread(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

	// IF THE CONFIG FILE  IS SET TO BRING GUI UP IN SONGVIEW MODE THEN THEN NEED TO FILL THE TRACKVIEW FIRST
	// TO PREVENT EMPTY TREEVIEW WHEN WHEN WE SWITCH FROM SONGVIEW TO TRACKVIEW MODE
	if(mpd_piVars->showViewMode == SHOW_SONGVIEW_MODE)
		trackView (mpd_piVars);

    struct mpd_connection * conn = NULL;
    conn = get_connection(mpd_piVars);
    static GMutex mutex;

    while(1)  // IF WE LOSE MPD CONNECTION, COME BACK HERE IF MPD RECONNECTS
    {
        enum mpd_idle mask =   MPD_IDLE_QUEUE 	|  MPD_IDLE_UPDATE | MPD_IDLE_STORED_PLAYLIST | MPD_IDLE_PLAYER;
        // FORCE IT TO UPDATE / FILL THE TRACKVIEW ON THE FIRST PASS OF THE "DO" LOOP
        enum mpd_idle idleRtn = MPD_IDLE_QUEUE;
        do	{
            switch ((gint) idleRtn)
            {
				case MPD_IDLE_QUEUE:
				case MPD_IDLE_UPDATE:
				case MPD_IDLE_STORED_PLAYLIST:
                // WE ONLY HAVE TO UPDATE THE TRACKVIEW IF THE QUEUE SOME HOW CHANGES
                g_mutex_lock (&mutex);
                globalUpdateTrackview++;
                g_mutex_unlock (&mutex);
                break;
            }

			// ANY CHANGE TO THE PLAYER STATUS (i.e. FF, PLAY, PAUSE, STOP etc. SHOULD SEND THE LOOP HERE.
            g_mutex_lock (&mutex);
			// UPDATE SONGVIEW
			globalUpdateSongview++;
            // CENTER THE CURRENT TRACK IN TREEVIEW
            globalUpdateTrackCenter++;
            g_mutex_unlock (&mutex);

        } while ((idleRtn = mpd_run_idle_mask(conn, mask)));

        // IF WE LOSE OR CAN'T GET THE  MPD CONNECTION,  THE  "DO" LOOP WILL DROP OUT HERE
        // AND LOOP IN THE OUTER "WHILE" LOOP UNTIL THE CONNECTION CAN BE RE-ESTABLISHED
        if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
            gint atLine = __LINE__ - 1;
            gchar *str = g_strdup_printf("Line %d -- MPD Idle Thread .... %s\n", __LINE__, mpd_connection_get_error_message(conn));
            printDebugMsg(str, atLine, __FILE__, __func__, mpd_piVars);
            mpd_connection_clear_error(conn);
            mpd_connection_free(conn);
            g_free(str);
        }

        // BUT IF IT CAN'T RECONNECT, THE THREAD WILL EXIT
        conn = attemptReConnect(conn, mpd_piVars);
        if(!conn) break;
    }
    g_mutex_clear (&mutex);
    g_thread_exit (NULL);
}  // End of idleThread
