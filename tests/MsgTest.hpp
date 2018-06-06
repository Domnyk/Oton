#ifndef MSGTEST_HPP
#define MSGTEST_HPP

#include <QObject>

class MsgTest : public QObject
{
    Q_OBJECT
private slots:
    void it_should_throw_exception_when_msg_body_size_is_bigger_than_max();
};

#endif
