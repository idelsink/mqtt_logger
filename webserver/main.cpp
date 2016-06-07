#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>
#include "../mqtt_logger/logger.hpp"
#include "../mqtt_logger/mqtt_logger.hpp"

// declarations
volatile bool receivedSIGINT{ false };
void handle_sigint (int);

using namespace std;
using namespace Mongoose;


class MyController : public WebController {
    public:
    MyController (mqtt_logger& logger)
    : _logger (logger) {
    }
    void hello (Request& request, StreamResponse& response) {
        response
        << "Hello "
        << htmlEntities (request.get ("name", "... what's your name ?")) << endl;
    }
    void get_data (Request& request, StreamResponse& response) {
        string data_request = request.get ("table", "message");
        response
        << "{\"table\": \"" << data_request << "\",\"nr_of_messages\": "
        << htmlEntities (to_string (_logger.get_message_count (data_request)))
        << "}" << endl;
    }
    void get_current_topic (Request& request, StreamResponse& response) {
        response << "{\"current_topic\": \"" << _logger.get_topic () << "\"}" << endl;
    }

    void setup () {
        addRoute ("GET", "/hello", MyController, hello);
        addRoute ("GET", "/nr_of_messages", MyController, get_data);
        addRoute ("GET", "/current_topic", MyController, get_current_topic);
    }

    private:
    mqtt_logger& _logger;
};


int main (int argc, char const* argv[]) {
    // if no configuration was given, exit
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " topic name" << std::endl;
        ;
        exit (EXIT_FAILURE);
    }
    mqtt_logger MQTT_logger (argv[1]);
    // set signal handlers
    signal (SIGINT, handle_sigint);
    MyController myController (MQTT_logger);
    std::cout << "MQTT Webserver with logger" << std::endl;

    // init libmosquitto
    int mosquitto_lib_version[] = { 0, 0, 0 };
    mosqpp::lib_init ();
    mosqpp::lib_version (&mosquitto_lib_version[0], &mosquitto_lib_version[1],
    &mosquitto_lib_version[2]);
    std::cout << "using Mosquitto lib version " << mosquitto_lib_version[0]
              << '.' << mosquitto_lib_version[1] << '.'
              << mosquitto_lib_version[2] << std::endl;

    Server server (8080, "./website/");
    server.registerController (&myController);

    server.start ();

    try {

        while (!receivedSIGINT) {

            int rc = MQTT_logger.loop ();
            if (rc) {
                std::cout << "MQTT: attempting reconnect" << std::endl;
                MQTT_logger.reconnect ();
            }
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
