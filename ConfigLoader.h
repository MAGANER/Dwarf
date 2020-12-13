#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H
#include<windows.h>
#include"VirtualMachine/VirtualMachine.h"
#include"Pos.h"
#include"ErrorProcessor.h"

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
	void load_config(const string& custom_path);
	Pos get_terminal_size();
	int get_max_path_char_number();
	bool is_smart_sort_enabled();
	void set_terminal_size();
};
};
#endif //CONFIG_LOADER_H