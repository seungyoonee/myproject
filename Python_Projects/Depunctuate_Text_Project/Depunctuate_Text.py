from collections import defaultdict
def count_ngrams(file_name, n=2):
    # reads an input file
    input_file = open(file_name, 'r')
    
    punctuations = '!@#$%^&*()-=_+[]{}\|;,\'\"./<>?`~' # punctuations to filter
    
    words_li = [] # a list of words to preprocess the file

    # reading the file line by line
    for line in input_file:
        # removing \n and spliting words
        line = line.strip().lower()            
        line = line.split(" ")
        
        word = []
        # removing punctuations from a line
        for i in range(len(line)):
            chars = list(line[i])
            # replacing punctuations by white space
            for j in range(len(chars)):
                if chars[j] in punctuations:
                    chars[j] = " "
            # combining chars to words
            combined_words = ""
            for k in chars:
                combined_words += k
            # appending the combined words to the words list
            word = combined_words.split()
            for l in range(len(word)):
                words_li.append(word[l])

    # creating the n-gram dictionary
    n_gram_dict = defaultdict(int)    
    for i in range(len(words_li) - n + 1):
        key = () # key tuple
        # creating key tuple to add to n-gram dictionary
        for j in range(0, n):
            key = key + (words_li[i + j],)
        # raise the count of a n-gram
        n_gram_dict[key] += 1
            
    return n_gram_dict
    
def single_occurences(ngram_count_dict):
    single_occur_li = [] # a list of all n-grams with only 1 occurence
    
    # appending the tuples with 1 occurence to the list
    for key in ngram_count_dict:
        if ngram_count_dict[key] == 1:
            single_occur_li.append(key)
    
    return single_occur_li

def most_frequent(ngram_count_dict, num = 5):
    # converting the n-gram dictionary into a list of (frequency, n-gram) tuples
    most_freq_li = []
    for key in ngram_count_dict:
        ngram = key
        freq = ngram_count_dict[key]
        most_freq_li.append((freq, ngram))
        
    # sorting the list of (frequency, n-gram) tuples based on frequency
    most_freq_li = sorted(most_freq_li, reverse=True)
    
    # creating the num most frequent ngrams list
    n_most_freq_li = []
    for n in range(num):
        n_most_freq_li.append(most_freq_li[n][1])
    
    return n_most_freq_li

def main():
    filename = "alice.txt"
    n = 2
    most_frequent_k = 5
    ngram_counts = count_ngrams(filename, n)
    print('{}-grams that occur only once:'.format(n))
    print(single_occurences(ngram_counts))
    print('{} most frequent {}-grams:'.format(most_frequent_k, n))
    print(most_frequent(ngram_counts, most_frequent_k))
    
if __name__ == "__main__":
    main()

