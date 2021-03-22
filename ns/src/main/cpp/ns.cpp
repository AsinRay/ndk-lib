#include <jni.h>
#include <string>

using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++, Native so by Alex Lio";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_decrypt(JNIEnv *env, jobject thiz, jstring s) {

    // TODO: implement decrypt()

    const char* params = env->GetStringUTFChars(s,0);
    string sign_str(params);
    sign_str.insert(0,"asdfasdf");
    sign_str = sign_str.substr(0,sign_str.length()-2);
    env->ReleaseStringUTFChars(s,params);
    return env->NewStringUTF(sign_str.c_str());
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_bcoin_ns_DataProvider_byteMethod(JNIEnv *env, jobject thiz, jbyteArray i_byte) {
    // TODO: implement byteMethod()
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_bcoin_ns_DataProvider_getStruct(JNIEnv *env, jobject thiz) {
    // TODO: implement getStruct()
}extern "C"
JNIEXPORT jint JNICALL
Java_com_bcoin_ns_DataProvider_add__II(JNIEnv *env, jobject thiz, jint x, jint y) {
    // TODO: implement add()
}extern "C"
JNIEXPORT jint JNICALL
Java_com_bcoin_ns_DataProvider_sub(JNIEnv *env, jclass clazz, jint x, jint y) {
    // TODO: implement sub()
}extern "C"
JNIEXPORT jchar JNICALL
Java_com_bcoin_ns_DataProvider_add__CC(JNIEnv *env, jobject thiz, jchar x, jchar y) {
    // TODO: implement add()
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_DataProvider_sayHelloInC(JNIEnv *env, jobject thiz, jstring s) {
    // TODO: implement sayHelloInC()
}extern "C"
JNIEXPORT jintArray JNICALL
Java_com_bcoin_ns_DataProvider_intMethod(JNIEnv *env, jobject thiz, jintArray i_num) {
    // TODO: implement intMethod()
}