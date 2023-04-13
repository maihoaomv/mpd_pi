// *** mpdConnection.c ***

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

// ********** GET A MPD CONNECTON ********
struct mpd_connection *get_connection(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    struct mpd_connection *conn = mpd_connection_new((const char *)mpd_piVars->mpdHostname,  mpd_piVars->mpdPortnumber, mpd_piVars->mpdTimeout);
    // A POSSIBILITY TO PIPE BETWEEN CHILD AND PARENT
    //https://www.codeproject.com/questions/746339/two-way-communication-between-child-and-parent-pro
    // IF CONNECTION SUCESSFUL, RETURN CONNECTION
    if(mpd_connection_get_error(conn) == MPD_ERROR_SUCCESS)
        return conn;

    // IF CONNECTION FAILED, -ALLOW UP TO, CURRENTLY 10, RETRIES BEFORE EXITING
    // THIS MAY TAKE AS LITTLE AS 1 SECOND, OR A MINUTE OR MORE BETWEEN ATTEMPS DEPENDING ON NETWORK STATUS
    //~ if(conn){ mpd_connection_free(conn);    conn = NULL; }
    mpd_connection_free(conn);
    conn = NULL;

    GPid child_pid = 0;

    // SPAWN THE ERR MSG DIALOG
    gchar *tmpStr = g_strdup_printf("\\tMPD Server -- %s --\nFailed to establish connection to server", mpd_piVars->mpdHostname);

    // USE ASYNCHRONOUS CHILD MODE TO SEND ERROR NOTIFICATION WITH A ZERO TIMEOUT SO IT DOESN'T HANG THE THIS FUNCTION
    // THIS SPAWN RETURNS THE SPAWN'D DIALOG CHILD PID SO THE DIALOG CAN BE TERMINATED IF attemptReConnectION IS SUCCESSFUL
    // attemptReConnectION TIMEOUT IS SET FOR 10 REINTERATIONS (AT MINIMUM 10 SECONDS) BEFORE GIVING UP
    child_pid = messageDialog(tmpStr,  __LINE__, __FILE__, __func__, CHILD_ERR_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);

    g_free(tmpStr);

    conn = attemptReConnect(conn, mpd_piVars);

    if(conn == NULL)
    {
        kill(child_pid, SIGTERM);   // KILL THE ERR MSG DIALOG
        exit(0);
        messageDialog ("MPD connection re-established", __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE, FIVE_SEC_TIMEOUT, false, mpd_piVars);
    }
    kill(child_pid, SIGTERM);   // KILL THE ERR MSG DIALO
    return conn;
    //~ // THIS RETURN IS NOT REACHABLE, BUT IT SURPRESSES THE NO RETURN WARNING
    //~ return NULL;
}  // ********** End of GET A MPD CONNECTON ********


struct mpd_connection *attemptReConnect(struct mpd_connection *conn, app_variables *mpd_piVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // START THE RETRY COUNT DOWN
    gint retryCount = 1;

    do
    {
        fprintf(stderr, "Retry Count = %d\n", retryCount);
        conn = mpd_connection_new((const char *)mpd_piVars->mpdHostname,  mpd_piVars->mpdPortnumber, mpd_piVars->mpdTimeout);
        g_usleep (1000000); // SLEEP 1 SECOND BEFORE RETRY

        // IF THE RECONNECT IS SUCESSFUL, RETURN CONNECTION
        if(mpd_connection_get_error(conn) == MPD_ERROR_SUCCESS)
        {   //  printf("CHILD PID %d\n", child_pid);
            //~ kill(child_pid, SIGTERM);   // KILL THE ERR MSG DIALOG

            return conn; // AND RETURN THE CONNECTION
        }
        // ELSE TRY AGAIN TO CONNECT UNTIL WE RUN OUT OF RETRIES
        if(conn) mpd_connection_free(conn);
        retryCount ++;
        if(retryCount > CONN_ERR_TIMEOUT)
        {   //  printf("CHILD PID %d\n", child_pid);
            gint atLine = __LINE__ - 2;
            // IF WE'VE RAN OUT OF RETRIES, AND UNABLE TO RE-ESTABLISH THE CONNECTION POST AN ERROR
            gchar *str = g_strdup_printf("Connection to MPD server -- %s -- has failed.", mpd_piVars->mpdHostname);

            messageDialog (str, atLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, false, mpd_piVars);

            g_free(str);
            return NULL;
        }
    } while(1);
}
