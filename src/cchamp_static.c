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
#include <cchamp_static.h>
#include <sys/mman.h>
#include <stdlib.h>


uint16_t __static_data_flags;
struct category* categories;


/*
 * Number of Anonymous pages to be backed for specific static api categories.
 * This number is determined by estimating the maximum required data storage of a specific category.
 */
#define PAGES_RUNES            2
#define PAGES_MASTERIES        2
#define PAGES_CHAMPIONS        2
#define PAGES_ITEMS            2
#define PAGES_MAPS             2
#define PAGES_PROFILE_ICONS    2
#define PAGES_REALMS           2
#define PAGES_SUMMONER_SPELLS  2
#define PAGES_LANGUAGES        2
#define PAGES_VERSIONS         2

static int __categories_pages[] = {
    PAGES_RUNES,
    PAGES_MASTERIES,
    PAGES_CHAMPIONS,
    PAGES_ITEMS,
    PAGES_MAPS,
    PAGES_PROFILE_ICONS,
    PAGES_REALMS,
    PAGES_SUMMONER_SPELLS,
    PAGES_LANGUAGES,
    PAGES_VERSIONS
};


/**
 * Loads the specified static data into memory.
 */
void cchamp_static_load(uint16_t data)
{

}


/**
 * Invalidates the loaded data for the specific static categories.
 * All operations on the invalidated static categories will result in a server update before proceeding.
 *
 * @param data The static categories to invalidate.
 */
void cchamp_static_invalidate(uint16_t data)
{

    // The corresponding anonymous pages are not affected by this invokation.
    // Anonymous pages are kept in memory throughout the whole life cycle of cchamp.
    __static_data_flags &= ~data;
}


/**
 * Mmaps a sufficient amount of anonymous pages for static categories.
 * The number of anonymous backing pages per static category is determined by __categories_pages config
 * array in this file. Manipulating PAGES_* constants will alter the number of pages mmaped for categories.
 *
 * @return 0    If the allocation succeeded.
 *         1    Failure in mmaping anonymous pages.
 */
int __static_pages_allocate()
{
    if (categories != NULL) {

        // if categories is not null, then this function must have been previously invoked.
        // Simply terminating with no effect is sufficient.
        return 0;
    }

    // Allocate, on the heap, the necessary headers for keeping track of the anonymous pages.
    categories = (struct category *)malloc(sizeof(struct category) * STATIC_CATEGORY_SIZE);
    struct category* cat = categories;

    for (int i = 0; i < STATIC_CATEGORY_SIZE; cat++, i++) {
        cat->__init_pages_size = __categories_pages[i];
        cat->__first_page = mmap(NULL, cat->__init_pages_size * 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

        // if the anonymous page mapping failed, then an error indicating value is returned.
        // errno should be read instead of cc_error in this case as mmap() sets errno on failure.
        if (cat->__first_page == MAP_FAILED) {
            return 1;
        }
    }

    return 0;
}


/**
 * Frees all memory tied up for tracking and maintaining anonymous pages for the static api.
 * Should only be invoked on exit (i.e. cchamp_close()).
 */
void __static_pages_free()
{
    struct category* cat = categories;

    for (int i = 0; i < STATIC_CATEGORY_SIZE; cat++, i++) {
        if (cat->__first_page != NULL && cat->__first_page != MAP_FAILED) {

            // free the anonymous pages backed for this category.
            munmap(cat->__first_page, cat->__init_pages_size * 4096);
        }
    }

    // all pages backed by the categories have now been freed. It is possible to free the headers
    // that track these pages.
    free(categories);
}
