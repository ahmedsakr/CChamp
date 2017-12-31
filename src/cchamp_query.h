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

struct path_param {
    char* value;
    struct path_param* next;
};
typedef struct path_param PathParam;

struct query_param {
    char key[64];
    char value[64];
    struct query_param* next;
};
typedef struct query_param QueryParam;

struct parameters {
    struct {
        int size;
        PathParam* head;
    } path;

    struct {
        int size;
        QueryParam* head;
    } query;
};

typedef struct parameters Parameters;

struct api_request {
    uint16_t    api;
    uint16_t    region;
    Parameters  params;
    char        response[8192];
    long        http_code;
};

typedef struct api_request Request;

extern struct curl_slist* http_headers;
extern char* regions[];


/*
 * Functions exported by the cchamp_query implementation.
 */


/*
 * Acquires the appropriate array index for the specified region.
 */
char    get_region_index(uint16_t region);


/*
 * Produces a dispatchable query by extracting data from the provided request.
 */
char*   query_format(Request* request);


/*
 * Initializes a new path parameter.
 */
PathParam*  path_param_create(char* value, PathParam* next);


/*
 * Internal functions that provide critical logic in the query implementation.
 */


/*
 * Updates the HTTP headers with the latest API key.
 */
void    _query_update_token(char* key);


/*
 * Invoked when data is received during an HTTP request.
 * Responsible for storing the result accordingly.
 */
size_t  _query_response_received(char* ptr, size_t size, size_t nmemb, void* request);


/*
 * Frees all memory used up by the parameter linking implementation (PathParam and QueryParam structs).
 */
void    _query_params_free_all(Request* request);
#endif
