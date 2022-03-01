#ifndef WORD_H
#define WORD_H

#include <string>
#include <utility>
#include <vector>
#include <stack>



class Word{

    int id{};
    static inline int count_for_id;
    static inline std::stack<int> available_ids; //стэк освободившихся

    std::string word;
    std::string translation;
    std::string context;
    std::vector<std::string> examples;

public:

    // id берётся из счётчика, увеличивающегося от каждого нового слова, если какой-то адйишник освободился, то он переприсвоится

    explicit Word(std::string word_): word(std::move(word_)){
        if (!available_ids.empty()){
            id = available_ids.top();
            available_ids.pop();
        }
        else{
            id = ++count_for_id;
        }
    }

    Word() = default;

    void kill_word(){ //id = 0 особый id, у переменных, которые не находятся в контейнере, чтобы в момент удаления, деструктор не добавил дубликат id, который уже есть в контейнере
        id = 0;
    }

    void setTranslation(std::string &translation_){
        translation = std::move(translation_);
    }

    void setContext(std::string context_){
        context = std::move(context_);
    }

    void addExample(std::string &example_){
        examples.push_back(std::move(example_));
    }

    [[nodiscard]]int getId() const{
        return id;
    }

    ~Word(){
        if (id) {
            available_ids.push(id);
        }
    }

};

#endif // WORD_H
