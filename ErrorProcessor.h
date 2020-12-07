#ifndef ERROR_PROCESSOR_H
#define ERROR_PROCESSOR_H
#include<string>
#include<fstream>
#include<cstdlib>
#include<iostream>

namespace
{
	void write_error_log_file(const std::string& error)
	{
		std::ofstream error_log("error.txt");
		error_log<<error;
		error_log.close();
	}
	void process_error(const std::string& error)
	{
		std::cout<<error<<std::endl;
		write_error_log_file(error);
		exit(0);
	}

};
#endif //ERROR_PROCESSOR_H