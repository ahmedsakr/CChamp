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
#ifndef CCHAMP_STATIC_H
#define CCHAMP_STATIC_H
#include <inttypes.h>

// The number of data categories accessible by the static API.
#define STATIC_CATEGORY_SIZE 10

/*
 * When cchamp_init() is called, it is expected to initialize a category struct for all categories present in
 * the static API.
 *
 * A category struct is in charge of:
 *  - declaring how many pages are to be allocated for the specifc category
 *  - Store the pointer to the first page of data.
 */
struct category {
    int     __init_pages_size;
    void*   __first_page;
};


/*
 * Categories should be initialized contiguously and in the manner defined by the STATIC_* constants,
 * respective to the bit index (see get_bit_index() in <cchamp_utils.c>).
 *
 * Under this assumption, checking of the data integrity for any category is quick.
 */
extern struct category* categories;

/*
 * Thanks to the contiguous and respective manner assumption, accessing the first data page ptr is constant time.
 *
 * For GET_FIRST_PAGE, a NULL ptr is expected if the categories bit for the specific category flag is cleared.
 */
#define GET_CATEGORY(category)      (categories + get_bit_index(category))
#define GET_FIRST_PAGE(category)    (void *)(GET_CATEGORY(category)->__first_page)

/*
 * There needs to be a quick way for determining if a category's pages have been initialized from the server.
 * Using the STATIC_* constants defined in <cchamp/cchamp.h>, You may read this variable's bits to determine
 * if the data is valid or not.
 *
 * A set (1) bit indicates data for the specific category is initialized, otherwise a cleared bit means the
 * data in the category's pages is either invalidated or never loaded in the first space.
 */
extern uint16_t __static_data_flags;


/*
 * Should only be called by cchamp_init() and no where else.
 *
 * This function is in charge of mmaping all necessary pages for the static API.
 */
int __static_pages_allocate();


/*
 * Should only be called by cchamp_close() and no where else.
 * If you wish to free specific static category pages, invoke cchamp_static_invalidate() instead.
 *
 * This function munmaps() all anonymously mmaped pages for static categories.
 */
void __static_pages_free();

#endif
