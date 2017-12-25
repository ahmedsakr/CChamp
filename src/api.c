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
#include <stdio.h>
#include "cchamp/cchamp.h"

RiotAPI api = {
    .rate.per_second        = MAX_REQUESTS_PER_SECOND,
    .rate.per_two_minutes   = MAX_REQUESTS_PER_TWO_MINUTES
};

static CURL *channel;
static char query[1024];
static char *regions[] = {"na1", "eun1", "euw1", "ru", "tr1", "kr", "br1", "oc1", "jp1", "la1", "la2"};

static char *build_query(Request* request);
static size_t response(char *ptr, size_t size, size_t nmemb, void *userdata);

int cchamp_init()
{
    channel = curl_easy_init();
    curl_easy_setopt(channel, CURLOPT_WRITEFUNCTION, response);
    return channel == 0;
}

void cchamp_close()
{
    curl_easy_cleanup(channel);
}

void cchamp_set_api_key(char *key)
{
    memcpy(api.key, key, strlen(key));
    api.key[strlen(key)] = 0x00;
}

void cchamp_set_max_requests(uint16_t per_second, uint16_t per_two_minutes)
{
    api.rate.per_second = per_second;
    api.rate.per_two_minutes = per_two_minutes;
}

void cchamp_send_request(Request* request)
{
    if (channel == NULL) return;

    curl_easy_setopt(channel, CURLOPT_URL, build_query(request));
    curl_easy_setopt(channel, CURLOPT_WRITEDATA, request);
    CURLcode res = curl_easy_perform(channel);
}

static size_t response(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    memcpy(((Request *)userdata)->response, ptr, size * nmemb);
    return size * nmemb;
}
static char get_region_index(uint16_t region)
{
    char index = 0;
    while ((region >>= 1) != 0) {
        index++;
    }

    return index;
}

static char* build_query(Request* request)
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

