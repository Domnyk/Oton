#ifndef MOVIELAYERTEST_H
#define MOVIELAYERTEST_H

#include <QtTest/QtTest>
#include <QObject>

class MovieListTest : public QObject
{
    Q_OBJECT
private slots:
    void add_movie_should_add_movie_if_it_is_not_already_added();
    void delete_movie_should_delete_existing_movie();
    void delete_movie_should_do_nothing_when_deleting_nonexisting_movie();
    void get_movie_location_should_return_iter_to_end_when_accessing_not_existing_movie();
    void encode_movie_list_as_string_should_return_correctly_formatted_movie_list();
};

#endif
