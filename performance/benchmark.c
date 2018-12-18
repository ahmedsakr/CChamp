#include <cchamp/cchamp.h>
#include <stdio.h>
#include <time.h>

clock_t begin;
clock_t delta;

int main(void)
{
    clock_t absolute_beginning = clock();
    begin = clock();
    cchamp_init();
    delta = clock() - begin;
    printf("cchamp_init() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    cchamp_set_api_key("RGAPI-72cdaa16-0b91-4f10-9231-6665d0ad05d9");
    delta = clock() - begin;
    printf("cchamp_set_api_key() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    cchamp_set_max_requests(100, 100);
    delta = clock() - begin;
    printf("cchamp_set_max_requests() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    cchamp_config_set(0x0080, 1);
    delta = clock() - begin;
    printf("cchamp_config_set() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    get_summoner_by_sid(REGION_NA, "21748566");
    delta = clock() - begin;
    printf("get_summoner_by_sid() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    get_summoner_by_aid(REGION_NA, "35259927");
    delta = clock() - begin;
    printf("get_summoner_by_aid() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    get_summoner_by_name(REGION_NA, "Scythick");
    delta = clock() - begin;
    printf("get_summoner_by_name() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    cchamp_static_invalidate(0x0001);
    delta = clock() - begin;
    printf("cchamp_static_invalidate() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    begin = clock();
    cchamp_close();
    delta = clock() - begin;
    printf("cchamp_close() took %f ms.\n", ((float)delta * 1000/ CLOCKS_PER_SEC));

    clock_t absolute_end = clock();
    printf("Total time taken: %f ms.\n", ((float)absolute_end - absolute_beginning) * ((float)(1000) / CLOCKS_PER_SEC));
    return 0;
}
