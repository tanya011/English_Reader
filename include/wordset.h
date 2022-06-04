#ifndef WORDSET_H
#define WORDSET_H

#include <map>
#include "word.h"

class WordSet {
    int id_{};

    std::string title_;
    std::map<int, Word> words_;

public:
    explicit WordSet(int id, std::string title)
            : id_(id), title_(std::move(title)) {
    }

    WordSet() = default;

    std::string getTitle() const{
        return title_;
    }

    [[nodiscard]] int getId() const{
        return id_;
    }

    std::map<int, Word> getWords() const{
        return words_;
    }

    [[nodiscard]] int getSize() const{
        return words_.size();
    }

    void addWord(Word &word){
        words_[word.getId()] = word;
    }

    void deleteWordById(int wordId){
        words_.erase(wordId);
    }
};

#endif  // WORDSET_H