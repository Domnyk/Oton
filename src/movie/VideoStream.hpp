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
    Frame get_frame(unsigned);

    unsigned get_num_of_frames() {
        return num_of_frames_;
    }
private:
    VideoCapture video_capture_;
    unsigned num_of_frames_;
};

#endif
