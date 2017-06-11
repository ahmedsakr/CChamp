#include "inttypes.h"
#include "summoner.h"

// Summoner API method definitions
Summoner* get_player_by_summoner_id(uint64_t summonerId);
Summoner* get_player_by_account_id(uint64_t accountId);
Summoner* get_player_by_name(char *summoner_name);
