package com.bcoin.ns;

import androidx.annotation.Nullable;

public class S {
    static {
        System.loadLibrary("ns");
    }




    /**
     * 依照算法取得参数md5的值
     * @param s
     * @return
     */
    public native String s(String s);
    public native String stringFromJNI();
    public native String decrypt(String s);


    public native String getStringX(String s);


    /**
     * flush token to c cache.
     *
     * @param s token to be flush.
     */
    public native String flushT(String s);
}
