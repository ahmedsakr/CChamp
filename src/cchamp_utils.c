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
#include "cchamp_utils.h"


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
    while ((val >>= 1)) {
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
void webstr(char *dest, char *unsafe)
{
    int len = strlen(unsafe);

    // I allocate len * 3 because at worst case, the whole string could be just spaces
    // and it would require len * 3 bytes to satisfy the translation.
    char safe[len * 3];
    int i = 0, j = 0;
    for (int i = 0; i < len; i++) {
        if (unsafe[i] == ' ') {
            safe[j++] = '%';
            safe[j++] = '2';
            safe[j++] = '0';
        } else {
            safe[j++] = unsafe[i];
        }
    }

    // Must null-terminate the local string to guarantee that a strlen() call would return
    // the correct size of the string.
    safe[j] = 0x00;
    memcpy(dest, safe, strlen(safe));
}
