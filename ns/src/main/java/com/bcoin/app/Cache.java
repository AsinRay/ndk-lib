package com.bcoin.app;

/**
 * 这个类的静态方法可以实时取到token的值，以供C代码调用，
 * 建议通过异步方式cache用户的token
 */
public class Cache {

    public byte[] getTBytes(){
        return "64c8c9eb88780ffa66c96f90c3026cf3".getBytes();
    }

    public String getToken(){
        return "64c8c9eb88780ffa66c96f90c3026cf3我是中国人";
    }

    //  在这个类里面需要有resetToken的功能
}
