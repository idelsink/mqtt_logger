#include <iostream>
#include <mosquittopp.h>
#include <signal.h>
#include <sys/stat.h>

#include "logger.hpp"

// declarations
volatile bool receivedSIGINT{ false };
void handle_sigint (int);

// main
int main () {
    // set signal handlers
    signal (SIGINT, handle_sigint);

    std::cout << "MQTT Logger" << std::endl;

    // init libmosquitto
    int mosquitto_lib_version[] = { 0, 0, 0 };
    mosqpp::lib_init ();
    mosqpp::lib_version (&mosquitto_lib_version[0], &mosquitto_lib_version[1],
    &mosquitto_lib_version[2]);
    std::cout << "using Mosquitto lib version " << mosquitto_lib_version[0]
              << '.' << mosquitto_lib_version[1] << '.'
              << mosquitto_lib_version[2] << std::endl;

    // run logger
    try {
        logger mqtt_logger;
        while (!receivedSIGINT) {

            // int rc = client.loop ();
            // if (rc) {
            //    LOG (ERROR) << "MQTT: attempting reconnect";
            //    client.reconnect ();
            //}
        }
        std::cout << "Revieced signal for signalhandler" << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception " << e.what ();
    } catch (...) {
        std::cout << "UNKNOWN EXCEPTION" << std::endl;
    }

    std::cout << " MQTT Logger stopped" << std::endl;
    mosqpp::lib_cleanup ();

    return 0;
}

// signal handlers
void handle_sigint (int) {
    receivedSIGINT = true;
}
