#include <stdio.h>
#include "summoner.h"

int main(void)
{
    Summoner *account = create_summoner("A Minion with IE", "NA");

    printf("Summoner Name: %s\n", account->summoner_name);
    printf("Region: %s\n", account->region);

    return 0;
}
