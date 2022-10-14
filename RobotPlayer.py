from WordlePlayer import Player

class RobotPlayer(Player):
    alphabet = 'abcdefghijklmnopqrstuvwxyz'
    first_guess = None

    def __init__(self, word_list, debug=0, word_len=5):
        super().__init__(word_list, debug=debug, word_len=word_len)
        self.last_guess = RobotPlayer.first_guess

    def guess(self):
        scored_words = self.score_words()
        #scored_words = self.positional_score_words()
        #scored_words = self.combined_score_words()
        self.last_guess = scored_words[-1][1]

        if (not RobotPlayer.first_guess):
            print("New first guess!!!!!:", self.last_guess)
            RobotPlayer.first_guess = self.last_guess

        return self.last_guess

    def positional_score_letters(self):
        letter_scores = []
        for i in range(len(self.green)):
            letter_scores.append(dict())

        for word in self.word_list:
            for (index, letter) in enumerate(word):
                if letter not in letter_scores[index]:
                    letter_scores[index][letter] = 1
                else:
                    letter_scores[index][letter] += 1
        return letter_scores

    def score_letters(self):
        letter_scores = dict()
        for letter in RobotPlayer.alphabet:
            letter_score = 0
            for word in self.word_list:
                if letter in word:
                    letter_score += 1
            letter_scores[letter] = letter_score
        return letter_scores

    def positional_score_words(self):
        letter_scores = self.positional_score_letters()
        scored_words = []
        for word in self.word_list:
            score = 0
            for (index, letter) in enumerate(word):
                score += letter_scores[index][letter]
            scored_words.append((score, word))

        scored_words.sort()
        if self.debug > 1:
            for word in scored_words:
                print(word)
        return scored_words

    def score_words(self):
        letter_scores = self.score_letters()

        scored_words = []
        for word in self.word_list:
            score = 0
            letters = set()
            for letter in word:
                if letter in letters:
                    continue
                letters.add(letter)
                score += letter_scores[letter]
            scored_words.append((score, word))

        scored_words.sort()
        if self.debug > 1:
            for word in scored_words:
                print(word)
        return scored_words

    def combined_score_words(self):
        letter_scores = self.score_letters()
        pos_letter_scores = self.positional_score_letters()
        scored_words = []
        for word in self.word_list:
            pos_score = 0
            score = 0
            letters = set()
            for (index, letter) in enumerate(word):
                pos_score += pos_letter_scores[index][letter]
                if letter not in letters:
                    letters.add(letter)
                    score += letter_scores[letter]

            #scored_words.append((score, pos_score, word))
            scored_words.append((pos_score, score, word))

        scored_words.sort()
        if self.debug > 1:
            for word in scored_words:
                print(word)
        return scored_words
