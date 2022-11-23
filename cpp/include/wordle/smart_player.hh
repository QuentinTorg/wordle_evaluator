
#pragma once

#include "wordle/utils.hh"
#include "wordle/player.hh"

namespace wordle
{

template <size_t word_len>
class SmartPlayer : public Player<word_len>
{
public:
    SmartPlayer(const WordList& solutions,
           const std::vector<std::string_view>& valid_guesses,
           const uint8_t debug) :
        Player<word_len>(solutions, valid_guesses, debug)
    {
    }

    std::string_view guess() const
    {
        std::pair<double, std::string_view> min_avg_remaining{Player<word_len>::valid_guesses_.size() + 1, ""};
        for (const auto &guess : Player<word_len>::valid_guesses_)
        {
            size_t n = 0;
            double avg_remaining_words = 0;
            for (const auto &[possible_solution, counts]: Player<word_len>::solutions_)
            {
                const auto num_remain = trim_list(Player<word_len>::solutions_, make_response<word_len>(guess, possible_solution)).size();
                ++n;

                avg_remaining_words = avg_remaining_words + ((num_remain - avg_remaining_words) / n);
            }

            if (avg_remaining_words < min_avg_remaining.first)
            {
                min_avg_remaining = {avg_remaining_words, guess};
            }
        }

        return min_avg_remaining.second;
    }
};

}
