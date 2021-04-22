//
// 保护装置
// protected code.
// Created by boot on 4/20/21.
//
//

#include <string>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include "c.h"


void attached_check()
{
    try
    {
        const int bufsize = 1024;
        char filename[bufsize];
        char line[bufsize];
        int pid = getpid();
        sprintf(filename, "/proc/%d/status", pid);
        FILE* fd = fopen(filename, "r");
        if (fd != nullptr)
        {
            while (fgets(line, bufsize, fd))
            {
                if (strncmp(line, "TracerPid", 9) == 0)
                {
                    int statue = atoi(&line[10]);
                    //LOGD("%s", line);
                    if (statue != 0)
                    {
                        LOGI("attached !! kill %d", pid);

                        // 在杀死之前先关闭文件
                        fclose(fd);
                        //int ret = kill(pid, SIGKILL);
                    }
                    break;
                }
            }
            fclose(fd);
        } else
        {
            LOGD("open %s fail...", filename);
        }
    } catch (...)
    {

    }

}

//检查线程, 每秒检查一下
void thread_task(int n)
{
    while (true)
    {
        LOGD("start attached_check...");
        attached_check();
        std::this_thread::sleep_for(std::chrono::seconds(n));
    }
}

//void anti_debug()
void ad()
{
    // ptrace(PTRACE_TRACEME, 0, 0, 0);
    auto checkThread = std::thread(thread_task, 10);
    checkThread.detach();
}


/**
 *
 * Java调用System.loadLibrary()加载一个库的时候，会首先在库中搜索JNI_OnLoad()函数，如果该函数存在，则执行它；
 * JNI_OnLoad()的作用主要有几点：
 * 1、告诉JVM，这个库需要要求使用的JNI版本是什么
 * 2、执行初始化操作
 * 3、将JavaVM参数保存为全局对象，方便以后在任何地方获取JNIEnv对象（后面会介绍）
 */

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    ad();
    JNIEnv* env;
    if (jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return -1;
    }
    return JNI_VERSION_1_6;
}

/**
 * According to the JNI documentation, it will be called w
 * hen the classloader that loaded the libarary is garbage collected.
 * 根据JNI文档的描述，当GC回收了加载这个库的ClassLoader时，该函数被调用
 */
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    //TODO-
}
