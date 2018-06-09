#ifndef MOVIETEST_HPP
#define MOVIETEST_HPP

#include <QObject>

#include <QtTest/QtTest>
#include <QObject>

class MovieTest : public QObject
{
    Q_OBJECT
private slots:
    void get_movie_filename_should_correctly_parse_unix_path();
    void get_movie_filename_should_correctly_parse_windows_path();
};

#endif // MOVIETEST_HPP
