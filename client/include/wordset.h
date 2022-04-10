#ifndef WORDSET_H
#define WORDSET_H

#include "word.h"
#include <map>

class WordSet{

    int id;
    static inline int idCounter_ = 0;

    std::string title_;
    std::map<int, Word*> words_;

public:


    explicit WordSet(std::string title_);

    WordSet() = default;

    std::string getTitle();

    void addWord(Word &word);
    
    void deleteWordById(int wordId);

    [[nodiscard]]int getId() const;
    
    std::map<int, Word*> getWords();

    bool checkWord(int wordId);
    
    void setId(int newId);

    static void setIdCounter(int id);

    static int counterSaver();

};

#endif // WORDSET_H
