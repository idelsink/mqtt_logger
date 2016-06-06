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
        // Begin transaction
        SQLite::Transaction transaction (database);
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
        "timestamp DATETIME DEFAULT (datetime('now','localtime')), "
        "FOREIGN KEY(topic) REFERENCES topic(id), "
        "FOREIGN KEY(payload) REFERENCES payload(id) "
        ");";
        database.exec (setup_message);
        // Commit transaction
        transaction.commit ();
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
}
int logger::add_topic (std::string &topic) {
    // check if exists
    try {
        SQLite::Statement query (
        database, "SELECT id FROM topic WHERE topic='" + topic + "'");
        if (query.executeStep ()) {
            return query.getColumn (0); // id
        }
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
    try {
        // Begin transaction
        SQLite::Transaction transaction (database);
        int nb = database.exec ("INSERT INTO topic VALUES (NULL, \"" + topic + "\")");
        /*std::cout
        << "INSERT INTO topic VALUES (NULL, \"" + topic + "\")\", returned "
        << nb << std::endl;
        // Commit transaction*/
        transaction.commit ();
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
    return database.getLastInsertRowid ();
}
int logger::add_payload (std::string &payload) {
    // check if exists
    try {
        SQLite::Statement query (
        database, "SELECT id FROM payload WHERE payload='" + payload + "'");
        if (query.executeStep ()) {
            return query.getColumn (0); // id
        }
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
    try {
        // Begin transaction
        SQLite::Transaction transaction (database);
        int nb = database.exec ("INSERT INTO payload VALUES (NULL, \"" + payload + "\")");
        /*std::cout
        << "INSERT INTO payload VALUES (NULL, \"" + payload + "\")\", returned "
        << nb << std::endl;*/
        // Commit transaction
        transaction.commit ();
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
    return database.getLastInsertRowid ();
}
int logger::add_message (std::string &topic, std::string &payload) {
    int topic_id = add_topic (topic);
    int payload_id = add_payload (payload);
    try {
        // Begin transaction
        SQLite::Transaction transaction (database);
        std::string insert_command = { "INSERT INTO message VALUES (NULL, \"" +
            std::to_string (topic_id) + "\", \"" +
            std::to_string (payload_id) + "\", CURRENT_TIMESTAMP)" };
        int nb = database.exec (insert_command);
        std::cout
        << "INSERT INTO message VALUES (NULL, \"" + payload + "\")\", returned "
        << nb << std::endl;
        // Commit transaction
        transaction.commit ();
    } catch (std::exception& e) {
        std::cout << "exception: " << e.what () << std::endl;
    }
    return database.getLastInsertRowid ();
}
