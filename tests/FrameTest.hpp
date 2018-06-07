#ifndef FRAMETEST_HPP
#define FRAMETEST_HPP

#include <QObject>
#include "../src/movie/Frame.hpp"

class FrameTest : public QObject
{
    Q_OBJECT
private slots:
    void it_should_clone_data_matrix();
    void it_should_throw_exception_when_resizing_to_bigger_resolution();
    void it_should_resize_image_when_resolution_is_smaller_than_original();
    void is_key_frame_should_always_return_true();
};

#endif
