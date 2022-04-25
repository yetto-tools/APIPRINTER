//#include <iostream>
//#include <stdexcept>
//#include <stdio.h>
//#include <string>
//
//std::string exec(const char* cmd) {
//    char buffer[128];
//    std::string result = "";
//    FILE* pipe = _popen(cmd, "r");
//
//    if (!pipe) throw std::runtime_error("popen() failed!");
//    try {
//        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
//            result += buffer;
//        }
//    }
//    catch (...) {
//        _pclose(pipe);
//        throw;
//    }
//    _pclose(pipe);
//    return result;
//}