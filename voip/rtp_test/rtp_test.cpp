// rtp_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ortp/ortp.h>
#include <signal.h>
#include <stdlib.h>

#ifndef _WIN32 
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#endif

#include <mediastreamer2/mediastream.h>

#include <fstream>

int main(int argc, char* argv[])
{
    ms_init();

    MSSndCard* sndcard,*sndpcard;
    sndcard = ms_snd_card_manager_get_default_capture_card(ms_snd_card_manager_get());
    sndpcard = ms_snd_card_manager_get_default_playback_card(ms_snd_card_manager_get());
    // capture filter
    MSFilter* s_read = ms_snd_card_create_reader(sndcard);
    MSFilter* s_write = ms_snd_card_create_writer(sndpcard);

    MSFilter* s_rec_file = ms_filter_new(MS_FILE_REC_ID);
    MSFilter* s_pla_file = ms_filter_new(MS_FILE_PLAYER_ID);

    // param
    int sr = 16000;
    int chan = 1;
    ms_filter_call_method(s_read, MS_FILTER_SET_SAMPLE_RATE, &sr);
    ms_filter_call_method(s_write, MS_FILTER_SET_SAMPLE_RATE, &sr);
    ms_filter_call_method(s_rec_file, MS_FILTER_SET_SAMPLE_RATE, &sr);
    ms_filter_call_method(s_pla_file, MS_FILTER_SET_SAMPLE_RATE, &sr);
    ms_filter_call_method(s_read, MS_FILTER_SET_NCHANNELS, &chan);
    ms_filter_call_method(s_write, MS_FILTER_SET_NCHANNELS, &chan);
    ms_filter_call_method(s_rec_file, MS_FILTER_SET_NCHANNELS, &chan);
    ms_filter_call_method(s_pla_file, MS_FILTER_SET_NCHANNELS, &chan);

    ms_filter_call_method(s_rec_file, MS_RECORDER_OPEN, "rr.wav");

    // link for graph
    ms_filter_link(s_read, 0, s_rec_file, 0);
    ms_filter_link(s_pla_file, 0, s_write, 0);


    // ticker
    MSTicker* tick = ms_ticker_new();
    ms_ticker_attach(tick, s_read);
    ms_ticker_attach(tick, s_pla_file);

    //ms_filter_call_method_noarg(s_read, MS_RECORDER_START);
    ms_filter_call_method_noarg(s_rec_file, MS_RECORDER_START);

    ms_sleep(5);

    ms_filter_call_method(s_read, MS_RECORDER_PAUSE, NULL);
    ms_filter_call_method(s_read, MS_RECORDER_CLOSE, NULL);

    // then play
    ms_filter_call_method(s_pla_file, MS_PLAYER_OPEN, "rr.wav");
    ms_filter_call_method_noarg(s_pla_file, MS_PLAYER_START);

    printf("Press Enter Key, Quit...");
    getchar();
	return 0;
}

void record(int rate, int channel){    
}

