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

// Number of pages to mmap for static categories.
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

void* categories;
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
 * Mmaps a sufficient amount of anonymous pages for static categories.
 *
 * @return 0    If the allocation succeeded.
 *         1    Failure in mmaping anonymous pages.
 */
int __allocate_static_pages()
{
    categories = malloc(sizeof(struct category) * STATIC_CATEGORY_SIZE);
    int i = 0;
    for (struct category* temp = (struct category *)categories; i < STATIC_CATEGORY_SIZE; temp++, i++) {
        temp->__init_pages_size = __categories_pages[i];
        temp->__first_page = mmap(NULL, temp->__init_pages_size * 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if (temp->__first_page == MAP_FAILED) {
            return 1;
        }
    }

    return 0;
}



/**
 * Munmaps all anonymously mapped pages for the static api usage and frees the heap memory allocated for
 * tracking these pages.
 */
void __free_static_pages()
{
    int i = 0;
    for (struct category* temp = (struct category *)categories; i < STATIC_CATEGORY_SIZE; temp++, i++) {
        if (temp->__first_page != NULL) {
            munmap(temp->__first_page, temp->__init_pages_size * 4096);
        }
    }

    free(categories);
}
