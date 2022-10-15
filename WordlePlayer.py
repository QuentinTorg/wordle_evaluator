
class Player:
    def __init__(self, word_list, debug=0, word_len=5):
        self.word_list = set(word_list)
        self.debug = debug
        self.max_letter_counts = dict()
        self.min_letter_counts = dict()
        self.last_guess = None
        self.green = ''
        for i in range(word_len):
            self.green += '_'

    # combine the new green info with what we already know
    @staticmethod
    def combine_greens(last_guess, response, old_green):
        new_green = ''
        for (letter, resp_letter, old_green_letter) in zip(last_guess, response, old_green):
            if old_green_letter != '_':
                new_green += old_green_letter
            elif resp_letter == 'g':
                new_green += letter
            else:
                new_green += '_'
        return new_green

    # return a set of all words that have letters in an index that is impossible
    # this is all instances of a yellow letter in an index
    # this is also all instances of words without the green letters in the correct index
    @staticmethod
    def incorrect_index_words(green, last_guess, word_list):
        # remove a word if it has a letter in a known wrong index
        # remove a word if it does not have correct letter for green
        removed_words = set()
        for (index, (green_letter, last_guess_letter)) in enumerate(zip(green, last_guess)):
            for word in word_list:
                if green_letter == '_' and word[index] == last_guess_letter:
                    # remove a word if it has a letter in a known wrong index
                    removed_words.add(word)
                elif green_letter != '_' and green_letter != word[index]:
                    # remove a word if it does not have correct letter for green
                    removed_words.add(word)

        return removed_words

    # count how many of each letter are expected
    def update_letter_counts(self, response, last_guess):
        resp_counts = dict()
        guess_counts = dict()

        for (resp_letter, letter) in zip(response, last_guess):
            guess_counts[letter] = guess_counts.get(letter, 0) + 1
            if resp_letter != '_':
                resp_counts[letter] = resp_counts.get(letter, 0) + 1

        for (letter, guess_count) in guess_counts.items():
            response_count = resp_counts.get(letter, 0)

            # can only set the max if any quantity of the letter was gray
            if guess_count > response_count:
                self.max_letter_counts[letter] = response_count

            # the word is proven to have at least this many of the letter
            if response_count > self.min_letter_counts.get(letter, -1):
                self.min_letter_counts[letter] = response_count

        if self.debug > 1:
            print("max_letter_counts:", self.max_letter_counts)
            print("min_letter_counts:", self.min_letter_counts)

    # use known_letter_counts to strip words from list if they don't have required letters
    @staticmethod
    def strip_words_by_letter_count(word_list, min_letter_counts, max_letter_counts):
        removed_words = set()
        for word in word_list:
            # min_letters are a superset of max_letters
            # iterate over min_letters then check if max_letters exist
            for (letter, min_letter_count) in min_letter_counts.items():
                word_letter_count = word.count(letter)
                if word_letter_count < min_letter_count:
                    removed_words.add(word)
                    continue
                max_letter_count = max_letter_counts.get(letter, len(word))
                if word_letter_count > max_letter_count:
                    removed_words.add(word)

        return removed_words


    def respond(self, response, last_guess):
        # update how many letters must exist in the word after the new guess
        # must use new green because it is coupled with the info from yellow
        self.update_letter_counts(response, last_guess)

        # combine the new green letters with existing green letters
        self.green = Player.combine_greens(last_guess, response, self.green)

        # letters that do not exist in the word are handled by functions above
        # this function only handles letters that exist where green should exist
        # words yellow letter in the wrong place
        self.word_list -= Player.incorrect_index_words(self.green, last_guess, self.word_list)

        # this removes words based on information given by yellow and gray letters
        self.word_list -= Player.strip_words_by_letter_count(self.word_list, self.min_letter_counts, self.max_letter_counts)

        if self.debug:
            print("Remaining words in list size:", len(self.word_list))
        if self.debug > 2:
            for word in self.word_list:
                print("  ", word)

    # this is the guess method that should be overridden by the child class
    def guess(self):
        self.last_guess = input("enter a guess: ")
        return self.last_guess
