#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
    static const unsigned short KEY_FRAME_PARAM = 5;

    Frame();
	Frame(unsigned, Mat);

	Frame resize(Size);

    const Mat& get_pixel_matrix() const;

    const Size get_size() const;

    bool is_key_frame() const;

    unsigned char* data();
    unsigned const char* data() const;
    unsigned int data_length() const;
private:
    unsigned frame_num_;
    Mat pixel_matrix_;
};
