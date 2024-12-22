#pragma once

#include "streamer.hpp"

#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <limits>
#include <set>
#include <map>
#include <sstream>
#include <nlohmann/json.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <thread>
#include <asio/ssl.hpp> 
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iomanip>
#include <csignal>
#include <atomic>

extern std::string CLIENT_ID;
extern std::string CLIENT_SECRET;
extern std::string ACCESS_TOKEN;
extern std::string BASE_URL;
extern std::set<std::string> order_type_values;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
bool readStringInput(const std::string &message, std::string &var);
bool readFloatInput(const std::string &message, float &var);
void readIntChoiceWithTitles(const std::map<int, std::string>& choicesWithTitles, int &choice);

void placeOrder();
void EditOrder();
void CancelOrder();
void GetOrderBook();
void GetCurrentPositions();

std::string getAccessToken();
std::string sendRequest(const std::string& url, const nlohmann::json& payload);