#include <stdio.h>
#include "lolaccount.h"

int main(void)
{
    Account *account = account_create("A Minion with IE", "NA");

    printf("Summoner Name: %s\n", account->summoner_name);
    printf("Region: %s\n", account->region);
    
    return 0;
}
