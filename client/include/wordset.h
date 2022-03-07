#ifndef WORDSET_H
#define WORDSET_H

#include "word.h"
#include <map>




class WordSet{

    int id{};
    static inline int count_for_id;
    static inline std::stack<int> available_ids; //стэк освободившихся

    std::string title;
    std::map<int, Word*> words;

public:

     explicit WordSet(std::string title_): title(std::move(title_)){
        if (!available_ids.empty()){
            id = available_ids.top();
            available_ids.pop();
        }
        else{
            id = ++count_for_id;
        }
    }

    WordSet() = default;

    void kill_set(){
        id = 0;
    }

    void addWord(Word &word){
        *words[word.getId()] = word;
    }

    void deleteWordById(int wordId){
        words.erase(wordId);
    }

    [[nodiscard]] int getId() const{
        return id;
    }

    [[nodiscard]] std::string getTitle() const{
        return title;
    }

    ~WordSet(){
        if (id){
            available_ids.push(id);
        }
    }

};


#endif // WORDSET_H
