#pragma once

namespace voip_core{

    struct record_context;
    class psm_record{
        record_context* context_;
    public:
        psm_record();
        ~psm_record();

        int start(int gap/*ms, read samples data */, int rate);
        int record(void* data, size_t bufsize);
        int stop();

        // call after starting or zero
        int    get_channels() const;
        int    get_type() const;
        size_t get_rate() const;

        size_t calc_frame_bytes() const;
        size_t calc_bytes_with_ms(size_t ms) const;
    };

    void _psm_record_check();
}