#include "header.hpp"
#include <csignal>
#include <atomic>
#include <thread>

using json = nlohmann::json;

// Global variables for credentials and API data
std::string CLIENT_ID;
std::string CLIENT_SECRET;
std::string ACCESS_TOKEN;
std::set<std::string> order_type_values = { 
    "limit", "stop_limit", "take_limit",
    "market", "stop_market", "take_market", "market_limit", "trailing_stop"
};

std::string BASE_URL = "https://test.deribit.com/api/v2/";

// Pointer to the DeribitStreamer object for real-time streaming
DeribitStreamer* streamer = nullptr;


// Main function handling the program flow
int main() {
    // Initialize the DeribitStreamer object
    streamer = new DeribitStreamer();

    // Choices map for menu options
    int choice;
    int symbol;
    std::map<int, std::string> choices = {
        {1, "Place Order"},
        {2, "Cancel Order"},
        {3, "Modify Order"},
        {4, "Get Order Book"},
        {5, "View Current Positions"},
        {6, "Real-time market data streaming"},
        {7, "Exit"}
    };

    // Retrieve environment variables for client credentials
    const char* client_id = std::getenv("CLIENT_ID");
    const char* client_secret = std::getenv("CLIENT_SECRET");

    if (!client_id || !client_secret) { // Check if credentials are available
        std::cerr << "CREDENTIALS ARE NOT PROVIDED" << std::endl;
        exit(1); // Exit if credentials are missing
    }

    CLIENT_ID = client_id;
    CLIENT_SECRET = client_secret;

    // Get the access token from the API
    json data = json::parse(getAccessToken());
    try {
        ACCESS_TOKEN = data["result"]["access_token"];
    } catch (const std::exception& e) {
        std::cerr << "ERROR: ACCESS TOKEN not found" << std::endl;
        exit(1); // Exit if access token is missing
    }

    // Main loop for handling user choices
    while (true) {
        readIntChoiceWithTitles(choices, choice); // Display options and get user input

        // Handle different choices based on user input
        switch (choice) {
            case 1: // Place an order
                placeOrder();
                break;
            case 2: // Cancel an order
                CancelOrder();
                break;
            case 3: // Modify an existing order
                EditOrder();
                break;
            case 4: // Get the order book
                GetOrderBook();
                break;
            case 5: // View current positions
                GetCurrentPositions();
                break;
            case 6: // Start real-time market data streaming
                readIntChoiceWithTitles(symbols, symbol);
                streamer->start_streaming(symbol);
                break;
            case 7: // Exit the program
                streamer->stop_streaming(); // Stop streaming before exit
                exit(0);
            default: // Invalid choice
                std::cout << "Invalid choice. Try again.\n";
                break;
        }
    }
}
