#include "header.hpp"

// Function to get an access token by sending a GET request
std::string getAccessToken() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string url =  BASE_URL + "public/auth";

    // Append GET parameters for authentication
    url += "?client_id=" + CLIENT_ID + "&client_secret=" + CLIENT_SECRET + "&grant_type=client_credentials";

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set curl options for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Set the write function to handle response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors in the request
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up curl
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return readBuffer; // Return the response
}

// Function to send a POST request with a JSON payload
std::string sendRequest(const std::string& url, const nlohmann::json& payload) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT); // Initialize curl globally
    curl = curl_easy_init();
    if (curl) {
        // Set curl options for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, (BASE_URL + url).c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L); // Set the request method to POST
        
        // Set the JSON payload
        std::string jsonStr = payload.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        
        // Set headers for the request (including authorization if available)
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!ACCESS_TOKEN.empty()) { // Include authorization header if access token is available
            headers = curl_slist_append(headers, ("Authorization: Bearer " + ACCESS_TOKEN).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        // Set the write function to capture the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the POST request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Fatal error -> in 'curl_easy_perform' result not CURLE_OK" << std::endl;
        }

        // Free resources
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup(); // Clean up global curl resources
    return readBuffer; // Return the response
}
