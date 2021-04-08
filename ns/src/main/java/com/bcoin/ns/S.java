package com.bcoin.ns;

public class S {
    static {
        Runtime.getRuntime().addShutdownHook(new Thread (){
            @Override
            public void run() {
                releaseJNIRes();
            }});
        System.loadLibrary("ns");
    }

    /**
     * 依照算法取得请求参数的签名值
     * @param s
     * @return
     */
    public native String s(String s);

    /**
     * 解密返回的数据
     * @param s
     * @return
     */
    public native String d(String s);

    /**
     * flush token to c cache.
     *
     * @param s token to be flush.
     */
    public native String ft(String s);

    /**
     * Get token
     * @param s
     * @return
     */
    public native String gt(String s);

    public static native boolean releaseJNIRes();

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String test(String s);


}
