#ifndef YAFR_CONFIG_H
#define YAFR_CONFIG_H
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

struct Config {
private:
    nlohmann::json config_;

public:
    explicit Config(const std::string& configFile) {
        std::ifstream file(configFile);
        std::stringstream buf;
        buf << file.rdbuf();
        config_ = nlohmann::json::parse(buf.str());
    }
    std::string get(const std::string& key) {
        return config_[key];
    }
};

#endif  // YAFR_CONFIG_H
