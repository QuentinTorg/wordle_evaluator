import WordleRunner
import HumanPlayer

# make your own class and test either a single game or the entire dictionary

WordleRunner.test_player(HumanPlayer.HumanPlayer, 'word_list.txt', debug=2, test_word_list=['books'])
