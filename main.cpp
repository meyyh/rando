#include <iostream>
#include <vector>
#include <string>



std::string appdata = std::getenv("APPDATA") ? std::getenv("APPDATA") : "himom"; // If APPDATA is empty, set to "himom"
std::string lappdata = std::getenv("LOCALAPPDATA") ? std::getenv("LOCALAPPDATA") : "himom"; // If LOCALAPPDATA is empty, set to "himom"




    
class BrowserPaths {
private:
    std::vector<std::string> paths;

public:
    BrowserPaths() {
        // Initialize browser paths
        paths.push_back(appdata + "\\Kometa\\User Data");
        paths.push_back(appdata + "\\Orbitum\\User Data");
        paths.push_back(appdata + "\\CentBrowser\\User Data");
        paths.push_back(appdata + "\\7Star\\7Star\\User Data");
        paths.push_back(appdata + "\\Sputnik\\Sputnik\\User Data");
        paths.push_back(appdata + "\\Vivaldi\\User Data");
        paths.push_back(appdata + "\\Google\\Chrome SxS\\User Data");
        paths.push_back(appdata + "\\Google\\Chrome\\User Data");
        paths.push_back(appdata + "\\Epic Privacy Browser\\User Data");
        paths.push_back(appdata + "\\Microsoft\\Edge\\User Data");
        paths.push_back(appdata + "\\uCozMedia\\Uran\\User Data");
        paths.push_back(appdata + "\\Yandex\\YandexBrowser\\User Data");
        paths.push_back(appdata + "\\BraveSoftware\\Brave-Browser\\User Data");
        paths.push_back(appdata + "\\Iridium\\User Data");
        paths.push_back(lappdata + "\\Opera Software\\Opera Stable");
        paths.push_back(lappdata + "\\Opera Software\\Opera GX Stable");
    }

    std::string getBrowserPath(size_t index) {
        if (index < paths.size()) {
            return paths[index];
        }
        return ""; // Return empty string if index is out of range
    }
};

int main() {
    BrowserPaths paths;

    // Example usage:

    for (int i = 0; i < 20; i++) {
        std::cout << "Path for index 0: " << paths.getBrowserPath(i) << std::endl;
    }
    

    return 0;
}