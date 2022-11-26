
#pragma once

#include "wordle/utils.hh"
#include "wordle/evaluator.hh"
#include "wordle/player.hh"

namespace wordle
{

template <size_t word_len>
class ExpensivePlayer : public Player<word_len>
{
public:
    ExpensivePlayer(const WordList& solutions,
                    const std::set<std::string>& valid_guesses,
                    const uint8_t debug,
                    const size_t num_threads=1) :
        Player<word_len>(solutions, valid_guesses, debug),
        first_word_("roate"),
        first_guess_(true),
        num_threads_(num_threads),
        thread_pool_(ThreadPool(num_threads_))
    {
//        if (not remaining_map)
//        {
//        // the threaded version of the detector
//        std::vector<std::pair<std::string, std::future<std::map<std::string, WordList>>>> remaining_words;
//        const auto& solutions = Player<word_len>::solutions_;
//        for (const auto &guess : Player<word_len>::valid_guesses_)
//        {
//            avg_remaining_words.emplace_back(guess, thread_pool_.enqueue(
//                    [guess=guess, &solutions]
//                    {
//                        std::map<std::string, WordList> remaining;
//                        for (const auto &[possible_solution, counts]: solutions)
//                        {
//                            remaining[possible_solution] = trim_list(solutions, make_response<word_len>(guess, possible_solution)).size();
//                        }
//
//                        return remaining;
//                    } ));
//        }
//
//        size_t guesses_evaluated = 0;
//        for (auto &[guess, remaining_map] : avg_remaining_words)
//        {
//            std::map<std::string, std::set<std::string> out_remains;
//            if (remaining_map.valid())
//            {
//                const auto r = remaining_map.get();
//
//                std::set<std::string> out_words;
//
//                for (const auto& [solution, remaining_words] : r)
//                {
//                    remaining_map_[guess][solution] = {};
//                    for (const auto& word : remaining_words)
//                    {
//                        remaining_map[
//                    }
//
//
//                }
//
//                if (Player<word_len>::debug_ > 3)
//                {
//                    std::cout << "guess:" << word << " score " << avg << std::endl;
//                }
//
//                if (avg < min_avg_remaining.second)
//                {
//                    if (Player<word_len>::debug_ > 2)
//                    {
//                        std::cout << "new best guess:" << word << " with score of " << avg << std::endl;
//                    }
//                    min_avg_remaining = {word, avg};
//                }
//            }
//
//            ++guesses_evaluated;
//            if (Player<word_len>::debug_ > 2 and guesses_evaluated % 500 == 0)
//            {
//                std::cout << "evaluated " << guesses_evaluated << "/" << avg_remaining_words.size() << std::endl;
//            }
//        }
//        return_guess = min_avg_remaining.first;
//        }
//        else
//        {
//            remaining_map_ = remaining_map.value()
//        }
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
            if (num_threads_ <= 1)
            {
                // when running single-threaded this is faster
                std::pair<std::string, double> min_avg_remaining{"", Player<word_len>::valid_guesses_.size() + 1};
                for (const auto &guess : Player<word_len>::valid_guesses_)
                {
                    double avg_remain = 0;
                    size_t n = 0;
                    for (const auto &[possible_solution, counts]: Player<word_len>::solutions_)
                    {
                        const auto num_remain = trim_list(Player<word_len>::solutions_, make_response<word_len>(guess, possible_solution)).size();
                        ++n;

                        avg_remain = avg_remain + ((num_remain - avg_remain) / n);
                    }

                    if (avg_remain < min_avg_remaining.second)
                    {
                        min_avg_remaining = {guess, avg_remain};
                    }
                }
                return_guess = min_avg_remaining.first;
            }
            else
            {
                // the threaded version of the detector
                std::vector<std::pair<std::string, std::future<double>>> avg_remaining_words;
                const auto& solutions = Player<word_len>::solutions_;
                for (const auto &guess : Player<word_len>::valid_guesses_)
                {
                    avg_remaining_words.emplace_back(guess, thread_pool_.enqueue(
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
    const size_t num_threads_;
    ThreadPool thread_pool_;

//    std::map<std::string, std::map<std::string, std::set<std::string>> remaining_map_;
};

}
