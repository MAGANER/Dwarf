#include"App.h"
using namespace Dwarf;

App::App()
{	
	SetConsoleOutputCP(1251);
	system("title Dwarf");
	clear();
	
	load_config();
	
	smart_sort   = is_smart_sort_enabled();
	searching_paths = get_searching_paths();
	size = get_terminal_size();
	max_path_char_number = get_max_path_char_number();
	
	get_music_files();
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
	draw_string("Dwarf Audio Player",red_label,pos);	
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
		draw_string(text[i],standart,pos);
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
	draw_string("enter path, where can be found music:",standart,pos);
	string path = get_path();
	
	bool exist = fs::exists(path);
	bool already_exist = find(searching_paths.begin(),searching_paths.end(),path) != searching_paths.end();
	if(!already_exist && exist)
	{
		add_new_search_paths(path);
	}
	current_mode = previos_mode;
	previos_mode = working_modes::Add;
	run_mode();
}
string App::get_path()
{
	string path;
	const int size = sizeof(istream)/sizeof(char);
	char* buffer = new char[size];
	cin.getline(buffer,size);
	path = buffer;
	delete buffer;
	return path;
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
		case 27:exit(0);break;
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
	};
	
	int min = 1;
	int max = 4;
	int current = 1;
	while(true)
	{
		for(size_t i = 0;i<text.size();++i)
		{
			if(i == 1)pos.X = 25;
			if(current != i)draw_string(text[i],standart,pos);
			else draw_string(text[i],green_label,pos);
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
			if(current == Groups) run_list_groups();
			if(current == Genres) run_list_genres(music);
			if(current == Artists)run_list_artists(music,L"");
			if(current == Albums) run_list_albums(music,L"",L"");
			break;
		}
		pos.Y = 5;
		pos.X = 20;
	}
	current_mode = previos_mode;
	previos_mode = working_modes::List;
	run_mode();
}
void App::run_list_groups()
{
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = groups.size()< visible_range?groups.size():visible_range;
	COORD elem_pos{2,1};
	while(true)
	{
		if(groups.empty())
		{
			COORD pos{10,5};
			draw_string("you have no any group!",standart,pos);
		}
		else
		{
			for(int i = start_counter;i<max_counter;++i)
			{
				string text = to_string(i)+":"+groups[i];
				if(current_elem == i)
					draw_string(text,green_label,elem_pos);
				else draw_string(text,standart,elem_pos);
				++elem_pos.Y;
			}
		}
		
		int input = _getch();
		if(input == ESCAPE) break;
		if(input == MINUS && current_elem != 0) --current_elem;
		if(input == PLUS  && current_elem < max_counter-1)++current_elem;
		if(input == DOWN  && max_counter != groups.size())
		{
			++max_counter;
			++start_counter;
		}
		if(input == UP    && start_counter != 0)
		{
			--max_counter;
			--start_counter;
		}

		elem_pos.Y = 1;
	}
}		   