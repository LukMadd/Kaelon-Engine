#include "Debug/Debugger.hpp"
#include <chrono>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string getFileName(const char* path){
    std::string p(path);
    size_t pos = p.find_last_of("/\\");
    return (pos == std::string::npos) ? p : p.substr(pos + 1);
}

std::string getTimeStamp(){
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    #ifdef WIN_32
        localtime_s(&local_tm, &now_time_t);
    #else
        localtime_r(&now_time_t, &local_tm);
    #endif

    std::stringstream ss;
    ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}

void Debugger::initDebugSystem(const char* subsystem_name){
    if(subsystem_name){
        subsystems.push_back(subsystem_name);
        activeSubsystems.push_back(subsystem_name);
    }
}

void Debugger::log(int logSeverity, const char* logMessage, const char* subsystem, 
                   const char* FILE, int LINE, const char* FUNC){
    if(std::binary_search(activeSubsystems.begin(), activeSubsystems.end(), 
subsystem) == false){
        return;
    } //Filters to make sure the subsystem is allowed to log

    if(logSeverity < 0 || logSeverity > LOG_SEVERITY_TYPE_COUNT - 1){
        std::cerr << "Invalid log type!\n";
    }
    if(!logMessage){
        std::cerr << "Invalid log message!\n";
    }

    displayLog(logSeverity, logMessage, subsystem, FILE, LINE, FUNC);

    std::cout << std::endl;

    LogInfo logInfo{};
    logInfo.logSeverity = logSeverity;
    logInfo.logMessage = logMessage;
    logInfo.FILE = FILE;
    logInfo.LINE = LINE;
    logInfo.FUNC = FUNC;
    
    logs[subsystem].push_back(logInfo);
}

void Debugger::displayLog(int logSeverity, const char* logMessage, const char* subsystem,
                          const char* FILE, int LINE, const char* FUNC){
    if(logSeverity == INFO){
        std::cout << "[" << getTimeStamp() << "] " << "[INFO] " << "[" << subsystem << "] " << "(" << getFileName(FILE) << ":" << LINE << ") " << logMessage; 
    } else if(logSeverity == WARNING){
        std::cout << "[" << getTimeStamp() << "] " << "[WARNING] " << "[" << subsystem << "] " << "(" << getFileName(FILE) << ":" << LINE << ") " << logMessage;
    } else if(logSeverity == ERROR){
        std::cout << "[" << getTimeStamp() << "] " << "[ERROR] " << "[" << subsystem << "] " << "(" << getFileName(FILE) << ":" << LINE << ") " << logMessage;
    } else if(logSeverity == CRITICAL){
        std::cout << "[" << getTimeStamp() << "] " << "[CRITICAL_ERROR] " << "[" << subsystem << "] " << "(" << getFileName(FILE) << ":" << LINE << ") " << logMessage;
    }
}