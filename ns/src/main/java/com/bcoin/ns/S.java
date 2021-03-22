package com.bcoin.ns;

public class S {
    static {
        System.loadLibrary("ns");
    }


    public native String stringFromJNI();
}
