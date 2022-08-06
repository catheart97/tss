#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Logger
{
    // Data
private:
    std::vector<std::string> _log;

    // Constructors
public:
    Logger() {}
    Logger(Logger & logger) : _log(logger._log) {}
    Logger(Logger && logger) : _log(std::move(logger._log)) {}
    ~Logger() {}

    // Methods
public:
    void log(const std::string & msg) noexcept { _log.push_back(msg); }
};


#include <mutex>

const size_t BUFSIZE = 30;


extern Logger logger;
std::mutex log_guard;

class NetworkHandler
{
    void process(SOCKET socket) override
    {
        while (1)
        {
            char buffer[BUFSIZE];
            size_t result = recv(socket, buffer, BUFSIZE, 0);
            if (result == -1 && errno == EWOULDBLOCK)
            {
                std::lock_guard<std::mutex> guard(log_guard);
                logger.log(sprintf("recv failed, errno = %d\n", errno));
            }
            else
                processBuffer(buffer);
        }
    }
};


