class HumanPlayer:
    def __init__(self, word_list, debug):
        self.word_list = word_list
        self.debug = debug

    def guess(self):
        return input("enter a guess: ")

    def respond(self, yellow, green):
        print("yellow:", yellow, "  green:", green)
