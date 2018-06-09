#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include "VideoStream.hpp"
#include "AudioStream.hpp"

using namespace std;

class Movie {
public:
    static std::string get_filename(const std::string&);

	Movie(const std::string&);

    VideoStream& videoStream() {
        return videoStream_;
    }

    AudioStream& audioStream() {
        return audioStream_;
    }
private:
	VideoStream videoStream_;
    AudioStream audioStream_;
};

#endif
