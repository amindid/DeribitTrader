#include "header.hpp"


// Function to place an order (buy or sell)
void placeOrder() {

    int choice; // Choice for buying or selling
    std::string instrument_name; // Name of the instrument to trade
    std::string type; // Order type (limit, market, etc.)
    float amount; // Amount to buy or sell
    float price; // Price of the instrument
    
    // Map of choices for buy and sell actions
    std::map<int, std::string> choices = {
        {1, "buy"},
        {2, "sell"}
    };

    // Prompt user for choice (buy or sell)
    readIntChoiceWithTitles(choices,choice);
    
    // Input validation for instrument name
    while(!readStringInput("Enter the instrument name : ", instrument_name)) {
        std::cout << "ERROR: please enter a non-empty string" << std::endl;
    }
    
    // Input validation for amount
    while(!readFloatInput("Enter the amount : ", amount)) {
        std::cout<< amount << "   " << "ERROR: please enter a non-empty and valid number" << std::endl;
    }
    
    // Input validation for price
    while(!readFloatInput("Enter the price : ", price)) {
        std::cout<< amount << "   " << "ERROR: please enter a non-empty and valid number" << std::endl;
    }
    
    // Input validation for order type (with default 'limit' type if empty)
    while(1) {
        if (!readStringInput("Enter the type (leave empty to take 'limit' by default): ", type)) {
            type = "limit";
            break;
        }
        else if (order_type_values.find(type) == order_type_values.end())
            std::cout << "ERROR: Invalid type value. It must be one of the following:\n'limit', 'stop_limit', 'take_limit','market', 'stop_market', 'take_market', 'market_limit', 'trailing_stop'" << std::endl;
        else
            break;
    }
    
    // Create the payload for the order request (buy or sell)
    nlohmann::json payload = {
        {"id", 1},
        {"method", (choice == 1 ? "private/buy" : "private/sell")},
        {"params", {
            {"instrument_name", instrument_name},
            {"type", type},
            {"price", price},
            {"amount", amount}
        }},
        {"jsonrpc", "2.0"}
    };
    
    // Send the request and parse the response
    nlohmann::json data = nlohmann::json::parse(sendRequest((choice == 1 ? "private/buy" : "private/sell"), payload));
    
    // Check if there was an error in the response
    if (data.contains("error")) {
        std::cout << "---> action failed : " << (data["error"].contains("message") ? data["error"]["message"] : "ERROR") << " ,please try again.\n" << std::endl;
    }
    else
        std::cout << "---> order placed successfully\n" << std::endl;
}

// Function to edit an existing order
void EditOrder() {
    std::string order_id; // Order ID to edit
    float amount; // New amount for the order
    float price; // New price for the order

    // Input validation for order ID
    while(!readStringInput("Enter the Order ID : ", order_id)) {
        std::cout << "ERROR: please enter a non-empty string" << std::endl;
    }
    
    // Input validation for amount
    while(!readFloatInput("Enter the amount : ", amount)) {
        std::cout<< amount << "   " << "ERROR: please enter a non-empty and valid number" << std::endl;
    }
    
    // Input validation for price
    while(!readFloatInput("Enter the price : ", price)) {
        std::cout<< amount << "   " << "ERROR: please enter a non-empty and valid number" << std::endl;
    }
    
    // Create the payload for the order edit request
    nlohmann::json payload = {
        {"id", 1},
        {"method", "private/edit"},
        {"params", {
            {"order_id", order_id},
            {"amount", amount},
            {"price", price}
        }},
        {"jsonrpc", "2.0"}
    };
    
    // Send the request and parse the response
    nlohmann::json data = nlohmann::json::parse(sendRequest("private/edit", payload));
    
    // Check if there was an error in the response
    if (data.contains("error")) {
        std::cout << "---> action failed : " << (data["error"].contains("message") ? data["error"]["message"] : "ERROR") << " ,please try again.\n" << std::endl;
    }
    else
        std::cout << "---> order edited successfully\n" << std::endl;
}

// Function to cancel an existing order
void CancelOrder() {
    std::string order_id; // Order ID to cancel

    // Input validation for order ID
    while(!readStringInput("Enter the Order ID : ", order_id)) {
        std::cout << "ERROR: please enter a non-empty string" << std::endl;
    }
    
    // Create the payload for the order cancel request
    nlohmann::json payload = {
        {"id", 1},
        {"method", "private/cancel"},
        {"params", {
            {"order_id", order_id}
        }},
        {"jsonrpc", "2.0"}
    };
    
    // Send the request and parse the response
    nlohmann::json data = nlohmann::json::parse(sendRequest("private/cancel", payload));
    
    // Check if there was an error in the response
    if (data.contains("error")) {
        std::cout << "---> action failed : " << (data["error"].contains("message") ? data["error"]["message"] : "ERROR") << " ,please try again.\n" << std::endl;
    }
    else
        std::cout << "---> order Cancled successfully\n" << std::endl;
}

// Function to get the open order book
void GetOrderBook() {
    // Create the payload for getting open orders
    nlohmann::json payload = {
        {"id", 1},
        {"method", "private/get_open_orders"},
        {"jsonrpc", "2.0"}
    };
    
    // Send the request and parse the response
    nlohmann::json data = nlohmann::json::parse(sendRequest("private/get_open_orders", payload));
    
    // Check if there was an error in the response
    if (data.contains("error"))
        std::cout << "---> action failed : " << (data["error"].contains("message") ? data["error"]["message"] : "ERROR") << " ,please try again.\n" << std::endl;
    else if (data.contains("result")) {
        // Check if there are no open orders
        if (data["result"].empty())
            std::cout << "---> there are no open orders yet.\n" << std::endl;
        else {
            // Print the open orders
            std::cout << "---> open orders: " << std::endl;
            for (const auto& order : data["result"]) {
                std::cout << "\torder_id: " << order["order_id"] << std::endl;
            }
            std::cout << std::endl;
        }
    }

}

// Function to get the current positions
void GetCurrentPositions() {
    // Create the payload for getting current positions
    nlohmann::json payload = {
        {"id", 1},
        {"method", "private/get_positions"},
        {"jsonrpc", "2.0"}
    };
    
    // Send the request and parse the response
    nlohmann::json data = nlohmann::json::parse(sendRequest("private/get_positions", payload));
    
    // Check if there was an error in the response
    if (data.contains("error"))
        std::cout << "---> action failed : " << (data["error"].contains("message") ? data["error"]["message"] : "ERROR") << " ,please try again.\n" << std::endl;
    else if (data.contains("result")) {
        // Check if there are no current positions
        if (data["result"].empty())
            std::cout << "---> there are no open orders yet.\n" << std::endl;
        else {
            // Print the current positions
            std::cout << "---> current positions: " << std::endl;
            for (const auto& order : data["result"]) {
                std::cout << "instrument_name: " << order["instrument_name"] << std::endl;
                std::cout << "\tsize: " << order["size"] << std::endl;
                std::cout << "\tdirection: " << order["direction"] << std::endl;
                std::cout << "\taverage_price: " << order["average_price"] << std::endl;
                std::cout << "\tmark_price: " << order["mark_price"] << std::endl;
                std::cout << "\tfloating_profit_loss: " << order["floating_profit_loss"] << std::endl;
                std::cout << "\trealized_profit_loss: " << order["realized_profit_loss"] << std::endl;
                std::cout << "\ttotal_profit_loss: " << order["total_profit_loss"] << std::endl;
                std::cout << "\testimated_liquidation_price: " << order["estimated_liquidation_price"] << std::endl;
                std::cout << "\tinitial_margin: " << order["initial_margin"] << std::endl;
                std::cout << "\tmaintenance_margin: " << order["maintenance_margin"] << std::endl;
            }
            std::cout << std::endl;
        }
    }

}
