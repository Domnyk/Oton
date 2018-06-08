#ifndef AUDIOSTREAM_HPP
#define AUDIOSTREAM_HPP

#include <string>
#include <vector>

extern "C" {
    #include <libavutil/mathematics.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/fifo.h>
    #include <ao/ao.h>
}

class AudioStream {
public:
    AudioStream(const std::string&);
    ~AudioStream();

    const std::vector<AVPacket>& get_packets() const;
private:
    void extract_sound_from_movie_into_file(const std::string&);
    std::pair<bool, int> is_audio_stream_in_ctx(AVFormatContext*) const;
    void read_packets_from_file(AVFormatContext*);
    void load_metadata(AVFormatContext*, int) const;

    std::vector<AVPacket> packets_;
};

#endif
