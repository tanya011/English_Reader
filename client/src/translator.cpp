#include <string>
#include <curl/curl.h>

namespace translate {

size_t curlWriteFunc(char *data,
                     size_t size,
                     size_t nmemb,
                     std::string *buffer) {
    size_t result = 0;
    if (buffer) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

std::string translate(const std::string &text) {
    CURL *curl = curl_easy_init();
    std::string url_enc = curl_easy_escape(curl, text.c_str(), text.size());
    std::string urlStr;
    urlStr.append(
        "https://script.google.com/macros/s/AKfycbxn7kf4lpeU93754VxFyZ9C915ski1RJ70J7lnorEq5wBIn3eL9zoW0PhwKyhFiwBXXqA/exec");
    urlStr.append("?q=" + url_enc + "&target=ru&source=en");
    const char *url = urlStr.c_str();
    char curlErrorBuffer[CURL_ERROR_SIZE];
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlErrorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        std::string answer;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunc);
        CURLcode curlResult = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (curlResult == CURLE_OK) {
            return answer;
        } else {
            return "Error!";
        }
    }
    return "Error";
}
}
