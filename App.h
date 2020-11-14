#ifndef APP_H
#define APP_H
#include"Console.h"
#include"TagReader.h"
#include"MusicData.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>
#include<filesystem>
#include<vector>
#include<fstream>

namespace fs = std::filesystem;
using namespace Bubblegum;
using namespace _VirtualMachine;
using namespace TAG_READER;

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
	vector<MusicData*> music;
	svector raw_music;
	
	svector able_extensions;
		
	enum working_modes
	{
		MainMenu,
		Search,
		Add,
		List,
		Play
	};
	int current_mode;
	int previos_mode;
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
	void get_music_files();
	bool is_extension_able(const string& extension);
	//.
	
	//error staff 
	void write_error_log_file(const string& error);
	void process_error(const string& error);
	//.
	
	//main menu staff
	void draw_label();
	void draw_help();
	void run_main_menu();
	//.
	
	//search menu staff
	void run_search_menu();
	string get_path();
	void add_new_search_paths(const string& value);
	//.
	
	//list menu staff
	
	//.
	
	//common staff
	void run_mode();
	void set_mode(int input_code);
	//.
	
};
};
#endif //APP_H