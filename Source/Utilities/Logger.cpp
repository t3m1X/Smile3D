#include "Logger.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>

namespace logger {

std::deque<std::string> logs;
int max_logs = 1000;

void InitLogger(const int log_size) {
	max_logs = log_size;
}

void ConsoleLog (const char file[], int line, const char* format, ...) {
    const int buf_size = 4096;
    static char tmp_string[buf_size];
	static char tmp_string2[buf_size];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, buf_size, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, buf_size, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugStringA(tmp_string2);

    // Add it to the logs
	sprintf_s(tmp_string2, buf_size, "\n%s", tmp_string);
	if (logs.size() >= max_logs)
		logs.pop_front();

	std::string to_add = tmp_string2;
	logs.push_back(to_add);
}

void SystemLog (const char file[], int line, const char* format, ...) {
    const int buf_size = 4096;
	static char tmp_string[buf_size];
	static char tmp_string2[buf_size];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, buf_size, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, buf_size, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugStringA(tmp_string2);

	std::cout << tmp_string2 << std::endl;
}

void ChangeLogSize(int new_log_size) {
    max_logs = new_log_size;
    while (logs.size() > max_logs)
        logs.pop_front();
}


} //namespace logger