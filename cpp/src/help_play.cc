
#include <wordle/expensive_player.hh>
#include <wordle/player.hh>
#include <wordle/letter_weighted_player.hh>
#include <wordle/evaluator.hh>
#include <wordle/io.hh>

#include <getopt.h>
#include <iostream>
#include <optional>

void usage(const char *prog_name_p)
{
    std::cout << "USAGE: " << prog_name_p << " <options>\n";
    std::cout << "\t-w <solve/words/file>: REQUIRED - Path to a file that contains valid solution words\n";
    std::cout << "\t-g <test/words/file> : OPTIONAL - Path to a file that contians additional words that can be guessed that aren't contained in the solutions\n";
    std::cout << "\t-l <int>             : OPTIONAL - Guess limit for games before player loses. (0 for unlimited guesses)\n";
    std::cout << "\t-j <num_threads>     : OPTIONAL - Number of threads to use during processing. (Default:" << std::thread::hardware_concurrency() << ")\n";
    std::cout << "\t-o <out/path.json>   : OPTIONAL - File path for saving results (Default: wordle_results.json)\n";
    std::cout << "\t-d FLAG              : FLAG - Increase debug level" << std::endl;
    std::cout << "\t-h                   : FLAG - Show this help printout" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    constexpr size_t word_len = 5;

    std::set<std::string> solution_words{};
    std::set<std::string> valid_guess_words{};
    size_t guess_limit = 0;
    size_t num_threads = std::thread::hardware_concurrency();
    uint8_t debug = 3;

    int c;
    while(-1 != (c = getopt(argc, argv, "w:g:l:j:o:dh")))
    {
        switch(c)
        {
            case 'w': solution_words = wordle::read_word_list(optarg, word_len); break;
            case 'g': valid_guess_words = wordle::read_word_list(optarg, word_len); break;
            case 'l': guess_limit = std::stoi(optarg); break;
            case 'j': num_threads = std::stoi(optarg); break;
            case 'd': ++debug; break;
            case 'h': usage(*argv); break;
            case '?': printf("unknown option: %c\n", optopt); usage(*argv); break;
            default: usage(*argv); break;
        }
    }

    valid_guess_words.insert(std::begin(solution_words), std::end(solution_words));
    const auto solutions_with_cnts = wordle::get_word_list(solution_words);

    if (solution_words.empty())
    {
        std::cout << "Must pass a valid word list file. See Usage:" << std::endl;
        usage(*argv);
    }

    std::cout << "Solution words\n";
    for (const auto &word : solution_words)
    {
        std::cout << "\t" << word << "\n";
    }
    std::cout << std::endl;

    wordle::ExpensivePlayer<word_len> player(solutions_with_cnts, valid_guess_words, debug, num_threads);

    std::vector<std::string> guesses;
    bool won = false;
    while(true)
    {
        const auto recommended_guess = player.guess();

        std::cout << "\nRecommended guess:\n\t" << recommended_guess << std::endl;

        std::string override_guess{};
        do
        {
            std::cout << "To override, enter a new " << word_len << " letter guess: " << std::flush;
            std::getline(std::cin, override_guess);
        } while (override_guess.size() != 0 and override_guess.size() != word_len);

        if (override_guess.size() == word_len)
        {
            std::cout << "Overriding recommended guess with '" << override_guess << "'" << std::endl;
            guesses.emplace_back(override_guess);
        }
        else
        {
            guesses.emplace_back(recommended_guess);
        }

        std::cout << "Enter wordle game response. green='g', yellow='y', gray='_'" << std::endl;
        std::string resp{};
        while (true)
        {
            std::cout << "response: ";
            std::getline(std::cin, resp);
            if (resp.size() != word_len)
            {
                std::cout << "response was not the correct size: " << resp << " (" << resp.size() << " chars)" << std::endl;
                continue;
            }
            if (word_len != std::count(std::begin(resp), std::end(resp), 'g') +
                            std::count(std::begin(resp), std::end(resp), 'y') +
                            std::count(std::begin(resp), std::end(resp), '_'))
            {
                std::cout << "Response contains invalid characters: " << resp << std::endl;
                continue;
            }

            // everything is okay, safe to exit
            break;
        }

        if (word_len == std::count(std::begin(resp), std::end(resp), 'g'))
        {
            std::cout << "Winner!" << std::endl;
            won = true;
            break;
        }
        else if (guess_limit != 0 and guesses.size() >= guess_limit)
        {
            // guess limit exceeded, which is a loss
            std::cout << "You lose" << std::endl;
            won = false;
            break;
        }

        wordle::WordResponse<word_len> response;
        for (size_t i = 0; i < response.size(); ++i)
        {
            if (resp[i] == 'g')
            {
                response[i] = wordle::LetterResponse(guesses.back()[i], wordle::Color::GREEN);
            }
            else if (resp[i] == 'y')
            {
                response[i] = wordle::LetterResponse(guesses.back()[i], wordle::Color::YELLOW);
            }
            else if (resp[i] == '_')
            {
                response[i] = wordle::LetterResponse(guesses.back()[i], wordle::Color::GRAY);
            }
            else
            {
                throw std::runtime_error("impossible letter " + resp);
            }
        }

        player.respond(response);
    }

    std::cout << (won ? "Won" : "Lost") << " with " << guesses.size() << " guesses" << std::endl;
    for (const auto &guess : guesses)
    {
        std::cout << "\t" << guess << std::endl;
    }

    //const auto results = wordle::test_player<wordle::LetterWeightedPlayer<5>>(test_words,
    //                                                                          solution_words,
    //                                                                          valid_guess_words,
    //                                                                          guess_limit,
    //                                                                          num_threads,
    //                                                                          debug);

//    std::cout << "average guesses:" << avg_guesses << "  win_rate:" << win_rate << std::endl;
}
