#include "../inc/Logger.hpp"

const std::string Logger::log_level_names[DEBUG + 1] = {GRN "[INFO]", YLW "[WARNING]", RED "[ERROR]", CYN "[FATAL]", MAG "[DEBUG]"};

Logger::Logger() : is_debug(false)
{
}

Logger::Logger(bool debug) : is_debug(debug)
{
}

Logger::~Logger()
{
}

void Logger::log(std::string msg, int log_level) const
{
	if (log_level < INFO || log_level > DEBUG)
		return;
	if (log_level == DEBUG && !is_debug)
		return;

	std::cout << log_level_names[log_level] << get_timestamp() << RST << "\t" << msg << "\n";
}

std::string Logger::make_logstr(std::string const &msg) const
{
	std::string result;

	result += get_timestamp();
	result += " " + msg;
	return result;
}

std::string Logger::get_timestamp(void) const
{
	time_t		now = 0;
	tm		   *ltm;
	char		buf[1024];
	std::string result;

	now = time(0);
	if (now)
		ltm = localtime(&now);
	strftime(buf, 1024, "%d/%m/%y %T", ltm);
	result = buf;
	result.insert(result.begin(), '[');
	result.insert(result.end(), ']');
	return result;
}