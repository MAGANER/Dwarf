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
#include"GroupManager.h"

namespace Dwarf
{	
class App: public SearchMenu,
		   public ConfigLoader,
		   public RawMusicMenu
{
private:	
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
	string search_path;
public:
	App();
	~App();
	void run();
	void set_custom_song_to_play(const string& path);
private:		
	//main menu stuff
	void draw_label();
	void draw_help();
	void run_main_menu();
	//.
	
	//adding menu stuff
	void run_add_menu();
	//.
	
	//listing menu stuff	
	void run_base_menu_list();
	//.
		
	//common stuff
	void run_mode();
	void set_mode(int input_code);
	//.
};
};
#endif //APP_H