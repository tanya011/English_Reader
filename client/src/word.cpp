#include <include/word.h>

Word::Word(std::string original_): original(std::move(original_)){
    id = ++count_for_id;
}

std::string Word::getOriginal(){
    return original;
}

std::string Word::getTranslation(){
    return translation;
}

void Word::setTranslation(std::string translation_){
    translation = std::move(translation_);
}

void Word::setContext(std::string context_){
    context = std::move(context_);
}

void Word::addExample(std::string example_){
    examples.push_back(std::move(example_));
}

[[nodiscard]]int Word::getId() const{
    return id;
}

void Word::setId(int newId){
    id = newId;
}

void Word::setIdCounter(int id){
    count_for_id = id;
}

int Word::counterSaver() {
    return count_for_id;
}
