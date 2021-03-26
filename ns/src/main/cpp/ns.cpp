#include <jni.h>
#include <string>
#include "md5.h"
#include "b64.h"
#include "AES.h"


using namespace std;


extern "C" {
    char* getKey();
    char* getIv();
}

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

extern "C"
std::string b64encode(const string &bytes)
{
    string out;
    out.resize(Base64encode_len(bytes.length()));
    Base64encode(&out[0], bytes.data(), bytes.length());
    return out;
}

extern "C"
std::string b64decode(const std::string &base64)
{
    std::string out;
    out.resize(Base64decode_len(&base64[0]));
    size_t actualSize = Base64decode(&out[0], base64.data());
    out.resize(actualSize);
    return out;
}


string encryptByAES(const char * data, const char* secretKey, const char* iv, int iMode) {
    string data_str(data);
    size_t length = data_str.length();
    int block_num = length / BLOCK_SIZE + 1;
    //明文
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, data_str.c_str());

    //进行PKCS7Padding填充。
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++)
    {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';

    //加密后的密文
    char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

    //进行进行AES的CBC模式加密
    AES aes;
    aes.MakeKey(secretKey, iv, 16, 16);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, iMode);
    string s(szDataOut);
    string str = b64encode(s);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}

/**
 * 解密
 * @param data
 * @param secretKey
 * @param iv
 * @return
 */
string decryptByAES(const char * data, const char* secretKey, const char* iv, int iMode) {
    string data_str(data);
    string strData = b64decode(data_str);
    size_t length = strData.length();
    //密文
    char* szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length + 1);
    //明文
    char* szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length + 1);

    //进行AES的CBC模式解密
    AES aes;
    aes.MakeKey(secretKey, iv, 16, 16);
    aes.Decrypt(szDataIn, szDataOut, length, iMode);

    //去PKCS7Padding填充
    if (0x00 < szDataOut[length - 1] <= 0x16)
    {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--)
        {
            if (szDataOut[i] != tmp)
            {
                memset(szDataOut, 0, length);
                //cout << "去填充失败！解密出错！！" << endl;
                break;
            }
            else
                szDataOut[i] = 0;
        }
    }
    string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}

/**
 * cbc方式解密
 *
 * @param data
 * @param secretKey
 * @param iv
 * @return
 */
string decryptAESCBC(const char * data) {
    return decryptByAES(data, getKey(),  getIv(), 1);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_bcoin_ns_S_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    string hello = "Hello from C++, Native so by Alex Lio";
    string b64 = "SGVsbG8gZnJvbSBDKyssIE5hdGl2ZSBzbyBieSBBbGV4IExpbw==";
    //string en = decode(b64);
    return env->NewStringUTF(b64.c_str());
}

/**
 * AES/CBC/PKCS7Padding decrypt
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
    const char* b64 = env->GetStringUTFChars(s, 0);
    string ss = decryptAESCBC(b64);
    env->ReleaseStringUTFChars(s, b64);
    return env->NewStringUTF(ss.c_str());
}

//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_bcoin_ns_S_test(JNIEnv *env, jobject thiz, jstring s) {
//    // TODO: implement test()
//
//    char* ch = env->GetStringUTFChars(token,JNI_FALSE);
//    int data_len = strlen(ch);
//    //char *rtn = base64_encode(ch,data_len);
//    //(*env)->ReleaseStringUTFChars(env, s, ch);
//
//    char * xx = "Y68Ejv8jGeMObwKrPsptBESSLxW9rc7x/Vrhl8DV66Z9rVQH+TogNTZSV8aFvcDZP6/gzYb9/9qrIY8MsdDQxQ==";
//
//    //return decrypt(env,xx);
//    //size_t  len = strlen(xx);
//    //char* rxx = base64_decode(xx,len);
//
//    char  chx[]  = {"Hello from C, Native so by Alex Lio"};
//    return env->NewStringUTF(ch.c_str());
//}





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


