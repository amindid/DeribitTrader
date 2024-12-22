#include "header.hpp"

// Callback function to handle data received from the HTTP request
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb); // Append received data to the string
    return size * nmemb;
}

// Function to read a string input from the user with a prompt message
bool readStringInput(const std::string &message, std::string &var) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore any leftover input
    std::cout << message << std::endl;
    std::cout << "\n> ";
    std::getline(std::cin, var); // Get the string input
    return !var.empty(); // Return true if input is not empty
}

// Function to read a floating-point number from the user with a prompt message
bool readFloatInput(const std::string &message, float &var) {
    std::cout << message << std::endl;
    std::cout << "\n> ";
    while (true) {
        if (std::cin >> var) { // Try to read a valid float
            return true;
        } else {
            std::cout << "Invalid input. Please enter a valid number: " << std::endl;
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
        }
    }
}

// Function to display a list of choices and get a valid integer input from the user
void readIntChoiceWithTitles(const std::map<int, std::string>& choicesWithTitles, int & choice) {
    while (true) {
        std::cout << "Enter your choice:\n";
        for (const auto& [key, title] : choicesWithTitles) {
            std::cout << "  " << key << ". " << title << std::endl; // Print each choice
        }
        std::cout << "\n> ";

        std::cin >> choice; // Read the user's choice

        // Validate the choice
        if (std::cin.fail() || choicesWithTitles.find(choice) == choicesWithTitles.end()) {
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Invalid choice. Please try again.\n";
        } else
            break; // Exit loop if valid choice
    }
}
