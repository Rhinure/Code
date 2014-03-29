#This was provided for my NLP class
# Module Bigram with function definitions for the bigram distributions
#   with several methods of defining bigrams with frequencies
#   and with the Mutual Information (Association Ratio) measure
# Assumes a python environment with the following imports already done
import nltk
from nltk import FreqDist
import re
from math import *

# All of the unigram Frequency Distribution functions return a frequency distribution in which 
#   the keys are tokens (which may be restricted by the various conditions)
#   and the values are the frequencies of those tokens in the word list.

# define a function to make a FreqDist from a list of tokens that has no tokens
#   that contain non-alphabetical characters (no special characters)
def alphaFreqDist(words):
    # make a new frequency distribution called adist    adist = FreqDist()
    # define the regular expression pattern to match non-alphabetical tokens    pattern = re.compile('.*[^a-z].*')
    # for every token, if it doesn't match the non-alphabetical pattern
    #   add it to the frequency distribution    for word in words:        if not pattern.match(word):          adist.inc(word)
    # return the frequency distribution as the result    return adist

# define a function to make a FreqDist from a list of tokens that has no tokens
#   that contain non-alphabetical characters or words in the stopword list

def alphaStopFreqDist(words, stoplist):
    # make a new frequency distribution called asdist    asdist = FreqDist()
    # define the regular expression pattern to match non-alphabetical tokens    pattern = re.compile('.*[^a-z].*')
    # for every token, if it doesn't match the non-alphabetical pattern
    #   and if it is not on the stop word list
    #     add it to the frequency distribution    for word in words:        if not pattern.match(word):            if not word in stoplist:
                asdist.inc(word)
    # return the frequency distribution as the result    return asdist# Bigram frequency distribution function.
# This version also makes sure that each word in the bigram occurs in a word
#   frequency distribution without non-alphabetical characters and stopwords
#       This will also work with an empty stopword list if you don't want stopwords.

def bigramDist(words, stoplist):
    # create a new empty frequency distribution
    biDist = FreqDist()
    # make a unigram frequency distribution of the tokens
    #   the unigram frequency distribution function can be changed on this line
    uniDist = alphaStopFreqDist(words, stoplist)
    # loop through the words in order, looking at all pairs of words
    for i in range(1, len(words)):
        if words[i-1] in uniDist and words[i] in uniDist:
            biword = words[i-1] + ' ' + words[i]
            biDist.inc(biword)
    return biDist

	
# Mutual Information (Association Ratio) function
# Parameters:
#    words should be a list of tokens (can be lower-cased or not)
#    stoplist should be a list of stop words, but can be empty []
#    threshold should be the minimum frequency of individual words
#        e.g. threshold of 2 omits words with frequency 1 (only occurs once)
# Returns a frequency distribution with bigram keys and mutual information scores as values

def mutualinfo(words, stoplist, threshold):
    # make an empty frequency distribution
    assocDist = nltk.FreqDist()
    # frequencies of words using the unigram freq dist with alphabetical and no stopwords
    uniDist = alphaStopFreqDist(words, stoplist)
    # compute frequencies of word pairs in window of length 2
    for i in range(1, len(words)):
        if (uniDist[words[i-1]] > threshold) and (uniDist[words[i]] > threshold):
            biword = words[i-1] + ' ' + words[i]
            assocDist.inc(biword)
    # compute mutual information ratio for each word pair
    # make a new frequency distribution whose values will be mutual information scores
    arDist = nltk.FreqDist()
    N = len(words)
    # loop over all the bigram strings that are keys in the bigram distribution
    for wordstring in assocDist.keys():
        wordlist = str.split(wordstring, ' ')
        w0 = wordlist[0]
        w1 = wordlist[1]
        ar = (assocDist[wordstring] * N * N) / (1.0 *uniDist[w0] * uniDist[w1])
        ar_log2 = log(ar, 2)
        arDist[wordstring] = ar_log2
    return arDist
