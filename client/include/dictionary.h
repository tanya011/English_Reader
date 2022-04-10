#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "wordset.h"

#include <QObject>
#include <QTextEdit>

class Dictionary : public QObject{

    Q_OBJECT

    std::map<int, Word> words_;

public:

    // TEST
    inline std::vector <Word> getWords(){
        std::vector <Word> a;
        for(auto w: words_){
            a.push_back(w.second);
        }
        return a;
    }
    // Test

    std::map<int, WordSet> wordSets_;

    WordSet allWords_ = WordSet("Все группы");

    int addWord(std::string original, std::string translation);
    int addWord(std::string original, std::string translation, int id);
    void deleteWordFromDictionary(int wordId);
    void addWordToWordSet(int wordId, int wordSetId);
    void deleteWordFromWordSet(int wordId, int wordSetId);
    int createWordSet(std::string title);
    int createWordSet(std::string title, int id);
    void deleteWordSet(int wordSetId);
    void addAllWordsToWordSets();  // Adds wordSet "AllWords" into the map wordSets[]
    void downloadDictionary();
    ~Dictionary();

signals:
    void groupWasCreated(int wordSetId, std::string title, std::map<int, Word*> newWordSet);
    void wordSetWasDeleted(int wordSetId);
    void wordWasAddedToDictionary(int wordId);

public slots:
    void addWordAllGroups(int wordId); //Updates wordSet "AllWords"
};


#endif // DICTIONARY_H
