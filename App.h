#ifndef APP_H
#define APP_H
#include"Console.h"
#include"TagReader.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>
#include<filesystem>
#include<vector>
#include<fstream>
#include<clocale>
#include<algorithm>

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
	vector<TagData*> music;
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
	
	enum listing_options
	{
		Groups = 1,
		Albums,
		Genres,
	};
	int current_list_option;
	
	enum key_codes
	{
		UP   = 72,
		DOWN = 80,
		ESCAPE = 27,
		ENTER  = 13,
	};
	
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
	bool is_file_readeful(TagData tags);
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
	
	//adding menu staff
	void run_add_menu();
	string get_path();
	void add_new_search_paths(const string& value);
	//.
	
	//searching menu staff
	void run_search_menu();
	//.
	
	//listing menu staff
	void run_base_menu_list();
	void run_list_menu();
	string clear_spaces_at_end(const string& str);
	//.
	
	//common staff
	void run_mode();
	void set_mode(int input_code);
	//.
	
};
};
#endif //APP_H