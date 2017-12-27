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
#include <cchamp_config.h>

uint16_t settings;


/**
 * Overrides the current specified setting.
 *
 * @param config    The selected setting to be modified.
 * @param value     The new value of the setting (must be 0 or 1).
 */
void cchamp_config_set(uint16_t config, char value)
{
    // value must either be 0 or 1
    if (value & 0xFE != 0) return;

    if (value == 0) {
        settings &= ~config;
    } else {
        settings |= config;
    }
}


/**
 * Acquires the installed value for the requested setting.
 *
 * @param config The requested setting.
 *
 * @return The value of the setting.
 */
char cchamp_config_get(uint16_t config)
{
    return (settings & config) != 0 ? 1 : 0;
}
