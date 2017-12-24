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
#include <stdio.h>
#include "cchamp.h"

int main(void)
{
    Summoner *account = create_summoner("A Minion with IE", "NA");

    printf("Summoner Name: %s\n", account->summoner_name);
    printf("Region: %s\n", account->region);
    printf("API_VERSION: %c\n", API_VERSION(SUMMONER));


    CURL *curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://na1.api.riotgames.com/lol/summoner/v3/summoners/by-name/Iorek?api_key=RGAPI-c753b44b-d611-4638-b52e-662e2d34560a");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return 0;
}
