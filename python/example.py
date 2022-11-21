import WordleRunner
from WordlePlayer import Player

# make your own class and test either a single game or the entire dictionary

WordleRunner.test_player(Player, 'word_list.txt', debug=2, test_word_list=['books'])
