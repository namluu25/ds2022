from collections import defaultdict

def tokenize(message):
    message = message.lower()
    all_word = message.strip().split()
    return set(all_word)

def wc_mapper(document):
    for word in tokenize(document):
        yield(word, 1)

def wc_reducer(word, counts):
    yield (word, sum(counts))

def word_count(documents):
    collector = defaultdict(list)
    with open(documents, 'r') as file:
        for document in file:
            for word, count in wc_mapper(document):
                collector[word].append(count)
    return [output
           for word, counts in collector.items()
           for output in wc_reducer(word, counts)]

print(word_count('count.txt'))