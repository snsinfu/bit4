import collections
import os
import re

import numpy as np

def main():
    corpus_path = os.getenv('CORPUS') or '/usr/share/dict/words'
    known_letters = 'abcdefghijklmnopqrstuvwxyz'

    with open(corpus_path) as corpus:
        words = [word.lower() for word in re.findall('[a-zA-Z]+', corpus.read())
                                       if len(word) >= 2]

    # Find Markovian stationary distribution from bigrams.
    bigrams = []
    for word in words:
        bigrams.extend(word[i:(i + 2)] for i in range(len(word) - 1))
    bigram_counter = collections.Counter(bigrams)

    transition_matrix = np.zeros((len(known_letters), len(known_letters)))
    for bigram, frequency in bigram_counter.items():
        i = known_letters.index(bigram[0])
        j = known_letters.index(bigram[1])
        transition_matrix[i, j] = frequency

    transition_matrix = transition_matrix / transition_matrix.sum(axis=1)[:, np.newaxis]

    p = null(transition_matrix.T - np.eye(26))
    p = p / p.sum()
    show('Markov', p)

    # Simple frequency calculation.
    letters = []
    for word in words:
        letters.extend(word)
    letter_counter = collections.Counter(letters)

    q = np.zeros(len(known_letters))
    for letter, frequency in letter_counter.items():
        i = known_letters.index(letter)
        q[i] = frequency
    q = q / q.sum()
    show('Simple', q)

def show(name, dist):
    print('{}:  {}'.format(name, '  '.join('%.1f' % p for p in dist * 100)))

def null(A, eps=1e-15):
    u, s, vh = np.linalg.svd(A)
    null_mask = (s <= eps)
    null_space = np.compress(null_mask, vh, axis=0)
    return null_space.reshape(-1)

if __name__ == '__main__':
    main()
