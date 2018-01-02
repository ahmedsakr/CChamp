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

/*
 * A request URL is created by appending specific path and query parameters.
 * path_param and query_param are simple linking structures that allow for the creation
 * of parameters.
 */
struct path_param {
    char* value;
    struct path_param*  next;
};

struct query_param {

    // The value attribute stores the full "key=val" sequence.
    char value[128];
    struct query_param* next;
};


/*
 * Path and Query parameters are now tracked through one main struct: parameters.
 * This struct also stores the current size of each category of parameters.
 */
struct parameters {
    struct {
        int size;
        struct path_param*  head;
    } path;

    struct {
        int size;
        struct query_param* head;
    } query;
};


/*
 * A catch-all api_request struct is now created that tracks all the needed data to:
 * -    Build a fully-qualified query URL.
 * -    Store the response text and the http code.
 */
struct api_request {
    uint16_t            region;
    uint16_t            api;
    struct parameters   params;
    char                response[8192];
    long                http_code;
};


typedef struct path_param   PathParam;
typedef struct query_param  QueryParam;
typedef struct parameters   Parameters;
typedef struct api_request  Request;

extern struct curl_slist* http_headers;
extern char* regions[];


/*
 * Functions exported by the cchamp_query implementation.
 */


/*
 * Initializes a new path parameter.
 */
PathParam*  path_param_create(char* value, PathParam* next);


/*
 * Initializes a new query parameter.
 */
QueryParam* query_param_create(char* key, char* value, QueryParam* next);


/*
 * Internal functions that provide critical logic in the query implementation.
 */


/*
 * Produces a dispatchable query by extracting data from the provided request.
 */
char*   _query_format(Request* request);


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
