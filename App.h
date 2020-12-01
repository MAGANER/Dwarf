#ifndef APP_H
#define APP_H

#include<filesystem>
#include<vector>
#include<fstream>

#include"SearchMenu.h"
#include"VirtualMachine/VirtualMachine.h"

#include "fileref.h"
#include "tag.h"


namespace fs = std::filesystem;
using namespace Bubblegum;
using namespace _VirtualMachine;
using namespace irrklang;

namespace Dwarf
{	
class App: public SearchMenu
{
private:
	VirtualMachine machine;
	
	Pos size;
	int max_path_char_number;
	
	svector searching_paths;
	vector<MusicData*> music;
	wsvector raw_music;
	
	svector groups;
	
	wsvector able_extensions;
			
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
		Artists,
		Genres,
	};
	enum music_data_type
	{
		Album,
		Title,
		Artist,
		Genre
		
	};
	bool smart_sort;
public:
	App();
	~App();
	void run();
private:

	//config stuff
	void load_config();
	Pos get_terminal_size();
	int get_max_path_char_number();
	bool is_smart_sort_enabled();
	
	void set_terminal_size();
	//.
	
	//path stuff
	svector get_searching_paths();
	void get_music_files();
	void apply_smart_sort();
	bool is_extension_able(const wstring& extension);
	wstring fix_path_slash(const wstring& path);
	//.
	
	//error stuff 
	void write_error_log_file(const string& error);
	void process_error(const string& error);
	//.
	
	//main menu stuff
	void draw_label();
	void draw_help();
	void run_main_menu();
	//.
	
	//adding menu stuff
	void run_add_menu();
	string get_path();
	void add_new_search_paths(const string& value);
	//.
	
	//listing menu stuff	
	void run_base_menu_list();
	void run_list_groups();
	//.
	
	//
	wstring get_path_to_title(const wstring& artist, 
							   const wstring& album,
							   const wstring& title);
	//.
	
	//common stuff
	void run_mode();
	void set_mode(int input_code);
	wstring clear_string(const wstring& str);
	//.
	
	
	//took it from https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Расстояние_Левенштейна#C++
	template<typename T>
	typename T::size_type LevenshteinDistance(const T &source,
											  const T &target);
	
};
};
#endif //APP_H