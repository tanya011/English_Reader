#ifndef WORDSET_H
#define WORDSET_H

#include "word.h"
#include <map>




class WordSet{

    int id;
    static inline int count_for_id = 0;

    std::string title;
    std::map<int, Word*> words;

public:

     explicit WordSet(std::string title_);

    WordSet() = default;

    std::string getName();

    void addWord(Word &word);

    void deleteWordById(int wordId);

    [[nodiscard]]int getId() const;

    std::map<int, Word*> getWords();

    bool checkWord(int wordId);
};


#endif // WORDSET_H
