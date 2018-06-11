#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T12:13:14
#
#-------------------------------------------------

QT       += core gui testlib widgets

CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++14 -m64

TARGET = tin_serwer
TEMPLATE = app

DEPENDPATH += . src tests bin/ui src/gui src/movie src/network src/server
INCLUDEPATH += . src/gui src/server src/network tests src/movie

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/gui/mainwindow.cpp \
    src/movie/AudioStream.cpp \
    src/movie/Frame.cpp \
    src/movie/Movie.cpp \
    src/movie/VideoStream.cpp \
    src/network/protocol/Header.cpp \
    src/network/Connection.cpp \
    src/network/protocol/Msg.cpp \
    src/movie/MovieList.cpp \
    src/controller/RootController.cpp \
    src/network/Acceptor.cpp \
    src/network/Reader.cpp

HEADERS += \
    src/gui/mainwindow.h \
    src/movie/AudioStream.hpp \
    src/movie/Frame.hpp \
    src/movie/Movie.hpp \
    src/movie/Resolution.hpp \
    src/movie/VideoStream.hpp \
    src/network/protocol/message_type.hpp \
    src/network/protocol/Header.hpp \
    src/network/protocol/Constants.hpp \
    src/network/Connection.hpp \
    src/network/protocol/Msg.hpp \
    src/movie/MovieList.hpp \
    src/controller/RootController.hpp \
    src/network/Acceptor.hpp \
    src/network/Reader.hpp

test {
    message(Test build)

    SOURCES = \
        tests/tests_main.cpp \
        tests/FrameTest.cpp \
        tests/HeaderTest.cpp \
        tests/MsgTest.cpp \
        tests/MovieTest.cpp \
        src/movie/Frame.cpp \
        src/network/protocol/Header.cpp \
        src/network/protocol/Msg.cpp \
        src/movie/Movie.cpp \
        src/movie/AudioStream.cpp \
        src/movie/VideoStream.cpp \
        tests/MovieListTest.cpp

    HEADERS = \
        tests/FrameTest.hpp \
        tests/HeaderTest.hpp \
        tests/MsgTest.hpp \
        tests/MovieTest.hpp \
        src/movie/Frame.hpp \
        src/network/protocol/Header.hpp \
        src/network/protocol/Msg.hpp \
        src/movie/Movie.hpp \
        src/movie/AudioStream.hpp \
        src/movie/VideoStream.hpp \
        tests/MovieListTest.hpp
} else {
    message(Standard build)
}

DESTDIR = bin/
OBJECTS_DIR = bin/obj
MOC_DIR = bin/moc
RCC_DIR = bin/rcc
UI_DIR = bin/ui

Release:DESTDIR = bin/release
Release:OBJECTS_DIR = bin/release/.obj
Release:MOC_DIR = bin/release/.moc
Release:RCC_DIR = bin/release/.rcc
Release:UI_DIR = bin/release/.ui

Debug:DESTDIR = bin/debug
Debug:OBJECTS_DIR = bin/debug/.obj
Debug:MOC_DIR = bin/debug/.moc
Debug:RCC_DIR = bin/debug/.rcc
Debug:UI_DIR = bin/debug/.ui

unix:!macx{
    INCLUDEPATH += "/home/ignacy/boost/boost_1_67_0/"

    LIBS += -lboost_system \
        -lboost_filesystem \
        -lboost_serialization \
#        -lopencv_highgui \
#        -lopencv_videoio \
#        -lopencv_imgproc \
#        -lopencv_imgcodecs

    LIBS += /usr/local/lib/libopencv_core.so.4.0 \
         /usr/local/lib/libopencv_highgui.so.4.0 \
         /usr/local/lib/libopencv_imgproc.so.4.0 \
        /usr/local/lib/libopencv_imgcodecs.so.4.0 \
        /usr/local/lib/libopencv_videoio.so.4.0
}


macx{

    LIBS += -L"/usr/local/lib"

    LIBS += -lboost_system \
        -lboost_filesystem \
        -lboost_serialization \
        -lopencv_highgui \
        -lopencv_videoio \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_core \
        -lao \
        -lavformat \
        -lavcodec \
        -lavutil \
        -lswscale \
        -ldl \
        -lm

    INCLUDEPATH += "/usr/local/include"
}

FORMS += \
        src/gui/mainwindow.ui

DISTFILES += \
    ./bin
