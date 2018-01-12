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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/mman.h>
#include <cchamp/cchamp.h>
#include <cchamp_api.h>
#include <cchamp_query.h>
#include <cchamp_utils.h>

/*
 * API path parameters for different combnations of regions and APIs.
 */
char *regions[] = {
    "na1", "eun1", "euw1", "ru", "tr1",
    "kr", "br1", "oc1", "jp1", "la1", "la2"
};
char *api_path[] = {
    "champion-mastery", "platform", "league", "static-data",
    "status", "match", "spectator", "summoner", "platform"
};


static __QBUFF buffer;
static char query[512];
struct curl_slist *http_headers;
extern char *get_web_safe_str(char *str);


/**
 * Allocates a PathParam struct on the heap.
 *
 * @param value The value of the path parameter being created.
 * @param next  The next PathParameter to link up with.
 *
 * @return A pointer to the newly created PathParam struct.
 */
PathParam* path_param_create(char* value, PathParam* next)
{
    PathParam* path_struct = malloc(sizeof(PathParam));
    path_struct->value = value;
    path_struct->next = next;

    return path_struct;
}


/**
 * Allocates and initializes a QueryParam struct on the heap.
 *
 * @param key   The key of the parameter.
 * @param value The value of the parameter.
 * @param next  The next QueryParameter to link up with.
 */
QueryParam* query_param_create(char* key, char* value, QueryParam* next)
{
    QueryParam* query_struct = malloc(sizeof(QueryParam));
    query_struct->next = next;

    strcpy(query_struct->value, key);
    strcat(query_struct->value, "=");
    strcat(query_struct->value, value);

    return query_struct;
}


/**
 * Anonymously maps necessary pages for having sufficient memory backing for query responses
 * for up to QUERY_BLOCK_NUM (8) concurrent queries.
 */
void __query_blocks_allocate()
{
    buffer.status = 0;
    buffer.addr = mmap(NULL, QUERY_BLOCK_NUM * QUERY_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}


/**
 * Frees all anonymously mapped pages for the query.
 */
void __query_blocks_free()
{
    munmap(buffer.addr, QUERY_BLOCK_NUM * QUERY_BLOCK_SIZE);
}


void * _query_blocks_claim()
{
    uint8_t free = 1;
    uint8_t b_index = 0x01;

    while ((buffer.status & b_index) != 0)
    {
        b_index <<= 1;
    }

    // No buffers are free
    if (b_index == 0xFF) {
        return NULL;
    } else {
        return buffer.addr + (QUERY_BLOCK_SIZE * get_bit_index(b_index));
    }

}


/**
 * Transfers the response received from the server to the request's response buffer.
 * Curl is instructed to pass the relevant Request struct into this function using curl_easy_setopt() in
 * cchamp_init().
 *
 * @param ptr           A pointer to the beginning of the received response.
 * @param size          The number of blocks of the response.
 * @param nmemb         The number of bytes in this block.
 * @param request       The request struct corresponding to the query.
 *
 * @return the total number of bytes received.
 */
size_t _query_response_received(char *ptr, size_t size, size_t nmemb, void *argument)
{
    Request *request = (Request *)argument;
    if (request->response.addr == NULL && buffer.status != 0xFF) {
        request->response.addr = _query_blocks_claim();
        request->response.size = size * nmemb;
        memcpy(request->response.addr, ptr, size * nmemb);
    } else {
        memcpy(request->response.addr + request->response.size, ptr, size * nmemb);
        request->response.size += size * nmemb;
    }

    return size * nmemb;
}


/**
 * Updates the API token into the HTTP header struct.
 * The official API requires this header so that it may authorize the query.
 *
 * @param key The API key used to authenticate the query.
 */
void _query_update_token(char *key)
{
    if (http_headers != NULL) {
        curl_slist_free_all(http_headers);
    }

    char token[100] = {0};
    strcpy(token, "X-Riot-Token: ");
    strcat(token, key);
    http_headers = curl_slist_append(http_headers, token);
}




/**
 * Builds the query url by extracting the necessary information from a request struct.
 *
 * @param request The struct used for information in building the query.
 *
 * @return The fully qualified query url that services the request.
 */
char* _query_format(Request* request)
{
    memset(query, 0x00, 512);

    strcpy(query, "https://");
    strcat(query, regions[get_bit_index(request->region)]);
    strcat(query, ".api.riotgames.com/lol/");
    strcat(query, api_path[get_bit_index(request->api)]);
    strcat(query, "/v");
    strcat(query, &api_version_ascii);

    char* web_safe;
    for (PathParam* param = request->params.path.head; param != NULL; param = param->next) {
        web_safe = get_web_safe_str(param->value);
        strcat(query, web_safe);
        free(web_safe);
    }

    if (request->params.query.head != NULL) {
        strcat(query, "?");

        for (QueryParam* param = request->params.query.head; param != NULL; param = param->next) {
            web_safe = get_web_safe_str(param->value);
            strcat(query, web_safe);
            free(web_safe);

            // If there is another query parameter then it must be prefixed with an "&".
            if (param->next != NULL) {
                strcat(query, "&");
            }
        }
    }

    return query;
}




/**
 * Frees all memory allocated for parameter linking (i.e. PathParam and QueryParam structs).
 *
 * @param request The request whose parameters logic is being freed.
 */
void _query_params_free_all(Request* request)
{
    void* temp;

    temp = request->params.path.head;
    while (temp != NULL) {
        temp = request->params.path.head->next;
        free(request->params.path.head);
        request->params.path.head = (PathParam*)temp;
    }

    temp = request->params.query.head;
    while (temp != NULL) {
        temp = request->params.query.head->next;
        free(request->params.query.head);
        request->params.query.head = (QueryParam*)temp;
    }
}
