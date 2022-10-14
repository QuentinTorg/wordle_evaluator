from WordleRunner import Player

class RobotPlayer(Player):
    def __init__(self, word_list, debug=0, word_len=5):
        self.word_list = set(word_list)
        self.debug = debug
        self.known_letter_counts = dict()
        self.green = ''
        self.last_guess = not None
        for i in range(word_len):
            self.green += '_'
        self.alphabet = 'abcdefghijklmnopqrstuvwxyz'

    def guess(self):
        if not self.last_guess:
            # once the algorith is defined, the first guess will always be the same. skip the computation
            #self.last_guess = 'soare'
            #self.last_guess = 'sores'
            self.last_guess = 'aeros'

        else:
            #scored_words = self.score_words()
            #scored_words = self.positional_score_words()
            scored_words = self.combined_score_words()
            self.last_guess = scored_words[-1][2]

        #self.last_guess = input("enter a guess: ")
        return self.last_guess

    def combine_greens(self, green, old_green):
        # combine the new green info with what we already know
        new_green = ''
        for old_green_letter, green_letter in zip(old_green, green):
            if old_green_letter != '_':
                new_green += old_green_letter
            elif green_letter != '_':
                new_green += green_letter
            else:
                new_green += '_'
        return new_green

    def update_known_letter_counts(self, yellow):
        # count how many of each letter are expected
        for letter in self.last_guess:
            response_count = yellow.count(letter) + self.green.count(letter)
            if self.known_letter_counts.get(letter, -1) < response_count:
                self.known_letter_counts[letter] = response_count
        if self.debug > 1:
            print("known_letter_counts:", self.known_letter_counts)

    # use known_letter_counts to strip words from list if they don't have required letters
    def strip_words_by_letter_count(self):
        # remove all words missing letters we know are in our word
        # handles the yellow letter cases
        removed_words = set()
        for word in self.word_list:
            for (letter, known_letter_count) in self.known_letter_counts.items():
                word_letter_count = word.count(letter)
                if (known_letter_count == 0 and word_letter_count != 0) or word_letter_count < known_letter_count:
                    removed_words.add(word)
                    break
        self.word_list -= removed_words

    # remove all words without green letters where they belong
    # only look at new green letters because old green were already stripped
    def strip_words_by_missing_green(self, new_green):
        removed_words = set()
        for word in self.word_list:
            for letter, green_letter in zip(word, new_green):
                if green_letter != '_' and letter != green_letter:
                    if self.debug > 2:
                        print("removing:", word)
                    removed_words.add(word)
                    break
        self.word_list -= removed_words

    # remove words that have a letter where we know a non-green letter has proved it cannot exist
    def strip_words_by_incorrect_index(self, green):
        removed_words = set()
        for (index, (green_letter, last_guess_letter)) in enumerate(zip(green, self.last_guess)):
            if green_letter != '_':
                continue
            for word in self.word_list:
                if word[index] == last_guess_letter:
                    removed_words.add(word)
        self.word_list -= removed_words

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
        for letter in self.alphabet:
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

    def respond(self, yellow, new_green):
        # combine the new green letters with existing green letters
        self.green = self.combine_greens(new_green, self.green)

        # remove any words from the list if they have letters in impossible index
        # this is fast, do it first
        self.strip_words_by_incorrect_index(new_green)

        # update how many letters we must know exist in the word after the new guess
        self.update_known_letter_counts(yellow)

        # this removes words based on information given by yellow and gray letters
        self.strip_words_by_letter_count()

        # this removes words based on information given by green letters
        self.strip_words_by_missing_green(new_green)

        if self.debug:
            print("Remaining words in list size:", len(self.word_list))
        if self.debug > 1:
            for word in self.word_list:
                print("  ", word)


