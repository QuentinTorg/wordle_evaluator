
#pragma once

#include <map>
#include <set>
#include <string_view>
#include <vector>

#include <cstdint>

namespace wordle
{

enum class Color : uint8_t
{
    GREEN,
    YELLOW,
    GRAY
};

using LetterResponse = std::pair<char, Color>;

template <size_t word_len>
using WordResponse = std::array<LetterResponse, word_len>;

using LetterCounts = std::map<char, uint8_t>;

using WordList = std::map<std::string_view, LetterCounts>;

// make a map of letter counts per letter in the given word
LetterCounts get_letter_counts(const std::string_view& word);

WordList get_word_list(const std::vector<std::string_view>& words);

template <size_t word_len>
std::pair<LetterCounts, LetterCounts>
get_min_max_letter_counts(const WordResponse<word_len>& word_resp)
{
    // make min counts
    // sum of all yellow and greens of a single kind of letter is the
    // minimum qty of that letter that must exist in the word
    LetterCounts min_cnts{};
    for (const auto& [letter, resp] : word_resp)
    {
        if (resp == Color::GREEN or resp == Color::YELLOW)
        {
            if (auto cnt_it = min_cnts.find(letter); cnt_it != std::end(min_cnts))
            {
                ++cnt_it->second;
            }
            else
            {
                min_cnts[letter] = 1;
            }
        }
    }

    // use min counts to make max counts
    // if letter is gray, number of yellows and greens for that letter
    // is the max number of that letter in the word. (in other words, the min count)
    LetterCounts max_cnts{};
    for (const auto& [letter, resp] : word_resp)
    {
        if (resp == Color::GRAY)
        {
            if (const auto cnt_it = min_cnts.find(letter); cnt_it != std::end(min_cnts))
            {
                // there are other instances of this letter in the word
                max_cnts[letter] = cnt_it->second;
            }
            else
            {
                // this letter does not exist in the word
                max_cnts[letter] = 0;
            }
        }
    }

    return {min_cnts, max_cnts};
}

// return false if any of the counts in word_counts are below the letter counts in min_counts
bool below_min_counts(const LetterCounts& word_counts, const LetterCounts& min_cnts);

// return false if any of the counts in word_counts are above the letter counts in max
bool above_max_counts(const LetterCounts& word_counts, const LetterCounts& max_cnts);

// return false if any of the letters exist where they should not
//   If green letter in response does not exist in the correct index of word return false
//   If gray or yellow letter in response exists in the wrong index of word return false
//   otherwise return true
template <size_t word_len>
bool invalid_letter_index(const std::string_view& word, const WordResponse<word_len>& response)
{
    if (word.size() != response.size())
    {
        throw std::runtime_error("word size and response size do not match");
    }

    // remove any words that do not have the correct letter for a green index
    for (size_t i = 0; i < word.size(); ++i)
    {
        const auto& [resp_letter, resp_color] = response[i];
        const auto& letter = word[i];

        if (resp_color == Color::GREEN and resp_letter != letter)
        {
            return true;
        }

        if ((resp_color == Color::YELLOW or resp_color == Color::GRAY) and resp_letter == letter)
        {
            return true;
        }
    }

    return false;
}

template <size_t word_len>
WordList trim_list(const WordList& words, const WordResponse<word_len>& response)
{
    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    WordList remaining_words;
    for (const auto& [word, counts] : words)
    {
        if (below_min_counts(counts, min_cnts))
        {
            continue;
        }

        if (above_max_counts(counts, min_cnts))
        {
            continue;
        }

        if (invalid_letter_index(word, response))
        {
            continue;
        }

        // all the checks passed, add it to the list
        remaining_words[word] = counts;
    }

    return remaining_words;
}

} // namespace wordle
