#include"App.h"
using namespace Dwarf;

App::App()
{
	system("title Dwarf");
	system("cls");
	
	standart    = Color(ColorCode::Black,ColorCode::White);
	green_label = Color(ColorCode::Black,ColorCode::Green);
	red_label   = Color(ColorCode::Black,ColorCode::Red);
	
	able_extensions = {"mp3"};
	
	load_config();
	searching_paths = get_searching_paths();
	get_music_files();
	size = get_terminal_size();
	max_path_char_number = get_max_path_char_number();
	set_terminal_size();
	system("cls");

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
	COORD pos;
	pos.X = 28;
	pos.Y = 0;
	draw_string("Dwarf Audio Player",red_label,pos);	
}
void App::draw_help()
{
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
		case working_modes::MainMenu: run_main_menu();   break;
		case working_modes::Add:      run_add_menu();    break;
		case working_modes::Search:   run_search_menu(); break;
		case working_modes::List:	  run_list_menu();   break;
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
	};
}
void App::load_config()
{
	svector script = machine.load_script("data/config.bg");
	machine.check_braces_equality(script);
	machine.run(script,"main");
}
Pos App::get_terminal_size()
{
	Module* main = machine.get_module("main");
	string swidth = main->memory.get_value("width",TypeChecker::Integer);
	string sheight= main->memory.get_value("height",TypeChecker::Integer);
	return Pos(atoi(swidth.c_str()),atoi(sheight.c_str()));
}
int App::get_max_path_char_number()
{
	Module* main = machine.get_module("main");
	string size = main->memory.get_value("path_char_max",TypeChecker::Integer);
	return atoi(size.c_str());
}
void App::set_terminal_size()
{
	HANDLE terminal = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = {75,75};
	SetConsoleScreenBufferSize(terminal,bufferSize);
	HWND hwnd = FindWindow(NULL,"Dwarf");
	const SMALL_RECT win_size ={100,100,(short)size.x,(short)size.y}; 
	SetConsoleWindowInfo(terminal,TRUE, &win_size);
}

void App::process_error(const string& error)
{
	cout<<error<<endl;
	write_error_log_file(error);
	exit(0);
}
void App::write_error_log_file(const string& error)
{
	ofstream error_log("error.txt");
	error_log<<error;
	error_log.close();
}
svector App::get_searching_paths()
{
	ifstream file;
	file.open("data/paths.txt");
	if(!file)
	{
		process_error("can not open data/paths.txt!");
	}
	
	svector paths;
	while(file)
	{
		string line;
		file>>line;
		if(!line.empty())paths.push_back(line);
	}
	file.close();
	return paths;
}
void App::add_new_search_paths(const string& value)
{
	svector existing_paths = get_searching_paths();
	bool exist = find(existing_paths.begin(),existing_paths.end(),value) != existing_paths.end();
	if(!exist)
	{
		ofstream file("data/paths.txt",ios::out | ios::app);
		if(file)
		{
			file<<endl;
			file<<value;
			file.close();
		}
		else
		{
			process_error("cannot open data/paths.txt!");
		}
	}
}
void App::get_music_files()
{
	for(auto& spath: searching_paths)
	{
		for(auto& data: fs::recursive_directory_iterator(spath))
		{
			string str = data.path().u8string();
			int point = str.find(".");
			if(point != string::npos)
			{
				string extension = get_substr(str,point+1,str.size());
				if(is_extension_able(extension))
				{
					TagReader tag(str);
					TagData tags = tag.get_tag_info();
					if(tag.is_ok() && is_file_readeful(tags))
					{
						TagData* data = new TagData(tags);
						music.push_back(data);
					}
					else raw_music.push_back(str);
				}
			}
		}
	}
}
bool App::is_extension_able(const string& extension)
{
	return find(able_extensions.begin(),able_extensions.end(),extension) != able_extensions.end();
}
void App::run_search_menu()
{
	COORD pos = {5,10};
	draw_string("look for:",standart, pos);
	string input = get_path();
}
void App::run_list_menu()
{
	run_base_menu_list();

}
string App::clear_spaces_at_end(const string& str)
{
	string cleared;
	bool start_is_found = false;
	for(int end = str.size();end>-1;--end)
	{
		auto curr = str[end];
		if(!isspace(curr) && !start_is_found) start_is_found = true;
		if(start_is_found)cleared.push_back(curr);
	}
	
	reverse(cleared.begin(),cleared.end());
	return cleared;
}
bool App::is_file_readeful(TagData tags)
{
	//for example uuuuuuuuuuuu is unreadeable
	string title = tags.title;
	char first = title[0];
	int number = count(title.begin(),title.end(),first);
	return !(number == title.size());
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
	};
	
	int min = 1;
	int max = 3;
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