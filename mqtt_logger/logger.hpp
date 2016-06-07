#ifndef logger_HPP
#define logger_HPP

#include <string>
#include <sqlite3.h>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>

class logger {
    public:
    logger ();
    ~logger ();
    int add_message(std::string &topic, std::string &payload);
    int get_message_count(std::string tablename);
    private:
    static const std::string database_name;
    struct sqlite_deleter {
        void operator()(sqlite3* p);
    };
    SQLite::Database database;
    void init_database ();
    int add_topic(std::string &topic);
    int add_payload(std::string &payload);
};

#endif // logger_HPP
