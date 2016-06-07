#ifndef MQTT_LOGGER_H
#define MQTT_LOGGER_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <mosquittopp.h>
#include <stdexcept>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <thread>
#include <utility>
#include "logger.hpp"

class mqtt_logger : public mosqpp::mosquittopp {
    public:
    const int MQTT_QoS_0{ 0 };
    const int MQTT_QoS_1{ 1 };
    const int MQTT_QoS_2{ 2 };
    const bool MQTT_RETAIN_OFF{ false };
    const bool MQTT_RETAIN_ON{ true };
    const int MQTT_KEEP_ALIVE{ 60 };
    mqtt_logger (std::string topic = "#");

    mqtt_logger (const mqtt_logger& other) = delete;
    mqtt_logger& operator= (const mqtt_logger& other) = delete;
    virtual ~mqtt_logger ();
    int get_message_count(std::string tablename){
        return _logger.get_message_count(tablename);
    }
    std::string get_topic(){
        return _topic;
    }

    protected:
    std::mutex _mtx;
    std::string _topic;
    logger _logger;

    virtual void on_connect (int rc) override;
    virtual void on_disconnect (int rc) override;
    virtual void on_message (const struct mosquitto_message* message) override;
    virtual void on_subscribe (int mid, int qos_count, const int* granted_qos) override;
    virtual void on_error () override;

    private:

};

#endif
