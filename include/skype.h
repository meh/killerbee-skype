/****************************************************************************
 * Copyleft meh. [http://meh.doesntexist.org | meh.ffff@gmail.com]          *
 *                                                                          *
 * This file is part of killerbee-skype.                                    *
 *                                                                          *
 * killerbee-skype is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU Affero General Public License as           *
 * published by the Free Software Foundation, either version 3 of the       *
 * License, or (at your option) any later version.                          *
 *                                                                          *
 * killerbee-skype is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU Affero General Public License for more details.                      *
 *                                                                          *
 * You should have received a copy of the GNU Affero General Public License *
 * along with killerbee-skype.  If not, see <http://www.gnu.org/licenses/>. *
 ****************************************************************************/

#ifndef _BITLBEE_SKYPE_PLUGIN
#define _BITLBEE_SKYPE_PLUGIN

#include "common.h"

enum {
    SKYPE_CALL_RINGING = 1,
    SKYPE_CALL_MISSED,
    SKYPE_CALL_CANCELLED,
    SKYPE_CALL_FINISHED,
    SKYPE_CALL_REFUSED
};

enum {
    SKYPE_FILETRANSFER_NEW = 1,
    SKYPE_FILETRANSFER_FAILED
};

typedef struct SkypeAwayState {
	char* code;
	char* fullName;
} SkypeAwayState;

#ifndef _BITLBEE_SKYPE_TYPESONLY
extern const SkypeAwayState SkypeAwayStateList[];
#endif

typedef struct SkypeData {
    im_connection* connection;

    int fd;
    int bfd;

    void* ssl;

    char* username;
} SkypeData;

/* The plugin entry point */
extern void init_plugin (void);

void Skype_init (account_t* account);

void Skype_login (account_t* account);

gboolean Skype_connected (gpointer data, void* source, b_input_condition condition);

gboolean Skype_begin (im_connection* connection);

int Skype_write (im_connection* connection, char* buffer, int length);

int Skype_printf (im_connection* connection, char* format, ...);

gboolean Skype_loop (gpointer data, gint fd, b_input_condition condition);

#endif
