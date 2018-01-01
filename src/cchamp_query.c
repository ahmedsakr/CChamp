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
#include <curl/curl.h>
#include <cchamp/cchamp.h>
#include <cchamp_query.h>
#include <cchamp_utils.h>

char *regions[] = {"na1", "eun1", "euw1", "ru", "tr1", "kr", "br1", "oc1", "jp1", "la1", "la2"};
static char query[1024];
struct curl_slist *http_headers;
extern char *get_web_safe_str(char *str);


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
size_t _query_response_received(char *ptr, size_t size, size_t nmemb, void *request)
{
    memcpy(((Request *)request)->response, ptr, size * nmemb);
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
 * Acquires the index of the region by shifting the bits until the value becomes zero.
 * Regions are distinguished by the bit that is on.
 *
 * @param region The 16-bit value hosting the specified region.
 *
 * @return The identifying index of the region.
 */
char get_region_index(uint16_t region)
{
    char index = 0;
    while ((region >>= 1) != 0) {
        index++;
    }

    return index;
}


/**
 * Builds the query url by extracting the necessary information from a request struct.
 *
 * @param request The struct used for information in building the query.
 *
 * @return The fully qualified query url that services the request.
 */
char* query_format(Request* request)
{
    memset(query, 0x00, 1024);

    strcpy(query, "https://");
    strcat(query, regions[get_region_index(request->region)]);
    strcat(query, ".api.riotgames.com/lol/summoner/v3/summoners");

    char* web_safe;
    for (PathParam* param = request->params.path.head; param != NULL; param = param->next) {
        web_safe = get_web_safe_str(param->value);
        strcat(query, web_safe);
        free(web_safe);
    }

    return query;
}


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
