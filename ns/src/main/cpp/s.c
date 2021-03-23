#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h"
#include<android/log.h>

#define TAG "bc-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


// global jstring token
jstring token;

/**
 * 从com.bcoin.app.Cache类的getToken方法中取出当前调用的token
 * @param env   当前的env
 * @return      com.bcoin.app.Cache#getToken()方法的返回值
 */
char* getToken(JNIEnv* env){
    //找到class对象
    jclass ndkdemo_clazz = (*env)->FindClass(env, "com/bcoin/app/Cache");

    char* method_name = "<init>";
    char* method_sign = "()V";
    //构造方法
    jmethodID constructor_method_id = (*env)->GetMethodID(env, ndkdemo_clazz, method_name, method_sign);

    //创建对象
    jobject jobj = (*env)->NewObject(env, ndkdemo_clazz, constructor_method_id);

    //得到获取getToken的方法的id
    jmethodID methodid = (*env)->GetMethodID(env,ndkdemo_clazz,"getToken","()Ljava/lang/String;");

    //调用获取getToken的方法
    jstring token = (*env)->CallObjectMethod(env,jobj,methodid);

    //将jstring转换为char*类型
    char* c_token = (*env)->GetStringUTFChars(env, token,JNI_FALSE);

    //打印token
    //printf("token = %s",c_token);

    //删除对象(关键代码)
    (*env)->DeleteLocalRef(env,jobj);
    return c_token;
}

char* join(char *s1, char *s2){
    char *result = (char *) malloc(strlen(s1)+strlen(s2));//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    if (result == NULL) exit (1);

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

/**
 * md5 with salt
 * @param env
 * @param thiz
 * @param s
 * @return
 */
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_s(JNIEnv *env, jobject thiz, jstring s) {
    char *szText = (char *) (*env)->GetStringUTFChars(env, s, 0);
    char* tk = (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    char *payload = join(payload,tk);

//    MD5_CTX context = {0};
//    MD5Init(&context);
//    MD5Update(&context, payload, strlen(payload));
//    unsigned char dest[16] = {0};
//    MD5Final(dest, &context);

    // Free
    (*env)->ReleaseStringUTFChars(env, s, szText);
    (*env)->ReleaseStringUTFChars(env, token, tk);

//    int i = 0;
//    char szMd5[32] = {0};
//    for (i = 0; i < 16; i++) {
//        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
//    }
//    return (*env)->NewStringUTF(env, szMd5);
    return (*env)->NewStringUTF(env, payload);
}



JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_getStringX(JNIEnv *env, jobject thiz, jstring s) {
    //char* ch = getToken(env);
    char* ch = (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    //(*env)->ReleaseStringUTFChars(env, ch, s);
    return (*env)->NewStringUTF(env, ch);
}


// flush Token when user modified the username or password.
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_flushT(JNIEnv *env, jobject thiz, jstring s) {
    //const char* params = (*env)->GetStringUTFChars(env,s,0);
    // TODO: implement flushT()
    //创建全局对象
    token = (*env)->NewGlobalRef(env, s);
    //删除全局变量
    //(*env)->DeleteGlobalRef(env, token);
    return token;
}