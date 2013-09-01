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

#include "audio_unit.h"
#include "psm_player.h"
#include "psm_record.h"

#include <fstream>

int main(int argc, char* argv[])
{

    voip_core::audio_unit& au = voip_core::audio_unit::instance();
    au.startup();

    // record 
    voip_core::psm_record pr;
    int sec = 5 * 50; // second
    pr.start(1000, 16000);
    size_t bsize = pr.calc_bytes_with_ms(20/*ms*/);
    char* buf = new char[bsize * sec + bsize];
    for(int i = 0; i < sec;){
        int res = pr.record((void*)(buf+bsize * i), bsize);
        if(res == 1){
            Sleep(1);
            continue;
        }else if(res == 0){
            i++; // record next
        }else{
            break; // error
        }
    }

    pr.stop();
    // save
    std::fstream fs("t1.pcm", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(fs.is_open()){
        fs.write(buf, sec * bsize );
        fs.close();
    }
    // player
    voip_core::psm_player pp;
    voip_core::psm_stream* ps = pp.start_stream(pr.get_channels(), pr.get_type(), pr.get_rate());
    bool bcommit = false;
    for(int i = 0; i < sec; ){
        int res = pp.queue_data(ps, (void*)(buf + i * bsize), bsize);
        if( res == 0 ) //wait
            Sleep(10);
        else
            i++;
    }

    printf("Press Enter Key, Quit...");
    getchar();
	return 0;
}

