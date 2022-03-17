#ifndef WORD_H
#define WORD_H

#include <string>
#include <utility>
#include <vector>



class Word{

    int id;
    static inline int count_for_id = 0;

    std::string original;
    std::string translation;
    std::string context;
    std::vector<std::string> examples;

public:


    explicit Word(std::string original_);

    Word() = default;

    std::string getOriginal();

    std::string getTranslation();

    void setTranslation(std::string translation_);


    void setContext(std::string context_);


    void addExample(std::string example_);

    [[nodiscard]]int getId() const;
    
    void setId(int newId);

    static void setIdCounter(int id);

    static int counterSaver();

};

#endif // WORD_H
