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

#include <sys/select.h>
#include <bitlbee/ssl_client.h>

#define _BITLBEE_SKYPE_TYPESONLY
#include "skype.h"
#undef  _BITLBEE_SKYPE_TYPESONLY

#include "commands.h"
#include "parser.h"

#define SKYPED_DEFAULT_SERVER   "localhost"
#define SKYPED_DEFAULT_PORT     "2727"

#define IRC_LINE_SIZE   1024

const SkypeAwayState SkypeAwayStateList[] = {
	{ "AWAY",      "Away" },
	{ "NA",        "Not available" },
	{ "DND",       "Do Not Disturb" },
	{ "INVISIBLE", "Invisible" },
	{ "OFFLINE",   "Offline" },
	{ "SKYPEME",   "Skype Me" },
	{ "ONLINE",    "Online" },
	{ NULL, NULL}
};

void
init_plugin (void)
{
    struct prpl* protocol = g_new0(struct prpl, 1);

    protocol->name  = "skype";
    protocol->init  = Skype_init;
    protocol->login = Skype_login;

    register_protocol(protocol);
}

void
Skype_init (account_t* account)
{
    /* Settings */
    set_add(&account->set, "skypeout_offline", "true", set_eval_bool, account);

    set_add(&account->set, "show_moods", "false", set_eval_bool, account);

    set_add(&account->set, "edit_prefix", "EDIT: ", NULL, account);

    set_add(&account->set, "server", SKYPED_DEFAULT_SERVER, set_eval_account, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    set_add(&account->set, "port", SKYPED_DEFAULT_PORT, set_eval_int, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    set_add(&account->set, "skypeconsole", "false", set_eval_bool, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    set_add(&account->set, "skypeconsole_receive", "false", set_eval_bool, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    set_add(&account->set, "auto_join", "false", set_eval_bool, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    set_add(&account->set, "test_join", "false", set_eval_bool, account)->flags
        |= ACC_SET_OFFLINE_ONLY;

    /* Commands */
    set_add(&account->set, "display_name", NULL, Skype_cmd_display_name, account)->flags
        |= ACC_SET_NOSAVE | ACC_SET_ONLINE_ONLY;

    set_add(&account->set, "call", NULL, Skype_cmd_call, account)->flags
        |= ACC_SET_NOSAVE | ACC_SET_ONLINE_ONLY;

    set_add(&account->set, "balance", NULL, Skype_cmd_balance, account)->flags
        |= ACC_SET_NOSAVE | ACC_SET_ONLINE_ONLY;

    set_add(&account->set, "search", NULL, Skype_cmd_search, account)->flags
        |= ACC_SET_NOSAVE | ACC_SET_ONLINE_ONLY;
}

void
Skype_login (account_t* account)
{
    im_connection* connection = imcb_new(account);
    SkypeData*     skype      = g_new0(struct SkypeData, 1);

    connection->proto_data = skype;

    imcb_log(connection, "Connecting");

    skype->ssl = ssl_connect(
        set_getstr(&account->set, "server"),
        set_getint(&account->set, "port"),
        Skype_connected,
        connection
    );

    skype->fd         = skype->ssl ? ssl_getfd(skype->ssl) : -1;
    skype->username   = g_strdup(account->user);
    skype->connection = connection;

    if (set_getbool(&account->set, "skypeconsole")) {
        imcb_add_buddy(connection, "skypeconsole", NULL);
    }
}

gboolean
Skype_connected (gpointer data, void* source, b_input_condition condition)
{
    im_connection* connection = data;
    SkypeData*     skype      = connection->proto_data;

    if (source == NULL) {
        skype->ssl = NULL;
        imcb_error(connection, "Could not connect to server");
        imc_logout(connection, TRUE);

        return FALSE;
    }

    imcb_log(connection, "Connected to server, logging in");

    return Skype_begin(connection);
}

gboolean
Skype_begin (im_connection* connection)
{
    SkypeData* skype = connection->proto_data;
    int        result;

    if (!skype) {
        return FALSE;
    }

    skype->bfd = b_input_add(skype->fd, B_EV_IO_READ, Skype_loop, connection);

    /* Identifying */
    Skype_printf(connection, "USERNAME %s\n", connection->acc->user);
    Skype_printf(connection, "PASSWORD %s\n", connection->acc->pass);

    result = Skype_printf(connection, "SEARCH FRIENDS\n");

    Skype_printf(connection, "SET USERSTATUS ONLINE\n");

    if (set_getbool(&connection->acc->set, "auto_join")) {
        Skype_printf(connection, "SEARCH BOOKMRKEDCHATS\n");
    }

    return result;
}

int
Skype_write (im_connection* connection, char* buffer, int length)
{
    SkypeData* skype = connection->proto_data;

    fd_set         rfds;
    struct timeval tv = { 1, 0 };

    FD_ZERO(&rfds);
    FD_SET(skype->fd, &rfds);

    if (select(1, NULL, &rfds, NULL, &tv) < 0) {
        imc_logout(connection, TRUE);

        return -1;
    }

    return ssl_write(skype->ssl, buffer, length);
}

int
Skype_printf (im_connection* connection, char* format, ...)
{
    va_list arguments;
    char    string[IRC_LINE_SIZE];

    va_start(arguments, format);
    vsnprintf(string, IRC_LINE_SIZE, format, arguments);
    va_end(arguments);

    return Skype_write(connection, string, strlen(string));
}

gboolean
Skype_loop (gpointer data, gint fd, b_input_condition condition)
{
    im_connection* connection = data;
    SkypeData*     skype      = connection->proto_data;
    char           buffer[IRC_LINE_SIZE];

    if (ssl_read(skype->ssl, buffer, sizeof(buffer)) <= 0 && !sockerr_again()) {
        closesocket(skype->fd);
        skype->fd = -1;

        imcb_error(connection, "Error while reading from server");
        imc_logout(connection, TRUE);

        return FALSE;
    }

    return TRUE;
}
