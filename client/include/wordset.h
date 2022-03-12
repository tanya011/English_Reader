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

     explicit WordSet(std::string title_): title(std::move(title_)){
        id = ++count_for_id;
    }

    WordSet() = default;

    std::string getTitle(){
        return title;
    }

    void addWord(Word &word){
        words[word.getId()] = &word;
    }

    void deleteWordById(int wordId){
        words.erase(wordId);
    }

    [[nodiscard]]int getId() const{
        return id;
    }

    std::map<int, Word*> getWords(){
        return words;
    }
    bool checkWord(int wordId){
        return words[wordId];
    }
};


#endif // WORDSET_H
