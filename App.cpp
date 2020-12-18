#include"App.h"
using namespace Dwarf;

App::App()
{	
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	system("title Dwarf");
	clear();
	
	if(fs::exists("data/config.bg"))
	{
		load_config();
		search_path = "data/paths.txt";
	}
	else if(fs::exists("C:/dwarf_data/config.bg"))
	{
	        search_path = "C:/dwarf_data/paths.txt";
		load_config("C:/dwarf_data/config.bg");
	}
	else
	{
  		cout<<"config file could not be found!"<<endl;
		cout<<"note:you can copy data folder to C:/, call it dwarf_data and it will be standart"<<endl;
		getch();
		exit(0);

	}
	
	smart_sort   = is_smart_sort_enabled();
	searching_paths = get_searching_paths(search_path);
	size = get_terminal_size();
	max_path_char_number = get_max_path_char_number();
	
	get_music_files(smart_sort);
	set_terminal_size();
	clear();

	current_mode = working_modes::MainMenu;
}
App::~App()
{
}

void App::run()
{
	while(true)
	{
		run_mode();
	
		int input = _getch();
		set_mode(input);
		system("cls");
	}
}
void App::draw_label()
{
	//draw label of main menu
	
	COORD pos;
	pos.X = 28;
	pos.Y = 0;
	Console::draw_string("Dwarf Audio Player",CommonMenu::red_label,pos);	
}
void App::draw_help()
{
	//draw list of able functions at main menu
	
	COORD pos;
	pos.X = 20;
	pos.Y = 5;
	svector text = 
	{
		"here the list of able commands:",
		"1 -> to go the main menu",
		"2 -> to go the default music list",
		"3 -> to search a track album group",
		"4 -> to add new path, where music is",
		"Exit -> to quit the application"
	};
	
	for(size_t i = 0;i<text.size();++i)
	{
		if(i == 1)pos.X = 25;
		Console::draw_string(text[i],CommonMenu::standart,pos);
		++pos.Y;
	}
}
void App::run_main_menu()
{
	draw_label();
	draw_help();
}

void App::run_add_menu()
{
	COORD pos;
	pos.X = 1;
	pos.Y = 10;
	Console::draw_string("enter path, where can be found music:",CommonMenu::standart,pos);
	string path = get_input();
	
	bool exist = fs::exists(path);
	bool already_exist = find(searching_paths.begin(),searching_paths.end(),path) != searching_paths.end();
	if(!already_exist && exist)
	{
		add_new_search_paths(path,search_path);
	}
	current_mode = previos_mode;
	previos_mode = working_modes::Add;
	run_mode();
}
void App::run_mode()
{
	system("cls");
	switch(current_mode)
	{
		case working_modes::MainMenu: run_main_menu();     break;
		case working_modes::Add:      run_add_menu();      break;
		case working_modes::Search:   
		{
			run_search(music);
			previos_mode = working_modes::Search;
			current_mode = working_modes::MainMenu;
			run_mode();
		};
		break;
		case working_modes::List:	  run_base_menu_list();break;
	}
}
void App::set_mode(int input_code)
{
	previos_mode = current_mode;
	switch(input_code)
	{
		case 27:
		{
			save_groups();
			exit(0);
		}
		break;
		case 49: current_mode = working_modes::MainMenu;
		break;
		case 50: current_mode = working_modes::List;
		break;
		case 51: current_mode = working_modes::Search;
		break;
		case 52: current_mode = working_modes::Add;
		break;
		default:break;
	};
}


void App::run_base_menu_list()
{
	COORD pos{20,5};
	svector text = 
	{
		"Choose option to list:",
		"Groups",
		"Albums",
		"Artists",
		"Genres",
		"Raw music"
	};
	
	int min = 1;
	int max = 5;
	int current = 1;
	while(true)
	{
		for(size_t i = 0;i<text.size();++i)
		{
			if(i == 1)pos.X = 25;
			if(current != i)Console::draw_string(text[i],CommonMenu::standart,pos);
			else Console::draw_string(text[i],CommonMenu::green_label,pos);
			++pos.Y;
		}
		
		int input = _getch();
		if(input == UP &&  current != min)
		{
			--current;
		}
		if(input == DOWN && current != max)
		{ 
			++current;
		}
		if(input == ESCAPE) break;
		if(input == ENTER)
		{
			system("cls");
			if(current == Groups) run_group_menu();
			if(current == Genres) run_list_genres(music);
			if(current == Artists)run_list_artists(music,L"");
			if(current == Albums) run_list_albums(music,L"",L"");
			if(current == RawMusic)show_raw_music_list();
			break;
		}
		pos.Y = 5;
		pos.X = 20;
	}
	current_mode = previos_mode;
	previos_mode = working_modes::List;
	run_mode();
}	   
void App::set_custom_song_to_play(const string& path)
{
	if(!path.empty())
	{
		string output_path;
		auto slash_pos = path.find_last_of("/");
		if(slash_pos != string::npos)
		{
			output_path = get_substr(path,slash_pos+1,path.size()); 
		}	else output_path = path;
		
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		wstring real_path = converter.from_bytes(path);
		auto pos = real_path.find_last_of(L".");
		wstring extension = get_substr(real_path,pos+1,real_path.size());
		if(!is_extension_able(extension))
		{
			wcout<<extension<<L" is not supported yet!Sorry :("<<endl;
			getch();
		}else play_raw_music(converter.from_bytes(output_path), real_path);
	}
}
