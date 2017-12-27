/* This file is part of CChamp.
 *
 * CChamp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CChamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CChamp.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CCHAMP_QUERY_H
#define CCHAMP_QUERY_H
#include "cchamp_api.h"

size_t query_response_write(char *ptr, size_t size, size_t nmemb, void *request);
char get_region_index(uint16_t region);
char* build_query(Request* request);

struct query_param {
    char key[64];
    char value[64];
};

typedef struct query_param query_param;
#endif
