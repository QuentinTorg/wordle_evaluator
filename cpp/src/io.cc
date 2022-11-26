
#include "wordle/io.hh"

#include <iostream>

namespace wordle
{

std::set<std::string> read_word_list(const std::filesystem::path& word_list_path,
                                     const size_t word_len)
{
    std::ifstream word_list_file;
    word_list_file.open(word_list_path);
    if (not word_list_file.is_open())
    {
        throw std::runtime_error("Failed to open word list at " + word_list_path.string());
    }

    std::string word;
    std::set<std::string> word_list;
    while (word_list_file >> word)
    {
        if (word.size() != word_len)
        {
            throw std::runtime_error("word does not have correct length: " + word);
        }
        word_list.insert(word);
    }

    return word_list;
}

void to_json(nlohmann::json& j, const Result& r)
{
    j = nlohmann::json{{"word", r.word}, {"win", r.win}, {"guesses", r.guesses}};
}

void from_json(const nlohmann::json& j, Result& r)
{
    j.at("word").get_to(r.word);
    j.at("win").get_to(r.win);
    j.at("guesses").get_to(r.guesses);
}

//void to_json(nlohmann::json& j, const Results& r)
//{
//    j = nlohmann::json{{"average_guesses", r.average_guesses},
//                       {"max_guesses", r.max_guesses},
//                       {"win_rate", r.win_rate},
//                       {"num_games", r.num_games},
//                       {"game_results", rgame_results}};
//}
//
//void from_json(const nlohmann::json& j, Results& r)
//{
//    j.at("average_guesses").get_to(r.average_guesses);
//    j.at("max_guesses").get_to(r.max_guesses);
//    j.at("win_rate").get_to(r.win_rate);
//    j.at("num_games").get_to(r.num_games);
//    j.at("game_results").get_to(r.game_results);
//}

void save_results(const std::filesystem::path& results_path, const std::vector<Result>& results)
{
    double total_guesses = 0;
    size_t wins = 0;
    size_t max_guesses = 0;
    for (const auto &result : results)
    {
        total_guesses += result.guesses.size();
        if (result.win)
        {
            ++wins;
        }

        if (result.guesses.size() > max_guesses)
        {
            max_guesses = result.guesses.size();
        }
    }

    std::ofstream f(results_path);

    nlohmann::json r(results);
    nlohmann::json j;
    j["_average_guesses"] = static_cast<double>(total_guesses) / results.size();
    j["_max_guesses"] = max_guesses;
    j["_win_rate"] = static_cast<double>(wins) / results.size();
    j["_num_games"] = results.size();
    j["game_results"] = results;

    f << std::setw(4) << j << std::endl;
}

std::vector<Result> load_results(const std::filesystem::path& results_path)
{
    std::ifstream f(results_path);
    nlohmann::json data = nlohmann::json::parse(f);

    return data["game_results"].get<std::vector<Result>>();
}

std::vector<Result> sort_results_by_num_guesses(std::vector<Result> results)
{
    std::sort(std::begin(results), std::end(results),
              [](const auto& a, const auto& b) -> bool
              {
                  return a.guesses.size() > b.guesses.size();
              });
    return results;
}

} // namespace wordle
