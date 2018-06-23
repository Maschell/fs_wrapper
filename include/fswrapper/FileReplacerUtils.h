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

#ifndef __FILE_REPLACER_UTILS_H_
#define __FILE_REPLACER_UTILS_H_

#include <vector>
#include <algorithm>

#include <system/CThread.h>
#include <coreinit/mcp.h>
#include <utils/logger.h>

#include "fs_retain_vars.h"

#define CHECKED_WITH_ALL_ERRORS     0x10000
#define CHECKED_MASK                0x01000

#define FS_WRAPPER_DEBUG_LOG        0

#define USE_OS_FS_FUNCTION          -1337


/**
Returns a modified error flag.
This will be used to save the information if a file/handle was already
tried to be patched.
The non-async function internally call the async functions, and this way
we avoid testing it twice.
If the result contains our mask, we just straight to the OS functions.
**/
int32_t setErrorFlag(int32_t error);

/**
Check if we already checked the file/handle.
Returns true if it was already checked (+ revert the error)
Return false if it should be (tried) to be patched.
**/
int32_t checkErrorFlag(int32_t * error);

class FileReplacerUtils{
    public:
        static FileReplacerUtils * getInstance() {
            if(!instance){
                instance = new FileReplacerUtils();
            }

            return instance;
        }

        static void destroyInstance() {
            if(instance){
                instance->StopAsyncThread();
                while(instance->serverRunning){
                    OSSleepTicks(OSMicrosecondsToTicks(1000));
                }
                OSSleepTicks(OSMicrosecondsToTicks(10000));
                delete instance;
                instance = NULL;
            }
        }

        static void addFileHandle(int32_t handle){
            getInstance()->addFileHandleInternal(handle);
        }

        static void removeFileHandle(int32_t handle){
            getInstance()->removeFileHandleInternal(handle);
        }

        static bool hasFileHandle(int32_t handle){
            return getInstance()->hasFileHandleInternal(handle);
        }

        static void addDirHandle(int32_t handle){
            getInstance()->addDirHandleInternal(handle);
        }

        static void removeDirHandle(int32_t handle){
            getInstance()->removeDirHandleInternal(handle);
        }

        static bool hasDirHandle(int32_t handle){
            return getInstance()->hasDirHandleInternal(handle);
        }

        static bool addFSQueueMSG(OSMessage* message){
            OSSendMessage(&fsFSQueue,message,OS_MESSAGE_FLAGS_BLOCKING);
            return true;
        }

        static bool setGroupAndOwnerID(){
            int32_t mcpHandle = MCP_Open();
            if(mcpHandle != 0)
            {
                MCPTitleListType titleInfo;
                memset(&titleInfo, 0, sizeof(titleInfo));

                MCP_GetOwnTitleInfo(mcpHandle, &titleInfo);
                MCP_Close(mcpHandle);
                uint32_t * test = (uint32_t*)&titleInfo;
                global_owner_id = test[1];
                global_group_id = test[2];
                DEBUG_FUNCTION_LINE("Set group_id to %08X and owner_id to %08X\n",global_group_id,global_owner_id);
                return true;
            }
            return false;
        }

        static void sendAsyncCommand(FSClient * client, FSCmdBlock * cmd,FSAsyncData* asyncParams,FSStatus status);

        void StartAsyncThread();

        void StopAsyncThread(){
            DEBUG_FUNCTION_LINE("StopAsyncThread\n");
            OSMessage message;
            message.message = (void*) 0xDEADBEEF;
            while(!OSSendMessage(&fsFSQueue,&message,OS_MESSAGE_FLAGS_NONE));
        }

	private:
	    FileReplacerUtils(){
            OSInitMessageQueue(&fsFSQueue, fsFSQueueMessages, FS_QUEUE_MESSAGE_COUNT);
            DEBUG_FUNCTION_LINE("Init queue done! \n");
	    }

        void addFileHandleInternal(int32_t handle){
            filehandles.push_back(handle);
        }

        void removeFileHandleInternal(int32_t handle){
            filehandles.erase(std::remove(filehandles.begin(), filehandles.end(), handle), filehandles.end());
        }

        bool hasFileHandleInternal(int32_t handle){
            if(std::find(filehandles.begin(), filehandles.end(), handle) != filehandles.end()) {
                return true;
            } else {
                return false;
            }
        }

        void addDirHandleInternal(int32_t handle){
            dirhandles.push_back(handle);
        }

        void removeDirHandleInternal(int32_t handle){
            dirhandles.erase(std::remove(dirhandles.begin(), dirhandles.end(), handle), dirhandles.end());
        }

        bool hasDirHandleInternal(int32_t handle){
            if(std::find(dirhandles.begin(), dirhandles.end(), handle) != dirhandles.end()) {
                return true;
            } else {
                return false;
            }
        }

        static void DoAsyncThread(CThread *thread, void *arg);

        static FSAsyncResult * getNewAsyncParamPointer();

        void DoAsyncThreadInternal();

        CThread *pThread;

        volatile bool serverRunning = false;

	    std::vector<int32_t> filehandles;
	    std::vector<int32_t> dirhandles;
        static FileReplacerUtils * instance;
};
#endif // __FILE_REPLACER_UTILS_H_
