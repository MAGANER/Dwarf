#include"App.h"
using namespace Dwarf;

App::App()
{	
	system("title Dwarf");
	system("cls");
	
	standart    = Color(ColorCode::Black,ColorCode::White);
	green_label = Color(ColorCode::Black,ColorCode::Green);
	red_label   = Color(ColorCode::Black,ColorCode::Red);
	empty       = Color(ColorCode::Black,ColorCode::Black);
	
	able_extensions = {L"mp3"};
	
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
	MoveWindow(hwnd,100,100,size.x,size.y,false);
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
			wstring str = fix_path_slash(data.path().wstring());
			int point = str.find(L".");
			if(point != wstring::npos)
			{
				wstring extension = get_substr(str,point+1,str.size());
				if(is_extension_able(extension))
				{
					TagLib::FileRef f(str.c_str());
					TagLib::Tag *tag = f.tag();
					wstring artist = tag->artist().toWString();
					wstring title  = tag->title().toWString();
					wstring album  = tag->album().toWString();
					wstring genre  = tag->genre().toWString();
					unsigned int year= tag->year();
					MusicData* data = new MusicData(artist,title,album,genre,year);
					music.push_back(data);
				}
			}
		}
	}
}
bool App::is_extension_able(const wstring& extension)
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
			if(current == Genres) run_list_genres();
			if(current == Artists)run_list_artists(L"");
			if(current == Albums) run_list_albums(L"",L"");
			break;
		}
		
		pos.Y = 5;
		pos.X = 20;
	}
	current_mode = previos_mode;
	previos_mode = working_modes::List;
	run_mode();
}
void App::run_common_list(const wsvector& data,
						  int& choosen_option,
						  const string& no_elems_text,
						  int& current_elem,
						  int& start_counter,
						  int& max_counter,
						  const string& title)
{
	COORD title_pos{0,0};
	draw_string(title,red_label,title_pos);
	
	COORD elem_pos{2,1};
	if(data.empty())
	{
		COORD pos{10,5};
		draw_string(no_elems_text,standart,pos);
	}
	else
	{
		for(int i = start_counter;i<max_counter;++i)
		{
			auto curr = data[i];
			if(!can_wstring_be_converted_to_std(curr))
			{
				if(current_elem == i)
					draw_string(curr,green_label,elem_pos);
				else draw_string(curr,standart,elem_pos);
			}
			else
			{
				string text = convert_wstring_to_std(curr);
				if(current_elem == i)
					draw_string(text,green_label,elem_pos);
				else draw_string(text,standart,elem_pos);					
			}
				
			++elem_pos.Y;
		}
	}
		
	int input = _getch();
	if(input == ESCAPE) choosen_option = -2;
	if(input == MINUS && current_elem != 0) --current_elem;
	if(input == PLUS  && current_elem < max_counter-1)++current_elem;
	if(input == DOWN  && max_counter != data.size())
	{
		++max_counter;
		++start_counter;
	}
	if(input == UP    && start_counter != 0)
	{
		--max_counter;
		--start_counter;
	}
	if(input == ENTER) choosen_option = current_elem;

	elem_pos.Y = 1;
	
	//clear all
	//write this istead of system clearing
	//because it blinks
	COORD clear_pos{0,0};
	for(int i = start_counter;i<max_counter+1;++i)
	{
		draw_string(string(75,' '),empty,clear_pos);
		clear_pos.Y++;
	}
	clear_pos.Y = 0;
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
void App::run_list_genres()
{
	wsvector genres = get_genre_data_from_music();

	int choosen_option = -1;
		
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = genres.size()< visible_range?genres.size():visible_range;
	
	while(true)
	{
		run_common_list(genres,
						choosen_option,
						"there is no any genre data",
						current_elem,
						start_counter,
						max_counter,
						"Genres:");
		if(choosen_option == -2) break;
		if(choosen_option > 0)
		{
			choose_what_to_run_from_genre_menu(genres[choosen_option]);
			choosen_option = -1;
		}
	}
}
void App::run_list_artists(const wstring& genre)
{
	wsvector artists   = get_artists_data_from_music(genre);
	int max_counter    = artists.size() < visible_range? artists.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	
	while(true)
	{
		run_common_list(artists,
						choosen_option,
						"there is no any artist!",
						current_elem,
						start_counter,
						max_counter,
						"Artists:");
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{			
			choose_what_to_run_from_artist_menu(artists[current_elem],genre);
			choosen_option = -1;
		}
	}
}
void App::run_list_albums(const wstring& genre,const wstring& artist)
{
	wsvector albums = get_album_data_from_music(artist,genre);
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = albums.size()< visible_range? albums.size():visible_range;
	
	while(true)
	{
		run_common_list(albums,
						choosen_option,
						"there is no any album!",
						current_elem,
						start_counter,
						max_counter,
						"Albums:");
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{	
			run_list_titles(genre,artist,albums[current_elem]);
			choosen_option = -1;
		}
	}
}
void App::run_list_titles(const wstring& genre,const wstring& artist, const wstring& album)
{
	wsvector titles;
	if(album.empty()) titles = get_title_data_from_music(artist,genre);
	else titles = get_title_data_from_music(artist,genre,album);
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = titles.size()< visible_range? titles.size():visible_range;
	
	while(true)
	{
		run_common_list(titles,
						choosen_option,
						"there is no any composition!",
						current_elem,
						start_counter,
						max_counter,
						"Compositions:");
		if(choosen_option == -2) break;
	}
	system("cls");
}
void App::run_common_choosing_list(const wsvector& text,
								   int& min,
								   int& max,
								   int& current,
								   int& choosen_option)
{
	COORD pos{20,5};
	while(true)
	{
		for(size_t i = 0;i<text.size();++i)
		{
			auto curr = text[i];
			
			if(i == 1)pos.X = 25;
			if(!can_wstring_be_converted_to_std(curr))
			{
				if(current == i)
					draw_string(curr,green_label,pos);
				else draw_string(curr,standart,pos);
			}
			else
			{
				string text = convert_wstring_to_std(curr);
				if(current == i)
					draw_string(text,green_label,pos);
				else draw_string(text,standart,pos);					
			}
				
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
		if(input == ESCAPE) 
		{
			choosen_option = -2;
			break;
		}
		if(input == ENTER)
		{
			system("cls");
			choosen_option = current;
			break;
		}
		
		pos.Y = 5;
		pos.X = 20;
	}	
}
								  
								  
void App::choose_what_to_run_from_genre_menu(const wstring& genre_name)
{
	wsvector text = 
	{
		L"What you want to see from "+genre_name+L":",
		L"Artists",
		L"Albums",
		L"Compositions",
	};
	int choosen_option = -1;
	
	int min = 1;
	int max = 3;
	int current = 1;
	while(true)
	{
		run_common_choosing_list(text,min,max,current,choosen_option);
		if(choosen_option == 1) run_list_artists(genre_name);
		if(choosen_option == 2) run_list_albums(genre_name,L"");
		if(choosen_option == 3) run_list_titles(genre_name,L"",L"");
		if(choosen_option == -2) break;
	}
	system("cls");
}
void App::choose_what_to_run_from_artist_menu(const wstring& artist,const wstring& genre)
{
	wsvector text = 
	{
		L"What you want to see from "+artist+L":",
		L"Albums",
		L"All compositions",
	};
	int choosen_option = -1;
	
	int min = 1;
	int max = 2;
	int current = 1;
	while(true)
	{
		run_common_choosing_list(text,min,max,current,choosen_option);
		if(choosen_option == 1) run_list_albums(genre,artist);
		if(choosen_option == 2) run_list_titles(genre,artist,L"");
		if(choosen_option == -2) break;
	}
	system("cls");	
}
wstring App::fix_path_slash(const wstring& path)
{
	//change \ to /
	wstring new_;
	for(int i = 0;i<path.size();++i)
	{
		auto curr = path[i];
		if(static_cast<int>(curr) != 92)new_+=curr;
		else new_+=L"/";
	}
	return new_;
}
wsvector App::get_genre_data_from_music()
{
	wsvector data;
	for(auto& m:music)
	{
		bool already_added = find(data.begin(),data.end(),m->genre) != data.end();
		if(!already_added) data.push_back(m->genre);
	}
	data.erase(--data.end());
	return data;
}
wsvector App::get_artists_data_from_music(const wstring& genre)
{
	wsvector data;
	for(auto& m:music)
	{
		bool already_added = find(data.begin(),data.end(),m->artist) != data.end();
		if(!already_added && genre.empty()) data.push_back(m->artist);
		else if(!already_added && genre == m->genre) data.push_back(m->artist);
	}
	return data;
}
wsvector App::get_album_data_from_music(const wstring& artist, const wstring& genre)
{
	wsvector data;
	for(auto& m:music)
	{
		bool already_added = find(data.begin(),data.end(),m->album) != data.end();
		if(!already_added)
		{
			if(artist.empty()      && genre.empty())     data.push_back(m->album);
			else if(artist.empty() && genre == m->genre) data.push_back(m->album);
			else if(genre.empty()  && artist== m->artist)data.push_back(m->album);
			else if(genre == m->genre && artist == m->artist) data.push_back(m->album);
		}
	}
	return data;
}
wsvector App::get_title_data_from_music(const wstring& artist, const wstring& genre)
{
	wsvector data;
	for(auto& m:music)
	{
		bool is_new = find(data.begin(),data.end(),m->title) == data.end();
		if(is_new)
		{
			
			if(artist.empty()      && genre.empty())       data.push_back(m->title);
			if(genre == m->genre   && artist.empty())      data.push_back(m->title);
			if(genre.empty()       && artist== m->artist)  data.push_back(m->title);
			if(genre == m->genre   && artist == m->artist) data.push_back(m->title);
		}
	}
	return data;
}
wsvector App::get_title_data_from_music(const wstring& artist, const wstring& genre, const wstring& album)
{
	wsvector data;
	for(auto& m:music)
	{
		bool is_new = find(data.begin(),data.end(),m->title) == data.end();
		if(is_new)
		{
			if(genre  == m->genre   && 
			   artist == m->artist &&
			   album  == m->album) data.push_back(m->title);
		}
	}
	return data;	
}
bool App::can_wstring_be_converted_to_std(const wstring& str)
{
	for(auto& ch:str)
	{
		if(wctob(ch) == EOF) return false;
	}
	return true;
}
string App::convert_wstring_to_std(const wstring& str)
{
	string new_str;
	for(auto& ch:str)
	{
		char new_ch = wctob(ch);
		new_str+=new_ch;
	}
	return new_str;
}