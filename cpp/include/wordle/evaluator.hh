
#pragma once

#include "wordle/utils.hh"
#include "wordle/thread_pool.hh"

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
Result play_game(const std::string& word,
                 const WordList& solutions,
                 const std::set<std::string>& valid_guesses,
                 const size_t guess_limit,
                 const uint8_t debug)
{
    if (debug > 1)
    {
        std::cout << "testing word '" << word << "'" << std::endl;
    }

    bool won = false;

    PlayerT player(solutions, valid_guesses, debug);
    std::vector<std::string> guesses;
    while(true)
    {
        guesses.emplace_back(player.guess());

        if (debug > 1)
        {
            std::cout << "player guessed '" << guesses.back() << "' for #" << guesses.size() << std::endl;
        }

        if (guesses.back() == word)
        {
            won = true;
            break;
        }

        if (guess_limit != 0 and guesses.size() >= guess_limit)
        {
            // guess limit exceeded, which is a loss
            won = false;
            break;
        }

        const auto response = make_response<PlayerT::size()>(guesses.back(), word);

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

    return Result{word, guesses, won};
}

template <typename PlayerT>
std::vector<Result> test_player(std::set<std::string> test_words,
                                std::set<std::string> solutions,
                                std::set<std::string> valid_guesses,
                                const size_t guess_limit,
                                const size_t num_threads,
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
        std::cout << "testing player with " << test_words.size() << " words and " << num_threads << " threads" << std::endl;
    }

    const auto solutions_with_cnts = get_word_list(solutions);

    ThreadPool thread_pool(num_threads);
    std::vector<std::future<Result>> processing_results;
    for (const auto& word : test_words)
    {
        processing_results.emplace_back(thread_pool.enqueue(
                [&word, &solutions_with_cnts, &valid_guesses, &guess_limit, &debug]
                {
                    return play_game<PlayerT>(word, solutions_with_cnts, valid_guesses, guess_limit, debug);
                } ));
    }

    size_t wins = 0;
    double avg_guesses = 0;

    std::vector<Result> results;
    for (auto& result : processing_results)
    {
        if (result.valid())
        {
            results.emplace_back(result.get());

            if (debug)
            {
                avg_guesses = avg_guesses + ((results.back().guesses.size() - avg_guesses) / results.size());

                if (results.back().win)
                {
                    ++wins;
                }

                std::cout << "'" << results.back().word << "': " << (results.back().win ? "win " : "loss") <<
                             " with " << results.back().guesses.size() << " guess" <<
                             (results.back().guesses.size() > 1 ? "es" : "") << " (totals: average guesses:" <<
                             avg_guesses << " win rate:" << static_cast<double>(wins)/results.size() << " in " <<
                             results.size() << " games" << std::endl;
                if (debug > 1)
                {
                    std::cout << std::endl;
                }
            }
        }
    }


//    double avg_guesses = 0;
//    size_t wins = 0;
//    std::vector<Result> results;
//    for (const auto& word : test_words)
//    {
//        const auto result = play_game<PlayerT>(word, solutions_with_cnts, valid_guesses, guess_limit, debug);
//        results.push_back(result);
//
//        if (result.win)
//        {
//            ++wins;
//        }
//
//        avg_guesses = avg_guesses + ((result.num_guesses - avg_guesses) / results.size());
//        if (debug)
//        {
//            std::cout << "'" << word << "': " << (result.win ? "win " : "loss") << " with " << result.num_guesses << " guess" << (result.num_guesses > 1 ? "es" : "");
//            std::cout << " (totals: average guesses:" << avg_guesses << " win rate:" << static_cast<double>(wins)/results.size() << " in " << results.size() << " games" << std::endl;
//            if (debug > 1)
//            {
//                std::cout << std::endl;
//            }
//        }
//    }

    return results;
}

} // namespace wordle
