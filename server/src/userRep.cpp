#include "include/userRep.h"

UserRep::UserRep(DBManager &m, std::mutex *mutex) : manager(m), mutex_(mutex) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());

    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "id INT UNIQUE,"
                  "name TINYTEXT,"
                  "passwordHash TINYTEXT,"
                  "token TINYTEXT"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
        stmt->executeQuery("SELECT MAX(id) FROM " + tableName));
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}
void UserRep::addUser(const std::string &name,
                      const std::string &hash,
                      const std::string &token) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::PreparedStatement> prst(
        manager.getConnection().prepareStatement(
            "INSERT INTO " + tableName +
            " (id, name, passwordHash, token) VALUES "
            "(?,?,?,?)"));
    prst->setInt(1, freeId);
    prst->setString(2, name);
    prst->setString(3, hash);
    prst->setString(4, token);
    prst->execute();
    freeId++;
}

std::string UserRep::getUserToken(const std::string &name) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT * FROM " + tableName + " WHERE name=" + name));
    if (reqRes->next()) {
        return static_cast<std::string>(reqRes->getString("token"));
    } else {
        throw UserNotFoundException("name", name);
    }
}

std::string UserRep::getUserName(const std::string &token) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT * FROM " + tableName + " WHERE token=" + token));
    if (reqRes->next()) {
        return static_cast<std::string>(reqRes->getString("token"));
    } else {
        throw UserNotFoundException("token", token);
    }
}

bool UserRep::isUserExist(const std::string &name, const std::string &hash) {
    std::unique_lock l(*mutex_);
    std::cout << "Requesting..." << std::endl;
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::string query = "SELECT token FROM " + tableName + " WHERE " + "name='" +
                        name + "' AND passwordHash='" + hash + "'";
    std::cout << query << std::endl;
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(query));

    std::cout << "Requested successfully" << std::endl;
    if (reqRes->next()) {
        return true;
    }
    return false;
}

int UserRep::getUserId(const std::string &token) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT id FROM " + tableName + " WHERE token=" + token));
    if (reqRes->next()) {
        return reqRes->getInt("id");
    } else {
        throw UserNotFoundException("token", token);
    }
}

UserNotFoundException::UserNotFoundException(const std::string &charact,
                                             const std::string &value)
    : std::runtime_error("User with " + charact + "= '" + value +
                         "' not found") {
}
