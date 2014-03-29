# open python and nltk packages needed for processing
import nltk
import re
from math import *
from nltk.corpus import gutenberg
#from nltk.book import *
# import module with bigram functions (in same directory as this script)
import Bigram

# get text from file, can use relative path if file not in same directory
#f = open('../newstext/desert.txt', 'r')
f1 = open('butlerodyssey2.txt', 'r')
file1text = f1.read()
f2 = open('butcherodyssey2.txt', 'r')
file2text = f2.read()
# tokenize by words and punctuation
file1tokens = nltk.wordpunct_tokenize(file1text)
file2tokens = nltk.wordpunct_tokenize(file2text)
# choose to treat upper and lower case the same
#    by putting all tokens in lower case
file1words = [w.lower() for w in file1tokens]
file2words = [w.lower() for w in file2tokens]
# read a stop word file
fstop = open('Smart.English.stop', 'r')
stoptext = fstop.read()
stopwords = nltk.wordpunct_tokenize(stoptext)
#display 50 freq words file1
file1freq = Bigram.alphaStopFreqDist(file1words, stopwords)
file2freq = Bigram.alphaStopFreqDist(file2words, stopwords)
print "Top 50 freq words file1"
for k in file1freq.keys()[:50]:
        print k, file1freq[k]
print "Top 50 freq words file2"
for k in file2freq.keys()[:50]:
        print k, file2freq[k]

#display 50 freq words file2

# call the mutual information function with empty stop word list and
#   threshold of 2 for individual word frequencies
#fileARDist = Bigram.mutualinfo(filewords, stopwords, 2)
fileBiDist1 = Bigram.bigramDist(file1words, stopwords)
fileBiDist2 = Bigram.bigramDist(file2words, stopwords)
print "Top 50 Bigrams from file1"
for k in fileBiDist1.keys()[:50]:
        print k, fileBiDist1[k]
print
print "Top 50 Bigrams from file2"
for k in fileBiDist2.keys()[:50]:
        print k, fileBiDist2[k]

fileARDist1 = Bigram.mutualinfo(file1words, [], 2)
fileARDist2 = Bigram.mutualinfo(file2words, [], 2)
# display the top 50 bigrams
print
print "Bigrams from file1 with top 50 mutual information values"
for k in fileARDist1.keys()[:50]:
        print k, fileARDist1[k]
print
print "Bigrams from file2 with top 50 mutual information values"
for k in fileARDist2.keys()[:50]:
        print k, fileARDist2[k]


