#ifndef WORD_H
#define WORD_H

#include <string>
#include <utility>
#include <vector>
#include <stack>



class Word{

    int id;
    static inline int count_for_id = 0;

    std::string original;
    std::string translation;
    std::string context;
    std::vector<std::string> examples;

public:

    explicit Word(std::string original_): original(std::move(original_)){
        id = ++count_for_id;
    }

    Word() = default;

    std::string getOriginal(){
        return original;
    }

    std::string getTranslation(){
        return translation;
    }

    void setTranslation(std::string translation_){
        translation = std::move(translation_);
    }

    void setContext(std::string context_){
        context = std::move(context_);
    }

    void addExample(std::string example_){
        examples.push_back(std::move(example_));
    }

    [[nodiscard]]int getId() const{
        return id;
    }
};

#endif // WORD_H
