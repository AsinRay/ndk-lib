#include "includes.h"
#include "md5.h"
#include "b64.h"
#include "c.h"

// global jstring token
jstring token;

unsigned char tk[16] = {0};

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
    const char* c_token =(char*) (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    //打印token
    //printf("token = %s",c_token);
    //删除对象(关键代码)
    (*env)->DeleteLocalRef(env,jobj);
    return c_token;
}




/**
 * C的字节数组转为jstring
 * @param env
 * @param pat
 * @return
 */
jstring charToJstring(JNIEnv *env, const char *pat) {
    // Java String类 strClass
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    // 获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>","([BLjava/lang/String;)V");
    // 建立byte数组
    jbyteArray bytes = (*env)->NewByteArray(env, strlen(pat));
    // 将char* 转换为byte数组
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(pat), (jbyte *) pat);
    //设置String的encoding, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
    //将byte数组转换为java String,并输出
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

char *jstringToChar(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "utf-8");
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte *ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    return rtn;
}


char k[16]="0123456789abcdef";
char iv[16]="0123456789abcdef";

char* getKey(){
    return tk;
}

char* getIv(){
    return tk;
}

/**
 * concat tow char array byte by byte.
 * @param s1
 * @param s2
 * @return
 */
char* join(char *s1, char *s2){
    // need free
    char *result = (char *) malloc(strlen(s1)+strlen(s2));
    //in real code you would check for errors in malloc here
    if (result == NULL) exit (1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * Gen md5 hex string or given string, which appended by the user's token.
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


/**
 *
 * Flash token when user modified the username or password.
 * @param env
 * @param thiz
 * @param s
 * @return
 */
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_flushT(JNIEnv *env, jobject thiz, jstring s) {
    //const char* ch = (*env)->GetStringUTFChars(env,s,0);
    // TODO: implement flushT()

    (*env)->DeleteGlobalRef(env, token);
    //创建全局对象
    token = (*env)->NewGlobalRef(env, s);
    //删除全局变量
    //(*env)->DeleteGlobalRef(env, token);


    char* ch = (*env)->GetStringUTFChars(env, s, JNI_FALSE);
    MD5_CTX context = {0};
    MD5Init(&context);
    MD5Update(&context, ch, strlen(ch));
    //unsigned char dest[16] = {0};
    MD5Final(tk, &context);
    return token;
}



/**
 * 测试工具的正确性
 * @param env
 * @param thiz
 * @param s
 * @return
 */
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_test(JNIEnv *env, jobject thiz, jstring s) {
    // TODO: implement test()

    char* c = jstringToChar(env,s);
    return charToJstring(env,c);

    //char* ch = (*env)->GetStringUTFChars(env, token,JNI_FALSE);
    //int data_len = strlen(ch);
    //char *rtn = base64_encode(ch,data_len);
    //(*env)->ReleaseStringUTFChars(env, s, ch);

    //char * xx = "Y68Ejv8jGeMObwKrPsptBESSLxW9rc7x/Vrhl8DV66Z9rVQH+TogNTZSV8aFvcDZP6/gzYb9/9qrIY8MsdDQxQ==";

    //return decrypt(env,xx);
    //size_t  len = strlen(xx);
    //char* rxx = base64_decode(xx,len);

    //char  chx[]  = {"Hello from C, Native so by Alex Lio"};
    //return (*env)->NewStringUTF(env, chx);
}


/**
 * Release the resource in jni
 * @param env
 * @param thiz
 */
JNIEXPORT jboolean JNICALL
Java_com_bcoin_ns_S_releaseJNIRes(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseJNIRes()
    //删除全局变量
    (*env)->DeleteGlobalRef(env, token);
    return JNI_TRUE;
}