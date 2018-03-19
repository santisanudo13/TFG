package com.example.santi.tfgsu;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try{
                Log.d("AAAA","1");


                Process process = Runtime.getRuntime().exec("echo a >> /storage/emulated/obb/holi.txt");

                process.waitFor();


        }catch(Exception e){
            Log.e("EXCEPCION", "exception: " + e.getMessage());
        }
    }
}
