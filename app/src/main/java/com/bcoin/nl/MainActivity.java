package com.bcoin.nl;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.bcoin.ns.*;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    String payload = "国中123456789+-*/.abcdefghijklimnopqrstuvwxyz |\t\\~!@#$%^&*()_+}{}[];:'\"?,<>";
    String payload2 = "asdf";
    @SuppressLint("LongLogTag")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());
        tv.setText("stringFromJNI()");
        S s = new S();
        String rtn = s.flushT("我是中国人");
        System.out.println(rtn);

        /*
        Log.e("==========",s.getStringX("admin"));
        Log.e("本地方法测试:", s.stringFromJNI());
        Log.e("本地方法decrypt测试:", s.decrypt("FFFFFFFFFFFFF333"));
        Log.e("FFFFFFFFF本地方法s测试:", s.s(payload2));

        String ss = s.s("s");
        Log.e("::s.s:",ss);
        */
        Button btn = findViewById(R.id.button);
        btn.setText("Call Java Method");


        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //String ss = s.s(payload);
                //Log.e("s.s:",ss);
                //ss = s.getStringX("s");
                //Log.e("token is:",ss);
                //ss = s.test("YXNkZ我是中国人AAA");
                //Log.e("decrypt:",ss);
                String ss = s.stringFromJNI();
                Log.e("base %s:",ss);
                ss = s.test("asdfasd");
                Log.e("test %s:",ss);
                //String str = s.getStringX("fuc_fuc");
                //Toast.makeText(getApplicationContext(),ss,Toast.LENGTH_SHORT).show();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}