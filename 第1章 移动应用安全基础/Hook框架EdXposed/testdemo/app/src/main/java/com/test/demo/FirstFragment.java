package com.test.demo;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.navigation.fragment.NavHostFragment;

import com.test.demo.databinding.FragmentFirstBinding;

public class FirstFragment extends Fragment {

    private FragmentFirstBinding binding;


    private final MyHandle handle = new MyHandle();



    @Override
    public View onCreateView(
            LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState
    ) {

        binding = FragmentFirstBinding.inflate(inflater, container, false);
        return binding.getRoot();

    }

    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);



        binding.buttonFirst.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Message ms = new Message();
                        Bundle mb = new Bundle();
                        mb.putString("test",getTextViewShowData());
                        ms.setData(mb);
                        handle.sendMessage(ms);
                    }
                }).start();
            }
        });
    }

    public String getTextViewShowData() {
        return "Hello, welcome to here!!!";
    }



    class MyHandle extends Handler {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle mb = msg.getData();
            binding.textviewFirst.setText(mb.getString("test"));
            binding.buttonFirst.setText(R.string.previous);
        }
    }
}