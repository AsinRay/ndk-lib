#include "includes.h"
#include "md5.h"
#include "c.h"
#include "base64.h"
#include "aes256.h"





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
/**
 * C的字节数组转为jstring
 * @param envPtr
 * @param src
 * @return
 */
/*jstring charToJstring(JNIEnv *env, char *src) {
    jsize len = strlen(src);
    // 定义java String类 strClass
    jclass jClz = (*env)->FindClass(env, "java/lang/String");
    // 获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorId = (*env)->GetMethodID(env, jClz, "<init>", "([BLjava/lang/String;)V");
    // 建立byte数组
    jbyteArray jbArr = (*env)->NewByteArray(env, len);
    // 将char* 转换为byte数组
    (*env)->SetByteArrayRegion(env, jbArr, 0, len, (jbyte*) src);
    //设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (*env)->NewStringUTF(env, "UTF-8");
    //将byte数组转换为java String,并输出
    return (jstring) (*env)->NewObject(env, jClz, ctorId, jbArr, encoding);
}*/

jstring charToJstring(JNIEnv *env, const char *pat) {
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>","([BLjava/lang/String;)V");
    jbyteArray bytes = (*env)->NewByteArray(env, strlen(pat));
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

char* join(char *s1, char *s2){
    // need free
    char *result = (char *) malloc(strlen(s1)+strlen(s2));
    //in real code you would check for errors in malloc here
    if (result == NULL) exit (1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


//jstring decrypt(JNIEnv* env, jstring miwen) {
//    jstring result;
//    //wojiubugaosunimenglalisadeyanlei 秘钥
//    unsigned char key[32] = {0x77,0x6f,0x6a,0x69,
//                             0x75,0x62,0x75,0x67,
//                             0x61,0x6f,0x73,0x75,
//                             0x6e,0x69,0x6d,0x65,
//                             0x6e,0x67,0x6c,0x61,
//                             0x6c,0x69,0x73,0x61,
//                             0x64,0x65,0x79,0x61,
//                             0x6e,0x6c,0x65,0x69};
//
//    //*********** 开始解密 ************
//    //1.初始化数据
//    //初始化向量 nglalisadeyanlei
////    uint8_t iv[16] = { 0x6e,0x67,0x6c,0x61,
////                       0x6c,0x69,0x73,0x61,
////                       0x64,0x65,0x79,0x61,
////                       0x6e,0x6c,0x65,0x69 };
//
//    uint8_t iv[32] = {0x77,0x6f,0x6a,0x69,
//                             0x75,0x62,0x75,0x67,
//                             0x61,0x6f,0x73,0x75,
//                             0x6e,0x69,0x6d,0x65,
//                             0x6e,0x67,0x6c,0x61,
//                             0x6c,0x69,0x73,0x61,
//                             0x64,0x65,0x79,0x61,
//                             0x6e,0x6c,0x65,0x69};
//    aes256_context ctx;
//    aes256_init(&ctx, key);
//
//    //2.将jstring转为char
//    const char *mwChar = (*env)->GetStringUTFChars(env, miwen, JNI_FALSE);
//    char *enc = base64_decode(mwChar, strlen(mwChar));
//    uint8_t output[4096];
//    aes256_decrypt_cbc(&ctx, (unsigned char *) enc, iv, output);
//    int size = strlen((const char *) output);
//    LOGD("output size=%d",size);
//    int i;
//    for(i=0;i<size;i++){
//        //LOGD("cha %d = %c",i,output[i]);
//        if(output[i]>=1&&output[i]<=16){
//            output[i] = 0;
//        }
//    }
//    result = charToJstring(env, (char *) output);
//    //LOGD("result=%s",(char *) output);
//    free(enc);
//    //释放mwChar
//    (*env)->ReleaseStringUTFChars(env, miwen, mwChar);
//    aes256_done(&ctx);
//    return result;
//}


/**
 * md5 with salt
 * @param env
 * @param thiz
 * @param s
 * @return
 */
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_s(JNIEnv *env, jobject thiz, jstring s) {
    //char* szText = (char *) (*env)->GetStringUTFChars(env, s, 0);
    char* szText = (*env)->GetStringUTFChars(env, s, JNI_FALSE);
    char* tk =     (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    char *payload = join(szText,tk);

    MD5_CTX context = {0};
    MD5Init(&context);
    MD5Update(&context, payload, strlen(payload));
    unsigned char dest[16] = {0};
    MD5Final(dest, &context);

    // Free
    (*env)->ReleaseStringUTFChars(env, s, szText);
    (*env)->ReleaseStringUTFChars(env, token, tk);
    free(payload);
    payload = NULL;

    int i = 0;
    char szMd5[32] = {0};

    for (i = 0; i < 16; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    LOGI("%s", szMd5);
    return (*env)->NewStringUTF(env, szMd5);
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

JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_test(JNIEnv *env, jobject thiz, jstring s) {
    // TODO: implement test()

    char* ch = (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    int data_len = strlen(ch);
    //char *rtn = base64_encode(ch,data_len);
    //(*env)->ReleaseStringUTFChars(env, s, ch);

    char * xx = "5oiR5piv5Lit5Zu95Lq6";

    size_t  len = strlen(xx);
    //char* rxx = base64_decode(xx,len);

    char  chx[]  = {"Hello from C, Native so by Alex Lio"};
    return (*env)->NewStringUTF(env, chx);
}


/**
 * Release the resource in jni
 * @param env
 * @param thiz
 */
JNIEXPORT void JNICALL
Java_com_bcoin_ns_S_releaseJNIRes(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseJNIRes()
    //删除全局变量
    (*env)->DeleteGlobalRef(env, token);
}