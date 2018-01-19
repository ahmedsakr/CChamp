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
 * By default, the number of API calls for 1 second / 100 seconds is the maximum allowed calls.
 * It should be 20 and 100 calls, respectively.
 */
void    cchamp_set_api_key(char* key);
void    cchamp_set_max_requests(uint16_t per_second, uint16_t per_two_minutes);


/*
 * The current API version.
 * You *must* update this once the version changes.
 */
#define API_VERSION 3

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
 * The following are all possible errors that may be encountered when using CChamp.
 */

// No error reported.
#define EPASS       0

// Error: Curl failed to initialize.
#define ECURL       1

// Error: API Key specified is invalid.
#define EAPIKEY     2

// Error: The data requested was not found on the servers.
#define ENOTFOUND   3

// Error: Maximum rate limit has been reached.
#define ERATELIMIT  4

// Error: Cannot establish a new connection due to all available resources being exhausted.
#define E2MANY      5

// Error: Unknown error has been reached.
#define EUNKNOWN    6



/*
 * Errors are reported and stored in cc_error.
 */
extern uint16_t cc_error;


/*
 * Lists all possible configuration alterations so thta you may customize cchamp behaviour to better
 * suit your needs.
 */
#define CCHAMP_STATIC_MULTITHREADING 0x0001


/*
 * Update a cchamp configuration.
 */
void cchamp_config_set(uint16_t config, char value);


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
 * API - Summoner (/lol/summoner)
 * Rate Limit Applicable: YES
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


/*
 * Defines all kinds of data retrievable by the static-data API.
 */
#define STATIC_RUNES            0x0001
#define STATIC_MASTERIES        0x0002
#define STATIC_CHAMPIONS        0x0004
#define STATIC_ITEMS            0x0008
#define STATIC_MAPS             0x0010
#define STATIC_PROFILE_ICONS    0x0020
#define STATIC_REALMS           0x0040
#define STATIC_SUMMONER_SPELLS  0x0080
#define STATIC_LANGUAGES        0x0100
#define STATIC_VERSIONS         0x0200
#define STATIC_ALL              0x02FF


/*
 * API - Static (/lol/static-data)
 * Rate Limit Applicable: NO
 * ---
 *
 * The static-data API allows you to parse through constant data of the game such as champions, masteries, and
 * runes.
 */

/*
 * Loads the specified categories metadata into the cchamp system.
 *
 * By default, the majority of this code runs on a separate thread because it is time intensive.
 * You may disable this by invoking cchamp_config(CCHAMP_STATIC_MULTITHREADING, 0).
 */
void cchamp_static_load(uint16_t data);


/*
 * Invalidates the specified static data categories.
 * Any operations on the targeted categories futher on will require a fresh read from the server.
 *
 * Keep in mind before invalidating data that Riot limits method calls to static data to 10 calls an hour.
 * Use wisely.
 */
void cchamp_static_invalidate(uint16_t data);

#endif
