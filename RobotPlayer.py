from WordlePlayer import Player

class RobotPlayer(Player):
    alphabet = 'abcdefghijklmnopqrstuvwxyz'
    first_guess = None

    def __init__(self, word_list, debug=0, word_len=5):
        super().__init__(word_list, debug=debug, word_len=word_len)
        self.last_guess = RobotPlayer.first_guess

        #self.full_list = dict()
        #for word in word_list:
        #    self.full_list[word] = [word, 0]

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

        missing_indexes = []
        for (index, letter) in enumerate(self.green):
            if letter == '_':
                missing_indexes.append(index)

        for word in self.word_list:
            for index in missing_indexes:
                letter = word[index]
                if letter not in letter_scores[index]:
                    letter_scores[index][letter] = 1
                else:
                    letter_scores[index][letter] += 1
        return letter_scores

    def score_letters(self):
        missing_indexes = []
        for (index, letter) in enumerate(self.green):
            if letter == '_':
                missing_indexes.append(index)

        letter_scores = dict()
        for word in self.word_list:
            scored_letters = set()
            for index in missing_indexes:
                letter = word[index]
                if letter not in letter_scores:
                    letter_scores[letter] = 0

                if letter not in scored_letters:
                    letter_scores[letter] += 1
                    scored_letters.add(letter)
        return letter_scores;


    def score_words(self):
        scores = []
        letter_scores = self.score_letters()
        for word in self.word_list:
            score = 0
            letters_scored = set()
            for (index, letter) in enumerate(word):
                scale = 1
                if letter in self.max_letter_counts:
                    # we can only gain position info from this letter, not letter info
                    scale *= 0.5

                if letter not in letters_scored:
                    score += letter_scores.get(letter, 0) * scale
                    letters_scored.add(letter)
            scores.append((score, word))
        scores.sort()
        if self.debug > 1:
            for word in scores:
                print(word)
        return scores;


    def score_words_bad(self):
        # every letter in a word gets a score only if its th e
        scores = []
        letter_scores = self.positional_score_letters()
        for word in self.word_list:
            score = 0
            letters_scored = set()
            for (index, letter) in enumerate(word):
                scale = 1
                if letter in self.max_letter_counts:
                    # we can only gain position info from this letter, not letter info
                    scale = 1

                if letter not in letters_scored:
                    for letter_score in letter_scores:
                        score += letter_score.get(letter, 0) * scale
            scores.append((score, word))

        scores.sort()
        if self.debug > 1:
            for word in scores:
                print(word)
        return scores;

#    def trim_full_list(self):
#        (cut_words, words) = self.full_list.items()
#        self.full_list = dict()
#        for word in words:
#            cut_word = ''
#            for (word_letter, green_letter) in zip(word, self.green):
#                if green_letter != '_':
#                    cut_word += '_'
#                else:
#                    cut_word += word_letter
#                # check if this cut word is useful first
#                # not enough of the yellow letters or too many of them are both bad
#            self.full_list[cut_word] = [word, 0]


#
#    def score_letters(self):
#        letter_scores = dict()
#        for letter in RobotPlayer.alphabet:
#            letter_score = 0
#            for word in self.word_list:
#                if letter in word:
#                    letter_score += 1
#            letter_scores[letter] = letter_score
#        return letter_scores
#
#    def positional_score_words(self):
#        letter_scores = self.positional_score_letters()
#        scored_words = []
#        for word in self.word_list:
#            score = 0
#            for (index, letter) in enumerate(word):
#                score += letter_scores[index][letter]
#            scored_words.append((score, word))
#
#        scored_words.sort()
#        if self.debug > 1:
#            for word in scored_words:
#                print(word)
#        return scored_words
#
#    def score_words(self):
#        letter_scores = self.score_letters()
#
#        scored_words = []
#        for word in self.word_list:
#            score = 0
#            letters = set()
#            for letter in word:
#                if letter in letters:
#                    continue
#                letters.add(letter)
#                score += letter_scores[letter]
#            scored_words.append((score, word))
#
#        scored_words.sort()
#        if self.debug > 1:
#            for word in scored_words:
#                print(word)
#        return scored_words
#
#    def combined_score_words(self):
#        letter_scores = self.score_letters()
#        pos_letter_scores = self.positional_score_letters()
#        scored_words = []
#        for word in self.word_list:
#            pos_score = 0
#            score = 0
#            letters = set()
#            for (index, letter) in enumerate(word):
#                pos_score += pos_letter_scores[index][letter]
#                if letter not in letters:
#                    letters.add(letter)
#                    score += letter_scores[letter]
#
#            #scored_words.append((score, pos_score, word))
#            scored_words.append((pos_score, score, word))
#
#        scored_words.sort()
#        if self.debug > 1:
#            for word in scored_words:
#                print(word)
#        return scored_words
