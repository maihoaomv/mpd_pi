// *** playview.h ***

//#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>
#include <mpd/client.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <gio/gio.h>
#include <getopt.h>

//~ #define exitAtLine printf("EXIT AT LINE %d\n", __LINE__); exit(0);
// THIS MACRO WILL EXIT 0 AND PRINT THE LINE NUMBER HOPEFULLY TO MAKE THEM EAISER TO FIND IN THE CODE
// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

#define COLOR_BOLD_SLOW_BLINKING      "\e[1;5m"
#define COLOR_BOLD_SLOW_BLINKING_RED  "\e[1;5;31m"

#define COLOR_BLACK "\033[42m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_WHITE	"\033[37m"

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define UNDERLINED		"\033[4m"
#define UNDERLINED_OFF		"\024[4m"

#define exitatline printf(COLOR_BOLD  COLOR_BLUE "EXIT AT LINE %d\n" COLOR_OFF, __LINE__); exit(0);
#define printline printf(COLOR_BOLD  COLOR_RED "Line # %d  " COLOR_OFF, __LINE__);
#define linenum printf(COLOR_BOLD  COLOR_RED "-- File %s -- Func %s -- Line %d --\n" COLOR_OFF,__FILE__,  __func__, __LINE__);
//******************************************************************************************

#define PLAYVIEW_COL_ZERO 0

//~ #define VERBOSE
// MACRO TO SEND DEBUG INFO TO FILE
#define debugMsgMacro(msg) ({ printDebugMsg(msg, __LINE__, __FILE__, __func__, playviewVars); })

#define CONFIG_RESOURCE   "/playview/playview.conf"
#define GLADE_RESOURCE   "/playview/playview.glade"
#define CSS_RESOURCE   "/playview/playview.css"
#define HELP_RESOURCE   "/playview/playview.help"

// DEFINE THE SIMPLE MSG DIALOG MODES
#define 	ERROR_MSG_MODE 1  // error dialog
#define 	NOTIFY_MSG_MODE 2  // notify dialog

// DEFINE SIMPLE MSG DIALOG TIMEOUTS
#define 	NO_TIMEOUT  0
#define 	FIVE_SEC_TIMEOUT  5
#define 	TEN_SEC_TIMEOUT  10

gint globalVerbose = 0; // THIS GLOBAL VARIABLE IS INTENED TO PRINT SOME DEBUG INFO

// *** DEFINE app_variables STRUCT ***
typedef struct {
    GtkWidget *w_lblMsgInfo;
    GtkWidget *w_lblExit;
//////////////////////////////////////////////////////////
    GtkWidget *w_btnExit;
    GtkWidget *w_scrollHelp;

    GtkWidget *w_tv_scrollwin_playview;
    GtkWidget *w_btnBox;
    GtkWidget *w_btnHelp;
    GtkWidget *w_btnPlay;
    GtkWidget *w_btnPause;
    GtkWidget *w_btnStop;

    GtkLabel *w_lblHelp;
    GtkLabel *w_lblStatus;

    GtkWidget *w_lblHostname;
    GtkWidget *w_box_InfoLabels;
    GtkWidget   *w_box_0;
    GtkWidget *w_window_main;
    GtkTreeView *w_playviewTreeView;
    GtkListStore *w_playviewListStore;
    gint playviewFirstPass;
    gint fileErr;

    gchar *mpdHostname;
    gint   mpdPortnumber;
    gint   mpdTimeout;
    gint winHeight;
    gint winWidth;
//////////////////////////////////////////////////////////
    gchar *cwdPath;
    gchar *gladeFile;
    gchar *cssFile;
    gchar *helpFile;
    gchar *cfgFile;
	gchar *simpleMsgDialog;
    gchar *windowTitle;

	gint iconSize;
	GtkImage *w_helpBtnImg;
	GtkImage *w_playBtnImg;
	GtkImage *w_pauseBtnImg;
	GtkImage *w_stopBtnImg;
	GtkImage *w_exitBtnImg;

	gchar *dialogFont;

	gint notifyDialogTimeout;
	gboolean showHideButtons;
	guint queueLengthStatus;
    gint mpdPlayerStateStatus;
    gint startUpFirstPass;
} app_variables;

gboolean showViewTimer(app_variables *playviewVars);
gboolean window_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *playviewVars);
int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *playviewVars);
int set_mpd_status_color(app_variables *playviewVars);
void signal_callback_sigFault(int signum);
struct mpd_connection *get_connection(app_variables *playviewVars, gchar *fileName, const gchar *funcName, int lineNum);
void setSignals(app_variables *playviewVars);
void showErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func, app_variables *playviewVars);
int playView (app_variables *playviewVars);
gboolean playview_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *playviewVars);
void unmap_playviewListView_signal(GtkWidget *widget, app_variables *playviewVars);
gboolean popup_menu_event_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *playviewVars);
void view_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *playviewVars);
void toggle_mpd_output(GtkWidget *menuItem, app_variables *playviewVars);
void startPlayer(GtkMenuItem *menuItem, app_variables *playviewVars);
void pausePlayer(GtkMenuItem *menuItem, app_variables *playviewVars);
void stopPlayer(GtkMenuItem *menuItem, app_variables *playviewVars);
void addPlaylist_signal	(GtkWidget *addPlaylistMi, app_variables *playviewVars);
void replacePlaylist_signal	(GtkWidget *replacePlaylistMi, app_variables *playviewVars);
void clearPlaylist_signal	(GtkWidget *clearPlaylistMi, app_variables *playviewVars);
void helpMe_signal(GtkWidget *helpBtn, app_variables *playviewVars);
//void stopStartPlayer(GtkWidget *btnStopStart, app_variables *playviewVars);
gchar * versionString(void);

gchar * versionString_Status(void);

int checkKeyPathFile(gchar *keyName, gint atLine, gchar *fileName, app_variables *playviewVars);

void getConfigKeys(app_variables *playviewVars);

void showNotify (gchar *msg,  app_variables *playviewVars);

gboolean destroyNotifyDialog(app_variables *playviewVars);

void getTheOptions(int argc, gchar **argv, app_variables *playviewVars);

void initilizeGladeWidgets(app_variables *playviewVars);

void loadCCSProvider(app_variables *playviewVars);

int spawnSimpleMessageDialog(gchar *msg, gint line, gchar *file, const gchar *function, gint mode, app_variables *playviewVars);


