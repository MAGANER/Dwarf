#ifndef APP_H
#define APP_H
#include"Console.h"
#include"VirtualMachine/VirtualMachine.h"
#include<cstdlib>
#include<filesystem>
#include<vector>
#include<fstream>
#include<clocale>
#include<algorithm>
#include<cwchar>
#include "fileref.h"
#include "tag.h"
#include "MusicData.h"

namespace fs = std::filesystem;
using namespace Bubblegum;
using namespace _VirtualMachine;

namespace Dwarf
{
typedef vector<wstring> wsvector;
class App: public Console
{
private:
	Color standart, green_label, red_label, empty;
	VirtualMachine machine;
	
	Pos size;
	int max_path_char_number;
	
	svector searching_paths;
	vector<MusicData*> music;
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

	const int visible_range = 28;
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
	bool is_extension_able(const wstring& extension);
	wstring fix_path_slash(const wstring& path);
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
	void run_common_list(const wsvector& data,
						 int& choosen_option,
						 const string& no_elems_text,
						 int& current_elem,
						 int& start_counter,
						 int& max_counter,
						 const string& title);
						 
	void run_common_choosing_list(const wsvector& text,
								  int& min,
								  int& max,
								  int& current,
								  int& choosen_option);
	
	void run_base_menu_list();
	void run_list_menu();
	void run_list_groups();
	
	void run_list_genres();
	void choose_what_to_run_from_genre_menu(const wstring& genre_name);
	void choose_what_to_run_from_artist_menu(const wstring& artist, const wstring& genre);
	
	void run_list_artists(const wstring& genre);
	void run_list_albums(const wstring& genre,const wstring& artist);
	void run_list_titles(const wstring& genre,const wstring& artist, const wstring& album);
	//.
	
	//
	wsvector get_genre_data_from_music();
	wsvector get_artists_data_from_music(const wstring& genre);
	wsvector get_album_data_from_music(const wstring& artist, const wstring& genre);
	wsvector get_title_data_from_music(const wstring& artist, const wstring& genre);
	wsvector get_title_data_from_music(const wstring& artist, const wstring& genre, const wstring& album);
	//.
	
	//common staff
	void run_mode();
	void set_mode(int input_code);
	//.
	
	//converting
	bool can_wstring_be_converted_to_std(const wstring& str);
	string convert_wstring_to_std(const wstring& str);
	//.
	
};
};
#endif //APP_H