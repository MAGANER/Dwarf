#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H
#include<windows.h>
#include"VirtualMachine/VirtualMachine.h"
#include"Pos.h"

namespace bg = Bubblegum::_VirtualMachine;
namespace Dwarf
{
class ConfigLoader
{
private:
	bg::VirtualMachine machine;
protected:
	Pos size;
	int max_path_char_number;
public:
	ConfigLoader();
	~ConfigLoader();
	
	void load_config();
	Pos get_terminal_size();
	int get_max_path_char_number();
	bool is_smart_sort_enabled();
	void set_terminal_size();
	
	void write_error_log_file(const std::string& error);
	void process_error(const std::string& error);
};
};
#endif //CONFIG_LOADER_H