#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <assert.h>

#include "alhelpers.h"
#include "psm_player.h"

namespace voip_core{

    static LPALBUFFERSAMPLESSOFT alBufferSamplesSOFT = wrap_BufferSamples;
    static LPALISBUFFERFORMATSUPPORTEDSOFT alIsBufferFormatSupportedSOFT = NULL;


/* Define the number of buffers and buffer size (in samples) to use. 4 buffers
 * with 8192 samples each gives a nice per-chunk size, and lets the queue last
 * for almost 3/4ths of a second for a 44.1khz stream. */
#define NUM_BUFFERS 4
#define BUFFER_SIZE 8192

    struct psm_stream{
        /* These are the buffers and source to play out through OpenAL with */
        ALuint buffers[NUM_BUFFERS];
        ALuint source;

        /* A temporary data buffer for readAVAudioData to write to and pass to
         * OpenAL with */
        ALbyte *data;
        ALsizei datasize;

        /* The format of the output stream */
        ALenum format;
        ALenum channels;
        ALenum type;
        ALuint rate;
    };
    // stream
    psm_stream*  psm_player::start_stream(int channels, int type, int rate){
        psm_stream *player;

        player = new psm_stream;
        assert(player != NULL);

        memset(player, 0, sizeof(*player));

        /* Generate the buffers and source */
        alGenBuffers(NUM_BUFFERS, player->buffers);
        assert(alGetError() == AL_NO_ERROR && "Could not create buffers");

        alGenSources(1, &player->source);
        assert(alGetError() == AL_NO_ERROR && "Could not create source");

        /* Set parameters so mono sources play out the front-center speaker and
         * won't distance attenuate. */
        alSource3i(player->source, AL_POSITION, 0, 0, -1);
        alSourcei(player->source, AL_SOURCE_RELATIVE, AL_TRUE);
        alSourcei(player->source, AL_ROLLOFF_FACTOR, 0);
        assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");

        player->channels = channels;
        player->type = type;
        player->rate = rate;
        player->format = GetFormat(player->channels, player->type, alIsBufferFormatSupportedSOFT);

        /* Allocate enough space for the temp buffer, given the format */
        player->datasize = FramesToBytes(BUFFER_SIZE, player->channels,
                                         player->type);
        player->data = (ALbyte*)new char[player->datasize];
        memset(player->data, 0, player->datasize);

        return player;
    }

    int psm_player::queue_data(psm_stream* stream, void* data, size_t size){
        if(is_stop_){
            return _begin_stream(stream, data, size);
        }else{ //update data
            return _update_stream(stream, data, size);
        }
        return 0;
    }

    int psm_player::_begin_stream(psm_stream* stream, void* data, size_t size){
        size_t i;

        /* Rewind the source position and clear the buffer queue */
        alSourceRewind(stream->source);
        alSourcei(stream->source, AL_BUFFER, 0);

        /* Fill the buffer queue */
        for(i = 0;i < 1/*NUM_BUFFERS*/;i++)
        {
            assert(size != stream->datasize);
            //memcpy(stream->data, data, stream->datasize);
            alBufferSamplesSOFT(stream->buffers[i], stream->rate, stream->format,
                                BytesToFrames(size, stream->channels, stream->type),
                                stream->channels, stream->type, data);
        }
        if(alGetError() != AL_NO_ERROR)
        {
            assert(false &&  "Error buffering for playback\n");
            is_stop_ = true;
            return -1;
        }

        /* Now queue and start playback! */
        alSourceQueueBuffers(stream->source, i, stream->buffers);
        alSourcePlay(stream->source);
        if(alGetError() != AL_NO_ERROR)
        {
            assert(false && "Error starting playback\n");
            is_stop_ = true;
            return -1;
        }
        is_stop_ = false;
        return 0;
    }

    int psm_player::_update_stream(psm_stream* stream, void* data, size_t size){
        ALint processed, state;
        int result = 0;
        /* Get relevant source info */
        alGetSourcei(stream->source, AL_SOURCE_STATE, &state);
        alGetSourcei(stream->source, AL_BUFFERS_PROCESSED, &processed);
        if(alGetError() != AL_NO_ERROR)
        {
            assert(false && "Error checking source state\n");
            is_stop_ = true;
            return -1;
        }

        /* Unqueue and handle each processed buffer */ 
        if(processed > 0)  // just one segment
        {
            ALuint bufid;

            alSourceUnqueueBuffers(stream->source, 1, &bufid);

            assert(size != stream->datasize);
            //memcpy(stream->data, data, stream->datasize);
            {
                alBufferSamplesSOFT(bufid, stream->rate, stream->format,
                                    BytesToFrames(size, stream->channels, stream->type),
                                    stream->channels, stream->type, data);
                alSourceQueueBuffers(stream->source, 1, &bufid);
            }
            if(alGetError() != AL_NO_ERROR)
            {
                assert(false &&  "Error buffering data\n");
                is_stop_ = true;
                return -1;
            }
            result = 1;
        }

        /* Make sure the source hasn't underrun */
        if(state != AL_PLAYING && state != AL_PAUSED)
        {
            ALint queued;

            /* If no buffers are queued, playback is finished */
            alGetSourcei(stream->source, AL_BUFFERS_QUEUED, &queued);
            if(queued == 0){
                is_stop_ = true;
                return -1;
            }
            alSourcePlay(stream->source);
            if(alGetError() != AL_NO_ERROR)
            {
                assert(false &&  "Error restarting playback\n");
                is_stop_ = true;
                return -1;
            }
            is_stop_ = false;
        }
        return result;
    }

    int psm_player::pause_stream(psm_stream* stream, bool bpause){
        return 0;
    }

    int psm_player::stop_stream(psm_stream* stream){

        delete [] stream->data;
        stream->data = NULL;
        stream->datasize = 0;

        alDeleteSources(1, &stream->source);
        alDeleteBuffers(NUM_BUFFERS, stream->buffers);
        if(alGetError() != AL_NO_ERROR)
            assert(false && "Failed to delete object IDs\n");

        memset(stream, 0, sizeof(*stream));
        delete stream;
        return 0;
    }

    bool psm_player::is_pause(psm_stream* stream){
        return is_pause_;
    }

    bool psm_player::is_stop(psm_stream* stream){
        return is_stop_;
    }

    // segment
    void psm_player::play_psm(void* data, size_t size){
    }

}