# Makefile for MQTT logger
CC=gcc
CXX=g++
LDFLAGS = -lmosquittopp -lsqlite3 -L/usr/local/lib
RM=rm -f
INC_DIR = ./mqtt_logger/SQLiteCpp/include/
CXXFLAGS= -g -std=c++11 -Wall -Wextra -Weffc++ -D_GLIBCXX_USE_NANOSLEEP -pthread -I$(INC_DIR)
BUILD_DIR=build
CHECK_BUILD=if [ ! -d "./$(BUILD_DIR)" ];then	\
				mkdir $(BUILD_DIR);     		\
			fi
MKDIR_P = mkdir -p

# MQTT Logger
mqtt_logger_out=mqtt_logger
mqtt_logger_src=./mqtt_logger/main.cpp \
				./mqtt_logger/mqtt_logger.cpp \
				./mqtt_logger/logger.cpp \
				./mqtt_logger/SQLiteCpp/src/*
$(mqtt_logger_out): $(mqtt_logger_src)
	@echo
	@echo Building MQTT Logger
	@echo
	$(CHECK_BUILD)
	$(CXX) $(CXXFLAGS) $(mqtt_logger_src) $(LDFLAGS) -o $(BUILD_DIR)/$(mqtt_logger_out)

# Make all
all : $(mqtt_logger_out)
.PHONY : all

# Clean build dir
.PHONY : clean
clean:
	$(RM) ./$(BUILD_DIR)/*

# Purge the build dir
.PHONY : purge
purge:
	$(RM)r ./$(BUILD_DIR)
