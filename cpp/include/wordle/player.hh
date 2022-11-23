
#pragma once

#include "wordle/utils.hh"

namespace wordle
{

template <size_t word_len>
class Player
{
public:
    Player(const WordList& solutions,
           const std::vector<std::string_view>& valid_guesses,
           const uint8_t debug) :
        solutions_(solutions),
        valid_guesses_(valid_guesses),
        debug_(debug)
    {
    }

    std::string_view guess() const
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
    }

    static constexpr size_t size()
    {
        return word_len;
    }

protected:
    WordList solutions_;
    std::vector<std::string_view> valid_guesses_;
    const uint8_t debug_;
};

} // namespace wordle
