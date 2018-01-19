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
 * @param op        The requested config operation. (0 for clear, 1 for set)
 */
void cchamp_config_set(uint16_t config, char op)
{
    // Value must be either 0 or 1.
    if (op & 0xFE != 0) return;

    if (op == 0) {
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

    /*
     * Only one bit is allowed in the config parameter.
     * This checks if config is a power-of-two (i.e. only one bit is set).
     *
     * If it is not power-of-two, then 0 is returned.
     */
    if (config & (config - 1)) return 0;

    return settings & config;
}
