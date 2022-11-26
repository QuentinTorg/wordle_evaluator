
#pragma once

#include "wordle/utils.hh"
#include "wordle/evaluator.hh"
#include "wordle/player.hh"

namespace wordle
{

std::map<char, size_t> score_letters(const WordList& words)
{
    std::map<char, size_t> letter_scores;
    for (const auto& [word, letter_counts] : words)
    {
        for (const auto& [letter, count] : letter_counts)
        {
            if (auto letter_score_it = letter_scores.find(letter); letter_score_it != std::end(letter_scores))
            {
                letter_score_it->second += count;
            }
            else
            {
                letter_scores[letter] = count;
            }
        }
    }

    return letter_scores;
}

std::map<std::string, size_t> score_words(const std::set<std::string>& words,
                                          const std::map<char, size_t>& letter_scores,
                                          const WordList& solutions)

{
    std::map<std::string, size_t> word_scores;
    for (const auto& word : words)
    {
        size_t word_score = 0;
        std::set<char> word_letters;
        for (const auto& letter : word)
        {
            // only count each letter once
            if (word_letters.count(letter))
            {
                continue;
            }
            word_letters.insert(letter);

            if (auto letter_score_it = letter_scores.find(letter); letter_score_it != std::end(letter_scores))
            {
                word_score += letter_score_it->second;
            }
        }

        // give the word a bump if it is in the solution list
        if (solutions.count(word) != 0)
        {
            word_score = (word_score + 1) * 1.1;
        }
        word_scores[word] = word_score;
    }

    return word_scores;
}

std::string get_max_scored_word(const std::map<std::string, size_t>& scored_words)
{
    if (scored_words.empty())
    {
        throw std::runtime_error("scored words is empty");
    }

    std::pair<std::string, size_t> max_word = *std::begin(scored_words);
    for (const auto& [word, score] : scored_words)
    {
        if (score > max_word.second)
        {
            max_word = {word, score};
        }
    }

    return max_word.first;
}

template <size_t word_len>
class LetterWeightedPlayer : public Player<word_len>
{
public:
    LetterWeightedPlayer(const WordList& solutions,
                         const std::set<std::string>& valid_guesses,
                         const uint8_t debug) :
        Player<word_len>(solutions, valid_guesses, debug),
        first_word_(""),
        first_guess_(true)
    {
    }

    std::string guess()
    {
        if (Player<word_len>::solutions_.size() == 1)
        {
            return std::begin(Player<word_len>::solutions_)->first;
        }

        std::string return_guess = "";
        if (not first_guess_ or first_word_.empty())
        {
            const auto letter_scores = score_letters(Player<word_len>::solutions_);
            const auto word_scores = score_words(Player<word_len>::valid_guesses_,
                                                 std::move(letter_scores),
                                                 Player<word_len>::solutions_);

            return_guess = get_max_scored_word(word_scores);
        }
        else
        {
            return_guess = first_word_;
            first_guess_ = false;
        }

        if (first_word_.empty())
        {
            first_word_ = return_guess;
            first_guess_ = false;
        }

        return return_guess;
    }

private:
    std::string first_word_;
    bool first_guess_;
};

}
