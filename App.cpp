#include"App.h"
using namespace Dwarf;

App::App()
{
	system("title Dwarf");
	system("cls");
	
	standart    = Color(ColorCode::Black,ColorCode::White);
	green_label = Color(ColorCode::Black,ColorCode::Green);
	red_label   = Color(ColorCode::Black,ColorCode::Red);
	
	able_extensions = {"mp3", "wav"};
	
	load_config();
	searching_paths = get_searching_paths();
	music_files   = get_music_files();
	
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
void App::run_search_menu()
{
	COORD pos;
	pos.X = 1;
	pos.Y = 10;
	draw_string("enter path, where can be found music:",standart,pos);
	string path = get_path();
	//TODO:checks path exists
	
	bool already_exist = find(searching_paths.begin(),searching_paths.end(),path) != searching_paths.end();
	if(!already_exist)
	{
		add_new_search_paths(path);
	}
	current_mode = previos_mode;
	previos_mode = working_modes::Add;
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
	switch(current_mode)
	{
		case working_modes::MainMenu: run_main_menu(); break;
		case working_modes::Add: run_search_menu();break;
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
	MoveWindow(hwnd,100,100,size.x,size.y,FALSE);
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
svector App::get_music_files()
{
	svector files;
	for(auto& spath: searching_paths)
	{
		for(auto& data: fs::recursive_directory_iterator(spath))
		{
			string str = data.path().u8string();
			int point = str.find(".");
			if(point != string::npos)
			{
				string extension = get_substr(str,point+1,str.size());
				if(is_extension_able(extension)) files.push_back(str);
			}
		}
	}
	return files;
}
bool App::is_extension_able(const string& extension)
{
	return find(able_extensions.begin(),able_extensions.end(),extension) != able_extensions.end();
}