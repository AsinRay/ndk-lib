#include <jni.h>
#include <string>
#include "md5.h"

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    string hello = "Hello from C++, Native so by Alex Lio";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_decrypt(JNIEnv *env, jobject thiz, jstring s) {

    // TODO: implement decrypt()
    int values[] {1,2,3};
    const char* params = env->GetStringUTFChars(s,0);
    string sign_str(params);
    sign_str.insert(0,"asdfasdf");
    sign_str = sign_str.substr(0,sign_str.length()-2);
    env->ReleaseStringUTFChars(s,params);
    return env->NewStringUTF(sign_str.c_str());
}


