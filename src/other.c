// *** other.c ***
#include "mpd_pi.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

// CONVERT SECONDS INTO TIME FORMATED STRING, THEN RETURN IT AS A STRING
gchar * convertTimeToString(int sec)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    int hrs = 0;
    int min = 0;

    min = sec / 60;
    sec = sec % 60;
    hrs = min / 60;
    min = min % 60;

    if(hrs) 	return g_strdup_printf("%d:%02d:%02d", hrs, min, sec);
    else 	return g_strdup_printf("%d:%02d", min, sec);
} // *** END OF CAL TIME TO STRING ***

// ***** CATCH SIGNAL HANDLER FUNCTION *****
void signal_callback_sigFault(int signum)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gchar *SigFaultFile  = SIG_FAULT_FILE; // DEFINED IN mpd_pi.h

    char timeBuff[70];
    time_t current_timeNOW = time(NULL);
    struct tm *tmNOW = localtime(&current_timeNOW);
    strftime(timeBuff, sizeof timeBuff, "%A %b/%d/%Y %I:%M:%S %p", tmNOW);

    // SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE
    //	syslog(LOG_INFO | LOG_NDELAY | LOG_PERROR, "%s -- %d", "SigFault sig number = ", signum);
    // SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE  SAVE FOR FUTURE USE

    void *array[10];
    gchar **strings;

    int size;
    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);
    GString *myMsg = g_string_new (NULL);

    if (strings != NULL)
    {
        g_string_append_printf(myMsg, "------------------------------------------------ SIGNAL FAULT ------------------------------------------------\n\n");
        g_string_append_printf(myMsg, "    Backtrace -- Obtained %d stack frames.\n", size);

        for (int i = 0; i < size; i++)
            g_string_append_printf(myMsg, "    %d -- \t%s\n",  i + 1, strings[i]);
    }

    g_string_append_printf(myMsg, "\n    %s -- Trapped signal -- %d -- %s --\n", timeBuff, signum, g_strsignal(signum));

    g_string_append_printf(myMsg, "    -- %s -- %s\n", g_get_prgname (),  versionString());
    g_string_append_printf(myMsg, "----------------------------------------------------------------------------------------------------------\n");

    g_fprintf(stdout,"%s\n\n", myMsg->str);

    GError *error = NULL;
    g_file_set_contents (SigFaultFile, myMsg->str, -1, &error);

    // IF THERE IS AN ERROR WRITING TO TEMP FILE,
    if(error) {
        g_fprintf(stderr, "ERROR %s\n", error->message);
        g_free(error);
    }

    gchar *textStr = g_strdup_printf("A Sig Fault has occured.\n%s\n%s\n", myMsg->str, SigFaultFile );
    // CAN'T USE DEBUG MACRO OR PRINT DEBUG MSG HERE BECAUSE CAN'T PASS TRACKVIEWVARS
    bareMinimumErrMsgDialog (textStr, __LINE__, __FILE__, __func__);

    g_free(textStr);
    g_free(SigFaultFile);
    g_free (strings);
    g_string_free(myMsg, TRUE);
    //	printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    exit(0);
}// ** End of signal_callback_handler ***
//*****************************************************************************************************************

// THIS FUNCTION FORMATS AND PRINTS ERROR MESSAGEES TO STDERR AND, IF POSSIBLE, A TEMP ERROR FILE
int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    //~ gchar *DebugFile = strdup(DEBUG_FILE);
    gchar *DebugFile = g_strdup_printf("%s/%s.debug",g_get_tmp_dir (), g_get_prgname ());
    printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    printf("DebugFile %s\n", DebugFile);

    char timeBuff[70];
    time_t current_timeNOW = time(NULL);
    struct tm *tmNOW = localtime(&current_timeNOW);
    strftime(timeBuff, sizeof timeBuff, "\t%m/%d/%Y %I:%M:%S %p", tmNOW);
    g_autoptr(GError) error = NULL;
    GString *myMsg = g_string_new (NULL);
    g_autofree gchar *contents = NULL;

    g_string_append_printf(myMsg, "\n--------------- START ---------------\n");
    g_string_append_printf(myMsg, "%s\n\n", timeBuff);
    g_string_append_printf(myMsg, "%s\n\n", debugMsg);

    // THIS IS SPECIAL CASE WHERE I DON'T WANT TO DISPLAY LINE, FILE AND FUNCTION IN THE MESSAGE. USE ZERO OR NEGATIVE NUMBER
    if(line > 0)
    {
        g_string_append_printf(myMsg, "\tLine %d\n", line);
        g_string_append_printf(myMsg, "\tFile %s\n", file);
        g_string_append_printf(myMsg, "\tFunction %s\n\n", func);
    }

    g_string_append_printf(myMsg, "%s/%s -- Compiled %s -- %s\n\n", mpd_piVars->cwdPath, g_get_prgname (), __DATE__, __TIME__);
    g_string_append_printf(myMsg, "-- Debug message written to file -- %s\n", DebugFile);
    g_string_append_printf(myMsg, "--------------- END ---------------\n");

    g_fprintf(stderr,"%s\n\n", myMsg->str);

    // GET CONTENTS OF THE EXISTING DEBUG FILE, IF IT EXIST,
    gint result = g_file_get_contents (DebugFile, &contents, NULL, NULL);
    // AND APPEND IT TO THE END OF THE MSG STRING SO THAT THE LATEST WILL BE AT THE VERY TOP OF THE FILE
    if(result)
        g_string_append_printf(myMsg, "\n%s\n", contents);

    // THEN WRITE THE STRING BACK TO DISK, OVERWRITING THE EXISING CONTENTS.
    g_file_set_contents (DebugFile, myMsg->str, -1, &error);

    // IF THERE IS AN ERROR WRITING TO TEMP FILE,
    if(error)
        g_fprintf(stderr, "ERROR writing to debug file %s -- %s\n", DebugFile, error->message);

    g_free(DebugFile);
    g_string_free(myMsg, TRUE);

    return 0;
}  // End of printDebugMsg

//////////////////////////////////// S E R V E R  P L A Y I N G  T I M E ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONVERT SECONDS TO DAYS / HRS / MINS / SECONDS TIME STRING
void timeStr(gchar **rtnStr, unsigned long inSeconds)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    unsigned long seconds 	= 	inSeconds 		% 60;
    unsigned long days 		= 	inSeconds 		/ 86400;
    unsigned long hours 		= 	(inSeconds 		% 86400) / 3600;
    unsigned long minutes 	= 	(inSeconds 		% 3600) / 60;

    gchar *myDays = NULL;
    gchar *myHrs = NULL;
    gchar *myMins = NULL;
    gchar *mySecs = NULL;

    if (days != 0)
    {
        if(days > 1)  myDays = g_strdup_printf("%li %s ", days, "days");
        else  myDays = g_strdup_printf("%li %s ", days, "day");
    }

    // IF EITHER DAYS OR HOURS ARE GREATER THAN ZERO
    if(hours | days)
    {
        if(hours != 1)  myHrs = g_strdup_printf("%li %s ", hours, "hours");
        else  myHrs = g_strdup_printf("%li %s ", hours, "hour");
    }

    // IF EITHER DAYS, HOURS OR MINUTES ARE GREATER THAN ZERO
    if(hours | days | minutes)
    {
        if(minutes != 1)  myMins = g_strdup_printf("%li %s ", minutes, "minutes");
        else  myMins = g_strdup_printf("%li %s ", minutes, "minute");
    }
    // SECONDS WILL ALWAYS BE SHOWN EVEN IF ZERO
    if(seconds != 1)  mySecs = g_strdup_printf("%li %s \n", seconds, "seconds");
    else  mySecs = g_strdup_printf("%li %s \n", seconds, "second");

    if(myDays == NULL) 	myDays = g_strdup("");
    if(myHrs == NULL) 	myHrs = g_strdup("");
    if(myMins == NULL) 	myMins = g_strdup("");
    //	if(mySecs == NULL) 	mySecs = g_strdup("0");

    *rtnStr = g_strdup_printf("%s %s %s %s",myDays, myHrs, myMins, mySecs);

    g_free(myDays);
    g_free(myHrs);
    g_free(myMins);
    g_free(mySecs);

    return;
} // End of timeStr

// THIS FUNCTION HAS TO RETURN GBOOLEAN TO ALLOW TRUE/FALSE FOR THE BUTTON PRESS EVENT
void showHelpMessage (app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

	if(mpd_piVars->helpFile)
	{
		g_autofree gchar *contents = NULL;
		g_file_get_contents (mpd_piVars->helpFile, &contents, NULL, NULL);
		messageDialog (g_markup_escape_text (contents, -1), __LINE__, __FILE__, __func__, INFORMATION_MSG_MODE, 0, FALSE, mpd_piVars);

//~ messageDialog (g_markup_escape_text ((gchar *)data, -1), __LINE__, __FILE__, __func__, INFORMATION_MSG_MODE, 0, FALSE, mpd_piVars);


		return;
	}

    GError *error = NULL;
    GBytes *bytes = NULL;
    gsize len;
    gconstpointer data = NULL;

    bytes = g_resources_lookup_data (HELP_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    if(error)
    {
        errMsgMacro (error->message);
        g_error_free(error);
        exit(0);
    }

    if (bytes != NULL)
        data = g_bytes_get_data (bytes, &len);
//~ printf("\n%s\n", (gchar *)data); exit(0);
    //~ gtk_label_set_markup (GTK_LABEL (mpd_piVars->w_lblHelpLabel), (gchar *)data);
    messageDialog (g_markup_escape_text ((gchar *)data, -1), __LINE__, __FILE__, __func__, INFORMATION_MSG_MODE, 0, FALSE, mpd_piVars);

    g_bytes_unref (bytes);
    return;

}  // End of showHelpMessage


//~ void listResources(app_variables *mpd_piVars)
void listResources()
{

	GString *myStr = NULL;
	myStr= g_string_new("");
    g_string_append_printf(myStr, "---- Resources compiled into %s ----\n", g_get_prgname());

    const char *nspath = SOURCE_PATH;
    gchar **resourceList;
    g_autoptr(GError) error = NULL;
	gint atLine = __LINE__ + 1;
    gsize size;
    gchar *path = NULL;

    resourceList = g_resources_enumerate_children(nspath, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);

    if (error)  bareMinimumErrMsgDialog (error->message, atLine, __FILE__, __func__)	;

        gchar **cur = resourceList;

        while (cur != NULL && *cur != NULL)
        {
            path = g_build_path ("/",  nspath, *cur, NULL);
            g_resources_get_info (path, G_RESOURCE_LOOKUP_FLAGS_NONE, &size, NULL, NULL);
            g_string_append_printf(myStr, "GRsource\t%8ld \t%s/%s\n",size, nspath, *cur);
            cur++;
            g_free(path);
        }
        g_strfreev(resourceList);

	fprintf(stdout, "%s\n", myStr->str);

		return;
}

