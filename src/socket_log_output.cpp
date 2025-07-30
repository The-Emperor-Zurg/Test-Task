#include "socket_log_output.h"

#include "logger.h"

#ifdef WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <cstring>
#endif


namespace MyLogger {
    SocketLogOutput::SocketLogOutput(const std::string& host, int port)
        : host_(host), port_(port), socket_(-1), connected_(false) {
#ifdef WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    }

    SocketLogOutput::~SocketLogOutput() {
        close();
    }

    LogResult SocketLogOutput::init() {
        socket_ = socket(AF_INET, SOCK_STREAM, 0);

        if (socket_ < 0) {
            return LogResult::SOCKET_CONNECTION_ERROR;
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port_);

        if (inet_pton(AF_INET, host_.c_str(), &serverAddr.sin_addr) <= 0) {
            hostent* he = gethostbyname(host_.c_str());
            if (!he) {
#ifdef WIN32
                closesocket(socket_);
#else
                ::close(socket_);
#endif
                return LogResult::SOCKET_CONNECTION_ERROR;
            }

            memcpy(&serverAddr.sin_addr, he->h_addr_list[0], he->h_length);
        }

        if (connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
#ifdef WIN32
            closesocket(socket_);
#else
            ::close(socket_);
#endif
            return LogResult::SOCKET_CONNECTION_ERROR;
        }

        connected_ = true;
        return LogResult::SUCCESS;
    }

    LogResult SocketLogOutput::write(const std::string& message) {
        if (!connected_) {
            return LogResult::SOCKET_CONNECTION_ERROR;
        }

        std::string fullMessage = message + "\n";

#ifdef WIN32
        int sent = send(socket_, fullMessage.c_str(), static_cast<int>(fullMessage.length()), 0);
#else
        ssize_t sent = send(socket_, fullMessage.c_str(), fullMessage.length(), MSG_NOSIGNAL);
#endif

        if (sent < 0 || static_cast<size_t>(sent) != fullMessage.length()) {
            return LogResult::SOCKET_SEND_ERROR;
        }

        return LogResult::SUCCESS;
    }

    void SocketLogOutput::close() {
        if (connected_) {
#ifdef WIN32
            closesocket(socket_);
            WSACleanup();
#else
            ::close(socket_);
#endif
            connected_ = false;
        }
    }
}