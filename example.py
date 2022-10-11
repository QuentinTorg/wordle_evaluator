import WordleRunner
import HumanPlayer

# make your own class and test either a single game or the entire dictionary

# play a single game
word_list = WordleRunner.load_word_list('word_list.txt')
player = HumanPlayer.HumanPlayer(word_list, 2)
[win, guesses] = WordleRunner.play_game(player, 'books', word_list, 2)
print('win:', win, 'guesses', guesses)

# run the entire game. better stats coming soon
#WordleRunner.test_player(HumanPlayer.HumanPlayer, 'word_list.txt', 2)
