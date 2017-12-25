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
#include "cchamp/cchamp.h"
#include <string.h>

static Request request;


/**
 * Dispatches a summoner information retrieval request.
 *
 * @param region    The region which the targeted summoner lies in.
 * @param keyword   The query keyword (i.e. summoner id, account id, or summoner name).
 * @param qualifier Specifies to the api which keyword is being supplied.
 */
static char *summoner_request(uint16_t region, char *keyword, char *qualifier)
{
    request.api = API_SUMMONER;
    request.keyword = keyword;
    request.url_qualifier = qualifier;
    request.region = region;
    cchamp_send_request(&request);

    return request.response;
}
