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
#include <stdlib.h>
#include "cchamp.c"
#include <cJSON.h>

static Summoner *parse_summoner(uint16_t region, char *json);
extern char *regions[];
extern char get_region_index(uint16_t region);

/**
 * Allocates and initializes a summoner object.
 *
 * @param summoner_name The name of the player.
 * @param region        The region which the player's account is in.
 */
Summoner* summoner_create(char *summoner_name, char *region, uint64_t account_id, uint64_t summoner_id)
{
    Summoner *summoner= calloc(1, sizeof(Summoner));
    summoner->account_id = account_id;
    summoner->summoner_id = summoner_id;
    memcpy(summoner->name, summoner_name, strlen(summoner_name));
    memcpy(summoner->region, region, strlen(region));
    return summoner;
}


/**
 * Dispatches a summoner information retrieval request using the summoner id as the keyword.
 *
 * @param region        The region which the player's account is being searched for.
 * @param summoner_id   The summoner id of the player's account.
 */
Summoner* get_summoner_by_sid(uint16_t region, char* summoner_id)
{
    char *response = summoner_request(region, summoner_id, "");
    return parse_summoner(region, response);
}


/**
 * Dispatches a summoner information retrieval request using the account id as the keyword.
 *
 * @param region        The region which the player's account is being searched for.
 * @param account_id    The account id of the player's account.
 */
Summoner* get_summoner_by_aid(uint16_t region, char* account_id)
{
    char *response = summoner_request(region, account_id, "by-account");
    return parse_summoner(region, response);
}

/**
 * Dispatches a summoner information retrieval request using the summoner name as the keyword.
 *
 * @param region        The region which the player's account is being searched for.
 * @param summoner_name The name of the player's account.
 */
Summoner* get_summoner_by_name(uint16_t region, char* summoner_name)
{
    char *response = summoner_request(region, summoner_name, "by-name");
    return parse_summoner(region, response);
}


static Summoner *parse_summoner(uint16_t region, char *json)
{
    cJSON *data = cJSON_Parse(json);
    char *name = cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring;
    uint64_t summoner_id = cJSON_GetObjectItemCaseSensitive(data, "id")->valueint;
    uint64_t account_id = cJSON_GetObjectItemCaseSensitive(data, "accountId")->valueint;
    int level = cJSON_GetObjectItemCaseSensitive(data, "summonerLevel")->valueint;
    int icon_id = cJSON_GetObjectItemCaseSensitive(data, "profileIconId")->valueint;
    char *region_str = regions[get_region_index(region)];

    Summoner *summoner = summoner_create(name, region_str, account_id, summoner_id);
    summoner->details.level = level;
    summoner->details.profile_icon_id = icon_id;

    cJSON_Delete(data);
    return summoner;
}
