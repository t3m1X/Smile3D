#ifndef SMILE3D_UTILITIES_LOGGER_H
#define SMILE3D_UTILITIES_LOGGER_H

#include <string>
#include <deque>

#define CONSOLE_LOG(format, ...) logger::ConsoleLog(__FILE__, __LINE__, format, __VA_ARGS__)
#define SYSTEM_LOG(format, ...) logger::SystemLog(__FILE__, __LINE__, format, __VA_ARGS__)
#define CONSOLE_AND_SYSTEM_LOG(format, ...) logger::SystemLog(__FILE__, __LINE__, format, __VA_ARGS__); logger::ConsoleLog(__FILE__, __LINE__, format, __VA_ARGS__)

namespace logger {

void InitLogger(const int log_size);
void ConsoleLog (const char file[], int line, const char* format, ...);
void SystemLog (const char file[], int line, const char* format, ...);

void ChangeLogSize (int new_log_size);



} //namespace logger


#endif //SMILE3D_UTILITIES_LOGGER_H