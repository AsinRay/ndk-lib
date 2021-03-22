package com.bcoin.ns;

/**
 * https://blog.csdn.net/maweisky531/article/details/47704569
 */
public class DataProvider {
    /**
     * 把两个java中的int传递给c语言, c语言处理完毕后,把相加的结果返回给java
     * @param x
     * @param y
     * @return
     */
    public native int add(int x ,int y);

    public static native int sub(int x ,int y);


    public native char add(char x, char y); //String
    /**
     * 把java中的string传递给c语言, c语言获取到java中的string之后 ,在string后面添加 一个hello 字符串
     * @param s
     * @return
     */
    public native String sayHelloInC(String s);
    /**
     * 把java中的一个int数组 传递给c语言,c语言处理完毕这个java数组
     * 把int数组中的每一个元素+10 ;
     * 然后把结果返回给java
     * @param iNum
     * @return
     */
    public native int[] intMethod(int[] iNum);

    public native byte[] byteMethod(byte[] iByte);

    public native DiskInfo getStruct();
}
