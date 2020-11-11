#ifndef APP_H
#define APP_H
#include"Console.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>

using namespace Bubblegum;
using namespace _VirtualMachine;

namespace Dwarf
{
class App: public Console
{
private:
	Color standart, green_label, red_label;
	VirtualMachine machine;
	
	Pos size;
	int max_path_char_number;
public:
	App();
	~App();
	void run();
private:
	void load_config();
	Pos get_terminal_size();
	int get_max_path_char_number();
	
	void set_terminal_size();

};
};
#endif //APP_H