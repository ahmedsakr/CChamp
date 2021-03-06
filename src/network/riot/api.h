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
#ifndef CCHAMP_API_H
#define CCHAMP_API_H

#include <inttypes.h>
#include <network/channel.h>

#define API_KEY_LENGTH 42

struct api_node {
    char key[API_KEY_LENGTH + 1];
    struct rate_limit {
        uint16_t per_second;
        uint16_t per_two_minutes;
    } rate;
};

typedef struct api_node RiotAPI;

#define MAX_REQUESTS_PER_SECOND 20
#define MAX_REQUESTS_PER_TWO_MINUTES 100

extern RiotAPI api;

void cchamp_send_request(Request* request);

// All different types of APIs available for requests
#define API_CHAMPION_MASTERY    0x0001
#define API_CHAMPION            0x0002
#define API_LEAGUE              0x0004
#define API_LOL_STATIC_DATA     0x0008
#define API_LOL_STATUS          0x0010
#define API_MATCH               0x0020
#define API_SPECTATOR           0x0040
#define API_SUMMONER            0x0080
#define API_THIRD_PARTY_CODE    0x0100

#endif
