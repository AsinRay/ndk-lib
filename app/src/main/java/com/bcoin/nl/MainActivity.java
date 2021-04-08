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

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {
    String TAG="main";
    S s = new S();
    String token = "6e4069678612b2f5b54e28be184967ab";

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

        //s.flushT("我是中国人");
        //System.out.println(rtn);

        /*
        Log.e("FFFFFFFFF本地方法s测试:", s.s(payload2));

        String ss = s.s("s");
        Log.e("::s.s:",ss);
        */
        Button btn = findViewById(R.id.button);
        btn.setText("Call Java Method");



        //final String token = "0123456789.~!@#$%^&*()_+}{我们一起来";

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                s.ft(token);
                getHttpRequest();
                //Toast.makeText(getApplicationContext(),ss,Toast.LENGTH_SHORT).show();
            }
        });
    }

    private String getHttpRequest(){
        String url = "https://blz.bicoin.com.cn/lrBigVIndex/firmOffer/getUserAccountInfoBySecretNew?userId=755";
        OkHttpClient okHttpClient = new OkHttpClient();
        String payload = "getUserAccountInfoBySecretNew";
        Log.e(TAG, "token: " + s.gt(""));
        Log.e(TAG, "payload: " + payload);
        String sign = s.s(payload);
        Log.e(TAG, "sign: " + sign);

        final Request request = new Request.Builder()
                .url(url)
                .addHeader("token",token)
                .addHeader("sign",sign)
                .get()//默认就是GET请求，可以不写
                .build();
        Call call = okHttpClient.newCall(request);
        call.enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.d(TAG, "onFailure: ");
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                //Log.d(TAG, "onResponse: " + response.body().string());

                int code2=response.code();
                if (code2==200){
                    //得到服务器返回的具体内容
                    String responseData=response.body().string();
                    //Log.d(TAG, "onResponse: " + responseData);
                    //获取数据
                    try {
                        JSONObject jsonObject = new JSONObject(responseData);
                        String d =jsonObject.getString("data");
                        Log.e(TAG,"d:"+d);
                        Log.e(TAG,"d:"+s.d(d));

                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        return "ok";
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}