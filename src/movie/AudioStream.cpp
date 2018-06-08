#include <iostream>
#include <cstdlib>
#include <AudioStream.hpp>



AudioStream::AudioStream(const std::string& path_to_movie) {
    std::cerr << "AudioStream constructor does nothing" << std::endl;

    /* av_register_all();
    ao_initialize();

    extract_sound_from_movie_into_file(path_to_movie); */
}

AudioStream::~AudioStream() {
    std::cerr << "AudioStream destructor does nothing" << std::endl;
    // ao_shutdown();
}

void AudioStream::extract_sound_from_movie_into_file(const std::string& path_to_movie) {
    std::string instruction = "ffmpeg -i " + path_to_movie + "-f mp3 -ab 192000 -vn ./data/music.mp3 -y";

    std::cerr << "No checking for ffmpeg instruction execution" << std::endl;
    system(instruction.c_str());

    AVFormatContext* av_format_ctx = avformat_alloc_context();
    int open_status = avformat_open_input(&av_format_ctx, path_to_movie.c_str(), nullptr, nullptr);
    if (open_status < 0) {
           throw std::runtime_error("Could not open file in AudioStream::extract_sound_from_movie_into_file");
    }


    int find_stream_info_status = avformat_find_stream_info(av_format_ctx, nullptr);
    if (find_stream_info_status < 0) {
           throw std::runtime_error("Inside AudioStream::extract_sound_from_movie_into_file(): Could not read file info");
    }

    /*
     *  false means that this is as input
     */
    bool is_output_flag = false;
    av_dump_format(av_format_ctx, 0, path_to_movie.c_str(), is_output_flag);


    std::pair<bool, int> audio_stream_id = is_audio_stream_in_ctx(av_format_ctx);
    if(!audio_stream_id.first) {
        throw std::runtime_error("Inside AudioStream::extract_sound_from_movie_into_file(): no audio stream");
    }



    load_metadata(av_format_ctx, audio_stream_id.second);
    read_packets_from_file(av_format_ctx);
}

std::pair<bool, int> AudioStream::is_audio_stream_in_ctx(AVFormatContext* av_format_ctx) const {
    const unsigned short ERROR = -1;

    for(unsigned int i = 0; i < av_format_ctx->nb_streams; i++) {
        if(av_format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
                return std::make_pair(true, i);
        }
    }
    return std::make_pair(false, ERROR);
}

void AudioStream::load_metadata(AVFormatContext* av_format_ctx, int audio_stream_id) const {
    AVCodecContext *ctx = av_format_ctx->streams[audio_stream_id]->codec;
    AVCodec *codec = avcodec_find_decoder(ctx->codec_id);

    if(codec == nullptr) {
        throw std::runtime_error("Inside AudioStream::extract_sound_from_movie_into_file(): cannot find decoder!");
    }

    if(avcodec_open2(ctx, codec, nullptr) < 0) {
        throw std::runtime_error("Inside AudioStream::extract_sound_from_movie_into_file(): Cannot init with avcodec_open2");
    }
}

void AudioStream::read_packets_from_file(AVFormatContext* ctx) {
    AVPacket av_packet;
    av_init_packet(&av_packet);

    bool is_data_to_read = av_read_frame(ctx ,&av_packet);
    while (is_data_to_read >= 0) {
        packets_.push_back(av_packet);
    }
}

const std::vector<AVPacket>& AudioStream::get_packets() const {
    return packets_;
}


