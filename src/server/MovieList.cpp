#include <iostream>
#include <algorithm>
#include "MovieList.hpp"
#include "../network/protocol/Constants.hpp"

MovieList::MovieList() : movies_filepaths_() {}

bool MovieList::add_movie(const string& movie_path) {
    bool is_already_on_list =
            find(movies_filepaths_.begin(), movies_filepaths_.end(), movie_path) != movies_filepaths_.end();

    if (is_already_on_list) {
        return false;
    }

    movies_filepaths_.push_back(movie_path);
    return true;
}

void MovieList::delete_movie(const string& movie_filename) {
    auto element_to_delelete = find_movie_location(movie_filename);

    if(element_to_delelete == movies_filepaths_.end()) {
        return;
    }

    movies_filepaths_.erase(element_to_delelete);
}

list<string>::const_iterator MovieList::find_movie_location(const string& movie_filename) const {
    return find_if(movies_filepaths_.begin(), movies_filepaths_.end(), [&](const string& movie_location){
                    return movie_location.find(movie_filename) != string::npos;
           });
}

std::string MovieList::get_movie_location(const string& movie_filename) const {
    auto element_to_return = find_movie_location(movie_filename);

    if(element_to_return == movies_filepaths_.end()) {
        throw runtime_error("No such element on list");
    }

    return *element_to_return;
}

/*
 * Convienience method used to provide properly formated string
 * to be send over network
 */
std::string encode_movie_list_as_string(const MovieList& movie_layer) {
    string movie_list;

    for(auto& movie_filepath: movie_layer.movies_filepaths_) {
        movie_list += Movie::get_filename(movie_filepath);
        movie_list += protocol::MOVIE_DELIMITER;
    }

    return movie_list;
}
