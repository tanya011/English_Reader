#include<include/dictionary.h>


int DictionaryLogic::add_word(std::string original, std::string translation){
    Word word(original);
    word.setTranslation(translation);
    words[word.getId()] = word;
    emit word_was_added_to_dictionary(word.getId());
    return word.getId();
}

int DictionaryLogic::add_word(std::string original, std::string translation, int id){
    Word word(std::move(original));
    word.setTranslation(std::move(translation));
    word.setId(id);
    words[word.getId()] = word;
    emit word_was_added_to_dictionary(word.getId());
    return word.getId();
}

void DictionaryLogic::delete_word(int wordId){

    for (auto &group: groups){
        delete_word_from_group(wordId, group.first);
    }
    if (!words[wordId].getOriginal().empty()){
        words.erase(wordId);
    }
}

void DictionaryLogic::add_word_to_group(int wordId, int groupId){
    groups[groupId].addWord(words[wordId]);
    std::string fullword = words[wordId].getOriginal() + " - " + words[wordId].getTranslation();
}

void DictionaryLogic::delete_word_from_group(int wordId, int wordsetId){
    groups[wordsetId].deleteWordById(wordId);
}

int DictionaryLogic::create_wordSet(std::string title){
    for (auto groupName : groups){
        if (groupName.second.getTitle() == title){
            return groupName.second.getId();
        }
    }
    WordSet new_wordSet(title);
    groups[new_wordSet.getId()] = new_wordSet;
    emit group_was_created(new_wordSet.getId(), std::move(title), groups[new_wordSet.getId()].getWords());
    return new_wordSet.getId();
}

int DictionaryLogic::create_wordSet(std::string title, int id){
    for (auto groupName : groups){
        if (groupName.second.getTitle() == title){
            return groupName.second.getId();
        }
    }
    WordSet new_wordSet(title);
    new_wordSet.setId(id);
    groups[new_wordSet.getId()] = new_wordSet;
    emit group_was_created(new_wordSet.getId(), std::move(title), groups[new_wordSet.getId()].getWords());
    return new_wordSet.getId();
}

void DictionaryLogic::delete_wordSet(int wordset_Id){
    emit wordset_was_deleted(wordset_Id);
    groups.erase(wordset_Id);
}

void DictionaryLogic::add_allgroups_to_map(){
    groups[all_words.getId()] = all_words;
}

void DictionaryLogic::add_word_allgroups(int wordId){
    add_word_to_group(wordId, all_words.getId());
}


DictionaryLogic::~DictionaryLogic() {
    std::ofstream file("dictionaryData.txt", std::ios::out); //файл лежит в cmake-build-debug
    file << Word::counterSaver() << std::endl;
    file << words.size() << std::endl;
    for (auto& word: words){
        file << word.second.getId() << std::endl;
        file << word.second.getOriginal() << std::endl;
        file << word.second.getTranslation() <<std::endl;
    }
    file << WordSet::counterSaver() << std::endl;
    file << groups.size() << std::endl;
    for (auto wordset: groups){
        file << wordset.second.getWords().size() << std::endl;
        file << wordset.second.getId() << std::endl;
        file << wordset.second.getTitle() << std::endl;
        for (auto word: wordset.second.getWords()){
            file << word.second->getId() << std::endl;
        }
    }
    file.close();
}
