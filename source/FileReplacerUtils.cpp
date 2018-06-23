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

#include <vector>
#include <stdio.h>
#include <string.h>
#include <fswrapper/FileReplacerUtils.h>
#include <fswrapper/fs_async_wrapper.h>
#include <coreinit/mcp.h>

OSMessageQueue lfFSQueue __attribute__((section(".data")));
OSMessage lfFSQueueMessages[FS_QUEUE_MESSAGE_COUNT] __attribute__((section(".data")));

FSAsyncResult lfAsyncResultCache[ASYNC_RESULT_CACHE_SIZE] __attribute__((section(".data")));
uint8_t lfAsyncResultCacheLock __attribute__((section(".data"))) = 0;
uint8_t lfAsyncResultCacheCur __attribute__((section(".data"))) = 0;

FileReplacerUtils *FileReplacerUtils::instance = NULL;

int32_t setErrorFlag(int32_t error){
    int32_t result = error;
    if(error == -1){
        result = CHECKED_WITH_ALL_ERRORS;
    }else{
        result |= CHECKED_MASK;
    }
    return result;
}

int32_t checkErrorFlag(int32_t * error){
    if(*error == CHECKED_WITH_ALL_ERRORS){
        *error = -1;
        return true;
    }else if ((*error & CHECKED_MASK) == CHECKED_MASK){
        *error &= ~CHECKED_MASK;
        return true;
    }
    return false;
}

void FileReplacerUtils::StartAsyncThread(){
    int32_t priority = 15;
    this->pThread = CThread::create(DoAsyncThread, this , CThread::eAttributeAffCore2,priority);
    this->pThread->resumeThread();
}

void FileReplacerUtils::DoAsyncThread(CThread *thread, void *arg){
    FileReplacerUtils * arg_instance = (FileReplacerUtils *) arg;
    return arg_instance->DoAsyncThreadInternal();
}

void FileReplacerUtils::DoAsyncThreadInternal(){
    serverRunning = true;
    DEBUG_FUNCTION_LINE("FSAsyncServer started! \n");

    OSMessage message;
    void (*callback)(CustomAsyncParam *);
    CustomAsyncParam cParam;
    while(true){
        if(FS_WRAPPER_DEBUG_LOG){DEBUG_FUNCTION_LINE("Waiting for message\n");}
        if(!OSReceiveMessage(&fsFSQueue,&message,OS_MESSAGE_FLAGS_BLOCKING)){
            OSSleepTicks(OSMicrosecondsToTicks(1000));
            continue;
        }
        if(FS_WRAPPER_DEBUG_LOG){DEBUG_FUNCTION_LINE("Received message %08X\n",message.message);}
        if((uint32_t)message.message == 0xDEADBEEF){
            DEBUG_FUNCTION_LINE("We should stop the server\n");
            break;
        }

        callback = (void(*)(CustomAsyncParam *))message.message;
        CustomAsyncParam * param = (CustomAsyncParam *)message.args[0];
        memcpy(&cParam,param,sizeof(CustomAsyncParam));
        if(FS_WRAPPER_DEBUG_LOG){DEBUG_FUNCTION_LINE("Calling callback at %08X, with %08X\n",callback,&cParam);}
        callback(&cParam);
    }
    serverRunning = false;
}

FSAsyncResult * FileReplacerUtils::getNewAsyncParamPointer(){
    while(fsAsyncResultCacheLock){
        OSSleepTicks(OSMicrosecondsToTicks(100));
    }
    fsAsyncResultCacheLock = 1;

    if(fsAsyncResultCacheCur >= ASYNC_RESULT_CACHE_SIZE){
        fsAsyncResultCacheCur = 0;
    }

    FSAsyncResult *result = &fsAsyncResultCache[fsAsyncResultCacheCur++];

    fsAsyncResultCacheLock = 0;
    return result;
}

void FileReplacerUtils::sendAsyncCommand(FSClient * client, FSCmdBlock * cmd,FSAsyncData* asyncParams,FSStatus status){
    if(asyncParams != NULL){
        if(asyncParams->callback != NULL){ //Using the userCallback
            if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("userCallback %08X param %08X\n",asyncParams->callback,asyncParams->param); }
            asyncParams->callback(client,cmd,status,asyncParams->param);
            return;
        }else if (asyncParams->ioMsgQueue != NULL){
            if(FS_WRAPPER_DEBUG_LOG){ DEBUG_FUNCTION_LINE("asyncParams->ioMsgQueue %08X \n",asyncParams->ioMsgQueue); }
            FSAsyncResult * result = FileReplacerUtils::getNewAsyncParamPointer();
            FSMessage message;
            result->asyncData.callback = asyncParams->callback;
            result->asyncData.param = asyncParams->param;
            result->asyncData.ioMsgQueue = asyncParams->ioMsgQueue;
            memset(&result->ioMsg,0,sizeof(result->ioMsg));
            result->ioMsg.data = &result;
            result->ioMsg.type = OS_FUNCTION_TYPE_FS_CMD_ASYNC;
            result->client = client;
            result->block = cmd;
            result->status = status;

            memset(&message,0,sizeof(message));

            message.data = (void*)result;
            message.type = OS_FUNCTION_TYPE_FS_CMD_ASYNC;

            OSSendMessage(asyncParams->ioMsgQueue,(OSMessage*)&message,OS_MESSAGE_FLAGS_BLOCKING);
        }
    }
}
