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

/**
 * 解密
 * @param data
 * @param secretKey
 * @param iv
 * @return
 */
string decryptByAES(const char * data, const char* secretKey, const char* iv, int iMode) {
    // Get the string of base64
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


