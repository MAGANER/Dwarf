/*
	Entry point for console edition of application.
*/
#ifndef APP_H
#define APP_H

#include<vector>
#include<fstream>

#include"ConfigLoader.h"
#include"SearchMenu.h"

#include "fileref.h"
#include "tag.h"

namespace Dwarf
{	
class App: public SearchMenu,
		   public ConfigLoader
{
private:
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
	//path stuff
	svector get_searching_paths();
	void get_music_files();
	void apply_smart_sort();
	bool is_extension_able(const wstring& extension);
	wstring fix_path_slash(const wstring& path);
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
		
	//common stuff
	void run_mode();
	void set_mode(int input_code);
	//.
};
};
#endif //APP_H