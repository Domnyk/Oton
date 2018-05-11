#include <vector>
#include "VideoStream.hpp"

using namespace std;

class Movie {
	typedef vector<AudioStream> AudioStreams;

private:
	VideoStream videoStream_;
	AudioStreams audioStreams_;
}