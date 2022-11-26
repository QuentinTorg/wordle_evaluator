
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
    std::cout << "\t-t <test/words/file> : OPTIONAL - Path to a file that contians a limited set of test words\n";
    std::cout << "\t-s <word>            : OPTIONAL - Pass a test word directly to the program without a file\n";
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
    std::set<std::string> test_words{};
    std::set<std::string> valid_guess_words{};
    std::filesystem::path output_path{"wordle_results.json"};
    size_t guess_limit = 0;
    size_t num_threads = std::thread::hardware_concurrency();
    uint8_t debug = 0;

    int c;
    while(-1 != (c = getopt(argc, argv, "w:t:s:g:l:j:o:dh")))
    {
        switch(c)
        {
            case 'w': solution_words = wordle::read_word_list(optarg, word_len); break;
            case 't': test_words = wordle::read_word_list(optarg, word_len); break;
            case 's': test_words.insert(optarg); break;
            case 'g': valid_guess_words = wordle::read_word_list(optarg, word_len); break;
            case 'l': guess_limit = std::stoi(optarg); break;
            case 'j': num_threads = std::stoi(optarg); break;
            case 'o': output_path = optarg; break;
            case 'd': ++debug; break;
            case 'h': usage(*argv); break;
            case '?': printf("unknown option: %c\n", optopt); usage(*argv); break;
            default: usage(*argv); break;
        }
    }


    if (solution_words.empty())
    {
        std::cout << "Must pass a valid word list file. See Usage:" << std::endl;
        usage(*argv);
    }

    if (debug > 1)
    {
        std::cout << "Solution words\n";
        for (const auto &word : solution_words)
        {
            std::cout << "\t" << word << "\n";
        }
        std::cout << std::endl;
    }

    const auto results = wordle::test_player<wordle::ExpensivePlayer<word_len>>(test_words,
                                                                                solution_words,
                                                                                valid_guess_words,
                                                                                guess_limit,
                                                                                num_threads,
                                                                                debug);

    //const auto results = wordle::test_player<wordle::LetterWeightedPlayer<5>>(test_words,
    //                                                                          solution_words,
    //                                                                          valid_guess_words,
    //                                                                          guess_limit,
    //                                                                          num_threads,
    //                                                                          debug);

    save_results(output_path, sort_results_by_num_guesses(results));



//    std::cout << "average guesses:" << avg_guesses << "  win_rate:" << win_rate << std::endl;
}
