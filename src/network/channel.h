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
#ifndef CCHAMP_CHANNEL_H
#define CCHAMP_CHANNEL_H

/*
 * A request URL is created by appending specific path and query arguments.
 * path_arg and query_arg are simple linking structures that allow for the creation
 * of arguments.
 */
struct arg {

    // The value attribute stores the full "key=val" sequence.
    char value[128];
    struct arg* next;
};


/*
 * Path and Query arguments are now tracked through one main struct: arguments.
 * This struct also stores the current size of each category of arguments.
 */
struct arguments {
    struct {
        int size;
        struct arg* head;
    } path;

    struct {
        int size;
        struct arg* head;
    } query;
};


/*
 * A catch-all api_request struct is now created that tracks all the needed data to:
 * -    Build a fully-qualified query URL.
 * -    Store the response text and the http code.
 */
struct api_request {
    uint16_t region;
    uint16_t api;
    struct arguments arguments;

    struct {
        int size;
        void *addr;
    } response;

    long http_code;
};


#define CHANNEL_BLOCK_NUM     8
#define CHANNEL_BLOCK_SIZE    256 * 1024

/*
 * Query buffer maintains 8 exclusive blocks of memory for use in receiving
 * data.
 *
 * Each block of memory is 256KB. So, a total of 256KB * 8 = 2MB is reserved contiguously.
 */
struct channel_buf {

    // tracks if a block of memory is in-use (1) or free (0).
    uint8_t status;

    // a pointer to the first block of memory.
    void*   addr;
};


typedef struct arg Argument;
typedef struct api_request Request;
typedef struct channel_buf __CBUFF;

extern struct curl_slist* http_headers;
extern char* regions[];


/*
 * Functions exported by the cchamp_channel implementation.
 */


/*
 * Creates a new path argument.
 */
Argument*  path_arg(Request *request, char* value, Argument* next);


/*
 * Creates a new query argument.
 */
Argument* query_arg(Request *request, char* key, char* value, Argument* next);


/*
 * Produces a fuly-qualified url by extracting data from the provided request.
 */
char*   channel_url(Request* request);


/*
 * Updates the HTTP headers with the latest API key.
 */
void    channel_update_token(char* key);


/*
 * Invoked when data is received during an HTTP request.
 * Responsible for storing the result accordingly.
 */
size_t  channel_response_received(char* ptr, size_t size, size_t nmemb, void* request);


/*
 * Cleans up all resources used by the request.
 */
void    channel_clean(Request* request);


/*
 * Internal functions that are exposed by this interface due to the necessity for external invocation.
 */

/*
 * Map necessary anonymous pages for query response storage.
 */
int     __channel_blocks_allocate();


/*
 * Return the mapped anonymous pages to the operating system.
 */
void    __channel_blocks_free();
#endif
