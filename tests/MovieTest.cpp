#include "MovieTest.hpp"
#include "../src/movie/Movie.hpp"

void MovieTest::get_movie_filename_should_correctly_parse_unix_path() {
    const std::string movie_name = "file.avi";
    const std::string path_to_movie = "/some/random/path/to/" + movie_name;

    QCOMPARE(Movie::get_filename(path_to_movie), movie_name);
}

void MovieTest::get_movie_filename_should_correctly_parse_windows_path() {
    const std::string movie_name = "file.avi";
    const std::string path_to_movie = "c:\\windows\\awesome\\" + movie_name;

    QCOMPARE(Movie::get_filename(path_to_movie), movie_name);
}
