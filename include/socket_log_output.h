#pragma once

#include "ilog_output.h"


namespace MyLogger {
    class MYLOGGER_API SocketLogOutput : public ILogOutput {
    public:
        SocketLogOutput(const std::string& host, int port);
        ~SocketLogOutput();
        LogResult init() override;
        LogResult write(const std::string& message) override;
        void close() override;

    private:
        std::string host_;
        int port_;
        int socket_;
        bool connected_;
    };

}