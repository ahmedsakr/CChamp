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
#include "static.h"
#include <sys/mman.h>
#include <stdlib.h>

/*
 * Using the STATIC_* constants defined in <cchamp/cchamp.h>, You may read this variable's bits to determine
 * if the data is valid or not.
 */
static uint16_t valid;
struct category* categories;

// Page status operations
#define PAGE_STATUS_VALIDATE    0x00
#define PAGE_STATUS_INVALIDATE  0x01

/*
 * Number of Anonymous pages to be backed for specific static api categories.
 * This number is determined by estimating the maximum required data storage of a specific category.
 */
#define PAGES_RUNES            8
#define PAGES_MASTERIES        8
#define PAGES_CHAMPIONS        8
#define PAGES_ITEMS            8
#define PAGES_MAPS             8
#define PAGES_PROFILE_ICONS    8
#define PAGES_REALMS           8
#define PAGES_SUMMONER_SPELLS  8
#define PAGES_LANGUAGES        8
#define PAGES_VERSIONS         8

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
 * Handles validation and invalidation of static categories.
 * Should not be invoked directly.
 *
 * @param data  The static categories to operate on.
 * @param op    The operation to be applied to the categories.
 */
static void __static_pages_status(uint16_t data, char op)
{
    // Although only internally invoked, it is safer to check for data correctness.
    if (op != PAGE_STATUS_INVALIDATE && op != PAGE_STATUS_VALIDATE) {
        return;
    }

    uint16_t cat_index = 1;
    while (cat_index <= data) {

        /*
         * Handles both validation and invaidation operations.
         *
         * On invalidation, all corresponding pages protection must be overriden to allow writing on top of
         * reading. This is to allow for future calls to load data into the pages.
         *
         * On validation, it is understood that the data is now considered final. Hence, the memory protection
         * for the pages is overriden to be read-only.
         */
        if (data & cat_index) {
            struct category* cat = GET_CATEGORY(cat_index);
            int prot = op == PAGE_STATUS_VALIDATE ? PROT_READ : PROT_WRITE | PROT_READ;
            mprotect(cat->__first_page, cat->__init_pages_size * PAGE_SIZE, prot);

            if (op == PAGE_STATUS_VALIDATE) {
                valid |= cat_index;
            } else if (op == PAGE_STATUS_INVALIDATE) {
                valid &= ~cat_index;
            }
        }

        cat_index <<= 1;
    }
}

/**
 * Validates the speicifed static categories.
 * Invocation of this function should only occur after the data for the specific static category
 * is understood to be final.
 *
 * @param data  The static categories to validate.
 */
static void __static_pages_validate(uint16_t data)
{

    __static_pages_status(data, PAGE_STATUS_VALIDATE);
}


/**
 * Frees all memory tied up for tracking and maintaining anonymous pages for the static api.
 * Should only be invoked on exit (i.e. cchamp_close()).
 */
static void __static_pages_free()
{
    struct category* cat = categories;

    for (int i = 0; i < STATIC_CATEGORY_SIZE; cat++, i++) {
        if (cat->__first_page != NULL && cat->__first_page != MAP_FAILED) {

            // free the anonymous pages backed for this category.
            munmap(cat->__first_page, cat->__init_pages_size * PAGE_SIZE);
        }
    }

    // all pages backed by the categories have now been freed. It is possible to free the headers
    // that track these pages.
    free(categories);
}

/**
 * Memory maps a sufficient amount of anonymous pages for static categories.
 * The number of anonymous backing pages per static category is determined by __categories_pages config
 * array in this file.
 *
 * @return A non-zero, positive number of how many bytes were allocated.
 *         0    Failure in mmaping anonymous pages.
 */
static int __static_pages_allocate()
{

    // Allocate, on the heap, the necessary headers for keeping track of the anonymous pages.
    categories = (struct category *)malloc(sizeof(struct category) * STATIC_CATEGORY_SIZE);
    struct category* cat = categories;
    int protflags = PROT_READ | PROT_WRITE;
    int pageflags = MAP_ANONYMOUS | MAP_PRIVATE;
    int pages_alloc = 0;

    for (int i = 0; i < STATIC_CATEGORY_SIZE; cat++, i++) {
        cat->__init_pages_size = __categories_pages[i];
        cat->__first_page = mmap(NULL, cat->__init_pages_size * PAGE_SIZE, protflags, pageflags, -1, 0);

        // if the anonymous page mapping failed, then an error indicating value is returned.
        // errno should be read instead of cc_error in this case as mmap() sets errno on failure.
        if (cat->__first_page == MAP_FAILED) {
            __static_pages_free();
            return 0;
        }

        pages_alloc += cat->__init_pages_size;
    }

    return pages_alloc * PAGE_SIZE;
}

/**
 * Invalidates the loaded data for the specific static categories.
 * All operations on the invalidated static categories will result in a server update before proceeding.
 *
 * @param data The static categories to invalidate.
 */
void cchamp_static_invalidate(uint16_t data)
{

    /*
     * The corresponding anonymous pages are not affected by this invocation.
     * Anonymous pages are kept in memory throughout the whole life cycle of cchamp.
     */

    __static_pages_status(data, PAGE_STATUS_INVALIDATE);
}

/**
 * Loads the specified static data into memory.
 */
void cchamp_static_load(uint16_t data)
{
    // Invalidate all pages that are about to be updated to prevent access until the load is complete.
    cchamp_static_invalidate(data);
}

/**
 * Acquires memory for the storage of static data.
 *
 * @return  The size (in bytes) of memory allocated; or <br>
 *          0 if categories has already been allocated.
 */
int static_pages_allocate()
{
    if (categories != NULL) {

        // If categories is not null, then this function must have been previously invoked.
        // Simply terminating with no effect is sufficient.
        return 0;
    }

    return __static_pages_allocate();
}

/**
 * Frees up the allocated pages for the static data storage.
 */
void static_pages_free()
{
    if (categories != NULL) {
        __static_pages_free();
    }
}
