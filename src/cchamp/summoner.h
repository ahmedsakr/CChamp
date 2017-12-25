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
#ifndef SUMMONER_H
#define SUMMONER_H
struct account_details {
    int level;
    int summoner_icon_id;
};

typedef struct account_details Details;
struct league_account {
    char *summoner_name;
    char *region;
    Details *details;
};

typedef struct league_account Summoner;
Summoner* summoner_create(char *summoner_name, char *region);

#define SUMMONER_NAME_MAX_LENGTH 16
#endif
