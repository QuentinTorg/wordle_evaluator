
#pragma once

#include "wordle/utils.hh"

#include <iostream>

namespace wordle
{

template <size_t word_len>
class Player
{
public:
    Player(const WordList& solutions,
           const std::set<std::string>& valid_guesses,
           const uint8_t debug) :
        solutions_(solutions),
        valid_guesses_(valid_guesses),
        debug_(debug)
    {
    }

    std::string guess()
    {
        if (solutions_.empty())
        {
            throw std::runtime_error("no solutions left to guess");
        }

        return std::begin(solutions_)->first;
    }

    void respond(const WordResponse<word_len>& response)
    {
        solutions_ = trim_list(solutions_, response);

        if (debug_ > 2)
        {
            std::cout << "Remaining words\n";
            for (const auto& [word, counts] : solutions_)
            {
                std::cout << "\t" << word << "\n";
            }
            std::cout << std::endl;
        }
    }

    static constexpr size_t size()
    {
        return word_len;
    }

protected:
    WordList solutions_;
    std::set<std::string> valid_guesses_;
    const uint8_t debug_;
};

} // namespace wordle
