#include <include/wordset.h>

WordSet::WordSet(std::string title_): title_(std::move(title_)){
   id = ++idCounter_;
}

std::string WordSet::getTitle(){
    return title_;
}

void WordSet::addWord(Word &word){
    words_[word.getId()] = &word;
}

void WordSet::deleteWordById(int wordId){
    words_.erase(wordId);
}

[[nodiscard]]int WordSet::getId() const{
    return id;
}

std::map<int, Word*> WordSet::getWords(){
    return words_;
}

bool WordSet::checkWord(int wordId){
    return words_[wordId];
}

void WordSet::setId(int newId){
    id = newId;
}

void WordSet::setIdCounter(int id){
    idCounter_ = id;
}

int WordSet::counterSaver() {
    return idCounter_;
}
