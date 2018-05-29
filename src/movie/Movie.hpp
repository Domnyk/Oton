#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include "VideoStream.hpp"
#include "AudioStream.hpp"

using namespace std;

class Movie {
public:
	typedef vector<AudioStream> AudioStreams;

	Movie(const std::string&);

    VideoStream& videoStream() {
        return videoStream_;
    }
private:
	VideoStream videoStream_;
	AudioStreams audioStreams_;
};

#endif
