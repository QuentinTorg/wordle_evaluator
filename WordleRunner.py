def load_word_list(word_list_file, word_len):
    word_list = set()

    with open(word_list_file) as f:
        for line in f:
            word = line.strip()
            if len(word) != word_len:
                raise Exception("invalid word length in word list:" + word)
            word_list.add(word)
    return word_list

# @param player The player object that will be tested
# @param word The word that the player is being tested against
# @param word_list All possible words that could be
# @param debug Debug level. Higher debug level prints more details
# @param guess_limit Limit how many guesses a user can make before it loses (0 is infinite)
# @param word_len expected word length
def play_game(player, word, word_list, debug=0, guess_limit=0, word_len=5):
    if word not in word_list:
        raise Exception("Word must exist in word list")

    guess_count = 0
    win = False

    while True:
        guess = player.guess()
        guess_count += 1

        if len(guess) != word_len:
            raise Exception("invalid word length")
        elif guess not in word_list:
            raise Exception("Guess does not exist in the dictionary:" + guess)

        if debug > 1:
            print("guess:", guess)

        green = ''
        for guess_letter, word_letter in zip(guess, word):
            if guess_letter == word_letter:
                green += guess_letter
            else:
                green += "_"

        if green.count('_') == 0:
            win = True
            break

        if guess_limit != 0 and guess_count >= guess_limit:
            # this is the last guess, did not win, skip populating yellow
            break

        yellow = ''
        for guess_letter, green_letter in zip(guess, green):
            if green_letter != '_':
                yellow += '_'
            elif ((guess_letter in word) and
                    (yellow.count(guess_letter) + green.count(guess_letter) < word.count(guess_letter))):
                yellow += guess_letter
            else:
                yellow += '_'

        player.respond(yellow, green)
    return (win, guess_count)

# @param WordlePlayer The player class that will be tested
# @param word_list_file The word list dictionary that contains all possible words
# @param debug Debug level. Higher debug level prints more details
# @param guess_limit Limit how many guesses a user can make before it loses (0 is infinite)
# @param word_len expected word length
# @param test_word_list An optional smaller list of words that the player will be tested against. Will test against entire list otherwise
def test_player(WordlePlayer, word_list_file, debug=0, guess_limit=0, word_len=5, test_word_list=None):
    word_list = load_word_list(word_list_file, word_len)

    if debug:
        print("word list contains", len(word_list), "words")

    guess_count = 0
    win_count = 0
    game_count = 0

    if not test_word_list:
        test_word_list = word_list

    for word in test_word_list:
        if debug:
            print(word)
        player = WordlePlayer(word_list, debug=debug, word_len=word_len)
        [win, guesses] = play_game(player, word, word_list, debug=debug, guess_limit=guess_limit, word_len=word_len)

        game_count += 1
        guess_count += guesses
        if win:
            win_count += 1

        if debug:
            print('word:', word, 'won:', win, ' guesses:', guesses, 'win count:', win_count, 'total guesses:', guess_count, 'game count:', game_count)

    print('win count:', win_count, 'total guesses:', guess_count, 'game count:', game_count)
