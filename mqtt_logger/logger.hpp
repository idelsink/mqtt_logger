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

    private:
    // static const std::string database_name = "data.db3";
    static const std::string database_name;
    struct sqlite_deleter {
        void operator()(sqlite3* p);
    };
    SQLite::Database database;
    void init_database ();
};

#endif // logger_HPP
