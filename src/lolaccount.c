#include <stdlib.h>
#include <assert.h>
#include "lolaccount.h"

Summoner* create_summoner(char *summoner_name, char *region)
{
    Summoner *account = malloc(sizeof(Summoner));
    assert(account != NULL);

    account->summoner_name = summoner_name;
    account->region = region;

    return account;
}
