#pragma once
#include <string>

void log_info(std::string);
void log_warn(std::string);
void log_error(std::string);


#define LOGE(...) log_error(__VA_ARGS__)
#define LOGI(...) log_info(__VA_ARGS__)
#define LOGW(...) log_warn(__VA_ARGS__)
