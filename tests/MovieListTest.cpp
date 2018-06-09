#include <iostream>
#include "MovieListTest.hpp"
#include "../src/server/MovieList.hpp"

void MovieListTest::add_movie_should_add_movie_if_it_is_not_already_added() {
    const std::string movie_name = "file.avi";
    const std::string path_to_movie = "/some/random/path/to/" + movie_name;
    MovieList movie_list;

    movie_list.add_movie(path_to_movie);


    const std::string actual_elem = movie_list.get_movie_location(movie_name);
    QCOMPARE(actual_elem, path_to_movie);
}

void MovieListTest::delete_movie_should_delete_existing_movie() {
    const std::string movie_name = "file.avi";
    const std::string path_to_movie = "/some/random/path/to/" + movie_name;
    MovieList movie_list;
    movie_list.add_movie(path_to_movie);

    movie_list.delete_movie(movie_name);

    QVERIFY_EXCEPTION_THROWN(movie_list.get_movie_location(movie_name), runtime_error);
}

/*
 * No way to check if movie_list has changed. This test will run and if some error will be thrown Qt will inform
 */
void MovieListTest::delete_movie_should_do_nothing_when_deleting_nonexisting_movie() {
    const std::string not_existing_movie_filename = "file.avi";
    MovieList movie_list;

    movie_list.delete_movie(not_existing_movie_filename);
}

void MovieListTest::get_movie_location_should_return_iter_to_end_when_accessing_not_existing_movie() {
    const std::string not_existing_movie_name = "file.avi";
    const std::string expected_value = "";
    MovieList movie_list;

    QVERIFY_EXCEPTION_THROWN(movie_list.get_movie_location(not_existing_movie_name), runtime_error);
}

void MovieListTest::encode_movie_list_as_string_should_return_correctly_formatted_movie_list() {
    const std::string movie_name_1 = "file.avi";
    const std::string movie_name_2 = "blob.avi";
    const std::string path_to_movie_1 = "/some/random/path/to/" + movie_name_1;
    const std::string path_to_movie_2 = "/some/other/path/to/" + movie_name_2;
    const std::string expected_result = "file.avi\nblob.avi\n";
    MovieList movie_list;
    movie_list.add_movie(path_to_movie_1);
    movie_list.add_movie(path_to_movie_2);

    QCOMPARE(encode_movie_list_as_string(movie_list), expected_result);
}
