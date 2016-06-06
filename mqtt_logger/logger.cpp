#include "logger.hpp"
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <SQLiteCpp/SQLiteCpp.h>

/*static*/ const std::string logger::database_name = "data.db3";

logger::logger ()
: database (database_name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) {
    init_database ();
}
logger::~logger () {
}

void logger::sqlite_deleter::operator()(sqlite3* p) {
    sqlite3_close (p);
}

void logger::init_database () {
    try {
        /* Setup topic table
         * topic [unique]
         */
        std::string setup_topic = "CREATE TABLE IF NOT EXISTS topic("
                                  "id INTEGER PRIMARY KEY NOT NULL, "
                                  "topic TEXT NOT NULL"
                                  ");";
        database.exec (setup_topic);
        /* Setup payload table
         * payload [unique]
         */
        std::string setup_payload = "CREATE TABLE IF NOT EXISTS payload("
                                    "id INTEGER PRIMARY KEY NOT NULL, "
                                    "payload TEXT NOT NULL"
                                    ");";
        database.exec (setup_payload);
        /* Setup message table
         * id [unique]
         * topic
         * payload
         * timestamp (default NOW)
         */
        std::string setup_message =
        "CREATE TABLE IF NOT EXISTS message("
        "id INTEGER PRIMARY KEY NOT NULL, "
        "topic INTEGER, "
        "payload INTEGER, "
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY(topic) REFERENCES topic(id), "
        "FOREIGN KEY(payload) REFERENCES payload(id) "
        ");";
        database.exec (setup_message);

    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
}
