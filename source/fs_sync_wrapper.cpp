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

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/logger.h>

#include <fswrapper/fs_sync_wrapper.h>
#include <fswrapper/fs_retain_vars.h>

int32_t fs_wrapper_FSCloseFile(int32_t handle){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! for handle: %08X \n",handle); }
    if(FileReplacerUtils::hasFileHandle(handle)){
        FileReplacerUtils::removeFileHandle(handle);
        close(handle);
        DEBUG_FUNCTION_LINE("closed handle %08X\n",handle);
        return FS_STATUS_OK;
    }
    return USE_OS_FS_FUNCTION;
}

int32_t fs_wrapper_FSGetPosFile(int32_t handle,int32_t * pos){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! for handle: %08X \n",handle); }
    if(FileReplacerUtils::hasFileHandle(handle)){
        off_t currentPos = lseek(handle, (off_t)0, SEEK_CUR);
        *pos = currentPos;

        DEBUG_FUNCTION_LINE("pos %08X for handle %08X\n",*pos,handle);

        return FS_STATUS_OK;
    }
    return USE_OS_FS_FUNCTION;
}

void setCommonStats(FSStat * stats){
    if(stats == NULL){
        DEBUG_FUNCTION_LINE("STATS EMPTY\n");
    }
    stats->mode = FS_MODE_READ_OWNER;
    stats->owner = global_owner_id;
    stats->group = global_group_id;
    stats->flags = (FSStatFlags) ((uint32_t) FS_STAT_DIRECTORY | 0x04000000);
    stats->created = 0x0003E8C3E677A740L;
    stats->modified = 0x0003F8AABBEAFBC0L;
}

int32_t fs_wrapper_FSGetStat(const char * path, FSStat * stats){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called!\n"); }
    int32_t result = USE_OS_FS_FUNCTION;
    if(path != NULL){
        DEBUG_FUNCTION_LINE("Searching for path %s\n",path);
        struct stat path_stat;
        if(stat(path, &path_stat) < 0){
            result = USE_OS_FS_FUNCTION;
            if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("failed for path %s\n",path); }
        }else{
            DEBUG_FUNCTION_LINE("success! path %s\n",path);
            //stats->flags = 0;
            memset(&(stats->flags),0,sizeof(stats->flags));
            if(S_ISDIR(path_stat.st_mode)){
                stats->flags = (FSStatFlags) ((uint32_t) FS_STAT_DIRECTORY | (uint32_t) stats->flags); ;
                DEBUG_FUNCTION_LINE("set stats->flag: DIR\n");
            }else{
                stats->size = path_stat.st_size;
                DEBUG_FUNCTION_LINE("stats->size: %08X\n",stats->size);
            }

            setCommonStats(stats);

            result = FS_STATUS_OK;
        }
    }
    return result;
}

int32_t fs_wrapper_FSGetStatFile(int32_t handle, FSStat * stats){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! for handle: %08X\n",handle); }
    if(FileReplacerUtils::hasFileHandle(handle)){
        struct stat path_stat;
        if(fstat(handle, &path_stat) < 0){
            DEBUG_FUNCTION_LINE("failed! handle: %08X\n",handle);
            return -1;
        }

        stats->size = path_stat.st_size;
        memset(&(stats->flags),0,sizeof(stats->flags));

        setCommonStats(stats);

        DEBUG_FUNCTION_LINE("success! handle: %08X size: %08X\n",handle,stats->size);

        return FS_STATUS_OK;
    }
    return USE_OS_FS_FUNCTION;
}

int32_t fs_wrapper_FSIsEof(int32_t handle){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! handle: %08X\n",handle); }
    int32_t result = USE_OS_FS_FUNCTION;
    if(FileReplacerUtils::hasFileHandle(handle)){
        off_t currentPos = lseek(handle, (off_t) 0, SEEK_CUR);
        off_t endPos = lseek(handle, (off_t) 0, SEEK_END);

        if(currentPos == endPos){
            result = FS_STATUS_END;
        }else{
            lseek(handle, currentPos, SEEK_CUR);
            result = FS_STATUS_OK;
        }
        DEBUG_FUNCTION_LINE("handle: %08X result: %08X\n",handle,result);
    }
    return result;
}

int32_t fs_wrapper_FSOpenFile(const char * path, const char * mode, int32_t * handle){
    int32_t result = USE_OS_FS_FUNCTION;
    if(path != NULL){
        result = USE_OS_FS_FUNCTION;
        DEBUG_FUNCTION_LINE("Searching for path %s\n",path);
        int32_t fd = open(path,O_RDONLY); //TODO: remove hardcoded mode.
        if(fd != -1){
            DEBUG_FUNCTION_LINE("opened path: %s handle: %08X\n",path,fd);
            FileReplacerUtils::addFileHandle(fd);
            *handle = fd;
            result = FS_STATUS_OK;

        }else{
            if(FS_WRAPPER_DEBUG_LOG){DEBUG_FUNCTION_LINE("failed path: %s\n",path);}
        }
    }

    return result;
}

#define MAXIMUM_READ_CHUNK 1024*1024

static int32_t readIntoBuffer(int32_t handle,void *buffer,size_t size, size_t count){
    int32_t sizeToRead = size*count;
    void * newBuffer = buffer;
    int32_t curResult = -1;
    int32_t totalSize = 0;
    int32_t toRead = 0;
    while(sizeToRead > 0){
        if(sizeToRead < MAXIMUM_READ_CHUNK){
            toRead = sizeToRead;
        }else{
            toRead = MAXIMUM_READ_CHUNK;
        }
        curResult = read(handle, newBuffer,toRead);
        if(curResult < 0){
            DEBUG_FUNCTION_LINE("Error: Reading %08X bytes from handle %08X. result %08X \n",size*count,handle,curResult);
            return -1;
        }
        if(curResult == 0 ){
            //EOF
            break;
        }
        newBuffer = (void*)(((uint32_t)newBuffer) + curResult);
        totalSize += curResult;
        sizeToRead -= curResult;
        if(sizeToRead > 0){
            DEBUG_FUNCTION_LINE("Reading. missing %08X bytes\n",sizeToRead);
        }
    }
    DEBUG_FUNCTION_LINE("Success: Read %08X bytes from handle %08X. result %08X \n",size*count,handle,totalSize);
    return totalSize;
}

int32_t fs_wrapper_FSReadFile(int32_t handle,void *buffer,size_t size, size_t count){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! for handle: %08X \n",handle); }
    int32_t result = USE_OS_FS_FUNCTION;
    if(FileReplacerUtils::hasFileHandle(handle)){
        result = readIntoBuffer(handle,buffer,size,count);
    }
    return result;
}

int32_t fs_wrapper_FSReadFileWithPos(void *buffer, size_t size, size_t count, uint32_t pos, int32_t handle){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! \n"); }
    int32_t result = USE_OS_FS_FUNCTION;
    if(FileReplacerUtils::hasFileHandle(handle)){
        off_t newOffset = -1;
        newOffset = lseek(handle, (off_t)pos, SEEK_SET);
        if(newOffset == (off_t)pos){
            result = readIntoBuffer(handle, buffer,size,count);
            DEBUG_FUNCTION_LINE("Reading %08X bytes from handle %08X at pos %08X. result %08X \n",size*count,handle,pos,result);
        }else{
            return -1;
        }
    }
    return result;
}

int32_t fs_wrapper_FSSetPosFile(int32_t handle,uint32_t pos){
    if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("Called! \n"); }
    int32_t result = USE_OS_FS_FUNCTION;
    if(FileReplacerUtils::hasFileHandle(handle)){
        off_t newOffset = -1;
        result = -1;
        newOffset = lseek(handle, (off_t)pos, SEEK_SET);
        if(newOffset == (off_t)pos){
            result = FS_STATUS_OK;
            DEBUG_FUNCTION_LINE("Set position to %08X for handle %08X\n",pos,handle);
        }else{
            DEBUG_FUNCTION_LINE("Failed set position to %08X for handle %08X\n",pos,handle);
            return FS_STATUS_OK;
        }
    }
    return result;
}
