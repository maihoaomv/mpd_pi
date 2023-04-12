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
// *** dataview.h ***

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
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
//#include <stdlib.h>
#include <getopt.h>
// MACRO TO SEND DEBUG INFO TO FILE
#define debugMsgMacro(msg) ({ printDebugMsg(msg, __LINE__, __FILE__, __func__, dataviewVars); })

//~ // THIS MACRO WILL EXIT 0 AND PRINT THE LINE NUMBER HOPEFULLY TO MAKE THEM EAISER TO FIND IN THE CODE
//~ #define exitAtLine printf("EXIT AT LINE %d\n", __LINE__); exit(0);

#define CONFIG_RESOURCE   "/dataview/dataview.conf"
#define GLADE_RESOURCE   "/dataview/dataview.glade"
#define CSS_RESOURCE   "/dataview/dataview.css"
#define HELP_RESOURCE   "/dataview/dataview.help"

#define TOP_FOLDER_ICON_RESOURCE "/dataview/FolderIcon.png"
#define SUB_FOLDER_ICON_RESOURCE "/dataview/SubFolderIcon.png"
#define TRACK_ICON_RESOURCE "/dataview/TrackIcon.png"

#define STOCK_TOP_FOLDER_ICON		"gnome-fs-home"
#define STOCK_SUB_FOLDER_ICON	"folder-music"
#define STOCK_TRACK_ICON				"gnome-music"


//////////////////////////////////////////////////////////////////////////
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

#define linenum printf(COLOR_BOLD  COLOR_RED "-- File %s -- Func %s -- Line %d --\n" COLOR_OFF,__FILE__,  __func__, __LINE__);

//////////////////////////////////////////////////////////////////////////
gint globalVerbose = 0; // THIS GLOBAL VARIABLE IS INTENED TO PRINT SOME DEBUG INFO

// SELECT TRACKS COLUMNS
enum
{
    COL_TRKPIXBUF,
    COL_FOLDER,
    COL_TRKTRACK,
    COL_TRKDISC,
    COL_TRKTITLE,
    COL_TRKARTIST,
    COL_TRKALBUM,
    COL_TRKPATH
};


// DATA VIEW COLUMNS
enum
{
    COL_DBDIR,
    COL_DBPATH
};

// DRAG-N--DROP
//* CONVENIENCE ENUMERATOR TO TAG DATA TYPES */
enum { TARGET_STRING,	TARGET_INTEGER, TARGET_FLOAT};

static const GtkTargetEntry drag_targets = { "STRING", GTK_TARGET_SAME_APP, TARGET_STRING };

// *** DEFINE APP_VARIABLES STRUCT ***
typedef struct {
    GtkLabel *w_lblHelp;
    GtkWidget *w_scrollHelp;
    GtkWidget *w_lblHostname;
    //~ GtkWidget *w_box_InfoLabels;
    GtkWidget   *w_box_0;
    GtkWidget *w_window_main;
    //~ gint fileErr;
    gchar *mpdHostname;
    gint   mpdPortnumber;
    gint   mpdTimeout;
    gint winHeight;
    gint winWidth;
    gint panedPosition;

// ADD ICON FILES TO CONFIG FILE

	GdkPixbuf   *dir_Icon_pixBuf;
    GdkPixbuf   *sub_dir_Icon_pixBuf;
    GdkPixbuf  	*trk_Icon_pixBuf;

    gchar *cwdPath;
    gchar *gladeFile;
    gchar *cssFile;
    gchar *cfgFile;
    //~ gchar *helpFile;
    guint queueLengthStatus;
    gint mpdPlayerStateStatus;
    gint startUpFirstPass;

    unsigned db_number_of_artists;
    unsigned db_number_of_albums;
    unsigned db_number_of_songs;
    unsigned long db_update_time;
    unsigned long db_play_time;
    gint dataviewFirstPass;
    gint selTrkFirstPass;
    GtkTreeStore   *w_dataviewTreeStore;
    GtkTreeView    *w_dataviewTreeView;
    GtkTreeStore 	*w_selTrkTreeStore;
    GtkTreeView 	*w_selTrkTreeView;
    //~ GtkCellRendererText *w_dataview_selTrkTrackTxtRender;
    GtkPaned *w_dataview_Pane;
    //~ GtkPaned *w_data_play_Pane;
    //~ GtkPaned *w_play_track_Pane;

    gint   iconSize;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gchar *windowTitle;
    gchar *dialogFont;
    gint notifyDialogTimeout;
	//~ gboolean showHideButtons;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
} app_variables;


/////////////////////////////////////////////////////////////////////////////////////////////

// DEFINE SIG FAULT AND DEBUG FILE AS GLOBALS
const gchar *globalSigFaultFile;

const gchar *globalDebugFile;

gboolean showViewTimer(app_variables *dataviewVars);

int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *dataviewVars);

int set_mpd_status_color(app_variables *dataviewVars);

void signal_callback_sigFault(int signum);

struct mpd_connection *get_connection(app_variables *dataviewVars, gchar *fileName, const gchar *funcName, int lineNum);

void setSignals(app_variables *dataviewVars);

void showErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func, app_variables *dataviewVars);

gboolean dataview_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *dataviewVars);

gboolean selTrack_popupMenu(GdkEventButton *event, app_variables *dataviewVars);

void selTrk_addTracks(GtkMenuItem *menuItem, app_variables *dataviewVars);

void addTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars);

void addTracksStop(GtkMenuItem *menuItem, app_variables *dataviewVars);

void replaceTracksStop(GtkMenuItem *menuItem, app_variables *dataviewVars);

void replaceTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars);

void dataview_clearQueue(GtkMenuItem *menuItem, app_variables *dataviewVars);

gboolean dataview_selTrk_onButtonPressed (GtkWidget *widget, GdkEventButton *event, app_variables *dataviewVars);

gboolean selTrack_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *dataviewVars);

gboolean directory_popupMenu(GdkEventButton *event, app_variables *dataviewVars);

void addDirToSelTrk(GtkMenuItem *menuItem, app_variables *dataviewVars);

void addDirToMpdQue(GtkMenuItem *menuItem, app_variables *dataviewVars);

void upDateDatabase(GtkMenuItem *menuItem, app_variables *dataviewVars);

//gboolean turnOffMsg(app_variables *dataviewVars);

gboolean dataview_directory_onButtonPressed (GtkWidget *widget, GdkEventButton *event, app_variables *dataviewVars);

gboolean dataview_directory_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *dataviewVars);

void clearAllRows_signal(GtkMenuItem *menuItem, app_variables *dataviewVars);

gboolean dataView(app_variables *dataviewVars);

void removeSelectedRows_signal(GtkMenuItem *menuItem, app_variables *dataviewVars);

void dataview_view_selected_foreach_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer conn);

gboolean window_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *dataviewVars);

gboolean main_popup_menu_event_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *dataviewVars);

void window_main_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *dataviewVars);

void stopPlayer(GtkMenuItem *menuItem, app_variables *dataviewVars);

void pausePlayer(GtkMenuItem *menuItem, app_variables *dataviewVars);

void startPlayer(GtkMenuItem *menuItem, app_variables *dataviewVars);

void toggle_mpd_output(GtkWidget *menuItem, app_variables *dataviewVars);

void clearPlaylist_signal	(GtkWidget *clearPlaylistMi, app_variables *dataviewVars);

void directory_replaceTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars);

gboolean selTrkGrab_signal(GtkWidget *tvWidget, app_variables *dataviewVars);

gboolean dataBaseGrab_signal(GtkWidget *tvWidget, app_variables *dataviewVars);

void helpMe_signal(GtkWidget *helpBtn, app_variables *dataviewVars);

void on_drag_data_get(	GtkWidget *widget, GdkDragContext *drag_context,
                        GtkSelectionData *sdata, guint info, guint time,
                        app_variables *dataviewVars);

void on_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context,
                           gint x, gint y, GtkSelectionData *sdata, guint info,
                           guint time, app_variables *dataviewVars);

gchar * versionString(void);

//~ void playButton_signal();

//~ void pauseButton_signal();

//~ void stopButton_signal();


void getTheOptions(int argc, gchar **argv, app_variables *dataviewVars);

void getConfigKeys(app_variables *dataviewVars);

void initilizeGladeWidgets(app_variables *dataviewVars);

void loadCCSProvider(app_variables *dataviewVars);

//~ void showNotifyDialog (gchar *notifyMsg, app_variables *dataviewVars);
void showNotify (gchar *notifyMsg, app_variables *dataviewVars);

gboolean destroyNotifyDialog(app_variables *dataviewVars);

//~ gboolean destroyNotifyDialog(GtkWidget *dialog);

int checkKeyPathFile(gchar *keyName, gint atLine, gchar *fileName, app_variables *dataviewVars);

void	expandAllRows(GtkMenuItem *menuitem, GtkTreeView *tvWidget);

void	expandSelectedRow(GtkMenuItem *menuitem, GtkTreeView *tvWidget);

void collapseAllRows(GtkMenuItem *menuitem, GtkTreeView *tvWidget);

void collapseSelectedRow(GtkMenuItem *menuitem, GtkTreeView *tvWidget);

void dragANDdrop(app_variables *dataviewVars);

// ****** END-OF-DECLARATIONS ****** //

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////