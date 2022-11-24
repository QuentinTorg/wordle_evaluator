
#pragma once

#include <fstream>
#include <filesystem>
#include <set>
#include <string>

namespace wordle
{

// load a word list file into a set
std::set<std::string> read_word_list(const std::filesystem::path& word_list_path,
                                     const size_t word_len);



} // namespace wordle
