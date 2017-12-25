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
#ifndef API_H
#define API_H

#include "cchamp/summoner.h"

#define MAX_REQUESTS_PER_SECOND 20
#define MAX_REQUESTS_PER_TWO_MINUTES 120

struct api_node {
    char key[64];
    struct rate_limit {
        uint16_t per_second;
        uint16_t per_two_minutes;
    } rate;
};

struct api_request {
    char *region;
    char parameter[512];
};

typedef struct api_node RiotAPI;
typedef struct api_request Request;
extern RiotAPI api;

int cchamp_init();
void set_api_key(char *key);
void set_api_region(char *region);
void set_api_max_requests(uint16_t per_second, uint16_t per_two_minutes);
char* send_request(Request* request);

static const char CHAMPION_MASTERY_API_VERSION = 3;
static const char CHAMPION_API_VERSION = 3;
static const char LEAGUE_API_VERSION = 3;
static const char LOL_STATIC_DATA_API_VERSION = 3;
static const char LOL_STATUS_API_VERSION = 3;
static const char MATCH_API_VERSION = 3;
static const char SPECTATOR_API_VERSION = 3;
static const char SUMMONER_API_VERSION = 3;
static const char THIRD_PARTY_CODE_API_VERSION = 3;


#define API_VERSION(X) X ## _API_VERSION

#endif
