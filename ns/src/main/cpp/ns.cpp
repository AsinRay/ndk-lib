#include <jni.h>
#include <string>
#include "md5.h"

#include "b64.h"

using namespace std;

//把字符串转成十六进制字符串
std::string char2hex(std::string s)
{
    std::string ret;
    for (unsigned i = 0; i != s.size(); ++i)
    {
        char hex[5];
        sprintf(hex, "%.2x", (unsigned char)s[i]);
        ret += hex;
    }
    return ret;
}
//把十六进制字符串转成字符串
std::string hex2char(std::string s)
{
    std::string ret;
    int length = (int) s.length();
    for (int i = 0; i <length ; i+=2) {
        std::string buf = "0x"+s.substr(i,2);
        unsigned int value;
        sscanf(buf.c_str(), "%x", &value);
        ret += ((char)value);
    }
    return ret;
}
int hexCharToInt(char c){
    if(c >= '0' && c <= '9') return (c - '0');
    if(c >= 'A' && c <= 'F') return (c - 'A'+10);
    if(c >= 'a' && c <= 'f') return (c - 'a'+10);
    return 0;
}

//十六进制字符串转成十六进制数组
char * hexstringToBytes(std::string s){
    int sz = (int) s.length();
    char  *ret = new char[sz/2];
    for (int i = 0; i < sz; i+=2) {
        ret[i/2] = (char)((hexCharToInt(s.at(i)) << 4)|hexCharToInt(s.at(i+1)));
    }
    return ret;
}
//十六进制数组转成十六进制字符串
std::string bytestohexstring(char *bytes,int bytelength){
    std::string str("");
    std::string str2("0123456789abcdef");
    for (int i = 0; i < bytelength; ++i) {
        int b;
        b = 0x0f&(bytes[i]>>4);
        char s1 = str2.at(b);
        str.append(1,str2.at(b));
        b = 0x0f&bytes[i];
        str.append(1,str2.at(b));
//        char s2 = str2.at(b);
    }
    return str;
}




string encode(const string &bytes)
{
    string out;
    out.resize(Base64encode_len(bytes.length()));
    Base64encode(&out[0], bytes.data(), bytes.length());
    return out;
}

std::string decode(const std::string &base64)
{
    std::string out;
    out.resize(Base64decode_len(&base64[0]));
    size_t actualSize = Base64decode(&out[0], base64.data());
    out.resize(actualSize);
    return out;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    string hello = "Hello from C++, Native so by Alex Lio";
    string b64 = "SGVsbG8gZnJvbSBDKyssIE5hdGl2ZSBzbyBieSBBbGV4IExpbw==";
    string en = decode(b64);
    return env->NewStringUTF(en.c_str());
}

/**
 * AES/CBC/PKCS5Padding decrypt
 * @param env
 * @param thiz
 * @param s
 * @return
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_d(JNIEnv *env, jobject thiz, jstring s) {
    if (s == NULL) {
        return NULL;
    }
    //return decrypt(env,s);
    return NULL;
}



//JNIEXPORT jstring JNICALL
//Java_com_bcoin_ns_S_d(JNIEnv *env, jobject thiz, jstring s) {
//
//    // TODO: implement decrypt()
//    int values[] {1,2,3};
//    const char* params = env->GetStringUTFChars(s,0);
//    string sign_str(params);
//    sign_str.insert(0,"asdfasdf");
//    sign_str = sign_str.substr(0,sign_str.length()-2);
//    env->ReleaseStringUTFChars(s,params);
//    return env->NewStringUTF(sign_str.c_str());
//}


