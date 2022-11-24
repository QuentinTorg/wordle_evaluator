
#pragma once

#include "wordle/utils.hh"
#include "wordle/evaluator.hh"
#include "wordle/player.hh"
#include "wordle/thread_pool.hh"

namespace wordle
{

template <size_t word_len>
class SmartPlayer : public Player<word_len>
{
public:
    SmartPlayer(const WordList& solutions,
                const std::set<std::string>& valid_guesses,
                const uint8_t debug) :
        Player<word_len>(solutions, valid_guesses, debug),
        first_word_("roate"),
        first_guess_(true),
        thread_pool_(std::thread::hardware_concurrency())
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
            std::vector<std::pair<std::string, std::future<double>>> avg_remaining_words;
            const auto& solutions = Player<word_len>::solutions_;
            ThreadPool thread_pool(std::thread::hardware_concurrency());
            for (const auto &guess : Player<word_len>::valid_guesses_)
            {
                avg_remaining_words.emplace_back(guess, thread_pool.enqueue(
                        [guess=guess, &solutions]
                        {
                            double avg_remain = 0;
                            size_t n = 0;
                            for (const auto &[possible_solution, counts]: solutions)
                            {
                                const auto num_remain = trim_list(solutions, make_response<word_len>(guess, possible_solution)).size();
                                ++n;

                                avg_remain = avg_remain + ((num_remain - avg_remain) / n);
                            }
                            return avg_remain;
                        } ));
            }

            std::pair<std::string, double> min_avg_remaining{"", Player<word_len>::valid_guesses_.size() + 1};
            size_t guesses_evaluated = 0;
            for (auto &[word, avg_remain] : avg_remaining_words)
            {
                if (avg_remain.valid())
                {
                    const auto avg = avg_remain.get();

                    if (Player<word_len>::debug_ > 3)
                    {
                        std::cout << "guess:" << word << " score " << avg << std::endl;
                    }

                    if (avg < min_avg_remaining.second)
                    {
                        if (Player<word_len>::debug_ > 2)
                        {
                            std::cout << "new best guess:" << word << " with score of " << avg << std::endl;
                        }
                        min_avg_remaining = {word, avg};
                    }
                }

                ++guesses_evaluated;
                if (Player<word_len>::debug_ > 2 and guesses_evaluated % 500 == 0)
                {
                    std::cout << "evaluated " << guesses_evaluated << "/" << avg_remaining_words.size() << std::endl;
                }
            }
            return_guess = min_avg_remaining.first;
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


        //for (const auto &guess : Player<word_len>::valid_guesses_)
        //{
        //    if (Player<word_len>::debug_ > 2)
        //    {
        //        std::cout << "evaluating guess of:" << guess << " " << guesses_evaluated << "/" << Player<word_len>::valid_guesses_.size() << std::endl;
        //    }
        //    size_t n = 0;
        //    double avg_remaining_words = 0;
        //    for (const auto &[possible_solution, counts]: Player<word_len>::solutions_)
        //    {
        //        const auto num_remain = trim_list(Player<word_len>::solutions_, make_response<word_len>(guess, possible_solution)).size();
        //        ++n;

        //        avg_remaining_words = avg_remaining_words + ((num_remain - avg_remaining_words) / n);
        //    }

        //    if (avg_remaining_words < min_avg_remaining.first)
        //    {
        //        if (Player<word_len>::debug_ > 2)
        //        {
        //            std::cout << "new best guess:" << guess << " with score of " << avg_remaining_words << std::endl;
        //        }
        //        min_avg_remaining = {avg_remaining_words, guess};
        //    }

        //    ++guesses_evaluated;
        //}
//        }


        return return_guess;
    }

private:
    std::string first_word_;
    bool first_guess_;
    ThreadPool thread_pool_;
};

}
