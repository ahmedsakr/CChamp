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
#ifndef CCHAMP_H
#define CCHAMP_H
#include <inttypes.h>


/*
 * Initializer and Finalizer.
 *
 * Invoke cchamp_init() before accessing any other methods to ensure correct behaviour.
 * Invoke cchamp_close() when you are done using cchamp_*() calls.
 */
int     cchamp_init();
void    cchamp_close();


/*
 * Provide cchamp with the necessary authentication token and maybe also limit the rate of access.
 *
 * By default, the number of API calls for 1 second / 120 seconds is the maximum allowed calls.
 * It should be 20 and 100 calls, respectively.
 */
void    cchamp_set_api_key(char *key);
void    cchamp_set_max_requests(uint16_t per_second, uint16_t per_two_minutes);


// The maximum length the name of a summoner could be.
#define SUMMONER_NAME_MAX_LENGTH 16

// The region's alias maximumum length (i.e. na1, eune).
#define REGION_MAX_LENGTH 4


/*
 * All regions that are officially supported by the LoL API.
 * Use these constants when providing a region parameter to any API calls.
 */
#define REGION_NA   0x0001
#define REGION_EUNE 0x0002
#define REGION_EUW  0x0004
#define REGION_RU   0x0008
#define REGION_TR   0x0010
#define REGION_KR   0x0020
#define REGION_BR   0x0040
#define REGION_OC   0x0080
#define REGION_JP   0x0100
#define REGION_LA1  0x0200
#define REGION_LA2  0x0400


/*
 * Summoner struct defining all the relevant information on a player.
 * All of the fields are populated through usage of the Summoner API.
 */
struct player {
    char        name[SUMMONER_NAME_MAX_LENGTH];
    char        region[REGION_MAX_LENGTH];

    int         level;
    int         profile_icon_id;

    // Maximum number representation of almost ~4.3b. This should suffice as I
    // don't think LoL will have > 50% of the human population as players any time soon!
    uint32_t    summoner_id;
    uint32_t    account_id;
};

typedef struct player Summoner;


/*
 * API - Summoner (/lol/summoner/)
 * ---
 *
 * The following methods allow you to acquire information on any particular player by providing
 * any of the accepted keywords: summoner id, account id, or summoner name.
 *
 * For the region parameter, please use one of REGION_* constants defined above.
 */
Summoner* get_summoner_by_sid(uint16_t region, char* summoner_id);
Summoner* get_summoner_by_aid(uint16_t region, char* account_id);
Summoner* get_summoner_by_name(uint16_t region, char* summoner_name);
#endif
