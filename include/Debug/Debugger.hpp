#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

typedef std::pair<std::string, int> LOG_INFO;

constexpr uint8_t LOG_SEVERITY_TYPE_COUNT = 4;

#define DEBUGGER_LOG(severity, message, subsystem) Debugger::get().log(severity, message, subsystem, __FILE__, __LINE__, __func__);

enum severity{
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

//Subsystem not included because that is used as a key to filter logs
struct LogInfo{
    int logSeverity;
    const char* logMessage;
    const char* FILE;
    int LINE; 
    const char* FUNC;
};

class Debugger{
    std::vector<std::string> subsystems;
    std::unordered_map<std::string, std::vector<LogInfo>> logs;

    std::vector<std::string> activeSubsystems; //The subsystems currently allowed to log info
    public:
        static Debugger& get(){
            static Debugger instance;
            return instance;
        }

        void initDebugSystem(const char* subsystem_name);

        void log(int logSeverity, const char* logMessage, const char* subsystem, 
                 const char* FILE, int LINE, const char* FUNC);

        void displayLog(int logSeverity, const char* logMessage, const char* subsystem,
                        const char* FILE, int LINE, const char* FUNC);
    };

#endif