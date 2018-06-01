#include <iostream>
#include "movie_layer.hpp"

movie_layer::movie_layer() : movies_() {

}

void movie_layer::add_new_movie(const string& movie_path) {
    string movie_filename = get_movie_filename(movie_path);

    if (is_movie_loaded(movie_filename)) {
        return;
    }

    Movie movie(movie_path);
    movies_.emplace(movie_filename, movie);

    std::cout << "Movie: " << movie_filename << " has been added" << std::endl;
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

string movie_layer::get_movie_filename(const string& movie_path) {
    // Based on: http://www.cplusplus.com/reference/string/string/find_last_of/
    size_t found = movie_path.find_last_of("/\\");
    string movie_filename = movie_path.substr(found + 1);

    return movie_filename;
}

std::string movie_layer::get_movie_list() {
    string movie_list;

    for(auto& elem: movies_) {
        movie_list += elem.first;
        movie_list += "\n";
    }

    return movie_list;
}
