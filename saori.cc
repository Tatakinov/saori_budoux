#include "saori.h"

#include <iostream>
#include <optional>
#include <sstream>

#include "budoux.h"
#include "sstp/request.h"
#include "sstp/response.h"
#include "util.h"

bool __load(std::string path) {
    return budoux::loadModel(path);
}

bool __unload() {
    return true;
}

std::string __request(std::string request) {
    sstp::Request req = sstp::Request::parse(request);
    if (req.getCommand() == "GET Version") {
        sstp::Response res{204, "No Content", "SAORI/1.0"};
        return res;
    }
    if ( ! req["Argument0"]) {
        sstp::Response res{400, "Bad Request", "SAORI/1.0"};
        return res;
    }
    std::string str = req["Argument0"].value();
    if (req["Charset"] && req["Charset"].value() == "Shift_JIS") {
        str = conv(str, 65001, 932);
    }
    int max_width   = 0;
    if (req["Argument1"]) {
        std::istringstream iss(req["Argument1"].value());
        iss >> max_width;
    }
    auto result = budoux::parse(str, max_width);
    sstp::Response res{200, "OK", "SAORI/1.0"};
    res.set("Charset", "UTF-8");
    res.set("Result", result.size());
    for (int i = 0; i < result.size(); i++) {
        std::ostringstream oss;
        oss << "Value" << i;
        res.set(oss.str(), result[i]);
    }
    return res;
}
