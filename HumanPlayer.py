class HumanPlayer:
    def __init__(self, word_list, debug=0, word_len=5):
        self.word_list = word_list
        self.debug = debug

    def guess(self):
        return input("enter a guess: ")

    def respond(self, yellow, green):
        print("yellow:", yellow, "  green:", green)
