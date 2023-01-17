#include "log.h"
#include <iostream>

void log_info(std::string msg) {
	std::cout << "[Thor] INFO >> " << msg << std::endl;
}
void log_warn(std::string msg) {
	std::cout << "[Thor] WARN >> " << msg << std::endl;

}
void log_error(std::string msg) {
	std::cout << "[Thor] ERROR >> " << msg << std::endl;

}