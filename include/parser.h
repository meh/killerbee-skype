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

#ifndef _BITLBEE_SKYPE_PARSER
#define _BITLBEE_SKYPE_PARSER

#include "common.h"

typedef void (*SkypeParserCallback) (im_connection* connection, char* line);

typedef struct SkypeParser {
    char*               matches;
    SkypeParserCallback callback;
} SkypeParser;

#ifndef _BITLBEE_SKYPE_TYPESONLY
extern SkypeParser SkypeParsers[];
#endif

#endif
