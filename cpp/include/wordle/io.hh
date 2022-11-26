
#pragma once

#include "wordle/utils.hh"

#include <nlohmann/json.hpp>

#include <fstream>
#include <filesystem>
#include <set>
#include <string>

namespace wordle
{

// load a word list file into a set
std::set<std::string> read_word_list(const std::filesystem::path& word_list_path,
                                     const size_t word_len);

void to_json(nlohmann::json& j, const Result& r);

void from_json(const nlohmann::json& j, Result& r);

void save_results(const std::filesystem::path& results_path, const std::vector<Result>& results);

std::vector<Result> load_results(const std::filesystem::path& results_path);

std::vector<Result> sort_results_by_num_guesses(std::vector<Result> results);

} // namespace wordle
