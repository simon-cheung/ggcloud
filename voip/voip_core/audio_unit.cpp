
#include "audio_unit.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "alhelpers.h"
#include "psm_player.h"
#include "psm_record.h"

namespace voip_core{

    audio_unit& audio_unit::instance(){
        static audio_unit _inst_;
        return _inst_;
    }

    audio_unit::audio_unit(){
        _psm_record_check();
    }

    audio_unit::~audio_unit(){
    }

    int audio_unit::startup(){
        if(InitAL() != 0)
            return 1;
        return 0;
    }

    int audio_unit::stop(){
        CloseAL();
        return 0;
    }
}