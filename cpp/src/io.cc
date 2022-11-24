
#include "wordle/io.hh"

#include <iostream>

namespace wordle
{

std::set<std::string> read_word_list(const std::filesystem::path& word_list_path,
                                     const size_t word_len)
{
    std::ifstream word_list_file;
    word_list_file.open(word_list_path);
    if (not word_list_file.is_open())
    {
        throw std::runtime_error("Failed to open word list at " + word_list_path.string());
    }

    std::string word;
    std::set<std::string> word_list;
    while (word_list_file >> word)
    {
        if (word.size() != word_len)
        {
            throw std::runtime_error("word does not have correct length: " + word);
        }
        word_list.insert(word);
    }

    return word_list;
}

} // namespace wordle
