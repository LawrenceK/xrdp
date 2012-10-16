/*
   Copyright (c) 2005-2012 Jay Sorg

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

*/

#ifndef LOG_H
#define LOG_H

#include <pthread.h>

#include "arch.h"

/* logging buffer size */
#define LOG_BUFFER_SIZE      1024

/* logging levels */
enum logLevels
{
  LOG_LEVEL_ALWAYS = 0,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_INFO,
  LOG_LEVEL_DEBUG
};

/* startup return values */
enum logReturns
{
  LOG_STARTUP_OK = 0,
  LOG_ERROR_MALLOC,
  LOG_ERROR_NULL_FILE,
  LOG_ERROR_FILE_OPEN,
  LOG_ERROR_NO_CFG,
  LOG_ERROR_FILE_NOT_OPEN,
  LOG_GENERAL_ERROR
};

#define SESMAN_CFG_LOGGING           "Logging"
#define SESMAN_CFG_LOG_FILE          "LogFile"
#define SESMAN_CFG_LOG_LEVEL         "LogLevel"
#define SESMAN_CFG_LOG_ENABLE_SYSLOG "EnableSyslog"
#define SESMAN_CFG_LOG_SYSLOG_LEVEL  "SyslogLevel"

/* enable threading */
/*#define LOG_ENABLE_THREAD*/

#ifdef DEBUG
#define LOG_DBG(args...) log_message(LOG_LEVEL_DEBUG, args);
#else
#define LOG_DBG(args...)
#endif

struct log_config
{
  char* program_name;
  char* log_file;
  int fd;
  unsigned int log_level;
  int enable_syslog;
  unsigned int syslog_level;
  pthread_mutex_t log_lock;
  pthread_mutexattr_t log_lock_attr;
};

/* internal functions, only used in log.c if this ifdef is defined.*/
#ifdef LOGINTERNALSTUFF

/**
 *
 * @brief Starts the logging subsystem
 * @param l_cfg loggging system configuration
 * @return
 *
 */
enum logReturns DEFAULT_CC
internal_log_start(struct log_config* l_cfg);

/**
 *
 * @brief Shuts down the logging subsystem
 * @param l_cfg pointer to the logging subsystem to stop
 *
 */
enum logReturns DEFAULT_CC
internal_log_end(struct log_config* l_cfg);

/**
 * Converts a log level to a string
 * @param lvl, the loglevel
 * @param str pointer where the string will be stored.
 */
void DEFAULT_CC
internal_log_lvl2str(const enum logLevels lvl, char* str);

/**
 *
 * @brief Converts a string to a log level
 * @param s The string to convert
 * @return The corresponding level or LOG_LEVEL_DEBUG if error
 *
 */
enum logLevels DEFAULT_CC
internal_log_text2level(char* s);

/**
 * A function that init our struct that holds all state and
 * also init its content.
 * @return  LOG_STARTUP_OK or LOG_ERROR_MALLOC
 */
enum logReturns DEFAULT_CC
internalInitAndAllocStruct(void);

/**
 * Read configuration from a file and store the values in lists.
 * @param file
 * @param lc
 * @param param_n
 * @param param_v
 * @param applicationName, the application name used in the log events.
 * @return
 */
enum logReturns DEFAULT_CC
internal_config_read_logging(int file, struct log_config* lc,
                             struct list* param_n,
                             struct list* param_v,
                             const char *applicationName);
/*End of internal functions*/
#endif
/**
 * This function initialize the log facilities according to the configuration
 * file, that is described by the in parameter.
 * @param iniFile
 * @param applicationName, the name that is used in the log for the running application
 * @return LOG_STARTUP_OK on success
 */
enum logReturns DEFAULT_CC
log_start(const char* iniFile, const char* applicationName);

/**
 * An alternative log_start where the caller gives the params directly.
 * @param iniParams
 * @return
 */
enum logReturns DEFAULT_CC
log_start_from_param(const struct log_config *iniParams);
/**
 * Function that terminates all logging
 * @return
 */
enum logReturns DEFAULT_CC
log_end(void);

/**
 * the log function that all files use to log an event.
 * @param lvl, the loglevel
 * @param msg, the logtext.
 * @param ...
 * @return
 */
enum logReturns DEFAULT_CC
log_message(const enum logLevels lvl, const char* msg, ...);

/**
 *
 * @brief Reads configuration
 * @param s translates the strings "1", "true" and "yes" in 1 (true) and
 * other strings in 0
 * @return 0 on success, 1 on failure
 *
 */
int APP_CC text2bool(char* s);

/**
 * This function returns the configured file name for the logfile
 * @param replybuf the buffer where the reply is stored
 * @param bufsize how big is the reply buffer.
 * @return
 */
char *getLogFile(char *replybuf, int bufsize);
#endif
