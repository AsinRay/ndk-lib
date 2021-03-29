package com.bcoin.ns;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
public class ExampleUnitTest {
    @Test
    public void addition_isCorrect() {
        assertEquals(4, 2 + 2);
    }

    @Test
    public void testMMD5() {
        S s = new S();
        String token = "0123456789.~!@#$%^&*()_+}{我们一起来";
        s.flushT(token);
        String md5 = s.s("p这是一个Json String.");
        String exp = "DBBB883861C778C2AC9A66AB1C0CEEAA";
        assertEquals(exp,md5);
    }
}