#include "../inc/Logger.hpp"

const std::string Logger::log_level_names[DEBUG + 1] = {
	GRN "[INFO]" RST,
	YLW "[WARNING]" RST,
	RED "[ERROR]" RST,
	CYN "[FATAL]" RST,
	MAG "[DEBUG]" RST
};

Logger::Logger()
{
}

Logger::~Logger(){}

void	Logger::log(std::string msg, int log_level) const
{
	if (log_level < INFO || log_level > DEBUG) return ;

	std::cout << log_level_names[log_level] << "\t" << msg << "\n";
}
