#pragma once

#include <string>


void runTest(const std::string& testName, bool (*testFunction)(), int& totalTests, int& passedTests);
int runAllTests();

bool testLoggerInitialization();
bool testSetLogLevel();
bool testLogMessage();
bool testLogLevelFilter();
bool testErrorHandling();

bool testSocketLoggerConnectionError();
bool testSocketLoggerInitialization();
bool testSocketLoggerInvalidHost();
bool testSocketLoggerInvalidPort();
bool testSocketLoggerUninitialized();
bool testSocketLoggerSetLevel();
bool testSetLogLevelNotInitialized();