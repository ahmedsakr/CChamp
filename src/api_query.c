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
#include <cchamp/cchamp.h>

char *regions[] = {"na1", "eun1", "euw1", "ru", "tr1", "kr", "br1", "oc1", "jp1", "la1", "la2"};
static char query[1024];


/**
 * Transfers the response received from the server to the request's response buffer.
 * Curl is instructed to pass the relevant Request struct into this function using curl_easy_setopt() in
 * cchamp_init().
 *
 * @param ptr           A pointer to the beginning of the received response.
 * @param size          The number of blocks of the response.
 * @param nmemb         The number of bytes in this block.
 * @param userdata      The request struct.
 *
 * @return the total number of bytes received.
 */
size_t query_response_write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    memcpy(((Request *)userdata)->response, ptr, size * nmemb);
    return size * nmemb;
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
char* build_query(Request* request)
{
    memset(query, 0x00, 1024);

    strcpy(query, "https://");
    strcat(query, regions[get_region_index(request->region)]);
    strcat(query, ".api.riotgames.com/lol/summoner/v3/summoners/");

    if (strlen(request->url_qualifier) > 0) {
        strcat(query, request->url_qualifier);
        strcat(query, "/");
    }

    strcat(query, request->keyword);
    strcat(query, "?api_key=");
    strcat(query, api.key);

    return query;
}
