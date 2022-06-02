#ifndef WORD_H
#define WORD_H

#include <string>
#include <utility>
#include <vector>

class Word {
    int id_{};

    std::string original_;
    std::string translation_;
    std::string context_;

public:
    explicit Word(int id,
                  std::string original,
                  std::string translation,
                  std::string context = "")
            : id_(id),
              original_(std::move(original)),
              translation_(std::move(translation)),
              context_(std::move(context)) {
    }

    Word() = default;

    [[nodiscard]] std::string getOriginal() const{
        return original_;
    }

    [[nodiscard]] std::string getTranslation() const{
        return translation_;
    }

    [[nodiscard]] std::string getContext() const{
        return context_;
    }

    [[nodiscard]] int getId() const{
        return id_;
    }
};

#endif  // WORD_H