#include "mqtt_logger.hpp"
#include "logger.hpp"
#include <algorithm>
mqtt_logger::mqtt_logger (std::string topic)
: mosqpp::mosquittopp{ "THIS_IS_THE_NSA" }
, _mtx{}
, _topic (topic)
, _logger () {
    connect ("test.mosquitto.org", 1883, MQTT_KEEP_ALIVE);
}

mqtt_logger::~mqtt_logger () {
    disconnect ();
}

void mqtt_logger::on_connect (int rc) {
    std::cout << "MQTT:\tconnected with broker, rc=" << rc << std::endl;
    if (rc == 0) {
        subscribe (nullptr, _topic.c_str(), MQTT_QoS_0);
    }
}

void mqtt_logger::on_disconnect (int rc) {
    std::cout << "disconnected from broker, rc=" << rc << std::endl;
}

void mqtt_logger::on_message (const mosquitto_message* message) {
    std::unique_lock<std::mutex> lk{ _mtx };
    std::string topic = message->topic;
    // myString.assign(data, size);
    // Remove all double-quote characters


    std::string payload;
    payload.assign ((char*)message->payload, (int)message->payloadlen);
    payload.erase (remove (payload.begin (), payload.end (), '"'), payload.end ());
    payload.erase (remove (payload.begin (), payload.end (), '\"'), payload.end ());
    // std::cout << "MQTT "
    //          << "topic=" << topic << ", payload=" << payload << std::endl;

    _logger.add_message (topic, payload);
}

void mqtt_logger::on_subscribe (int mid, int qos_count, const int* granted_qos) {
    std::cout << "MQTT:\tsubscription succeeded = " << mid << ", qos_count = " << qos_count
              << ", granted_qos = " << *granted_qos << std::endl;
}


void mqtt_logger::on_error () {
    std::cout << "MQTT:\tunspecified error" << std::endl;
}
