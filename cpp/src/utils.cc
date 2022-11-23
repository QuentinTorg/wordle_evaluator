

#include <string>
#include <set>
#include <map>

#include "wordle/utils.hh"


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

// return false if any of the counts in word_counts are below the letter counts in min_counts
bool below_min_counts(const LetterCounts& word_cnts, const LetterCounts& min_cnts)
{
    for (const auto &[letter, min_cnt] : min_cnts)
    {
        // if either letter is not in word or count is lower than minimum
        if (const auto cnt_it = word_cnts.find(letter); cnt_it == std::end(word_cnts) or cnt_it->second < min_cnt)
        {
            return true;
        }
    }

    // nothing out of bounds above, nothing out of bounds
    return false;
}

// return false if any of the counts in word_counts are above the letter counts in max
bool above_max_counts(const LetterCounts& word_cnts, const LetterCounts& max_cnts)
{
    for (const auto &[letter, max_cnt] : max_cnts)
    {
        // if either letter is not in word or count is lower than minimum
        if (const auto cnt_it = word_cnts.find(letter); cnt_it != std::end(word_cnts) and cnt_it->second > max_cnt)
        {
            return true;
        }
    }

    // nothing out of bounds above, nothing out of bounds
    return false;
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
