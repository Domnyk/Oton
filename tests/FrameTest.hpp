#ifndef FRAMETEST_HPP
#define FRAMETEST_HPP

#include <QObject>
#include "../src/movie/Frame.hpp"

class FrameTest : public QObject
{
    Q_OBJECT
private slots:
    void constructor_should_clone_pixel_matrix();
    void resize_should_throw_exception_when_resizing_to_bigger_resolution();
    void resize_should_resize_image_when_resolution_is_smaller_than_original();
    void frame_with_number_divisible_by_param_should_be_key_frame();
    void frame_with_number_not_divisible_by_param_should_not_be_key_frame();
};

#endif
