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
#include <assert.h>
#include "cchamp.c"


/**
 * Allocates and initializes a summoner object.
 *
 * @param summoner_name The name of the player.
 * @param region        The region which the player's account is in.
 */
Summoner* summoner_create(char *summoner_name, char *region)
{
    Summoner *account = malloc(sizeof(Summoner));
    assert(account != NULL);

    account->summoner_name = summoner_name;
    account->region = region;

    return account;
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
    return NULL;
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
    return NULL;
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
    return NULL;
}
