package com.zhougang.gstaudiorecord;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.freedesktop.gstreamer.GStreamer;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            init(this);
        } catch (Exception e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        Button btn1 =(Button)findViewById(R.id.button1);

        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                NativeRecordStart();
            }
        });

        Button btn2 =(Button)findViewById(R.id.button2);

        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                NativeRecordStop();
            }
        });

    }

    private void init(Context context) throws Exception {
        System.loadLibrary("gstreamer_android");
        GStreamer.init(context);
        System.loadLibrary("gstAudioRecord");
    }

    public static native void NativeRecordStart();
    public static native void NativeRecordStop();
}