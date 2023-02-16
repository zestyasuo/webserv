#include "../inc/Logger.hpp"

const std::string Logger::log_level_names[DEBUG + 1] = {GRN "[INFO]", YLW "[WARNING]", RED "[ERROR]", CYN "[FATAL]", MAG "[DEBUG]"};

Logger::Logger() : is_debug(false), name("InPlace logger"), is_active(true)
{
}

Logger::Logger(bool debug, std::string const &n) : is_debug(debug), name(n), is_active(true)
{
}

Logger::Logger(bool debug) : is_debug(debug), name("debug logger"), is_active(true)
{
}

Logger::~Logger()
{
}

void Logger::log(std::string msg, int log_level) const
{
	if (!is_active)
		return ;
	if (log_level < INFO || log_level > DEBUG)
		return;
	if (log_level == DEBUG && !is_debug)
		return;

	std::cout << log_level_names[log_level] << get_timestamp() << " ${" + name << "}" <<  RST << " " << msg << "\n";
}

void	Logger::deactivate()
{
	if (is_active)
		is_active = false;
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