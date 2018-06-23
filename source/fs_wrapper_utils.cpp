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
#include <utils/logger.h>

#include <fswrapper/fs_wrapper_utils.h>
#include <fswrapper/fs_async_wrapper.h>

typedef int32_t (*FallbackHelperInternal)(FallbackParamWrapper*, OSAsyncParamWrapper *);

static int32_t fallbackCaller(FallbackHelperInternal _function, void * fallbackparams){
    FallbackParamWrapper * paramsWrapper = (FallbackParamWrapper *)fallbackparams;
    OSAsyncParamWrapper * params = &(paramsWrapper->params);

    if(paramsWrapper->realFunctionAddress == NULL){ return FS_STATUS_FATAL_ERROR; } // OSFatal("paramsWrapper->realFunctionAddress was NULL;");}
    return _function(paramsWrapper,params);
}

static int32_t fallbackFSCloseFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSCloseFileAsyncCallback)(FSClient *, FSCmdBlock *, int32_t, int32_t, FSAsyncData *);
    real_FSCloseFileAsyncCallback = (int32_t(*)(FSClient *, FSCmdBlock *, int32_t, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSCloseFileAsyncCallback(params->pClient, params->pCmd, params->handle, params->error,params->asyncParams);
    return res;
}

static int32_t fallbackFSGetPosFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSGetPosFileAsyncFunc)(FSClient *, FSCmdBlock *, int32_t, int32_t *, int32_t, FSAsyncData *);
    real_FSGetPosFileAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, int32_t, int32_t *, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSGetPosFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->posPtr, params->error,params->asyncParams);
    return res;
}

static int32_t fallbackFSGetStatAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSGetStatAsyncFunc)(FSClient *, FSCmdBlock *, const char *, FSStat *, int32_t, FSAsyncData *);
    real_FSGetStatAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, const char *, FSStat *, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSGetStatAsyncFunc(params->pClient, params->pCmd, params->path, params->stats, params->error,params->asyncParams);
    return res;
}

static int32_t fallbackFSGetStatFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSGetStatFileAsyncFunc)(FSClient *, FSCmdBlock *, int32_t, FSStat *, int32_t, FSAsyncData *);
    real_FSGetStatFileAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, int32_t, FSStat *, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSGetStatFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->stats, params->error,params->asyncParams);
    return res;
}

static int32_t fallbackFSIsEofAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSIsEofAsyncFunc)(FSClient *, FSCmdBlock *, int32_t, int32_t, FSAsyncData *);
    real_FSIsEofAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, int32_t, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSIsEofAsyncFunc(params->pClient, params->pCmd, params->handle, params->error,params->asyncParams);
    return res;
}

static int32_t fallbackFSOpenFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSOpenFileAsyncCallback)(FSClient *, FSCmdBlock *, const char *, const char *, int32_t *, int32_t, FSAsyncData *);
    real_FSOpenFileAsyncCallback = (int32_t(*)(FSClient *, FSCmdBlock *, const char *, const char *, int32_t *, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSOpenFileAsyncCallback(params->pClient, params->pCmd, params->path, params->mode, params->handlePtr, params->error, params->asyncParams);
    return res;
}

static int32_t fallbackFSReadFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSReadFileAsyncFunc)(FSClient *, FSCmdBlock *, void *, int32_t, int32_t, int32_t, int32_t, int32_t, FSAsyncData *);
    real_FSReadFileAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, void *, int32_t, int32_t, int32_t, int32_t, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSReadFileAsyncFunc(params->pClient, params->pCmd, params->buffer, params->size, params->count, params->handle, params->flag, params->error, params->asyncParams);
    return res;
}

static int32_t fallbackFSReadFileWithPosAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSReadFileWithPosAsyncFunc)(FSClient *, FSCmdBlock *, void *, int32_t, int32_t, uint32_t, int32_t, int32_t, int32_t, FSAsyncData *);
    real_FSReadFileWithPosAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, void *, int32_t, int32_t, uint32_t, int32_t, int32_t, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSReadFileWithPosAsyncFunc(params->pClient, params->pCmd, params->buffer, params->size, params->count, params->pos, params->handle, params->flag, params->error, params->asyncParams);
    return res;
}

static int32_t fallbackFSSetPosFileAsyncInternal(FallbackParamWrapper * paramsWrapper, OSAsyncParamWrapper * params){
    int32_t (*real_FSSetPosFileAsyncFunc)(FSClient *, FSCmdBlock *, int32_t, uint32_t, int32_t, FSAsyncData *);
    real_FSSetPosFileAsyncFunc = (int32_t(*)(FSClient *, FSCmdBlock *, int32_t, uint32_t, int32_t, FSAsyncData *)) paramsWrapper->realFunctionAddress;
    int32_t res = real_FSSetPosFileAsyncFunc(params->pClient, params->pCmd, params->handle, params->pos, params->error, params->asyncParams);
    return res;
}

#define DEFINE_FS_FALLBACK_CALLER(name) \
        int32_t fallback##name(void * fallbackparams){\
            return fallbackCaller(fallback##name##Internal,fallbackparams);\
        }

DEFINE_FS_FALLBACK_CALLER(FSCloseFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetStatAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetStatFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSGetPosFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSIsEofAsync)
DEFINE_FS_FALLBACK_CALLER(FSOpenFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSReadFileAsync)
DEFINE_FS_FALLBACK_CALLER(FSReadFileWithPosAsync)
DEFINE_FS_FALLBACK_CALLER(FSSetPosFileAsync)
