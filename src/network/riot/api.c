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
#include <curl/curl.h>
#include <cchamp/cchamp.h>
#include "api.h"
#include "ddragon/static.h"

static CURL* channel;
uint16_t cc_error;
extern struct curl_slist* http_headers;

RiotAPI api = {
    .rate.per_second        = MAX_REQUESTS_PER_SECOND,
    .rate.per_two_minutes   = MAX_REQUESTS_PER_TWO_MINUTES
};


/**
 * Initialize the library by preparing curl.
 * An option is passed to curl to override the default write function (i.e. fwrite) to a custom one.
 *
 * @return  0 If curl successfully initialized.
 *          1 If curl failed.
 */
int cchamp_init()
{
    // map all necessary pages for backing cchamp internal data.
    int reserved = static_pages_allocate() + channel_blocks_allocate();

    // Attempt to initialize the curl instance which will be used as the http medium.
    channel = curl_easy_init();
    if (channel == NULL) {
        cc_error = ECURL;
        cchamp_close();

        return 1;
    }

    curl_easy_setopt(channel, CURLOPT_WRITEFUNCTION, channel_response_received);
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
    if (channel != NULL) {
        curl_easy_cleanup(channel);
        channel = NULL;
    }

    // return all anonymously backed pages to the OS.
    static_pages_free();
    channel_blocks_free();
}


/**
 * Sets the API key that will be used when accessing the riot games API.
 * You cannot make any API calls before configuring the API key.
 *
 * @param key A sequence of characters used to identify the person making the API call.
 */
void cchamp_set_api_key(char* key)
{
    if (strlen(key) != API_KEY_LENGTH) return;

    memcpy(api.key, key, API_KEY_LENGTH);
    api.key[API_KEY_LENGTH] = 0x00;
    channel_update_token(api.key);
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
 * Sends the request to the server, writes the response to a buffer, then exits.
 *
 * @param request A struct containing the request data to be sent out.
 */
void cchamp_send_request(Request* request)
{
    if (channel == NULL) return;

    curl_easy_setopt(channel, CURLOPT_URL, channel_url(request));
    curl_easy_setopt(channel, CURLOPT_WRITEDATA, request);
    curl_easy_setopt(channel, CURLOPT_HTTPHEADER, http_headers);
    CURLcode res = curl_easy_perform(channel);

    // Store the http response code.
    curl_easy_getinfo(channel, CURLINFO_RESPONSE_CODE, &request->http_code);

    // Check the status of the request. Any errors reported are stored in cc_error.
    if (request->http_code == 200) {
        cc_error = EPASS;
    } else if (request->http_code == 401 || request->http_code == 403) {
        cc_error = EAPIKEY;
    } else if (request->http_code == 404) {
        cc_error = ENOTFOUND;
    } else if (request->http_code == 429) {
        cc_error = ERATELIMIT;
    } else {
        cc_error = EUNKNOWN;
    }
}
