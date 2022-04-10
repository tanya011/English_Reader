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
    std::string fullWord =
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

void Dictionary::deleteWordSet(int wordSetId){
    emit wordSetWasDeleted(wordSetId);
    wordSets_.erase(wordSetId);
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
    for (auto wordSet : wordSets_){
        file << wordSet.second.getWords().size() << std::endl;
        file << wordSet.second.getId() << std::endl;
        file << wordSet.second.getTitle() << std::endl;
        for (auto word: wordSet.second.getWords()){
            file << word.second->getId() << std::endl;
        }
    }
    file.close();
}

void Dictionary::downloadDictionary() {
    std::ifstream file("dictionaryData.txt"); //файл лежит в cmake-build-debug
    std::string curString;
    getline(file, curString);
    if (!curString.empty()) {
        int counterWordId = std::stoi(curString);
        Word::setIdCounter(counterWordId);
        getline(file, curString);
        int wordsNumber = std::stoi(curString);
        for (int word = 0; word < wordsNumber; word++) {
            getline(file, curString);
            int id = std::stoi(curString);

            getline(file, curString);
            std::string original = curString;

            getline(file, curString);
            std::string translation = curString;

            addWord(original, translation, id);
        }
        getline(file, curString);
        int counterWordSetId = std::stoi(curString);
        WordSet::setIdCounter(counterWordSetId);

        getline(file, curString);
        int wordSetNumber = std::stoi(curString);

        for (int wordSet = 0; wordSet < wordSetNumber; wordSet++) {
            getline(file, curString);
            int wordSetSize = std::stoi(curString);

            getline(file, curString);
            int wordSetId = std::stoi(curString);

            getline(file, curString);
            std::string wordSetName = curString;

            createWordSet(wordSetName, wordSetId);

            for (int word = 0; word < wordSetSize; word++) {
                getline(file, curString);
                int wordId = std::stoi(curString);
                addWordToWordSet(wordId, wordSetId);
            }
        }
        file.close();
    }
    else{
        file.close();
    }
}
