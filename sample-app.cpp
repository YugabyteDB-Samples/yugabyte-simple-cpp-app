#include <iostream>
#include <pqxx/pqxx>

const std::string HOST = "127.0.0.1";
const std::string PORT = "5433";
const std::string DB_NAME = "yugabyte";
const std::string USER = "yugabyte";
const std::string PASSWORD = "yugabyte";
const std::string SSL_MODE = "";
const std::string SSL_ROOT_CERT = "";

pqxx::connection* connect();
void createDatabase(pqxx::connection *conn);
void selectAccounts(pqxx::connection *conn);
void transferMoneyBetweenAccounts(pqxx::connection *conn, int amount);

int main(int, char *argv[]) {
    pqxx::connection *conn = NULL;

    try {
        conn = connect();

        createDatabase(conn);
        selectAccounts(conn);
        transferMoneyBetweenAccounts(conn, 800);
        selectAccounts(conn);

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;

        if (conn != NULL) {
            delete conn;
        }
        return 1;
    } 

    if (conn != NULL) {
        delete conn;
    }
    
    return 0;
}

pqxx::connection* connect() {
    std::string url = "host=" + HOST + " port=" + PORT + " dbname=" + DB_NAME +
        " user=" + USER + " password=" + PASSWORD;

    if (SSL_MODE != "") {
        url += " sslmode=" + SSL_MODE;

        if (SSL_ROOT_CERT != "") {
            url += " sslrootcert=" + SSL_ROOT_CERT;
        }
    }

    std::cout << ">>>> Connecting to YugabyteDB!" << std::endl;

    pqxx::connection *conn = new pqxx::connection(url);

    std::cout << ">>>> Successfully connected to YugabyteDB!" << std::endl;

    return conn;
} 

void createDatabase(pqxx::connection *conn) {
    pqxx::work txn(*conn);

    txn.exec("DROP TABLE IF EXISTS DemoAccount");

    txn.exec("CREATE TABLE DemoAccount ( \
                id int PRIMARY KEY, \
                name varchar, \
                age int, \
                country varchar, \
                balance int)");

    txn.exec("INSERT INTO DemoAccount VALUES \
                (1, 'Jessica', 28, 'USA', 10000), \
                (2, 'John', 28, 'Canada', 9000)");
    
    txn.commit();

    std::cout << ">>>> Successfully created table DemoAccount." << std::endl;
}

void selectAccounts(pqxx::connection *conn) {
    pqxx::work txn(*conn);
    pqxx::result res;

    std::cout << ">>>> Selecting accounts:" << std::endl;

    res = txn.exec("SELECT name, age, country, balance FROM DemoAccount");

    for (auto row: res) {
      std::cout 
          << "name=" << row["name"].c_str() << ", "
          << "age=" << row["age"].as<int>() << ", "
          << "country=" << row["country"].c_str() << ", "
          << "balance=" << row["balance"].as<int>() << std::endl;
    }

    // Not really needed, since we made no changes, but good habit to be
    // explicit about when the transaction is done.
    txn.commit();
}

void transferMoneyBetweenAccounts(pqxx::connection *conn, int amount) {
    try {
        pqxx::work txn(*conn);

        txn.exec("UPDATE DemoAccount SET balance = balance -" + std::to_string(amount) 
            + " WHERE name = \'Jessica\'");

        txn.exec("UPDATE DemoAccount SET balance = balance +" + std::to_string(amount) 
            + " WHERE name = \'John\'");

        txn.commit();

        std::cout << ">>>> Transferred " << amount << " between accounts." << std::endl;
    } catch (pqxx::sql_error const &e) {
        if (e.sqlstate().compare("40001") == 0) {
            std::cerr << "The operation is aborted due to a concurrent transaction that is modifying the same set of rows." 
                      << "Consider adding retry logic or switch to the pessimistic locking." << std::endl;
        }
        throw e;   
    }
}

