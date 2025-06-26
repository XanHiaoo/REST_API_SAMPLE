#ifndef DETECTRESULT_H
#define DETECTRESULT_H

#include <string>
#include <nlohmann/json.hpp>

struct DetectResult {
    std::string snCode;
    std::string result;
    double score;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DetectResult, snCode, result, score)
};

#endif  // DETECTRESULT_H
