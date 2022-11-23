


#include <gtest/gtest.h>
#include <wordle/wordle_player.hh>

using namespace wordle;

TEST(letter_counts, empty)
{
    const std::string word("");

    const auto counts = get_letter_counts(word);

    ASSERT_EQ(counts.size(), 0);
}

TEST(letter_counts, trivial)
{
    const std::string word("aaaaa");

    const auto counts = get_letter_counts(word);

    ASSERT_EQ(counts.size(), 1);
    EXPECT_EQ(counts.at('a'), std::count(word.begin(), word.end(), 'a'));
}

TEST(letter_counts, real_word)
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

TEST(min_max_counts, empty)
{
    const std::string guess{};
    const WordResponse<0> response{};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 0);
    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(min_max_counts, min_all_green)
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

TEST(min_max_counts, min_all_yellow)
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

TEST(min_max_counts, min_all_gray)
{
    const WordResponse<5> response{{{'t', Color::GRAY},
                                    {'e', Color::GRAY},
                                    {'a', Color::GRAY},
                                    {'r', Color::GRAY},
                                    {'s', Color::GRAY}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(min_cnts.size(), 0);
}

TEST(min_max_counts, min_mixed)
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

TEST(min_max_counts, max_all_green)
{
    const WordResponse<5> response{{{'t', Color::GREEN},
                                    {'e', Color::GREEN},
                                    {'a', Color::GREEN},
                                    {'r', Color::GREEN},
                                    {'s', Color::GREEN}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(min_max_counts, max_all_yellow)
{
    const WordResponse<5> response{{{'t', Color::YELLOW},
                                    {'e', Color::YELLOW},
                                    {'a', Color::YELLOW},
                                    {'r', Color::YELLOW},
                                    {'s', Color::YELLOW}}};

    const auto [min_cnts, max_cnts] = get_min_max_letter_counts(response);

    EXPECT_EQ(max_cnts.size(), 0);
}

TEST(min_max_counts, max_all_gray)
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

TEST(min_max_counts, max_mixed_duplicate_partial)
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

TEST(min_max_counts, max_mixed_duplicate_both)
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

TEST(min_max_counts, max_mixed_duplicate_neither)
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

