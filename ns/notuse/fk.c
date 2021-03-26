/* jstring decrypt(JNIEnv* env, jstring miwen) {
    jstring result;
    //wojiubugaosunimenglalisadeyanlei 秘钥
    unsigned char key[32] = {0x77,0x6f,0x6a,0x69,
                             0x75,0x62,0x75,0x67,
                             0x61,0x6f,0x73,0x75,
                             0x6e,0x69,0x6d,0x65,
                             0x6e,0x67,0x6c,0x61,
                             0x6c,0x69,0x73,0x61,
                             0x64,0x65,0x79,0x61,
                             0x6e,0x6c,0x65,0x69};

    //*********** 开始解密 ************
    //1.初始化数据
    //初始化向量 nglalisadeyanlei
//    uint8_t iv[16] = { 0x6e,0x67,0x6c,0x61,
//                       0x6c,0x69,0x73,0x61,
//                       0x64,0x65,0x79,0x61,
//                       0x6e,0x6c,0x65,0x69 };

    uint8_t iv[32] = {0x77,0x6f,0x6a,0x69,
                             0x75,0x62,0x75,0x67,
                             0x61,0x6f,0x73,0x75,
                             0x6e,0x69,0x6d,0x65,
                             0x6e,0x67,0x6c,0x61,
                             0x6c,0x69,0x73,0x61,
                             0x64,0x65,0x79,0x61,
                             0x6e,0x6c,0x65,0x69};
    aes256_context ctx;
    aes256_init(&ctx, key);

    //2.将jstring转为char
    //const char *mwChar = (*env)->GetStringUTFChars(env, miwen, JNI_FALSE);
    const char *mwChar = "Y68Ejv8jGeMObwKrPsptBESSLxW9rc7x/Vrhl8DV66Z9rVQH+TogNTZSV8aFvcDZP6/gzYb9/9qrIY8MsdDQxQ==";
    char *enc = decode(mwChar);
    uint8_t output[4096];
    aes256_decrypt_cbc(&ctx, (unsigned char *) enc, iv, output);
    int size = strlen((const char *) output);
    LOGD("output size=%d",size);
    int i;
    for(i=0;i<size;i++){
        //LOGD("cha %d = %c",i,output[i]);
        if(output[i]>=1&&output[i]<=16){
            output[i] = 0;
        }
    }
    result = charToJstring(env, (char *) output);
    //LOGD("result=%s",(char *) output);
    free(enc);
    //释放mwChar
    (*env)->ReleaseStringUTFChars(env, miwen, mwChar);
    aes256_done(&ctx);
    return result;
} */
