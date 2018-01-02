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
#include <stdlib.h>
#include <string.h>
#include <cchamp_utils.h>

/**
 * Acquires the index of the set bit by shifting the bits until the value becomes zero.
 * Intended Usage of this method goes with the assumption that the value you provide only has one
 * bit set.
 *
 * @param val   A 16-bit value that presumably has one bit on.
 *
 * @return The bit index in the value.
 */
char get_bit_index(uint16_t val)
{
    char index = 0;
    while ((val >>= 1) != 0) {
        index++;
    }

    return index;
}


/**
 * Replaces all illegal web strings (i.e. spaces) to their safe equivalent.
 * Always remember to free the returned string after usage is complete.
 *
 * @param unsafe A potentially web unsafe string.
 */
char *get_web_safe_str(char *unsafe)
{
    int len = strlen(unsafe);

    // I allocate len * 3 because at worst case, the whole string could be just spaces
    // and it would require len * 3 bytes to satisfy the translation.
    char *web_safe_str = calloc(1, len * 3);

    for (int i = 0, j = 0; i < len; i++) {
        if (unsafe[i] == ' ') {
            web_safe_str[j++] = '%';
            web_safe_str[j++] = '2';
            web_safe_str[j++] = '0';
        } else {
            web_safe_str[j++] = unsafe[i];
        }
    }

    return web_safe_str;
}
