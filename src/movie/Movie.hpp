#include <vector>
#include "VideoStream.hpp"
#include "AudioStream.hpp"

using namespace std;

class Movie {
public:
	typedef vector<AudioStream> AudioStreams;

	Movie(const std::string&);
private:
	VideoStream videoStream_;
	AudioStreams audioStreams_;
};