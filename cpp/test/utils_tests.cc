


#include <gtest/gtest.h>

#include <wordle/utils.hh>

using namespace wordle;

TEST(get_letter_counts, empty)
{
    const std::string word("");

    const auto counts = get_letter_counts(word);

    ASSERT_EQ(counts.size(), 0);
}

TEST(get_letter_counts, trivial)
{
    const std::string word("aaaaa");

    const auto counts = get_letter_counts(word);

    ASSERT_EQ(counts.size(), 1);
    EXPECT_EQ(counts.at('a'), std::count(word.begin(), word.end(), 'a'));
}

TEST(get_letter_counts, real_word)
{
    const std::string word("tears");

    const auto counts = get_letter_counts(word);

    ASSERT_EQ(counts.size(), 5);
    EXPECT_EQ(counts.at('t'), std::count(word.begin(), word.end(), 't'));
    EXPECT_EQ(counts.at('e'), std::count(word.begin(), word.end(), 'e'));
    EXPECT_EQ(counts.at('a'), std::count(word.begin(), word.end(), 'a'));
    EXPECT_EQ(counts.at('r'), std::count(word.begin(), word.end(), 'r'));
    EXPECT_EQ(counts.at('s'), std::count(word.begin(), word.end(), 's'));
}

TEST(get_min_max_counts, empty)
{
    const std::string guess{};
    const WordResponse<0> response{};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 0);
    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(get_min_max_counts, min_all_green)
{
    const WordResponse<5> response{{{'t', Color::GREEN},
                                    {'e', Color::GREEN},
                                    {'a', Color::GREEN},
                                    {'r', Color::GREEN},
                                    {'s', Color::GREEN}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 5);
    EXPECT_EQ(min_cnts.at('t'), 1);
    EXPECT_EQ(min_cnts.at('e'), 1);
    EXPECT_EQ(min_cnts.at('a'), 1);
    EXPECT_EQ(min_cnts.at('r'), 1);
    EXPECT_EQ(min_cnts.at('s'), 1);
}

TEST(get_min_max_counts, min_all_yellow)
{
    const WordResponse<5> response{{{'t', Color::YELLOW},
                                    {'e', Color::YELLOW},
                                    {'a', Color::YELLOW},
                                    {'r', Color::YELLOW},
                                    {'s', Color::YELLOW}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 5);
    EXPECT_EQ(min_cnts.at('t'), 1);
    EXPECT_EQ(min_cnts.at('e'), 1);
    EXPECT_EQ(min_cnts.at('a'), 1);
    EXPECT_EQ(min_cnts.at('r'), 1);
    EXPECT_EQ(min_cnts.at('s'), 1);
}

TEST(get_min_max_counts, min_all_gray)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'e', Color::GRAY},
                                    {'a', Color::GRAY},
                                    {'r', Color::GRAY},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 0);
}

TEST(get_min_max_counts, min_mixed)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'e', Color::GREEN},
                                    {'a', Color::YELLOW},
                                    {'r', Color::GRAY},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 2);
    EXPECT_EQ(min_cnts.at('e'), 1);
    EXPECT_EQ(min_cnts.at('a'), 1);
}

TEST(get_min_max_counts, max_all_green)
{
    const WordResponse<5> response{{{'t', Color::GREEN},
                                    {'e', Color::GREEN},
                                    {'a', Color::GREEN},
                                    {'r', Color::GREEN},
                                    {'s', Color::GREEN}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(get_min_max_counts, max_all_yellow)
{
    const WordResponse<5> response{{{'t', Color::YELLOW},
                                    {'e', Color::YELLOW},
                                    {'a', Color::YELLOW},
                                    {'r', Color::YELLOW},
                                    {'s', Color::YELLOW}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(get_min_max_counts, max_all_gray)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'e', Color::GRAY},
                                    {'a', Color::GRAY},
                                    {'r', Color::GRAY},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 5);
    EXPECT_EQ(max_cnts.at('t'), 0);
    EXPECT_EQ(max_cnts.at('e'), 0);
    EXPECT_EQ(max_cnts.at('a'), 0);
    EXPECT_EQ(max_cnts.at('r'), 0);
    EXPECT_EQ(max_cnts.at('s'), 0);
}

TEST(get_min_max_counts, max_mixed_duplicate_partial)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'o', Color::YELLOW},
                                    {'o', Color::GRAY},
                                    {'l', Color::GREEN},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 3);
    EXPECT_EQ(max_cnts.at('t'), 0);
    EXPECT_EQ(max_cnts.at('o'), 1);
    EXPECT_EQ(max_cnts.at('s'), 0);
}

TEST(get_min_max_counts, max_mixed_duplicate_both)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'o', Color::YELLOW},
                                    {'o', Color::GREEN},
                                    {'l', Color::GREEN},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 2);
    EXPECT_EQ(max_cnts.at('t'), 0);
    EXPECT_EQ(max_cnts.at('s'), 0);
}

TEST(get_min_max_counts, max_mixed_duplicate_neither)
{
    const WordResponse<5> response{{{'t', Color::GREEN},
                                    {'o', Color::GRAY},
                                    {'o', Color::GRAY},
                                    {'l', Color::GREEN},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 2);
    EXPECT_EQ(max_cnts.at('o'), 0);
    EXPECT_EQ(max_cnts.at('s'), 0);
}

TEST(below_min_counts, empty)
{
    const LetterCounts word_cnts{};
    const LetterCounts min_cnts{};

    ASSERT_FALSE(below_min_counts(word_cnts, min_cnts));
}

TEST(below_min_counts, same_counts)
{
    const LetterCounts word_cnts{{'a', 1}};
    const LetterCounts min_cnts{{'a', 1}};

    ASSERT_FALSE(below_min_counts(word_cnts, min_cnts));
}

TEST(below_min_counts, higher_counts)
{
    const LetterCounts word_cnts{{'a', 2}};
    const LetterCounts min_cnts{{'a', 1}};

    ASSERT_FALSE(below_min_counts(word_cnts, min_cnts));
}

TEST(below_min_counts, lower_counts)
{
    const LetterCounts word_cnts{{'a', 1}};
    const LetterCounts min_cnts{{'a', 2}};

    ASSERT_TRUE(below_min_counts(word_cnts, min_cnts));
}

TEST(below_min_counts, missing)
{
    const LetterCounts word_cnts{};
    const LetterCounts min_cnts{{'a', 1}};

    ASSERT_TRUE(below_min_counts(word_cnts, min_cnts));
}

TEST(above_max_counts, empty)
{
    const LetterCounts word_cnts{};
    const LetterCounts max_cnts{};

    ASSERT_FALSE(above_max_counts(word_cnts, max_cnts));
}

TEST(above_max_counts, same_counts)
{
    const LetterCounts word_cnts{{'a', 1}};
    const LetterCounts max_cnts{{'a', 1}};

    ASSERT_FALSE(above_max_counts(word_cnts, max_cnts));
}

TEST(above_max_counts, higher_counts)
{
    const LetterCounts word_cnts{{'a', 2}};
    const LetterCounts max_cnts{{'a', 1}};

    ASSERT_TRUE(above_max_counts(word_cnts, max_cnts));
}

TEST(above_max_counts, lower_counts)
{
    const LetterCounts word_cnts{{'a', 1}};
    const LetterCounts max_cnts{{'a', 2}};

    ASSERT_FALSE(above_max_counts(word_cnts, max_cnts));
}

TEST(above_max_counts, missing)
{
    const LetterCounts word_cnts{};
    const LetterCounts max_cnts{{'a', 1}};

    ASSERT_FALSE(above_max_counts(word_cnts, max_cnts));
}

TEST(invalid_letter_index, empty)
{
    const std::string word{""};
    const WordResponse<0> resp{};

    ASSERT_FALSE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, size_mismatch)
{
    {
        const std::string word{"aa"};
        const WordResponse<1> resp{{{'a', Color::GRAY}}};

        ASSERT_ANY_THROW(invalid_letter_index(word, resp));
    }

    {
        const std::string word{"a"};
        const WordResponse<2> resp{{{'a', Color::GRAY}, {'a', Color::GRAY}}};

        ASSERT_ANY_THROW(invalid_letter_index(word, resp));
    }
}

TEST(invalid_letter_index, green_valid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'a', Color::GREEN}}};

    ASSERT_FALSE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, green_invalid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'b', Color::GREEN}}};

    ASSERT_TRUE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, gray_valid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'b', Color::GRAY}}};

    ASSERT_FALSE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, gray_invalid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'a', Color::GRAY}}};

    ASSERT_TRUE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, yellow_valid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'b', Color::YELLOW}}};

    ASSERT_FALSE(invalid_letter_index(word, resp));
}

TEST(invalid_letter_index, yellow_invalid)
{
    const std::string word{"a"};
    const WordResponse<1> resp{{{'a', Color::YELLOW}}};

    ASSERT_TRUE(invalid_letter_index(word, resp));
}
