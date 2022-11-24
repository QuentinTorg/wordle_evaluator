
#pragma once

#include "wordle/utils.hh"

#include <algorithm>
#include <iostream>
#include <set>

namespace wordle
{

// make a yellow/green response based on the guess and the real word
template <size_t word_len>
WordResponse<word_len> make_response(const std::string_view& guess, const std::string_view& word)
{
    if (guess.size() != word_len or word.size() != word_len)
    {
        throw std::runtime_error("word size and guess size do not match");
    }

    // first make a response full of all gray values for each letter
    WordResponse<word_len> response{};
    for (size_t i = 0; i < word_len; ++i)
    {
        response[i] = {guess[i], Color::GRAY};
    }

    // make a letters_used counter to make sure we keep track of how many times each letter has been used when
    // deciding when to populate with yellow vs gray
    LetterCounts guess_letters_used_cnts{};
    const auto word_letter_cnts = get_letter_counts(word);

    // fill all green letters in response
    for (size_t i = 0; i < word_len; ++i)
    {
        const auto& guess_letter = guess[i];
        const auto& resp_letter = word[i];

        // add to the letter counter if it does not exist
        if (not guess_letters_used_cnts.count(guess_letter))
        {
            guess_letters_used_cnts[guess_letter] = 0;
        }

        if (guess_letter == resp_letter)
        {
            response[i].second = Color::GREEN;
            ++guess_letters_used_cnts[guess_letter];
        }
    }

    // fill all of the yellow letters
    for (size_t i = 0; i < word_len; ++i)
    {
        if (response[i].second == Color::GREEN)
        {
            continue;
        }

        const auto& guess_letter = guess[i];

        if (const auto word_cnt_it = word_letter_cnts.find(guess_letter); word_cnt_it != std::end(word_letter_cnts) and guess_letters_used_cnts.at(guess_letter) < word_cnt_it->second)
        {
            // this guess letter exists in the real word and has not been used too many times in the response
            response[i].second = Color::YELLOW;
            ++guess_letters_used_cnts[guess_letter];
        }
    }

    return response;
}

template <typename PlayerT>
std::pair<size_t, bool> play_game(const std::string_view& word,
                                  const WordList& solutions,
                                  const std::set<std::string>& valid_guesses,
                                  const size_t guess_limit,
                                  const uint8_t debug)
{
    if (debug > 1)
    {
        std::cout << "testing word '" << word << "'" << std::endl;
    }

    size_t guess_count = 0;
    bool won = false;

    PlayerT player(solutions, valid_guesses, debug);
    while(true)
    {
        const auto guess = player.guess();
        ++guess_count;

        if (debug > 1)
        {
            std::cout << "player guessed '" << guess << "' for #" << guess_count << std::endl;
        }

        if (guess == word)
        {
            won = true;
            break;
        }
        else if (guess_count != 0 and guess_count == guess_limit)
        {
            // guess limit exceeded, which is a loss
            won = false;
            break;
        }

        const auto response = make_response<PlayerT::size()>(guess, word);

        if (debug > 1)
        {
            std::cout << "\t";
            for (const auto& [letter, color] : response)
            {
                if (color == Color::GREEN)
                {
                    std::cout << "g";
                }
                else if (color == Color::YELLOW)
                {
                    std::cout << "y";
                }
                else if (color == Color::GRAY)
                {
                    std::cout << "_";
                }
            }
            std::cout << std::endl;
        }

        player.respond(response);
    }

    return {guess_count, won};
}

template <typename PlayerT>
std::pair<double, double> test_player(std::set<std::string> test_words,
                                      std::set<std::string> solutions,
                                      std::set<std::string> valid_guesses,
                                      const size_t guess_limit,
                                      const uint8_t debug)
{
    // for protection, add test words to solutions and add all solutions to the valid guesses list
    solutions.insert(std::begin(test_words), std::end(test_words));
    valid_guesses.insert(std::begin(solutions), std::end(solutions));

    if (test_words.empty())
    {
        // test all answers if there are no specific test cases requested
        test_words.insert(std::begin(solutions), std::end(solutions));
    }

    if (debug)
    {
        std::cout << "testing player with " << test_words.size() << " words" << std::endl;
    }

    const auto solutions_with_cnts = get_word_list(solutions);

    double avg_guesses = 0;
    size_t wins = 0;
    size_t games_played = 0;
    for (const auto& word : test_words)
    {
        const auto [guesses, won] = play_game<PlayerT>(word, solutions_with_cnts, valid_guesses, guess_limit, debug);

        ++games_played;
        if (won)
        {
            ++wins;
        }

        avg_guesses = avg_guesses + ((guesses - avg_guesses) / games_played);
        if (debug)
        {
            std::cout << "'" << word << "': " << (won ? "win " : "loss") << " with " << guesses << " guess" << (guesses > 1 ? "es" : "");
            std::cout << " (totals: average guesses:" << avg_guesses << " win rate:" << static_cast<double>(wins)/games_played << " in " << games_played << " games" << std::endl;
            if (debug > 1)
            {
                std::cout << std::endl;
            }
        }
    }

    return {avg_guesses, static_cast<double>(wins)/games_played};
}

} // namespace wordle
