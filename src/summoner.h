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
Summoner* create_summoner(char *summoner_name, char *region);
