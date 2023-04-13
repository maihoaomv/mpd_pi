// mpd_pi.h

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

//#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
//#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <math.h>

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gstdio.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>

#include <syslog.h>
#include <execinfo.h>

#include <sys/time.h>
#include <time.h>

// AUTO VERSIONING
//~ #include "mpd_pi-version.h"

// THESE COLOR MACRO'S WILL HOPEFULLY MAKE IT EAISER TO DEBUG THE CODE
// https://talyian.github.io/ansicolors/

#define COLOR_BOLD_SLOW_BLINKING      "\e[1;5m"
#define COLOR_BOLD_BLINKING_OFF      "\e[1;25m"
#define COLOR_BOLD_SLOW_BLINKING_RED  "\e[1;5;31m"
#define COLOR_BOLD_SLOW_BLINKING_BLUE  "\e[1;5;34m"
#define COLOR_BOLD_SLOW_BLINKING_BLACK  "\e[1;5;42m"

#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_WHITE	"\033[37m"

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define UNDERLINED		"\033[4m"

#define exitatline printf(COLOR_BOLD  COLOR_BLUE "EXIT AT LINE %d -- %s -- %s\n" COLOR_OFF, __LINE__,__FILE__,  __func__); exit(0);
#define printline printf(COLOR_BOLD_SLOW_BLINKING COLOR_BOLD  COLOR_BLUE "Line # %d  \n" COLOR_OFF, __LINE__);
//~ #define printline printf(COLOR_BOLD_SLOW_BLINKING_RED "Line # %d  " COLOR_OFF, __LINE__);
#define linenum printf(COLOR_BOLD  COLOR_RED "-- File %s -- Func %s -- Line %d --\n" COLOR_OFF,__FILE__,  __func__, __LINE__);

//~ # \033[1;31m           # "red"
//~ # \033[32m           # "green"
//~ # \033[33m           # "yellow"
//~ # \033[34m           # "blue"
//~ # \033[36m           # "cyan"
//~ # \033[1;33m         # "yellow bold"

//~ COLOR_BOLD_SLOW_BLINKING_RED "hello " COLOR_OFF
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//~ // THIS MACRO WILL EXIT 0 AND PRINT THE LINE NUMBER HOPEFULLY TO MAKE THEM EAISER TO FIND IN THE CODE
//~ #define exitAtLine printf("EXIT AT LINE %d\n", __LINE__); exit(0);

// THESE ARE THE PLAYER AND TITLEBAR ICONS IN RESOURCES

#define SOURCE_PATH                        "/resources"
#define TITLEBAR_ICON_RESOURCE    "/resources/mpd_pi-titlebar-icon.svg"

#define PLAY_ICON_RESOURCE            "/resources/PlayIcon-64x64.png"
#define PAUSE_ICON_RESOURCE         "/resources/PauseIcon-64x64.png"
#define STOP_ICON_RESOURCE           "/resources/StopIcon-64x64.png"
#define REWIND_ICON_RESOURCE      "/resources/RewindIcon-64x64.png"
#define NEXT_ICON_RESOURCE           "/resources/NextIcon-64x64.png"
#define FORWARD_ICON_RESOURCE   "/resources/ForwardIcon-64x64.png"
#define PREVIOUS_ICON_RESOURCE   "/resources/PreviousIcon-64x64.png"

#define CONFIG_RESOURCE 				"/resources/mpd_pi.conf"
#define HELP_RESOURCE 					"/resources/mpd_pi.help"
#define GLADE_RESOURCE 				"/resources/mpd_pi.glade"
#define CSS_RESOURCE 						"/resources/mpd_pi.css"

// THESE ARE THE VARIOUS DIRECTORIES/FILENAMES USED WITH THIS PROGRAM
#define CONFIG_FILES_DIR					"CONFIG_FILES"
#define HELPER_FILES_DIR					"HELPER_FILES"

#define MSG_DIALOG_FILE	"simpleMsgDialog"
//~ #define HELPER_PLAYVIEW_FILE			"HELPER_FILES/playview"
//~ #define HELPER_DATAVIEW_FILE			"HELPER_FILES/dataview"

#define SIG_FAULT_FILE 	g_strdup_printf("%s/%s.sigfault",g_get_tmp_dir (), g_get_prgname ())

// DEFINE THE SIMPLE MSG DIALOG MODES
#define 	ERROR_MSG_MODE 1  // error dialog
#define 	NOTIFY_MSG_MODE 2  // notify dialog
#define 	QUESTION_MSG_MODE 3  //question dialog
#define 	INFORMATION_MSG_MODE 4  //question dialog
#define	CHILD_ERR_MSG_MODE 5  // special err msg mode -returns child pid

// DEFINE SIMPLE MSG DIALOG TIMEOUTS
#define 	DEFAULT_TIMEOUT  0
#define 	FIVE_SEC_TIMEOUT  5
#define 	TEN_SEC_TIMEOUT  10

// THESE ARE THE SIMPLE MSG DIALOG CSS AND GLADE FAIL CODES.
#define        SIMPLE_MSG_DIALOG_GLADE_FAILED  		64  // decimal
#define        SIMPLE_MSG_DIALOG_CSS_FAILED  			65  // decimal


#define VOLUME_MUTE 0   // SET VOLUME MUTE TO ZERO

#define CONN_ERR_TIMEOUT 10 // MPD connection timeout to get a reconnection

#define MPD_ADJUST 5 // +/- percentage to adjust the mpd volume in incremental steps

// MPD FLAGS
enum {
    MPD_PI_FLAG_PREVIOUS,
    MPD_PI_FLAG_VOLMUTE,
    MPD_PI_FLAG_VOLUP,
    MPD_PI_FLAG_VOLDN,
    MPD_PI_FLAG_REWIND,
    MPD_PI_FLAG_FORWARD,
    MPD_PI_FLAG_STOP,
    MPD_PI_FLAG_PLAY,
    MPD_PI_FLAG_PAUSE,
    MPD_PI_FLAG_NEXT,
    MPD_PI_FLAG_ELASPED_INC,
    MPD_PI_FLAG_ELASPED_DEC,
    MPD_PI_FLAG_QUEUE_INC,
    MPD_PI_FLAG_QUEUE_DEC,
    MPD_PI_FLAG_TRACK_FIRST,
    MPD_PI_FLAG_TRACK_LAST,
    MPD_PI_FLAG_ZERO_TRACK
};

// ENUMLATE THE TRACKVIEW COLUMNS AS 0,1,2,3,4,5,6
enum
{
    TRACKVIEW_COL_QUEUE = 0,
    TRACKVIEW_COL_DISC = 1,
    TRACKVIEW_COL_TRACK = 2,
    TRACKVIEW_COL_TIME = 3,
    TRACKVIEW_COL_TITLE = 4,
    TRACKVIEW_COL_ARTIST = 5,
    TRACKVIEW_COL_ALBUM = 6,
    TRACKVIEW_COL_PATH_FILENAME = 7,
    TRACKVIEW_COL_ROW_FONT = 8,
    TRACKVIEW_COL_ROW_COLOR = 9,
    TRACKVIEW_COL_SORT_REORDER = 10
};

// SHOW SONGVIEW /  TRACKVIEW MODE
enum {
    SHOW_SONGVIEW_MODE = 0,
    SHOW_TRACKVIEW_MODE = 1
};

// THIS IS A MACRO TO SEND ERROR INFO TO THE ERR MESSAGE DIALOG POPUP
// MAY BE USED AS SYNCHRONOUS (TRUE) OR AS ASYNCHRONOUS (FALSE)
// IT DEFAULTS TO SYNCHRONOUS SPAWN, AND DOES NOT AUTO-CLOSE
// NOTE: IT ALSO SENDS THE SAME MESSAGE TO THE DEBUG MSG FILE IN /tmp AND TO STDERR
#define errMsgMacro(msg) ({ messageDialog (msg, __LINE__, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars); })

// *** DEFINE app_variables STRUCT ***
typedef struct {
// WIDGETS

    GtkBox *w_box_0; // master box -contains all the following
    GtkBox *w_boxButtons; // master box -contains all the following

    GtkBox *w_boxTrackview; // trackview display

    GtkBox *w_btnBoxPlayerControls; // holds the player buttons
    GtkBox *w_boxProgressBars;  // progress bars
    GtkBox *w_boxFlipHorzVertBP;  // Buttons/Progress Bars

    GtkBox *w_boxSongview; // songview display
    GtkBox *w_box_for_SongviewLabel; // songview display

    GtkEventBox *w_eventBoxProgressBarVolume;
    GtkEventBox *w_eventBoxProgressBarTime;
    GtkEventBox *w_eventBoxProgressBarQueue;

    GtkProgressBar *w_progressBarVolume;  	// volume progress bar display
    GtkProgressBar *w_progressBarTime;	//  track play elasped, total, remaining time progress bar display
    GtkProgressBar *w_progressBarQueue;	// track queue progress bar display

    GtkButton *w_btnNext; // next track button
    GtkButton *w_btnForward; // next track button
    GtkButton *w_btnPause; // pause button
    GtkButton *w_btnPlay; // play button
    GtkButton *w_btnPrevious; // previous button
    GtkButton *w_btnStop; // stop button
    GtkButton *w_btnRewind; // reset track to time zero

    GtkWindow *w_window_main; // the main gui window itself
    GtkTreeView *w_trackviewTreeView; // treeview display
    GtkListStore *w_trackviewListStore;  // treeview list store
    GtkTreeModelSort *w_trackViewModelSort; // treeview sort model


    gint trackSkipTime; // NUMBER OF SECONDS TO SKIP TRACK FORWARD WHEN (SKIP? FORWARD?) BUTTON IS PRESSED

// MPD HOST SETTINGS
    gchar *mpdHostname;   // DEFAULT HOST NAME (LOCALHOST)
    gint   mpdPortnumber; // DEFAULT PORT NUMBER
    gint   mpdTimeout;    // DEFAULT TIMEOUT

    gchar *cwdPath;

    gchar *cfgFile;
    gchar *userDefaultCfgFile;
    gchar *gladeFile;
    gchar *cssFile;
    gchar *helpFile;

    gchar *msgDialogFile;
    gchar *msgNotifySendFile;
    gchar *playviewDialogFile;
    gchar *dataviewDialogFile;

    gchar *windowFont;

    gchar *debugFile;  // WRITE DEBUG INFO TO TEMP (/TMP)

// ENABLE CENTER TRACK IN TRACKVIEW
    gboolean centerTrackEnable;

    gint showViewMode;
// HIGHLIGHT THE CURRENT PLAYING TRACK IN THE TREEVIEW
    gchar *trackviewHiLiteColor;

// ADDED NEW STUFF HERE
// THESE ARE THE STATUS VARIABLES
    gint volumeStatus;
    guint elaspedTimeStatus;
    guint totalTimeStatus;
    guint remainingTimeStatus;

    guint queueLengthStatus;
    guint queueVersionStatus;
    gint mpdPlayerStateStatus;

    gint currentSongPos;
    gint prevSongPos;

    gint trackviewFirstPass;

// KEY FILE VARIABLES
    gchar *playIconFile;
    gchar *pauseIconFile;
    gchar *stopIconFile;
    gchar *nextIconFile;
    gchar *forwardIconFile;
    gchar *previousIconFile;
    gchar *rewindIconFile;

    gint   playerIconSize;

    gint ShowPlayerControls;

    gint ShowProgressDisplay;

    gint ShowNotification;

    gchar *WindowTitle;
    gchar *GuiStartUp;
    gchar *titleBarIconFile;

    gint windowHeight;
    gint windowWidth;

    gint playviewHeight;
    gint playviewWidth;

    gint dataviewHeight;
    gint dataviewWidth;

    gint flipMode;  // widget ordering of the widgets in the boxFlipHorzVertBP box and box_0

// SONGVIEWGLADE VARIABLES
    GtkLabel *w_lblSongName;
    GtkLabel *w_lblSongTitle;
    GtkLabel *w_lblSongArtist;
    GtkLabel *w_lblSongAlbumArtist;
    GtkLabel *w_lblSongAlbum;
    GtkLabel *w_lblSongDiskTrack;
    GtkLabel *w_lblSongURI;

} app_variables;
//************************

// *** DEFINE THE FUNCTIONS ***
gboolean showViewTimer(app_variables *mpd_piVars);

gboolean window_main_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *mpd_piVars);

gchar * convertTimeToString(int sec);

int changePlayerStatus(app_variables *mpd_piVars, int data, int bitFlag);

void setConfigVariables(app_variables *mpd_piVars);

void view_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars);

gboolean gui_button_press_event (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars);

int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *mpd_piVars);

void volume_scroll_event  (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

void queue_scroll_event   (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

void elasped_scroll_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

void volumePercent_click_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

int setMpdPlayerState(app_variables *mpd_piVars);

void setStatusDisplay(app_variables *mpd_piVars);

gboolean trackview_onRowActivated_signal (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *mpd_piVars);

void trackView(app_variables *mpd_piVars);

gboolean window_main_button_press_event (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars);

gboolean trackview_onButtonPressed_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars);

void signal_callback_sigFault(int signum);

void getKeyVars(app_variables *mpd_piVars);

void mode_repeat(GtkWidget *window_main, app_variables *mpd_piVars);

void mode_random(GtkWidget *window_main, app_variables *mpd_piVars);

void mode_single(GtkWidget *window_main, app_variables *mpd_piVars);

void mode_consume(GtkWidget *window_main, app_variables *mpd_piVars);

void setFullscreen(GtkWidget *fullscreenMi, app_variables *mpd_piVars);

void setNormal(GtkWidget *normalMi, app_variables *mpd_piVars);

void setMaximize(GtkWidget *maximizeMi, app_variables *mpd_piVars);

void setMinimize(GtkWidget *minimizeMi, app_variables *mpd_piVars);

void toggle_mpd_output(GtkWidget *menuItem, app_variables *mpd_piVars);

gboolean centerTrackClickEvent (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

void trackTime_click_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars);

gboolean getMpdStatus(app_variables *mpd_piVars);

struct mpd_connection *get_connection(app_variables *mpd_piVars);

void popupMenu_startPlayer(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_stopPlayer(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void toggle_PlayerDisplay	(GtkWidget *playerControlsMi, app_variables *mpd_piVars);

void setSignals(app_variables *mpd_piVars);

gint trueFalse(gchar *keyString, gchar *rtnString, gint atKeyLine,  gchar *file, const gchar * function, app_variables *mpd_piVars);

void toggle_ProgressDisplay	(GtkWidget *statusDisplayMi, app_variables *mpd_piVars);

//~ int createTheConfigFile(app_variables *mpd_piVars);

void spawn_PlayviewDialog(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void spawn_DataviewDialog(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void upDateDatabase(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_nextTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_previousTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_restartTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_incrementQueue(GtkMenuItem *menuItem, app_variables *mpd_piVars);

void popupMenu_decrementQueue(GtkMenuItem *menuItem, app_variables *mpd_piVars);

gchar * versionString(void);

int messageDialog(gchar *msg, gint line, gchar *file, const gchar *function, gint mode, gint timeout, gboolean sync, app_variables *mpd_piVars);

void showStatusMessage(app_variables *mpd_piVars);

void showHelpMessage (app_variables *mpd_piVars);

void getCmdLineOptions(int argc, gchar **argv, app_variables *mpd_piVars);

void spawnPlayviewDialog(app_variables *mpd_piVars);

void spawnDataviewDialog(app_variables *mpd_piVars);

//~ int checkKeyPathFile(gchar *keyName, gchar *fileName, app_variables *mpd_piVars, gint atLineNumber, const gchar *atFunction, gchar *atFile);

void btnNext_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnForward_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnStop_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnPause_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnPlay_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnPrevious_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void btnRewind_clicked(GtkWidget *widget, app_variables *mpd_piVars);

void timeStr(gchar **rtnStr, unsigned long timeInSeconds);

char * append_string(char * string, const char * format, const char delim, const char * s);

void notifyThread(app_variables *mpd_piVars);

void initilazeGladeBuilder(app_variables *mpd_piVars);

gboolean helpStatusClickEvent(GtkWidget *widget, GdkEventButton  *event, app_variables *mpd_piVars);

//~ void popupStatusMessage(GtkMenuItem *menuitem, app_variables *mpd_piVars);

void popupHelpMessage(GtkMenuItem *menuitem, app_variables *mpd_piVars);

void createKeyFile(GtkMenuItem *menuitem, app_variables *mpd_piVars);

void idleThread(app_variables *mpd_piVars);

void loadCssProvider(app_variables *mpd_piVars);

void highLightTrack (app_variables *mpd_piVars);

struct mpd_connection *attemptReConnect(struct mpd_connection *conn, app_variables *mpd_piVars);

void trackViewModelSort_sort_column_changed_cb(GtkTreeSortable *sortable, app_variables *mpd_piVars);

void reorderWidgets(app_variables *mpd_piVars);

void bareMinimumErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func);

void setViewMode(app_variables *mpd_piVars);

gchar * setFileName(gchar *keyName, gchar *fileVariable, gint atLine, const gchar *atFunc, gchar *atFile, app_variables *mpd_piVars);

// SONGVIEW
gboolean setSongLabel(gchar *meta, GtkLabel *widget, gchar * name);

void songView(app_variables *mpd_piVars);

void setTheWidgets(gint rtn_widgetOrdering, app_variables *mpd_piVars);

void loadConfigVariablesToProvider(app_variables *mpd_piVars);

void getExternalFilenames(app_variables *mpd_piVars);


void toggleViewMode (GtkMenuItem *menuItem,  app_variables *mpd_piVars);

void setButtonImage(gchar *imageName, gchar *resourcePath, gchar *iconFile, GtkButton *buttonName, app_variables *mpd_piVars);

void DecimalToBinary(int dec);

void listResources();
