#include "movie_layer.hpp"

movie_layer::movie_layer() : movies_() {

}

void movie_layer::add_new_movie(const string& movie_path) {
    // Based on: http://www.cplusplus.com/reference/string/string/find_last_of/
    size_t found = movie_path.find_last_of("/\\");
    string movie_filename = movie_path.substr(found + 1);

    if (is_movie_loaded(movie_filename)) {
        return;
    }

    Movie movie(movie_path);
    movies_.emplace(movie_filename, movie);
}

void movie_layer::delete_movie(const string& movie_filename) {
    if (!is_movie_loaded(movie_filename)) {
        return;
    }

    movies_.erase(movie_filename);
}

Movie& movie_layer::get_movie(const string& movie_filename) {
    return movies_.at(movie_filename);
}

bool movie_layer::is_movie_loaded(const string& movie_filename) {
    auto it = movies_.find(movie_filename);
    return !(it == movies_.end());
}

std::string movie_layer::get_movie_list() {
    string movie_list;

    for(auto& elem: movies_) {
        movie_list += elem.first;
        movie_list += "\n";
    }

    movie_list += "\r\n\r\n";

    return movie_list;
}
