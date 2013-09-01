#pragma once

namespace voip_core{
    class audio_unit
    {
    public:
        audio_unit();
        ~audio_unit();

        static audio_unit& instance();

        int startup();
        int stop();
    };
}