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
#ifndef RIOTAPI_H
#define RIOTAPT_H
struct api_node {
    char *api_key;
    int max_requests;
};

struct api_request {
    struct api_node api;
    char *region;
    char *qualified_request;
}

typedef struct api_node RiotAPI;
typedef struct api_request Request;
#endif
