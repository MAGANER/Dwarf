#ifndef APP_H
#define APP_H
#include"CommonMenu.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>
#include<filesystem>
#include<vector>
#include<fstream>
#include<clocale>
#include<algorithm>
#include<cwchar>
#include<utility>
#include<functional>
#include "fileref.h"
#include "tag.h"
#include "MusicData.h"
#include "PlayTime.h"
#include "irrKlang.h"
#pragma execution_character_set( "utf-8" )

namespace fs = std::filesystem;
using namespace Bubblegum;
using namespace _VirtualMachine;
using namespace irrklang;

namespace Dwarf
{
typedef vector<wstring> wsvector;
typedef pair<wstring,wstring> wspair;
typedef vector<wspair> wspvector;

class App: public CommonMenu
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
	
	ISoundEngine* engine;
	svector loaded_music;
	
	wspvector current_play_list;
	int current_play_list_pos;
	irrklang::ik_f32 volume_cash;
	
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

	const int visible_range = 28;
	bool play_next = false;
	
	bool smart_search, smart_sort;
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
	bool is_smart_search_enabled();
	
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
	
	//searching menu stuff
	void run_search_menu();
	//.
	
	//listing menu stuff	
	void run_base_menu_list();
	void run_list_menu();
	void run_list_groups();
	
	void run_list_genres();
	void choose_what_to_run_from_genre_menu(const wstring& genre_name);
	void choose_what_to_run_from_artist_menu(const wstring& artist, const wstring& genre);
	
	void run_list_artists(const wstring& genre);
	void run_list_albums(const wstring& genre,const wstring& artist);
	void run_list_titles(const wstring& genre,const wstring& artist, const wstring& album);
	void run_playing_composition(bool& play_next,
								 const wstring& artist, 
								 const wstring& album,
								 const wspair& title);
	//.
	
	//
	wstring get_path_to_title(const wstring& artist, 
							   const wstring& album,
							   const wstring& title);
	wstring get_genre_of_title(const wstring& artist, 
							   const wstring& album,
							   const wstring& title);
	wsvector get_genre_data_from_music();
	wsvector get_artists_data_from_music(const wstring& genre);
	wsvector get_album_data_from_music(const wstring& artist, const wstring& genre);
	wspvector get_title_data_from_music(const wstring& artist, const wstring& genre);
	wspvector get_title_data_from_music(const wstring& album);
	//.
	
	//common stuff
	void run_mode();
	void set_mode(int input_code);
	PlayTime* compute_time(irrklang::ik_u32 time);
	wstring clear_string(const wstring& str);
	//.
	
};
};
#endif //APP_H