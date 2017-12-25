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
#include <curl/curl.h>
#include "summoner.h"
#include "api.h"

// Summoner API method definitions
Summoner* get_player_by_summoner_id(uint16_t region, char* summoner_id);
Summoner* get_player_by_account_id(char* accountId);
Summoner* get_player_by_name(char* summoner_name);
#endif
