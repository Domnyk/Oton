#ifndef VIDEOSTREAM_HPP
#define VIDEOSTREAM_HPP

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "Frame.hpp"

using namespace cv;

class VideoStream {
public:
	VideoStream(const std::string&);
	void open(const std::string&);
	Frame getFrame(unsigned);

    unsigned get_num_of_frames() {
        return numOfFrames;
    }
private:
	VideoCapture videoCapture_;
 	unsigned numOfFrames;
};

#endif
