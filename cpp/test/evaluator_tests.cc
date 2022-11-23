
#include <wordle/evaluator.hh>
#include <wordle/player.hh>
#include <wordle/smart_player.hh>

#include <gtest/gtest.h>

using namespace wordle;

TEST(make_response, empty)
{
    constexpr std::string_view guess{};
    constexpr std::string_view word{};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());
}

TEST(make_response, size_mismatch)
{
    {
        constexpr std::string_view guess{"1234"};
        constexpr std::string_view word{"12345"};

        ASSERT_ANY_THROW(make_response<5>(guess, word));
    }

    {
        constexpr std::string_view guess{"12345"};
        constexpr std::string_view word{"1234"};

        ASSERT_ANY_THROW(make_response<5>(guess, word));
    }
}

TEST(make_response, all_green)
{
    constexpr std::string_view guess{"aaaaa"};
    constexpr std::string_view word{"aaaaa"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
        EXPECT_EQ(response[i].second, Color::GREEN);
    }
}

TEST(make_response, all_gray)
{
    constexpr std::string_view guess{"aaaaa"};
    constexpr std::string_view word{"bbbbb"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
        EXPECT_EQ(response[i].second, Color::GRAY);
    }
}

TEST(make_response, one_yellow)
{
    constexpr std::string_view guess{"caaaa"};
    constexpr std::string_view word{"bbbbc"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
    }

    EXPECT_EQ(response[0].second, Color::YELLOW);
}

TEST(make_response, two_yellow)
{
    constexpr std::string_view guess{"cdaaa"};
    constexpr std::string_view word{"bbbdc"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
    }

    EXPECT_EQ(response[0].second, Color::YELLOW);
    EXPECT_EQ(response[1].second, Color::YELLOW);
}

TEST(make_response, yellow_and_gray_duplicate_letter)
{
    constexpr std::string_view guess{"ccaaa"};
    constexpr std::string_view word{"bbbbc"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
    }

    EXPECT_EQ(response[0].second, Color::YELLOW);
    EXPECT_EQ(response[1].second, Color::GRAY);
}

TEST(make_response, green_and_gray_duplicate_letter)
{
    constexpr std::string_view guess{"ccaaa"};
    constexpr std::string_view word{"cbbbb"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
    }

    EXPECT_EQ(response[0].second, Color::GREEN);
    EXPECT_EQ(response[1].second, Color::GRAY);
}

TEST(make_response, green_and_yellow_and_gray_duplicate_letter)
{
    constexpr std::string_view guess{"cccaa"};
    constexpr std::string_view word{"cbbbc"};

    const auto response = make_response<guess.size()>(guess, word);

    ASSERT_EQ(response.size(), guess.size());

    for (size_t i = 0; i < guess.size(); ++i)
    {
        EXPECT_EQ(guess[i], response[i].first);
    }

    EXPECT_EQ(response[0].second, Color::GREEN);
    EXPECT_EQ(response[1].second, Color::YELLOW);
    EXPECT_EQ(response[2].second, Color::GRAY);
}

TEST(make_response, known_edge_cases)
{
    // these cases were part of the test cases for the python program
    {
        constexpr std::string_view guess{"soare"};
        constexpr std::string_view word{"zobos"};

        const auto response = make_response<guess.size()>(guess, word);

        ASSERT_EQ(response.size(), guess.size());

        for (size_t i = 0; i < guess.size(); ++i)
        {
            EXPECT_EQ(guess[i], response[i].first);
        }

        EXPECT_EQ(response[0].second, Color::YELLOW);
        EXPECT_EQ(response[1].second, Color::GREEN);
        EXPECT_EQ(response[2].second, Color::GRAY);
        EXPECT_EQ(response[3].second, Color::GRAY);
        EXPECT_EQ(response[4].second, Color::GRAY);
    }

    {
        constexpr std::string_view guess{"tools"};
        constexpr std::string_view word{"zobos"};

        const auto response = make_response<guess.size()>(guess, word);

        ASSERT_EQ(response.size(), guess.size());

        for (size_t i = 0; i < guess.size(); ++i)
        {
            EXPECT_EQ(guess[i], response[i].first);
        }

        EXPECT_EQ(response[0].second, Color::GRAY);
        EXPECT_EQ(response[1].second, Color::GREEN);
        EXPECT_EQ(response[2].second, Color::YELLOW);
        EXPECT_EQ(response[3].second, Color::GRAY);
        EXPECT_EQ(response[4].second, Color::GREEN);
    }

    {
        constexpr std::string_view guess{"dines"};
        constexpr std::string_view word{"nines"};

        const auto response = make_response<guess.size()>(guess, word);

        ASSERT_EQ(response.size(), guess.size());

        for (size_t i = 0; i < guess.size(); ++i)
        {
            EXPECT_EQ(guess[i], response[i].first);
        }

        EXPECT_EQ(response[0].second, Color::GRAY);
        EXPECT_EQ(response[1].second, Color::GREEN);
        EXPECT_EQ(response[2].second, Color::GREEN);
        EXPECT_EQ(response[3].second, Color::GREEN);
        EXPECT_EQ(response[4].second, Color::GREEN);
    }
}

TEST(play_game, trivial_player)
{
    std::vector<std::string_view> words{"aaaaa"};
    const auto [count, win] = play_game<Player<5>>(words.front(),
                                                   get_word_list(words),
                                                   words,
                                                   0,
                                                   0);

    ASSERT_TRUE(win);
    ASSERT_EQ(count, 1);
}

TEST(test_player, trivial_player)
{
    std::vector<std::string_view> words{"aaaaa"};
    const auto [avg_guesses, win_rate] = test_player<Player<5>>(words,
                                                                get_word_list(words),
                                                                words,
                                                                0,
                                                                0);

    ASSERT_EQ(avg_guesses, 1);
    ASSERT_EQ(win_rate, 1);
}


