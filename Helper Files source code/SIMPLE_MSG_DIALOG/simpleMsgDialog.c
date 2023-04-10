// simpleMsgDialog.c

/* This app is intended as an multi-message helper dialog for mpd_pi, dataview and playview apps
* and is not inteded to ran standalone but it can be ran from the command line.
*
* simpleMsgDialog requires at minimum a quoted "" message string.
* it can use PANGO markup text or control characters such as \n, \t ,etc.
*
* simpleMsgDialog requires at minimum a message string enclosed in quotes "message" on command line with or without an option (-e, -n, -q, i)
*
* simpleMsgDialog accepts the following options:
*	Help:	   -h  OR  --help	Print this help and exit
*	Options:   -v  OR  --version	Print version and exit
*	Options:   -m  "error message"  OR  --msg="error message"
*	ParentPid: -p parentPid  OR  --pid=parentPid
*	Timeout:   -t seconds  OR  timeout=seconds
*	Verbose:   -V
*
*	-e,  --error		Display error messag dialog.
*	-n,  --notify		Display notify message dialog.
*	-q,  --question	Display question message dialog.
*	-i,  --info			Display help/status message dialog.
*
*/

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include <getopt.h>

// THESE 2 FAILURE CODES MUST BE AN INT BETWEEN -127 (0x81) AND 127 (0x7F)
#define        GLADE_FAILED  	64 	// decimal
#define        CSS_FAILED  		65 // decimal

#define CSS_RESOURCE 			"/simpleMsgDialog/simpleMsgDialog.css"

#define GLADE_RESOURCE 	"/simpleMsgDialog/simpleMsgDialog.glade"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THESE MACRO ARE INTENDED FOR DEBUGGING PURPOSES
// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

#define COLOR_RED "\033[31m"

#define COLOR_BLUE "\033[34m"

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define COLOR_YELLOW "\033[33m"

// THIS MACRO PRINTS LINE NUMBER THEN EXITS WITH ZERO
#define exitatline printf(COLOR_BOLD  COLOR_BLUE "EXIT AT LINE %d\n" COLOR_OFF, __LINE__); exit(0);

// THIS MACRO PRINTS THE LINE NUMBER
#define printline printf(COLOR_BOLD  COLOR_RED "Line # %d  " COLOR_OFF, __LINE__);

// THIS MACRO PRINTS FILE, FUNCTION AND LINE NUMBER
//~ #define linenum printf(COLOR_BOLD  COLOR_RED "-- File %s -- Func %s -- Line %d --\n" COLOR_OFF,__FILE__,  __func__, __LINE__);
#define linenum printf(COLOR_BOLD  COLOR_YELLOW "-- File %s -- Func %s -- Line %d --\n" COLOR_OFF,__FILE__,  __func__, __LINE__);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// NOTE: Need a dash '  - ' at start of optstring[] to capture a message
// from command line without the  --msg or -m option
static const char optstring[] = "-hm:t:vp:enqiT:f:";
static const struct option options_long[] = {
    /* name		has_arg			flag	val */
    { "help",	no_argument,		NULL,	'h' },  // HELP
    { "msg", required_argument, NULL, 'm' },  // ERROR MSG STRING
    { "timeout",	required_argument,	NULL,	't' }, // DIALOG TIMEOUT
    { "version",	no_argument,		NULL,	'v' }, // VERSION
    { "pid",	required_argument,	NULL,	'p' }, // PARENT PID
    { "error", no_argument, NULL, 'e' },  // ERROR MSG STRING
    { "notify", no_argument, NULL, 'n' },  // NOTIFY MSG STRING
    { "question", no_argument, NULL, 'q' },  // QUESTION MSG STRING
    { "info", no_argument, NULL, 'i' },  // INFORMATION MSG STRING
    { "title", required_argument, NULL, 'T' },  // TITLE STRING
    { "font", required_argument, NULL, 'f' },  // FONT SIZE, FAMILY, EXT STRING
    { 0, 0, 0, 0 }
};

void printHelp(gint argc);

gboolean timeoutOfDialog()
{   // g_fprintf(stdout, "Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    exit(0);
    return FALSE;
}

int main(int argc, gchar *argv[])
{   // g_fprintf(stdout, "Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_init(&argc, &argv);
//////////////////////////////////////////////////////////////////////////////////////////
    int verbose = 0;

// IF VERBOSE OPTION -V OR --verbose ARE ON THE COMMAND LINE
// THEN SET VERBOSE TO PRINT STATUS TO STDOUT
	int x = 0;
    while(argv[x] != NULL) {
        if(g_strcmp0(argv[x], "-V") == 0)
            verbose++;
        if(g_ascii_strcasecmp(argv[x], "--verbose") == 0)
            verbose++;
		x++;
	}

	if(verbose) printf("\nLine %d -- Verbose is on.\n", __LINE__);

	g_autofree gchar * cwdPath = NULL;
	// IS THIS EXECUTABLE A LINKED FILENAME, RELATIVE FILENAME OR AN ABSOULTE PATH?
    g_autofree gchar *isLink = g_file_read_link (argv[0], NULL);
    g_autofree gchar *isDir = g_path_get_dirname (argv[0]);

    if(isLink == NULL)
    {
        if(0 == g_strcmp0(isDir, "."))
            cwdPath = g_get_current_dir ();
        else cwdPath = g_strdup(isDir);
    }
    else {
        cwdPath = g_path_get_dirname(isLink);
    }
	if(verbose) printf("\nLine %d -- CWD PATH = %s\n", __LINE__,  cwdPath);

	if(argc == 1)
	{
		printHelp(argc);
		exit(0);
	}

    g_autofree gchar * msg = NULL;
    g_autofree gchar *errMsg = NULL;
    g_autofree gchar *dialogTitle = NULL;
    g_autofree gchar *dialogFont = NULL;

    int timeout = 0;
    GPid parentPid = 0;
    int i = 0;

// IF VERBOSE PRINT THE COMMAND LINE TO STDOUT
    if(verbose)
    {
        printf("\nLine %d -- ARGC = %d\n",__LINE__,  argc);
        printf("\nLine %d -- Command line = ", __LINE__);
        x = 0;
        while(argv[x] != NULL)
        {
            printf("%s ", argv[x]);
            x++;
        }
        printf("\n");
    }

    gint response = 1234;
    guint failCode = 0;

	g_autoptr(GError) error = NULL;
////////////////// LOAD THE GLADE XML UI //////////////////////////////
    GtkBuilder *builder = gtk_builder_new();

	// TO USE AN EXTERNAL GLADE FILE -UNCOMMENT THESE 3 LINES OF CODE
	// THE GLADE FILE MUST BE IN THE SAME DIRECTORY AS THE EXECUTABLE
	// g_autofree gchar *gladeFile = g_strdup_printf("%s/%s.glade", cwdPath,  g_get_prgname ());
	// if(verbose) printf("\nLine %d -- GLADE FILE = %s\n", __LINE__,  gladeFile);
	// gtk_builder_add_from_file (builder, gladeFile, &error); //EXTERNAL FILE VERSION
	//
	// THEN COMMENT OUT THE gtk_builder_add_from_resource LINE OF CODE
	gtk_builder_add_from_resource (builder, GLADE_RESOURCE, &error);
    if(error) {
linenum;        g_fprintf(stdout, "\nGlade Error -- Line %d -- %s\n\n", __LINE__, error->message);
        failCode = GLADE_FAILED;
        goto hasFailed;
    }
////////////////// Done with LOAD THE GLADE XML UI //////////////////////////////

//~ //******* INITILAZE THE DIALOG AND LABEL WIDGETS *********
    GtkWidget *messageDialog = GTK_WIDGET(gtk_builder_get_object(builder, "messageDialog"));
    GtkWidget *message_label = GTK_WIDGET(gtk_builder_get_object(builder, "message_label"));

    GtkWidget *dialog_yes_button = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_yes_button"));
    GtkWidget *dialog_no_button = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_no_button"));
    GtkWidget *dialog_exit_button = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_exit_button"));
    GtkWidget *dialog_quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_quit_button"));

	gtk_widget_hide(GTK_WIDGET(dialog_yes_button));
	gtk_widget_hide(GTK_WIDGET(dialog_no_button));
	gtk_widget_hide(GTK_WIDGET(dialog_exit_button));
	gtk_widget_hide(GTK_WIDGET(dialog_quit_button));

    g_object_unref(builder);

// GET COMMAND LINE OPTIONS
    while ((i = getopt_long(argc, argv, optstring, options_long, NULL))) {
// IF VERBOSE IS ON PRINT SOME STATUS
        if(verbose)
        {
            printf("\nLine %d -- I = %d, OPTIND %d -- OPTOPT %d -- ", __LINE__, i, optind, optopt);
            x = 0;
            while(argv[x] != NULL)
            {
                fprintf(stdout, "%s ", argv[x]);
                x++;
            }
            fprintf(stdout, "\n\n");
        }

        switch (i) {
        case -1:
			if(verbose)	printf("\nLine %d -- = %d END OF OPTIONS \n", __LINE__, i);
			goto done;
            break;

        case 'h':  // HELP OPTION
            if(verbose)
				fprintf(stdout, "\nLine %d -- HELP OPTION -h , --help \n", __LINE__);
			printHelp(argc);
            exit(0);
            break;

        case 'v': // VERSION
            if(verbose)
				fprintf(stdout, "\nLine %d -- VERSION OPTION -v , --version\n", __LINE__);
            fprintf(stdout, "Version: %s -- %s\n\n", __DATE__, __TIME__);
            exit(0);
            break;

        case 1:  // MESSAGE STRING WITHOUT COMMAND LINE OPTION
			if(msg);
            else
				msg = g_strdup(optarg);
            if(verbose)
				fprintf(stdout, "\nLine %d -- NON-OPTION MSG STRING = %s\n", __LINE__, msg);
            break;

        case 'm':	// MESSAGE STRING WITH COMMAND LINE OPTION
			if(msg);
            else
				msg = g_strdup(optarg);;
            if(verbose)
				fprintf(stdout, "\nLine %d -- MSG STRING OPTION  -m , --msg  = %s\n", __LINE__, msg);
            break;

        case 't':	// TIMEOUT
            timeout = atoi(optarg);
            if(verbose)
				fprintf(stdout, "\nLine %d -- TIMEOUT  OPTION = %d seconds\n", __LINE__, timeout);
            if(timeout)
                g_timeout_add_seconds ((timeout), (GSourceFunc)timeoutOfDialog, NULL);
            break;

        case 'p':	// PARENT PID
            parentPid = atoi(optarg);
            if(verbose)
				fprintf(stdout, "\nLine %d -- OPT PID = %d\n", __LINE__, parentPid);
            break;

		case 'e':
			if(verbose) printf("\nLine %d -- ERROR DIALOG OPTION = -e , --error\n", __LINE__);

			gtk_window_set_title (GTK_WINDOW (messageDialog), "Error Message");
			gtk_widget_set_name (messageDialog, "CSS_errorDialog");
			gtk_widget_show(GTK_WIDGET(dialog_exit_button));
			gtk_widget_show(GTK_WIDGET(dialog_quit_button));

		break;

       		case 'n':
				if(verbose) printf("\nLine %d -- NOTIFY DIALOG OPTION = -n , --notify\n", __LINE__);

				gtk_window_set_title (GTK_WINDOW (messageDialog), "Notify Message");
				gtk_widget_set_name (messageDialog, "CSS_notifyDialog");
				gtk_widget_show(GTK_WIDGET(dialog_quit_button));
// this will position dialog on the screen. from top left corner(0,0) to + horz, +vert. i.e (150,150)
			gtk_window_move (GTK_WINDOW (messageDialog), 0,0); // PUT DIALOG IN TOP LEFT CORNER

		break;

       		case 'q':
				if(verbose) printf("\nLine %d --QUESTION DIALOG OPTION = -q , --question\n", __LINE__);

				gtk_window_set_title (GTK_WINDOW (messageDialog), "Question Message");
				gtk_widget_set_name (messageDialog, "CSS_questionDialog");
				gtk_widget_show(GTK_WIDGET(dialog_yes_button));
				gtk_widget_show(GTK_WIDGET(dialog_no_button));

		break;

       		case 'i': //INFORMATION DIALOG
				if(verbose) printf("\nLine %d -- INFORMATION DIALOG OPTION = -i , --info\n", __LINE__);

				gtk_window_set_title (GTK_WINDOW (messageDialog), "Information Message");
				gtk_widget_set_name (messageDialog, "CSS_informationDialog");
				gtk_widget_show(GTK_WIDGET(dialog_quit_button));

		break;

		case 'T' :
			dialogTitle = g_strdup(optarg);;
		if(verbose)
				fprintf(stdout, "\nLine %d __ TITLE OPTION = %s\n", __LINE__,  dialogTitle);
		break;

		case 'f' :  // dialog { font: Bold 18pt Sans; }
			dialogFont = g_strdup(optarg);
			if(verbose)
			{
				fprintf(stdout, "\nLine %d -- FONT OPTION = %s\n", __LINE__, dialogFont);
				fprintf(stdout, "\nLine %d -- dialogFont SIZE = %ld\n", __LINE__, strlen(dialogFont));
			}
		break;
        }
    }

// End of OPTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////
done:

    if(msg == NULL)
    {
        printHelp(argc);
        exit(0);
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** LOAD THE CSS  FROM RESOURCE ***** //
    GtkCssProvider *cssProvider = gtk_css_provider_new ();
    GdkDisplay *display = gdk_display_get_default ();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (cssProvider), \
            GTK_STYLE_PROVIDER_PRIORITY_USER);
	gtk_css_provider_load_from_resource (cssProvider, CSS_RESOURCE);

    g_object_unref(cssProvider);

//*****************************************************
// ***** SET THE FONT ***** //
// IF DIALOG FONT IS NOT NULL OR THE STRING LENGTH IS GREATER THAN ZERO
// THIS REQUIRES I HAVE DIALOG DEFINED IN THE CSS FILE AS dialog { font: 20px Sans;}
	if((dialogFont != NULL) && (strlen(dialogFont) > 1))
	{
		g_autofree gchar *windowFont = NULL;
		windowFont = g_strdup_printf("dialog { font: %s;}\n", dialogFont);
        GtkCssProvider *fontProvider = gtk_css_provider_new ();
        gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (fontProvider), \
                GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_css_provider_load_from_data (fontProvider, windowFont, -1, &error);

		if(error) {   // IF THE DIALOG FONT CAUSES A CSS ERROR THEN WRITE IT TO STDOUT
			linenum;        fprintf(stdout, "\nCSS Error -- Line %d -- %s\n\n", __LINE__, error->message);
			gtk_widget_destroy (messageDialog);
			failCode = CSS_FAILED;
			goto hasFailed;
		}

        g_object_unref(fontProvider);
	}
//*****************************************************

// IF VERBOSE PRINT SOME STATUS
    if(verbose)
    {
        printf("\nLine %d -- PRINT SOME COMMAND LINE STATS\n", __LINE__);
        printf("\nLine %d -- VERBOSE OUTPUT to stdout from %s\n", __LINE__, g_get_prgname ());
        printf("\nLine %d -- MSG = %s\n", __LINE__, msg);
        printf("\nLine %d -- TIMEOUT = %d seconds\n", __LINE__, timeout);
        printf("\nLine %d -- VERBOSE = %d\n", __LINE__, verbose);
        printf("\nLine %d -- PARENT Process ID = %d\n", __LINE__, parentPid);
        printf("\nLine %d -- %s  Process ID = %d\n", __LINE__, g_get_prgname (), getpid());
        printf("\nLine %d -- %s msg=%s timeout=%d pid=%d\n", __LINE__, argv[0], msg, timeout, parentPid);
        printf("\nLine %d -- End of OUTPUT to stdout from %s\n", __LINE__, g_get_prgname ());

    }

// PUT THE DIALOG ALWAYS ON TOP
    gtk_window_set_keep_above (GTK_WINDOW(messageDialog), TRUE);

//	printf("dialogTitle %s ==== optarg %s\n", dialogTitle, optarg);
	if(dialogTitle)
		gtk_window_set_title (GTK_WINDOW (messageDialog), dialogTitle);

// DISPLAY THE MESSAGE ON THE LABEL
    errMsg = g_strcompress(msg);
// JUST IN CASE, SAVE THIS. 	NO g_markup_escape_text MIGHT CAUSE PROBLEMS.
//   SAVE!!!! gtk_label_set_markup (GTK_LABEL(message_label), g_markup_escape_text(g_strcompress(errMsg), -1)); SAVE!!!!
    gtk_label_set_markup (GTK_LABEL(message_label), errMsg);

	if(verbose) printf("\nLine %d -- ERROR MSG = %s\n", __LINE__, errMsg);

    response = gtk_dialog_run (GTK_DIALOG (messageDialog));

    gtk_widget_destroy (messageDialog);
	if(verbose) printf("parentPid =  %i\n", parentPid);
hasFailed:


    if(failCode)
	{
		linenum;	fprintf(stdout, "FAIL CODE dec %d -- HEX 0x%X\n\n",failCode, failCode);
        response = failCode;
	}

    if(parentPid) {
        if(response == GTK_RESPONSE_CLOSE)
            kill(parentPid, SIGTERM);
    }

// THIS LINE IS NEEDED TO RETURN SUCCESS/FAIL CODES VIA STDERR BACK TO CALLING PROGRAM
	fprintf(stderr,COLOR_RED COLOR_BOLD "%d" COLOR_OFF, response); // MUST BE STDERR
// IT WILL CAUSE APP TO PRINT THE ERROR CODE TO THE COMMAND LINE OF THE PARENT -IN RED BOLD COLOR

    //~ exit(response);
    return response;
}  // End of main

void printHelp(gint argc)
{ // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

// BECAUSE OF RETURNING RESULT OVER STDERR USE PRINT OUTPUT TO  STDOUT
    if(argc == 1)
		g_printf("\n  ** No options found on command line. **\n\n");

	GString *myMsg = g_string_new (NULL);

	g_string_append_printf(myMsg, "  %s is inteded as a helper to display error, noitfy, question, or information dialogs.\n", g_get_prgname ());

	g_string_append_printf(myMsg, "  %s requires at minimum a message string enclosed in quotes and one dialog option on the command line.\n\n", g_get_prgname ());

	g_string_append_printf(myMsg, "  Usage:  %s \"message string\" [ option ] .... [ dialog option ]\n\n", g_get_prgname ());

	g_string_append_printf(myMsg, "  %s accepts the following options:\n",g_get_prgname ());

    g_string_append(myMsg, "     -h ,  --help\t\t\t\t\t\t\tPrint help to stdout and exit.\n");

    g_string_append(myMsg, "     -v ,  --version\t\t\t\t\t\t\tPrint version to stdout and exit.\n");

    g_string_append(myMsg, "     -V ,  --Verbose\t\t\t\t\t\t\tPrint verbose debug status to stdout.\n");

    g_string_append(myMsg, "     -m  \"message string\" ,   --msg=\"message string\"\t\t\tEnclose Message string in quotes.\n");

    g_string_append(myMsg, "     -T  \"dialog title string\" ,  --title=\"dialog title string\"\t\tEnclose Title string in quotes.\n");

    g_string_append(myMsg, "     -f  \"Bold 16px Sans\" ,  --font=\"Bold italic 16px Sans\"\t\tEnclose Font string in quotes.\n");

    g_string_append(myMsg, "     -p parentPid ,  --pid=parentPid	\t\t\t\tParent process ID (integer => 0).\n");

    g_string_append(myMsg, "     -t seconds ,  --timeout=seconds\t\t\t\t\tTimeout to auto exit this app  (integer => 0).\n");

	g_string_append(myMsg, "\n  These are the dialog options\n");

   g_string_append(myMsg, "     -e ,  --error\t\t\t\t\t\t\tDisplay error messag dialog.\n");

    g_string_append(myMsg, "     -n ,  --notify\t\t\t\t\t\t\tDisplay notify message dialog.\n");

    g_string_append(myMsg, "     -q ,  --question\t\t\t\t\t\t\tDisplay question message dialog.\n");

    g_string_append(myMsg, "     -i ,  --info 	\t\t\t\t\t\tDisplay help/status/other message dialog.\n");

	printf("\n\n%s\n", myMsg->str);

	g_string_free(myMsg, TRUE);

}

// END!!  END!!  END!!  END!!  END!!  END!!  END!!  END!!  END!!  END!!  END!!