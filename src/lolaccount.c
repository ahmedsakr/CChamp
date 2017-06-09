#include <stdlib.h>
#include <assert.h>
#include "lolaccount.h"

Account* account_create(char *summoner_name, char *region)
{
    Account *account = malloc(sizeof(Account));
    assert(account != NULL);

    account->summoner_name = summoner_name;
    account->region = region;

    return account;
}
