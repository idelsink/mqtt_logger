# Makefile for MQTT logger
CC=gcc
CXX=g++
LDFLAGS = -lmosquittopp -lsqlite3 -L/usr/local/lib
RM=rm -f
INC_DIRS = /usr/local/include ./mqtt_logger/SQLiteCpp/include/ ./webserver/mongoose-cpp/mongoose/ ./webserver/mongoose-cpp/
INC_PARAMS=$(foreach d, $(INC_DIRS), -I$d)
CXXFLAGS= -g -std=c++11 -Wall -Wextra -Weffc++ -D_GLIBCXX_USE_NANOSLEEP -pthread $(INC_PARAMS) -DSQLITE_THREADSAFE=2
BUILD_DIR=build
CHECK_BUILD=if [ ! -d "./$(BUILD_DIR)" ];then	\
				mkdir $(BUILD_DIR);     		\
			fi
MKDIR_P = mkdir -p

# Webserver
webserver_out=webserver
webserver_src=./webserver/main.cpp \
			  ./webserver/mongoose-cpp/*.c \
			  ./webserver/mongoose-cpp/mongoose/Utils.cpp \
	          ./webserver/mongoose-cpp/mongoose/Controller.cpp \
	          ./webserver/mongoose-cpp/mongoose/Mutex.cpp \
	          ./webserver/mongoose-cpp/mongoose/Request.cpp \
	          ./webserver/mongoose-cpp/mongoose/Response.cpp \
	          ./webserver/mongoose-cpp/mongoose/Server.cpp \
	          ./webserver/mongoose-cpp/mongoose/Session.cpp \
	          ./webserver/mongoose-cpp/mongoose/Sessions.cpp \
	          ./webserver/mongoose-cpp/mongoose/StreamResponse.cpp \
	          ./webserver/mongoose-cpp/mongoose/UploadFile.cpp \
			  ./webserver/mongoose-cpp/mongoose/WebController.cpp \
			  ./webserver/mongoose-cpp/mongoose/WebSocket.cpp \
	          ./webserver/mongoose-cpp/mongoose/WebSockets.cpp \
			  ./mqtt_logger/mqtt_logger.cpp \
			  ./mqtt_logger/logger.cpp \
			  ./mqtt_logger/SQLiteCpp/src/*

$(webserver_out): $(webserver_src)
	@echo
	@echo Building $(webserver_out)
	@echo
	$(CHECK_BUILD)
	$(CXX) $(CXXFLAGS) $(webserver_src) $(LDFLAGS) -o $(BUILD_DIR)/$(webserver_out)

# MQTT Logger
mqtt_logger_out=mqtt_logger
mqtt_logger_src=./mqtt_logger/main.cpp \
				./mqtt_logger/mqtt_logger.cpp \
				./mqtt_logger/logger.cpp \
				./mqtt_logger/SQLiteCpp/src/*
$(mqtt_logger_out): $(mqtt_logger_src)
	@echo
	@echo Building $(mqtt_logger_out)
	@echo
	$(CHECK_BUILD)
	$(CXX) $(CXXFLAGS) $(mqtt_logger_src) $(LDFLAGS) -o $(BUILD_DIR)/$(mqtt_logger_out)

# Make all
all : $(mqtt_logger_out) \
	$(webserver_out)
.PHONY : all

# Clean build dir
.PHONY : clean
clean:
	$(RM) ./$(BUILD_DIR)/*

# Purge the build dir
.PHONY : purge
purge:
	$(RM)r ./$(BUILD_DIR)
