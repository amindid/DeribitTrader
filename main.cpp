#include <iostream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <set>
std::string CLIENT_ID;
std::string CLIENT_SECRET;
std::set<std::string> order_type_values = { 
    "limit", "stop_limit", "take_limit",
    "market", "stop_market", "take_market",
    "market_limit", "trailing_stop"
};

// function to handel the httpresponse and change to a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool readStringInput(const std::string &message, std::string &var) {
    std::cout << message << std::endl;
    std::cin >> var;
    return !var.empty();
}

bool readFloatInput(const std::string &message, float &var) {
    std::cout << message << std::endl;
    return (std::cin >> var).good();
}

// generate the http request to get the access token
std::string sendRequest(std::string url, const std::string& method, const std::string& payload = "") {
    CURL* curl;
    CURLcode res;
    std::string response;
    // std::string url = "https://test.deribit.com/api/v2/public/auth";
    

    // Append the GET parameters to the URL
    url += "?client_id=" + CLIENT_ID + "&client_secret=" + CLIENT_SECRET + "&grant_type=client_credentials";

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Client-ID: " + CLIENT_ID).c_str());
        headers = curl_slist_append(headers, ("Client-Secret: " + CLIENT_SECRET).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        }

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }


    curl_global_cleanup();

    return response;
}

void placeOrder() {
    std::string instrument_name;
    std::string type;
    std::string direction;
    float amount;

    while(!readStringInput("Enter the instrument name : ", instrument_name))
        std::cout << "ERROR: please enter a non-empty string" << std::endl;
    while(!readFloatInput("Enter the amount : ", amount)) {
        std::cout << "ERROR: please enter a non-empty and valid number" << std::endl;
    }

}


int main() {
    // load client_id and client_secret from the .env file
    const char * client_id = std::getenv("CLIENT_ID");
    const char * client_secret = std::getenv("CLIENT_SECRET");

    if (!client_id || !client_secret) {
        std::cerr << "CREDENTIALS ARE NOT PROVIDED" << std::endl;
        exit(1);
    }
    CLIENT_ID = client_id;
    CLIENT_SECRET = client_secret;
    std::string url = "https://test.deribit.com/api/v2/public/test";
    std::string response = sendRequest(url, "GET");
    std::cout << "GET Response:\n" << response << std::endl;
}