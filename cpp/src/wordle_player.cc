

#include <string>
#include <set>
#include <map>

#include "wordle/wordle_player.hh"


// - green letter definitely exists at the index
// - gray or yellow letter definitely does not exist at index
//

namespace wordle
{

LetterCounts get_letter_counts(const std::string& word)
{
    LetterCounts counts;
    for (const auto& letter : word)
    {
        if (auto count_it = counts.find(letter); count_it != std::end(counts))
        {
            ++count_it->second;
        }
        else
        {
            counts[letter] = 1;
        }
    }

    return counts;
}





//class WordlePlayer
//{
//public:
//    WordlePlayer(const std::set<std::string>& all_words) :
//        all_words_(all_words)
//    {
//    }
//
//
//private:
//    std::set<std::string> all_words_;
//}

} // namespace wordle
