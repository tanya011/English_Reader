#include "include/user.h"
#include <nlohmann/json.hpp>
#include "../include/config.h"
#include "include/actCollectionsHistory.h"
#include "include/serverProblemsException.h"
#include "include/serverProblemsWindow.h"

inline Config config(CONFIG_PATH);

namespace userRepLocal {
void newValue(int value);
}

User::User(WordRep *wordRep,
           WordSetRep *wordSetRep,
           WordSetContentRep *wordSetContentRep,
           BookRep *bookRep)

    : bookRep_(bookRep),
      wordRep_(wordRep),
      wordSetRep_(wordSetRep),
      wordSetContentRep_(wordSetContentRep),
      client_(config.get("SERVER_ADDRESS"),
              std::stoi(config.get("SERVER_PORT"))) {
    client_.enable_server_certificate_verification(false);
}

User::~User() {
    timer_.stop();
}

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params{{"name", username}, {"password", password}};
    auto res = client_.Post("/init-user", params);

    if (!res) {
        ServerProblemsWindow serverProblemsWindow;
        serverProblemsWindow.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't /init, error code: " +
                                 std::to_string(res->status));

    token_ = res->body;
    std::cout << "Got token: " << token_ << std::endl;
    isAuthorized_ = true;

    downloadDictDataFromServer();

    // личная коллекция книг, получаем последний номер в истории данного
    // пользователя
    int lastCollectionAction = getLastCollectionAction();
    std::cout << "Last Collection action number: " << lastCollectionAction
              << std::endl;

    // создаем файл "numColl", в котором будет храниться этот номер
    std::filesystem::create_directories(NUM_COLL);
    std::ofstream file(NUM_COLL, std::ios::out);
    if (!file.good())
        throw std::runtime_error("Problems with numCollection.txt file");
    file << lastCollectionAction;

    getCollectionBooks();
}

bool User::isAuthorized() const {
    return isAuthorized_;
}

void User::exit() {
    updateDictionaryChanges();
    token_ = "";
    isAuthorized_ = false;
    bookRep_->clear();
    clearTablesDict();
}

std::vector<Book> User::getCollectionBooks() {
    std::cout << "Getting collection books..." << std::endl;

    httplib::Params param;
    param.emplace("token", token_);
    auto res = client_.Post("/collection", param);

    if (res->status != 200) {
        ServerProblemsWindow serverProblemsWindow;
        serverProblemsWindow.show();
        throw std::runtime_error("Can't load /collection, error code: " +
                                 std::to_string(res->status));
    }

    nlohmann::json params = nlohmann::json::parse(res->body);
    std::vector<Book> books;
    for (auto &book : params) {
        bookRep_->addAndSaveBook(book["id"], book["name"], book["author"],
                                 book["text"]);
    }
    std::cout << "Got " << params.size() << " books from /collection"
              << std::endl;

    return books;
}

std::vector<Book> User::getLibraryBooks() {
    std::cout << "Getting library books..." << std::endl;

    auto res = client_.Post("/library");

    if (!res) {
        ServerProblemsWindowUpdateLibrary updateLibraryProblem;
        updateLibraryProblem.show();
    }

    if (res->status != 200) {
        throw std::runtime_error("Can't load /library, error code: " +
                                 std::to_string(res->status));
    }

    nlohmann::json params = nlohmann::json::parse(res->body);
    std::vector<Book> books;
    for (auto &p : params) {
        books.emplace_back(p["id"], p["name"], p["author"]);
    }
    std::cout << "Got " << params.size() << " books from /library" << std::endl;
    return books;
}

void User::addBookToCollection(int bookId) {
    std::cout << "Synchronizing collection..." << std::endl;
    syncCollection();

    try {
        std::cout << "Checking if book already exists" << std::endl;
        bookRep_->getBookById(bookId);
        return;
    } catch (std::exception &e) {
    }

    std::cout << "Sending /add-book request..." << std::endl;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));
    auto res = client_.Post("/add-book", params);

    if (!res) {
        ServerProblemsWindowAddInCollection addInCollection;
        addInCollection.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));

    nlohmann::json book = nlohmann::json::parse(res->body);
    bookRep_->addAndSaveBook(book["id"], book["name"], book["author"],
                             book["text"]);

    newActionInCollection("add", bookId);
    userRepLocal::newValue(userRepLocal::getValue() + 1);
}

void User::deleteCollectionBook(int bookId) {
    std::cout << "Synchronizing collection..." << std::endl;
    syncCollection();

    std::cout << "Deleting book from server..." << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));

    auto res = client_.Post("/delete-book", params);

    if (!res) {
        ServerProblemsWindowDeleteInCollection deleteInCollection;
        deleteInCollection.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't delete book, error code: " +
                                 std::to_string(res->status));
    bookRep_->deleteBookById(bookId);
    newActionInCollection("delete", bookId);
    userRepLocal::newValue(userRepLocal::getValue() + 1);
}

void User::newActionInCollection(const std::string &action, int bookId) {
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("action", action);
    params.emplace("bookId", std::to_string(bookId));
    auto res = client_.Post("/new-collections-action", params);
    if (res->status != 200)
        throw std::runtime_error("Can't add action, error code: " +
                                 std::to_string(res->status));
}

int User::getLastCollectionAction() {
    std::cout << "Getting last collection action..." << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    auto res = client_.Post("/get-last-collection-action", params);
    nlohmann::json param = nlohmann::json::parse(res->body);
    if (res->status != 200)
        throw std::runtime_error("Can't get last action, error code: " +
                                 std::to_string(res->status));
    return param["lastAct"];
}

std::vector<ActCollectionsHistory> User::getNewActions(int startAt) {
    std::cout << "Try to get new actions from server..." << std::endl;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("startAt", std::to_string(startAt));

    auto res = client_.Post("/new-actions", params);
    if (res->status != 200)
        throw std::runtime_error("Can't load new actions, error code: " +
                                 std::to_string(res->status));

    nlohmann::json param = nlohmann::json::parse(res->body);

    std::vector<ActCollectionsHistory> books;
    for (auto &p : param) {
        books.push_back({p["type"], p["bookId"]});
    }
    return books;
}

void User::syncCollection() {
    std::vector<ActCollectionsHistory> vec =
        this->getNewActions(userRepLocal::getValue() + 1);
    for (const auto &action : vec) {
        std::cout << "Working with action: " << action.type << std::endl;
        if (action.type == "delete")
            deleteCollectionBook(action.bookId);
        else
            addBookToCollection(action.bookId);
    }
    userRepLocal::newValue(userRepLocal::getValue() + vec.size());
}

std::vector<Word> User::getWords() {
    std::cout << "Getting words from dictionary..." << std::endl;
    httplib::Headers headers = {{"token", token_}};
    auto res = client_.Get("/getWords", headers);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error(
            "Can't load words from dictionary, error code: " +
            std::to_string(res->status));

    nlohmann::json wordsTmp = nlohmann::json::parse(res->body);
    std::vector<Word> words;
    for (auto &word : wordsTmp)
        words.emplace_back(word["id"], word["original"], word["translation"],
                           word["context"]);

    return words;
}

std::vector<WordSet> User::getWordSets() {
    std::cout << "Getting word sets from dictionary..." << std::endl;
    httplib::Headers headers;
    headers.emplace("token", token_);
    auto res = client_.Get("/getWordSets", headers);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error(
            "Can't load word sets from dictionary, error code: " +
            std::to_string(res->status));

    nlohmann::json wordSetsTmp = nlohmann::json::parse(res->body);
    std::vector<WordSet> wordSets;
    for (auto &wordSet : wordSetsTmp) {
        wordSets.emplace_back(wordSet["id"], wordSet["name"]);
    }
    return wordSets;
}

std::vector<std::pair<int, int>> User::getSetContents() {
    std::cout << "Getting word set's content..." << std::endl;
    httplib::Headers headers;
    headers.emplace("token", token_);
    auto res = client_.Get("/getSetsContents", headers);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error(
            "Can't load word set's content from dictionary, error code : " +
            std::to_string(res->status));

    nlohmann::json contentTmp = nlohmann::json::parse(res->body);
    std::vector<std::pair<int, int>> wordSetsContents;
    for (auto &word_wordset : contentTmp) {
        wordSetsContents.emplace_back(word_wordset["wordSetId"],
                                      word_wordset["wordId"]);
    }
    return wordSetsContents;
}

void User::sendWordRepHistoryChange(HistoryChangeWordRep change) {
    std::cout << "Sending wordRep's history change..." << std::endl;
    httplib::Params params;
    if (change.operation == "wordDeleted") {
        params = {{"token", token_},
                  {"operation", change.operation},
                  {"id", std::to_string(change.wordId)}};
    } else {
        params = {{"token", token_},
                  {"operation", change.operation},
                  {"id", std::to_string(change.wordId)},
                  {"original", change.original},
                  {"translation", change.translation},
                  {"context", change.context}};
    }
    auto res = client_.Post("/wordRepChange", params);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't change word repo, error code: " +
                                 std::to_string(res->status));
}

void User::sendWordSetRepHistoryChange(HistoryChangeWordSetRep change) {
    std::cout << "Sending wordSetRep's history change..." << std::endl;
    httplib::Params params = {{"token", token_},
                              {"id", std::to_string(change.wordSetId)},
                              {"name", change.wordSetName}};
    auto res = client_.Post("/wordSetRepChange", params);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error("Can't change wordSetRep, error code: " +
                                 std::to_string(res->status));
}

void User::sendWordSetContentRepHistoryChange(
    HistoryChangeWordSetContentRep change) {
    std::cout << "Sending wordSetContentRep's history change..." << std::endl;
    httplib::Params params = {{"token", token_},
                              {"operation", change.operation},
                              {"wordSetId", std::to_string(change.wordSetId)},
                              {"wordId", std::to_string(change.wordId)}};
    auto res = client_.Post("/wordSetContentRepChange", params);

    if (!res) {
        ServerProblemsWindowSaveDict saveDict;
        saveDict.show();
    }

    if (res->status != 200)
        throw std::runtime_error(
            "Can't change wordSetContentRep, error code: " +
            std::to_string(res->status));
}

void User::downloadDictDataFromServer() {
    std::vector<Word> words = getWords();
    std::vector<WordSet> wordSets = getWordSets();
    std::vector<std::pair<int, int>> content = getSetContents();
    clearTablesDict();
    for (auto &word : words)
        wordRep_->addWord(word);

    for (auto &wordSet : wordSets)
        if (wordSet.getId() != 1)
            wordSetRep_->addWordSet(wordSet);

    for (auto &wordSet_word : content)
        wordSetContentRep_->addWordToSetTable(wordSet_word.first,
                                              wordSet_word.second);
}

void User::clearTablesDict() {
    wordRep_->clear();
    wordSetRep_->clear();
    wordSetContentRep_->clear();
}

void User::updateDictionaryChanges() {
    while (true) {
        try {
            std::deque<HistoryChangeWordRep> wordRepHistory =
                wordRep_->getHistoryChanges();
            std::deque<HistoryChangeWordSetRep> wordSetRepHistory =
                wordSetRep_->getHistoryChanges();
            std::deque<HistoryChangeWordSetContentRep>
                wordSetContentRepHistory =
                    wordSetContentRep_->getHistoryChanges();

            while (!wordRepHistory.empty()) {
                sendWordRepHistoryChange(wordRepHistory.back());
                wordRepHistory.pop_back();
            }
            while (!wordSetRepHistory.empty()) {
                sendWordSetRepHistoryChange(wordSetRepHistory.back());
                wordSetRepHistory.pop_back();
            }
            while (!wordSetContentRepHistory.empty()) {
                sendWordSetContentRepHistoryChange(
                    wordSetContentRepHistory.back());
                wordSetContentRepHistory.pop_back();
            }

            wordRep_->clearHistory();
            wordSetRep_->clearHistory();
            wordSetContentRep_->clearHistory();
            break;

        } catch (ServerProblemsExceptionReconnect &) {
            continue;
        } catch (ServerProblemsExceptionNotSaveDict &) {
            break;
        }
    }
}

void User::startRequestThread() {
    timer_.start(100000, [&]() {
        if (isAuthorized()) {
            updateDictionaryChanges();
            std::cout << "send request" << std::endl;
        }
    });
}