#pragma once

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

extern std::atomic<bool> is_streaming;
extern std::map<int, std::string> symbols;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::connection_hdl connection_hdl;

std::string formatTimestamp(long long timestamp);

std::shared_ptr<asio::ssl::context> on_tls_init(std::weak_ptr<void> hdl);


class DeribitStreamer {
    private:
        client deribit_client;
        std::thread streaming_thread;

    public:
        DeribitStreamer();
        void start_streaming(int symbol);
        void stop_streaming();
        ~DeribitStreamer();

    private:
        void run_streaming(int symbol);
        void subscribe_to_symbols(connection_hdl hdl, const std::vector<std::string>& symbols);
        void handle_message(const std::string& message);
};