

#pragma once

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



// make a map of letter counts per letter in the given word
LetterCounts get_letter_counts(const std::string& word);

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

} // namespace wordle
