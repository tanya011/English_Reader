#include "include/user.h"
#include <nlohmann/json.hpp>
#include "include/actCollectionsHistory.h"
#include "include/serverProblemsWindow.h"

namespace userRepLocal{
    void newValue(int value);
}

User::User(WordRep *wordRep, WordSetRep *wordSetRep, WordSetContentRep *wordSetContentRep, BookRep *bookRep) : bookRep_(bookRep), wordRep_(wordRep), wordSetRep_(wordSetRep), wordSetContentRep_(wordSetContentRep) {
}

void User::init(const std::string &username, const std::string &password) {

        httplib::Params params{{"name",     username},
                               {"password", password}};
        auto res = client_.Post("/init-user", params);

        if(!res){
            ServerProblemsWindow serverProblemsWindow;
            serverProblemsWindow.show();
        }

        if (res->status != 200) {  // TODO: Check if smth went wrong
            throw std::runtime_error("Can not init");
        }
        token_ = res->body;
        isAuthorized_ = true;

        downloadDictDataFromServer();

        // личная коллекция книг, получаем последний номер в истории данного пользователя
        int lastCollectionAction = getLastCollectionAction();
        std::cout << "lastcoll = " << lastCollectionAction << std::endl;

        // создаем файл "numColl", в котором будет храниться этот номер
        std::string filename = "numCollection.txt";
        std::string folderName = "yafr_files/files";
        std::filesystem::create_directories(folderName);
        std::filesystem::path appFolder = std::filesystem::absolute("./yafr_files");
        auto folder = appFolder / "files" / filename;
        std::ofstream file(folder, std::ios::out);
        if (!file.good())
            throw std::runtime_error("Problems with app directory");
        file << lastCollectionAction;

        getCollectionBooks();

}

bool User::isAuthorized() const {
    return isAuthorized_;
}


void User::exit() {
    token_ = "";
    isAuthorized_ = false;
    bookRep_->clear();
    clearTablesDict();
    // TODO: drop tables with words
}

std::vector<Book> User::getCollectionBooks() {
        std::cout << "Getting Collection Books..." << std::endl;

        httplib::Params param;
        param.emplace("token", token_);

        auto res = client_.Post("/collection", param);
        if (res->status != 200) {
            ServerProblemsWindow serverProblemsWindow;
            serverProblemsWindow.show();

            throw std::runtime_error("Can't load collection, error code: " + std::to_string(res->status));
        }
        nlohmann::json params = nlohmann::json::parse(res->body);
        std::vector<Book> books;
        for (auto &book: params) {
            bookRep_->addAndSaveBook(book["id"], book["name"], book["author"], book["text"]);
        }
        std::cout << "Got " << params.size() << " books from collection" << std::endl;
        return books;

}

std::vector<Book> User::getLibraryBooks() {
    std::cout << "Getting Library Books..." << std::endl;

    auto res = client_.Post("/library");

    if(!res){
        ServerProblemsWindowUpdateLibrary updateLibraryProblem;
        updateLibraryProblem.show();
    }

    if (res->status != 200){
        throw std::runtime_error("Can't load library, error code: " +
                                 std::to_string(res->status));
    }


    nlohmann::json params = nlohmann::json::parse(res->body);
    std::vector<Book> books;
    for (auto &p: params) {
        books.emplace_back(p["id"], p["name"], p["author"]);
    }
    std::cout << "Got " << params.size() << " books from library" << std::endl;
    return books;
}

int User::addBookToCollection(int bookId) {
    std::unique_ptr<sql::Statement> stmt(
            bookRep_->manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM collection WHERE id=" + std::to_string(bookId)));

    if (reqRes->next()) {
        return 0;
    }

    std::cout << "Adding book to collection" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));

    auto res = client_.Post("/add-book", params);

    if(!res){
        ServerProblemsWindowAddInCollection addInCollection;
        addInCollection.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));

    nlohmann::json book = nlohmann::json::parse(res->body);
    bookRep_->addAndSaveBook(book["id"], book["name"], book["author"], book["text"]);

    syncCollection();
    newActionInCollection("add", bookId);
    userRepLocal::newValue(userRepLocal::getValue() + 1);
    return 1;
}

void User::deleteCollectionBook(int bookId) {
    std::cout << "Deleting book from server" << std::endl;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));

    auto res = client_.Post("/delete-book", params);

    if(!res){
        ServerProblemsWindowDeleteInCollection deleteInCollection;
        deleteInCollection.show();
    }

    if (res->status != 200) {
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));
    } else {
        syncCollection();
        newActionInCollection("delete", bookId);
        userRepLocal::newValue(userRepLocal::getValue() + 1);
    }

}

void User::newActionInCollection(std::string action, int bookId) {
    std::cout << "Deleting book from server" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("action", action);
    params.emplace("bookId", std::to_string(bookId));
    auto res = client_.Post("/new-collections-action", params);
    if (res->status != 200) {
        throw std::runtime_error("Can't add action, error code: " +
                                 std::to_string(res->status));
    }
}


int User::getLastCollectionAction() {
    std::cout << "Getting last collection action" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    auto res = client_.Post("/get-last-collection-action", params);
    nlohmann::json param = nlohmann::json::parse(res->body);
    return param["lastAct"];
}

std::vector<ActCollectionsHistory> User::getNewActions(int startAt) {
    std::cout << "Try to get new actions from server" << std::endl ;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("startAt", std::to_string(startAt));

    auto res = client_.Post("/new-actions", params);
    if (res->status != 200)
        throw std::runtime_error("Can't load new actions, error code: " +
                                 std::to_string(res->status));

    nlohmann::json param = nlohmann::json::parse(res->body);

    std::vector<ActCollectionsHistory> books;
    for (auto &p: param) {
        books.push_back({p["type"], p["bookId"]});
    }
    return books;
}

void User::syncCollection() {
    std::vector<ActCollectionsHistory> vec = this->getNewActions(userRepLocal::getValue() + 1);
    for (auto action: vec) {
        std::cout << " action = " << action.type << std::endl;
        if (action.type == "delete") {
            bookRep_->deleteBookById(action.bookId);
        }
        else {
            this->addBookToCollection(action.bookId);
        }
    }
    userRepLocal::newValue(userRepLocal::getValue() + vec.size());
}

std::vector<Word> User::getWords(){
    std::cout << "Getting words..." << std::endl;
    httplib::Headers headers = {{"token", token_}};
    auto res = client_.Get("/getWords", headers);
    if (res->status != 200)
        throw std::runtime_error("Can't load words from dictionary, error code: " +
                                 std::to_string(res->status));
    nlohmann::json wordsTmp  = nlohmann::json::parse(res->body);
    std::vector<Word> words;
    for (auto &word: wordsTmp){
        words.emplace_back(word["id"], word["original"], word["translation"], word["context"]);
    }
    return words;
}


std::vector<WordSet> User::getWordSets(){
    std::cout << "Getting wordsets..." << std::endl;
    httplib::Headers headers;
    headers.emplace("token", token_);
    auto res = client_.Get("/getWordSets", headers);
    if (res->status != 200)
        throw std::runtime_error("Can't load wordsets from dictionary, error code: " + std::to_string(res->status));
    nlohmann::json wordSetsTmp = nlohmann::json::parse(res->body);
    std::vector<WordSet> wordSets;
    for (auto &wordSet : wordSetsTmp){
        wordSets.emplace_back(wordSet["id"], wordSet["name"]);
    }
    return wordSets;
}

std::vector<std::pair<int, int>> User::getSetContents(){
    std::cout << "Getting wordset's content..." << std::endl;
    httplib::Headers headers;
    headers.emplace("token", token_);
    auto res = client_.Get("/getSetsContents", headers);
    if (res->status != 200)
        throw std::runtime_error("Can't load wordset's content from dictionary, error code : " + std::to_string(res->status));
    nlohmann::json contentTmp = nlohmann::json::parse(res->body);
    std::cout << "getSetContents got content size : " << contentTmp.size() << "\n";
    std::vector<std::pair<int,int>> wordSetsContents;
    for (auto &word_wordset : contentTmp){
        wordSetsContents.emplace_back(word_wordset["wordSetId"], word_wordset["wordId"]);
    }
    std::cout << "still getSetContents, parsed json size : " << wordSetsContents.size() << "\n";
    return wordSetsContents;
}

void User::sendWordRepHistoryChange(HistoryChangeWordRep change) {
    std::cout << "Sending wordRep's history change..." << std::endl;
    httplib::Params params;
    if (change.operation == "wordDeleted") {
        params = {{"token", token_},
                  {"operation", change.operation},
                  {"id",std::to_string(change.wordId)}};
    }
    else{
        params = {{"token", token_},
                  {"operation", change.operation},
                  {"id",        std::to_string(change.wordId)},
                  {"original", change.original},
                  {"translation", change.translation},
                  {"context", change.context}
        };
    }
    auto res = client_.Post("/wordRepChange", params);

    if(!res){
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't change wordRep, error code: " +
                                 std::to_string(res->status));
}

void User::sendWordSetRepHistoryChange(HistoryChangeWordSetRep change) {
    std::cout << "Sending wordSetRep's history change..." << std::endl;
    httplib::Params params = {{"token", token_},
                              {"id", std::to_string(change.wordSetId)},
                              {"name", change.wordSetName}
    };
    auto res = client_.Post("/wordSetRepChange", params);
    if (res->status != 200)
        throw std::runtime_error("Can't change wordSetRep, error code: " +
                                 std::to_string(res->status));
}

void User::sendWordSetContentRepHistoryChange(HistoryChangeWordSetContentRep change) {
    std::cout << "Sending wordSetContentRep's history change..." << std::endl;
    httplib::Params params = {
            {"token", token_},
            {"operation", change.operation},
            {"wordSetId", std::to_string(change.wordSetId)},
            {"wordId", std::to_string(change.wordId)}
    };
    auto res = client_.Post("/wordSetContentRepChange", params);
    if (res->status != 200)
        throw std::runtime_error("Can't change wordSetContentRep, error code: " + std::to_string(res->status));
}

void User::downloadDictDataFromServer() {
    std::vector<Word> words = getWords();
    std::vector<WordSet> wordSets = getWordSets();
    std::vector<std::pair<int,int>> content = getSetContents();
    clearTablesDict();
    for (auto &word: words){
        wordRep_->addWord(word);
    }
    for (auto &wordSet: wordSets){
        if (wordSet.getId() != 1) {
            wordSetRep_->addWordSet(wordSet);
        }
    }
    for (auto &wordSet_word: content){
        wordSetContentRep_->addWordToSetTable(wordSet_word.first, wordSet_word.second);
    }
}

void User::clearTablesDict() {
    wordRep_->clear();
    wordSetRep_->clear();
    wordSetContentRep_->clear();
}