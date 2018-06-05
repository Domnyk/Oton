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
    tests/ClientTest.cpp \
    tests/test_example.cpp \
    src/main.cpp \
    src/gui/mainwindow.cpp \
    src/server/Server.cpp \
    src/movie/AudioStream.cpp \
    src/movie/Frame.cpp \
    src/movie/Movie.cpp \
    src/movie/VideoStream.cpp \
    src/server/movie_layer.cpp \
    src/server/network_layer.cpp \
    src/network/client.cpp \
    src/network/protocol/parser.cpp \
    src/network/protocol/Header.cpp \
    src/network/protocol/Message.cpp \
    src/network/Acceptor.cpp \
    src/network/Connection.cpp

HEADERS += \
    tests/ClientTest.hpp \
    tests/test_example.h \
    src/gui/mainwindow.h \
    src/server/Server.hpp \
    src/movie/AudioStream.hpp \
    src/movie/Frame.hpp \
    src/movie/Movie.hpp \
    src/movie/Resolution.hpp \
    src/movie/VideoStream.hpp \
    src/server/movie_layer.hpp \
    src/server/network_layer.hpp \
    src/network/client.hpp \
    src/network/protocol/message_type.hpp \
    src/network/protocol/parser.hpp \
    src/network/protocol/Header.hpp \
    src/network/protocol/Constants.hpp \
    src/network/protocol/Message.hpp \
    src/network/Acceptor.hpp \
    src/network/Connection.hpp

test {
    message(Test build)

    SOURCES -= src/main.cpp
    SOURCES += tests/tests_main.cpp \
            tests/HeaderTest.cpp \

    HEADERS += tests/HeaderTest.hpp \
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
        -lopencv_core

        INCLUDEPATH += "/usr/local/include"
}

FORMS += \
        src/gui/mainwindow.ui

DISTFILES += \
    ./bin
