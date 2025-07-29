#pragma once

#include <string>

void runAllUnitTests(const std::string& testName, bool (*testFunction)(), int& totalTests, int& passedTests);

bool testLoggerInitialization();
bool testSetLogLevel();
bool testLogMessage();
bool testLogLevelFilter();
bool testErrorHandling();