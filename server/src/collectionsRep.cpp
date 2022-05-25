#include "include/collectionsRep.h"
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

CollectionsRep::CollectionsRep(DBManager &m, std::mutex *mutex)
    : manager_(m), mutex_(mutex) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "userId INT,"
                  "bookId INT"
                  ")");
}
std::vector<int> CollectionsRep::getUserBookId(int userId) {
    mutex_->lock();
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(
        stmt->executeQuery("SELECT bookId FROM " + tableName_ +
                           " WHERE userId=" + std::to_string(userId)));
    mutex_->unlock();
    std::vector<int> booksId;
    while (reqRes->next()) {
        booksId.emplace_back(reqRes->getInt("bookId"));
    }
    return booksId;
}

void CollectionsRep::addBookToUser(int userId, int bookId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::PreparedStatement> prst(
        manager_.getConnection().prepareStatement("INSERT INTO " + tableName_ +
                                                  " (userId, bookId) VALUES "
                                                  "(?,?)"));
    prst->setInt(1, userId);
    prst->setInt(2, bookId);
    prst->execute();
}

void CollectionsRep::deleteBookFromUser(int userId, int bookId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::PreparedStatement> prst(
        manager_.getConnection().prepareStatement(
            "DELETE FROM " + tableName_ + " WHERE bookId=? AND userId=?"));
    prst->setInt(1, userId);
    prst->setInt(2, bookId);
    prst->execute();
}
