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

Summoner* get_player_by_summoner_id(char* summoner_id)
{
    memset(&request, 0x00, sizeof(Request));

    request.region = "na1";
    memcpy(&request.parameter, summoner_id, strlen(summoner_id));

    send_request(&request);
}
