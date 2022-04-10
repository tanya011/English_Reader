#include<include/dictionaryWindow.h>



int Dictionary::addWord(std::string original, std::string translation){
    Word word(std::move(original));
    word.setTranslation(std::move(translation));
    words_[word.getId()] = word;
    emit wordWasAddedToDictionary(word.getId());
    return word.getId();
}

int Dictionary::addWord(std::string original, std::string translation, int id){
    Word word(std::move(original));
    word.setTranslation(std::move(translation));
    word.setId(id);
    words_[word.getId()] = word;
    emit wordWasAddedToDictionary(word.getId());
    return word.getId();
}

void Dictionary::deleteWordFromDictionary(int wordId){
    for (auto &group: wordSets_){
        deleteWordFromWordSet(wordId, group.first);
    }
    if (!words_[wordId].getOriginal().empty()){
        words_.erase(wordId);
    }
}

void Dictionary::addWordToWordSet(int wordId, int wordSetId){
    wordSets_[wordSetId].addWord(words_[wordId]);
    std::string fullword =
        words_[wordId].getOriginal() + " - " + words_[wordId].getTranslation();
}

void Dictionary::deleteWordFromWordSet(int wordId, int wordSetId){
    wordSets_[wordSetId].deleteWordById(wordId);
}

int Dictionary::createWordSet(std::string title){
    for (auto groupName : wordSets_){
        if (groupName.second.getTitle() == title){
            return groupName.second.getId();
        }
    }
    WordSet new_wordSet(title);
    wordSets_[new_wordSet.getId()] = new_wordSet;
    emit groupWasCreated(new_wordSet.getId(), std::move(title),
                         wordSets_[new_wordSet.getId()].getWords());
    return new_wordSet.getId();
}

int Dictionary::createWordSet(std::string title, int id){
    for (auto groupName : wordSets_){
        if (groupName.second.getTitle() == title){
            return groupName.second.getId();
        }
    }
    WordSet new_wordSet(title);
    new_wordSet.setId(id);
    wordSets_[new_wordSet.getId()] = new_wordSet;
    emit groupWasCreated(new_wordSet.getId(), std::move(title),
                         wordSets_[new_wordSet.getId()].getWords());
    return new_wordSet.getId();
}

void Dictionary::deleteWordSet(int wordset_Id){
    emit wordSetWasDeleted(wordset_Id);
    wordSets_.erase(wordset_Id);
}

void Dictionary::addAllWordsToWordSets(){
    wordSets_[allWords_.getId()] = allWords_;
}

void Dictionary::addWordAllGroups(int wordId) {
    addWordToWordSet(wordId, allWords_.getId());
}

Dictionary::~Dictionary() {
    std::ofstream file("dictionaryData.txt", std::ios::out); //файл лежит в cmake-build-debug
    file << Word::counterSaver() << std::endl;
    file << words_.size() << std::endl;
    for (auto& word: words_){
        file << word.second.getId() << std::endl;
        file << word.second.getOriginal() << std::endl;
        file << word.second.getTranslation() <<std::endl;
    }
    file << WordSet::counterSaver() << std::endl;
    file << wordSets_.size() << std::endl;
    for (auto wordset: wordSets_){
        file << wordset.second.getWords().size() << std::endl;
        file << wordset.second.getId() << std::endl;
        file << wordset.second.getTitle() << std::endl;
        for (auto word: wordset.second.getWords()){
            file << word.second->getId() << std::endl;
        }
    }
    file.close();
}
