#include <include/wordset.h>

WordSet::WordSet(std::string title_): title(std::move(title_)){
   id = ++count_for_id;
}

std::string WordSet::getName(){
    return title;
}

void WordSet::addWord(Word &word){
    words[word.getId()] = &word;
}

void WordSet::deleteWordById(int wordId){
    words.erase(wordId);
}

[[nodiscard]]int WordSet::getId() const{
    return id;
}

std::map<int, Word*> WordSet::getWords(){
    return words;
}

bool WordSet::checkWord(int wordId){
    return words[wordId];
}

void WordSet::setId(int newId){
    id = newId;
}

void WordSet::setIdCounter(int id){
    count_for_id = id;
}

int WordSet::counterSaver() {
    return count_for_id;
}
