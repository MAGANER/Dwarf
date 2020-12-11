/*
	Entry point for console edition of application.
*/
#ifndef APP_H
#define APP_H

#include<vector>
#include<fstream>

#include"ConfigLoader.h"
#include"SearchMenu.h"
#include"RawMusicMenu.h"

namespace Dwarf
{	
class App: public SearchMenu,
		   public ConfigLoader,
		   public RawMusicMenu
{
private:	
	svector groups;
				
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
		RawMusic,
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
	//main menu stuff
	void draw_label();
	void draw_help();
	void run_main_menu();
	//.
	
	//adding menu stuff
	void run_add_menu();
	string get_path();
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