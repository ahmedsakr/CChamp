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
#include <curl/curl.h>
#include <cchamp/cchamp.h>
#include "api.h"
#include "api_query.h"

static CURL *channel;
static size_t response(char *ptr, size_t size, size_t nmemb, void *userdata);
extern size_t query_response_write(char *ptr, size_t size, size_t nmemb, void *userdata);
extern char *build_query(Request* request);

RiotAPI api = {
    .rate.per_second        = MAX_REQUESTS_PER_SECOND,
    .rate.per_two_minutes   = MAX_REQUESTS_PER_TWO_MINUTES
};


/**
 * Initialize the library by preparing curl. An option is passed to curl to override the default write
 * function (i.e. fwrite) to a custom one.
 *
 * @return  0 If curl successfully initialized.
 *          1 If curl failed.
 */
int cchamp_init()
{
    channel = curl_easy_init();
    if (channel == NULL) return 1;

    curl_easy_setopt(channel, CURLOPT_WRITEFUNCTION, query_response_write);
    return 0;
}


/**
 * Close the curl instance which shuts down the ability for cchamp to communicate with the API.
 *
 * Invoke when you are done using CChamp. You could reinvoke cchamp_init whenever you wish to
 * reuse cchamp.
 */
void cchamp_close()
{
    curl_easy_cleanup(channel);
}


/**
 * Sets the API key that will be used when accessing the riot games API.
 * You cannot make any API calls before configuring the API key.
 *
 * @param key A sequence of characters used to identify the person making the API call.
 */
void cchamp_set_api_key(char *key)
{
    if (strlen(key) != API_KEY_LENGTH) return;

    memcpy(api.key, key, API_KEY_LENGTH);
    api.key[API_KEY_LENGTH] = 0x00;
}


/**
 * Overrides the default configurations for maximum api requests per second and per two minutes.
 *
 * @param per_second        The new value for maximum API calls per second.
 * @param per_two_minutes   The new value for maximum API calls per two minutes.
 */
void cchamp_set_max_requests(uint16_t per_second, uint16_t per_two_minutes)
{
    api.rate.per_second = per_second;
    api.rate.per_two_minutes = per_two_minutes;
}


/**
 * Sends the request to the server, writes the response to a buffer, and exits.
 *
 * @param request A struct containing the request data to be sent out.
 */
void cchamp_send_request(Request* request)
{
    if (channel == NULL) return;

    curl_easy_setopt(channel, CURLOPT_URL, build_query(request));
    curl_easy_setopt(channel, CURLOPT_WRITEDATA, request);
    CURLcode res = curl_easy_perform(channel);
}
