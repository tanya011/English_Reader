#ifndef DICTIONARY_LOGIC_H
#define DICTIONARY_LOGIC_H

#include "wordset.h"
#include <QObject>
#include <QTextEdit>
#include <QDebug>

class DictionaryLogic : public QObject{

    Q_OBJECT



    std::map<int, Word> words;



public:
    WordSet all_words = WordSet("Все группы");
    std::map<int, WordSet> groups;

    int add_word(std::string original, std::string translation){
        Word word(original);
        word.setTranslation(translation);
        words[word.getId()] = word;
        emit word_was_added_to_dictionary(word.getId());
        return word.getId();
    }

    void delete_word(int wordId){

        for (auto &group: groups){
            delete_word_from_group(wordId, group.first);
        }
        if (!words[wordId].getOriginal().empty()){
            words.erase(wordId);
        }
    }

    void add_word_to_group(int wordId, int groupId){
        groups[groupId].addWord(words[wordId]);
        std::string fullword = words[wordId].getOriginal() + " - " + words[wordId].getTranslation();
    }

    void delete_word_from_group(int wordId, int wordsetId){
        qDebug() << groups[wordsetId].getWords().size();
        qDebug() << groups[wordsetId].getName().c_str() << groups[wordsetId].checkWord(wordId);
        groups[wordsetId].deleteWordById(wordId);
    }

    int create_wordSet(std::string title){
        WordSet new_wordSet(title);
        groups[new_wordSet.getId()] = new_wordSet;
        emit group_was_created(new_wordSet.getId(), std::move(title), groups[new_wordSet.getId()].getWords());
        return new_wordSet.getId();
     }

    void delete_wordSet(int wordset_Id){
        emit wordset_was_deleted(wordset_Id);
        groups.erase(wordset_Id);
    }

    void add_allgroups_to_map(){
        groups[all_words.getId()] = all_words;
    }


signals:
    void group_was_created(int wordset_id, std::string title, std::map<int, Word*> new_wordset);
    void wordset_was_deleted(int wordset_id);
    void word_was_added_to_dictionary(int wordId);

public slots:
    void add_word_allgroups(int wordId){
        add_word_to_group(wordId, all_words.getId());
    }
};


#endif // DICTIONARY_LOGIC_H
