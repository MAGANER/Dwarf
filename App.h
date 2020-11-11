#ifndef APP_H
#define APP_H
#include"Console.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>
#include<filesystem>
#include<vector>
#include<fstream>

namespace fs = std::filesystem;
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
	
	svector searching_paths;
	vector<fs::path> music_folders;
public:
	App();
	~App();
	void run();
private:

	//config staff
	void load_config();
	Pos get_terminal_size();
	int get_max_path_char_number();
	
	void set_terminal_size();
	//.
	
	//path staff
	svector get_searching_paths();
	vector<fs::path> get_music_folders();
	//.
	
	//error staff 
	void write_error_log_file(const string& error);
	void process_error(const string& error);
	//.
	
};
};
#endif //APP_H