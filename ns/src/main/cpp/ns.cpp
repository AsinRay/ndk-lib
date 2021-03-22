#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++, Native so by Alex Lio";
    return env->NewStringUTF(hello.c_str());
}