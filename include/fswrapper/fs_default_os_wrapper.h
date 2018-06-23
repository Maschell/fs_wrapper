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

#ifndef __FS_DEFAULT_OS_WRAPPER_H_
#define __FS_DEFAULT_OS_WRAPPER_H_

#include "fs_async_wrapper.h"
#include "fs_sync_wrapper.h"
#include "fs_wrapper_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t fs_default_os_wrapper_FSCloseFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int32_t fd, int32_t error, FSAsyncData * asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSGetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int32_t fd, int32_t *pos, int32_t error, FSAsyncData * asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSGetStatAsync(const char *libpath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, FSStat *stats, int32_t error, FSAsyncData * asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSGetStatFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int32_t fd, FSStat * stats, int32_t error, FSAsyncData * asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSIsEofAsync(FSClient *pClient, FSCmdBlock *pCmd, int32_t fd, int32_t error, FSAsyncData * asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSOpenFileAsync(const char *libpath, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int32_t *handle, int32_t error, FSAsyncData *asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSReadFileAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int32_t size, int32_t count, int32_t fd, int32_t flag, int32_t error, FSAsyncData *asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSReadFileWithPosAsync(FSClient *pClient, FSCmdBlock *pCmd, void *buffer, int32_t size, int32_t count, uint32_t pos, int32_t fd, int32_t flag, int32_t error, FSAsyncData *asyncParams, void * realAddress);
int32_t fs_default_os_wrapper_FSSetPosFileAsync(FSClient *pClient, FSCmdBlock *pCmd, int32_t handle, uint32_t pos, int32_t error, FSAsyncData *asyncParams, void * realAddress);

#ifdef __cplusplus
}
#endif

#endif // __FS_DEFAULT_OS_WRAPPER_H_
