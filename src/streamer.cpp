#include "streamer.hpp"

using json = nlohmann::json;

// Atomic flag to track streaming state
std::atomic<bool> is_streaming(false);

// Mapping of symbol IDs to names
std::map<int, std::string> symbols = {
    {1, "btc_usd"},
    {2, "eth_usd"},
    {3, "btc_usdc"},
    {4, "eth_usdc"},
    {5, "xrp_usdc"},
    {6, "sol_usdc"},
    {7, "link_usdc"},
    {8, "uni_usdc"},
    {9, "matic_usdc"},
    {10, "avax_usdc"},
    {11, "fil_usdc"},
    {12, "trx_usdc"},
    {13, "ltc_usdc"}
};

// Convert timestamp (milliseconds) to human-readable format
std::string formatTimestamp(long long timestamp) {
    std::time_t time = timestamp / 1000; // Convert to seconds
    std::tm* localTime = std::localtime(&time); // Convert to local time
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // Format time
    return oss.str();
}

// SSL context initialization handler
std::shared_ptr<asio::ssl::context> on_tls_init(std::weak_ptr<void> hdl) {
    std::shared_ptr<asio::ssl::context> ctx =
        std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
    ctx->set_options(asio::ssl::context::sslv23 | asio::ssl::context::no_sslv2);
    return ctx;
}

DeribitStreamer::DeribitStreamer() {}

void DeribitStreamer::start_streaming(int symbol) {
    if (is_streaming.load()) {
        std::cout << "Streaming is already running.\n";
        return;
    }

    is_streaming.store(true);
    streaming_thread = std::thread([this, symbol]() { run_streaming(symbol); }); // Start streaming in a separate thread
}

void DeribitStreamer::stop_streaming() {
    if (!is_streaming.load()) return;
    is_streaming.store(false);

    if (streaming_thread.joinable()) {
        streaming_thread.join(); // Join the thread if it's running
    }
    std::cout << "Stopped streaming.\n";
}

DeribitStreamer::~DeribitStreamer() { this->stop_streaming(); }

void DeribitStreamer::run_streaming(int symbol) {
    deribit_client.init_asio(); // Initialize Asio for WebSocket
    deribit_client.set_tls_init_handler(on_tls_init); // Set SSL handler

    deribit_client.set_message_handler([this](connection_hdl hdl, client::message_ptr msg) {
        handle_message(msg->get_payload()); // Handle incoming message
    });

    deribit_client.set_open_handler([this, symbol](connection_hdl hdl) {
        subscribe_to_symbols(hdl, {symbols[symbol]}); // Subscribe to symbol channels on connection open
    });

    websocketpp::lib::error_code ec;
    client::connection_ptr con = deribit_client.get_connection("wss://www.deribit.com/ws/api/v2", ec);
    if (ec) {
        std::cerr << "Error connecting to Deribit: " << ec.message() << std::endl;
        return;
    }

    deribit_client.connect(con); // Connect to the WebSocket
    deribit_client.run(); // Start the WebSocket event loop
}

void DeribitStreamer::subscribe_to_symbols(connection_hdl hdl, const std::vector<std::string>& symbols) {
    for (const auto& symbol : symbols) {
        json subscription = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "public/subscribe"},
            {"params", {{"channels", {"deribit_price_index." + symbol}}}}
        };
        deribit_client.send(hdl, subscription.dump(), websocketpp::frame::opcode::text); // Send subscription message
    }
}

void DeribitStreamer::handle_message(const std::string& message) {
    try {
        json parsed = json::parse(message); // Parse the incoming JSON message
        std::string channel = parsed["params"]["channel"];
        std::string index_name = parsed["params"]["data"]["index_name"];
        auto priceJson = parsed["params"]["data"]["price"];
        long long timestamp = parsed["params"]["data"]["timestamp"];
        std::string price = priceJson.is_null() ? "N/A" : std::to_string(priceJson.get<double>());
        std::string formattedTimestamp = formatTimestamp(timestamp); // Convert timestamp

        // Display the message in a user-friendly format
        std::cout << "\033[2J\033[H"; // Clear console
        std::cout << "---------------------------------------------\n";
        std::cout << "Channel: " << channel << "\n";
        std::cout << "Index Name: " << index_name << "\n";
        std::cout << "Price: " << price << "\n";
        std::cout << "Timestamp: " << formattedTimestamp << "\n";
        std::cout << "---------------------------------------------\n";
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << "\n"; // Handle JSON parsing errors
    }
}
