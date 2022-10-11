
def play(player, word, word_list, debug):
    guess_count = 0
    win = False

    for guess_num in range(6):
        guess_count += 1

        guess = player.guess()

        if len(guess) != 5:
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
            break;
        if guess_num == 5:
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

def load_word_list(word_list_file):
    word_list = set()

    with open(word_list_file) as f:
        for line in f:
            word = line.strip()
            if len(word) != 5:
                raise Exception("invalid word length in word list:" + word)
            word_list.add(word)
    return word_list


def test_player(WordlePlayer, word_list_file, debug):
    word_list = load_word_list(word_list_file)

    if debug:
        print("word list contains", len(word_list), "words")

    guess_count = 0
    win_count = 0
    game_count = 0

    for word in word_list:
        player = WordlePlayer(word_list, debug)
        [win, guess_count] = play(player, word, debug)

        game_count += 1
        guess_count += guess_count
        if win:
            win_count += 1

        if debug:
            print(word, 'guess count:', guess_count, 'win:', win, 'total wins:', total_win_count, 'total guesses:', total_guess_count)


    print(word, 'guess count:', guess_count, 'win:', win, 'total wins:', total_win_count, 'total guesses:', total_guess_count)



class HumanPlayer:
    def __init__(self, word_list, debug):
        self.word_list = word_list
        self.debug = debug

    def guess(self):
        return input("enter a guess: ")

    def respond(self, yellow, green):
        print("yellow:", yellow, "  green:", green)


# make your own class and test either a single game or the entire dictionary

# play a single game
word_list = load_word_list('word_list.txt')
player = HumanPlayer(word_list, 2)
[win, guesses] = play(player, 'howdy', word_list, 2)
print('win:', win, 'guesses', guesses)

# run the entire game. better stats coming soon
test_player(QWordleMaster, 'word_list.txt', 2)

