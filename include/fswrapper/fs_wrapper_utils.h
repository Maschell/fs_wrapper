/****************************************************************************
 * Copyright (C) 2017 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#ifndef __FS_WRAPPER_UTILS_H_
#define __FS_WRAPPER_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

    int32_t fallbackFSCloseFileAsync(void * fallbackparams);

    int32_t fallbackFSGetPosFileAsync(void * fallbackparams);

    int32_t fallbackFSGetStatAsync(void * fallbackparams);

    int32_t fallbackFSGetStatFileAsync(void * fallbackparams);

    int32_t fallbackFSFSGetPosFileAsync(void * fallbackparams);

    int32_t fallbackFSIsEofAsync(void * fallbackparams);

    int32_t fallbackFSOpenFileAsync(void * fallbackparams);

    int32_t fallbackFSReadFileAsync(void * fallbackparams);

    int32_t fallbackFSReadFileWithPosAsync(void * fallbackparams);

    int32_t fallbackFSSetPosFileAsync(void * fallbackparams);

#ifdef __cplusplus
}
#endif

#endif // __FS_WRAPPER_UTILS_H_
