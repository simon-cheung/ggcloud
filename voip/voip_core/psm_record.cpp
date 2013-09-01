
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <assert.h>

#include "alhelpers.h"
#include "psm_record.h"

namespace voip_core{

    static LPALBUFFERSAMPLESSOFT alBufferSamplesSOFT = wrap_BufferSamples;
    static LPALISBUFFERFORMATSUPPORTEDSOFT alIsBufferFormatSupportedSOFT = NULL;

    struct record_context{
        ALCdevice *device;
        ALenum format;
        ALenum channels;
        ALenum type;
        ALuint rate;

        int gap;
    };

    psm_record::psm_record(){
        context_ = NULL;
    }

    psm_record::~psm_record(){
    }

    int psm_record::get_channels() const{
        return context_->channels;
    }

    int psm_record::get_type() const{
        return context_->type;
    }

    size_t psm_record::get_rate() const{
        return context_->rate;
    }

    size_t psm_record::calc_frame_bytes() const{
        return FramesToBytes(1, context_->channels, context_->type);
    }

    size_t psm_record::calc_bytes_with_ms(size_t ms) const{
        return FramesToBytes(context_->rate * (ms / 1000.0f), context_->channels, context_->type);
    }

    static record_context rc_;
    int psm_record::start(int gap, int rate){
        alGetError();
        int channels = AL_MONO_SOFT;
        int type = AL_SHORT_SOFT;
        context_ = &rc_;
        context_->gap = gap;
        context_->channels = channels;
        context_->type = type;
        context_->rate = rate;
        int SSIZE = rate * FramesToBytes(1, channels, type) * 1/*1000ms, samples bytes*/;
        context_->format = GetFormat(channels, type, alIsBufferFormatSupportedSOFT);
        ALCdevice *device = alcCaptureOpenDevice(NULL, context_->rate, context_->format, SSIZE);
        if (alGetError() != AL_NO_ERROR) {
            return -1;
        }
        alcCaptureStart(device);
        context_->device = device;
        return 0;
    }

    int psm_record::record(void* data, size_t bufsize){
        ALint sample;
        if(context_->device){
            alcGetIntegerv(context_->device, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &sample);
            if(sample >= bufsize){
                alcCaptureSamples(context_->device, (ALCvoid *)data, bufsize);
                return 0;
            }else{
                return 1; // wait
            }
        }
        return -1; // error
    }

    int psm_record::stop(){
        if(context_->device){
            alcCaptureStop(context_->device);
            alcCaptureCloseDevice(context_->device);
            context_->device = NULL;
        }
        return 0;
    }

    void _psm_record_check(){
    }

}