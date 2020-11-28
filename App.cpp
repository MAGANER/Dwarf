#include"App.h"
using namespace Dwarf;

App::App()
{	
	SetConsoleOutputCP(1251);
	system("title Dwarf");
	system("cls");
	
	standart    = Color(ColorCode::Black,ColorCode::White);
	green_label = Color(ColorCode::Black,ColorCode::Green);
	red_label   = Color(ColorCode::Black,ColorCode::Red);
	magenta_label=Color(ColorCode::Black,ColorCode::Magenta);
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
	volume_cash = 1.0f;
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
		default:break;
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
	COORD bufferSize = {75,450};
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
			int point = str.find_last_of(L".");
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
					MusicData* data = new MusicData(artist,title,album,genre,str,year);
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
	system("cls");
	COORD title_pos{0,0};
	COORD elem_pos{2,1};
	
	auto draw = [&]() 
	{
		draw_string(title,red_label,title_pos);
		for(int i = start_counter;i<max_counter;++i)
		{
			auto curr = data[i];
			if(curr.size() > 73) curr = get_substr(curr,0,73)+L"...";
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
		elem_pos.Y = 1;
	};
	
	if(data.empty())
	{
		COORD pos{10,5};
		draw_string(no_elems_text,standart,pos);
	}
	else
	{
		draw();
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
	
	draw();
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
		if(choosen_option == current_elem)
		{
			system("cls");
			choose_what_to_run_from_genre_menu(genres[choosen_option]);
			choosen_option = -1;
		}
	}
	system("cls");
}
void App::run_list_artists(const wstring& genre)
{
	wsvector artists   = get_artists_data_from_music(genre);
	int max_counter    = artists.size() < visible_range? artists.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	bool started = true;
	
	while(true)
	{
		draw_string(to_string(play_next),standart,COORD{20,0});
		if(kbhit() || started)
		{
			run_common_list(artists,
							choosen_option,
							"there is no any artist!",
							current_elem,
							start_counter,
							max_counter,
							"Artists:");
			started = false;
		}
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{		
			system("cls");
			choose_what_to_run_from_artist_menu(artists[current_elem],genre);
			started = true;
			choosen_option = -1;
		}
	}
	system("cls");
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
	system("cls");
}
void App::run_list_titles(const wstring& genre,const wstring& artist, const wstring& album)
{
	wspvector titles;
	wsvector titles_only;

	if(album.empty()) titles = get_title_data_from_music(artist,genre);
	else titles = get_title_data_from_music(album);
	
	for(auto t:titles) titles_only.push_back(t.first);
	current_play_list = titles;
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = titles.size()< visible_range? titles.size():visible_range;
	
	
	auto play = [&]()
	{
		system("cls");
		run_playing_composition(play_next,artist,album,titles[current_elem]);
	};
	
	while(true)
	{
		if(!play_next)
		{
			run_common_list(titles_only,
							choosen_option,
							"there is no any composition!",
							current_elem,
							start_counter,
							max_counter,
							"Compositions:");
		}
		else
		{
			current_elem++;
			choosen_option = current_elem;
		}
		
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{
			current_play_list_pos = current_elem;
			play();
			choosen_option = -1;
		}
	}
	system("cls");
}
void App::run_common_choosing_list(const wsvector& text,
								   int& min,
								   int& max,
								   int& current,
								   int& choosen_option)
{
	COORD pos{10,4};
	while(true)
	{
		for(size_t i = 0;i<text.size();++i)
		{
			auto curr = text[i];
			
			if(i == 1)
			{
				pos.X = 25;
				pos.Y = 10;
			}
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
		
		pos.Y = 4;
		pos.X = 10;
	}	
}
void App::run_playing_composition(bool& play_next,
								  const wstring& artist,
								  const wstring& album,
								  const wspair& title)
{	
	engine = createIrrKlangDevice();
	engine->setSoundVolume(volume_cash);
	system("cls");

	wstring _genre = get_genre_of_title(artist,album,title.first);
	wstring _path = title.second;
	string  path  = convert_wstring_to_std(_path); 
	
	wstring label = L"Playing:";
	COORD label_pos = {2,0};
	
	COORD title_pos  ={4,2};
	COORD artist_pos ={4,3};
	COORD album_pos  ={4,4};
	COORD genre_pos  ={4,5};
	COORD volume_pos ={4,7};
	COORD length_pos ={4,8};
	COORD curr_pos_pos={4,9};
	COORD next_pos   ={4,10};
	
	COORD play_next_pos = {4,12};
	COORD repeat_pos    ={4,13};
	COORD pause_pos     ={4,14};

	COORD help1 = {5,18};
	COORD help2 = {5,19};
	COORD help3 = {5,20};
	COORD help4 = {5,21};
	
	
	ISoundSource* music = nullptr;
	unsigned int hours, mins, secs;
	
	bool repeat = false;
	bool play_next_after_finishing = true;
	bool play = false;
	bool pause= false;
	bool space_isnt_pressed = true;
	bool n_isnt_pressed     = true;
	bool r_isnt_pressed      = true;
	
	while(true)
	{
		int volume_output = engine->getSoundVolume()*100.0f;
		string volume_str = to_string(volume_output);
		
		draw_string(label,red_label,label_pos);
		
		draw_string(L"Title:"+title.first,standart,title_pos);
		draw_string(L"Artist:"+artist,standart,artist_pos);
		draw_string(L"Album:"+album,standart,album_pos);
		draw_string(L"Genre:"+_genre,standart,genre_pos);
		draw_string("volume:"+volume_str+"% ",standart,volume_pos);
		
		draw_string("+/- to change volume",green_label,help1);
		draw_string("SPACE to pause/unpause",green_label,help2);
		draw_string("r to set repeat on/off",green_label,help3);
		draw_string("n to play/(not) next song",green_label,help4);
		
		
		bool can_play_next = current_play_list_pos+1 < current_play_list.size();
		if(can_play_next)
		{
			wstring next_song = current_play_list[current_play_list_pos+1].first;
			draw_string(L"next:"+next_song,standart,next_pos);
		}
		else
		{
			draw_string("next:None",standart,next_pos);
		}
		
		string play_next_option = play_next_after_finishing?"on  ":"off ";
		draw_string("play next after current song:"+play_next_option,magenta_label,play_next_pos);
		
		string repeat_option = repeat?"on  ":"off ";
		draw_string("repeat:"+repeat_option,magenta_label,repeat_pos);
		
		string pause_option = pause?"on  ":"off ";
		draw_string("pause:"+pause_option,magenta_label,pause_pos);
		
		if(!play)
		{
			bool is_loaded_already = find(loaded_music.begin(),loaded_music.end(),path) != loaded_music.end();
			if(!is_loaded_already)
			{
				music = engine->addSoundSourceFromFile(path.c_str());
				loaded_music.push_back(path);
			}
			else
			{
				music = engine->getSoundSource(path.c_str());
			}
			irrklang::ik_u32 mlseconds= engine->getSoundSource(path.c_str())->getPlayLength();
			
			PlayTime* play_time = compute_time(mlseconds);
			hours = play_time->hour;
			mins  = play_time->minutes;
			secs  = play_time->secs;
			delete play_time;
			
			engine->play2D(music);
			play = true;
		}
		string _text = "length = "+to_string(hours)+":"+to_string(mins)+":"+to_string(secs);
		draw_string(_text,standart,length_pos);


		if(!engine->isCurrentlyPlaying(music) && repeat) engine->play2D(music);
		if(!engine->isCurrentlyPlaying(music) && !repeat && play_next_after_finishing && can_play_next)
		{
			play_next = true;
			break;
		}
		if(!engine->isCurrentlyPlaying(music) && repeat && !can_play_next) break;
		
		if(kbhit())
		{
			int input = _getch();
		
			if(input == ESCAPE) 
			{
				engine->stopAllSounds();
				engine->drop();
				play_next = false;
				break;
			}
		
			float val = engine->getSoundVolume();
			if(input == PLUS)
			{
				if(val < 1.0f)
				{					
					engine->setSoundVolume(val+0.01f);
					volume_cash = engine->getSoundVolume();
				}
			}
			if(input == MINUS)
			{
				if(val > 0.0f) 
				{
					engine->setSoundVolume(val-0.01f);
					volume_cash = engine->getSoundVolume();
				}
			}
			
		
			if(input == SPACE && !pause && space_isnt_pressed) 
			{
				engine->setAllSoundsPaused(true); 
				pause = true;
				space_isnt_pressed = false;
			}
			if(input == SPACE &&  pause && space_isnt_pressed) 
			{
				engine->setAllSoundsPaused(false);
				pause = false;
				space_isnt_pressed = false;
			}
		
			if(input == R && !repeat && r_isnt_pressed)
			{
				repeat = true;
				r_isnt_pressed = false;
			}
			if(input == R && repeat && r_isnt_pressed)
			{
				repeat = false;
				r_isnt_pressed = false;
			}
		
			if(input == N && !play_next_after_finishing && n_isnt_pressed)
			{
				play_next_after_finishing = true;
				n_isnt_pressed = false;
			}
			if(input == N && play_next_after_finishing && n_isnt_pressed)
			{
				play_next_after_finishing = false;
				n_isnt_pressed = false;
			}
			
			n_isnt_pressed      = true;
			r_isnt_pressed      = true; 
			space_isnt_pressed  = true;
		}
		
		Sleep(10);
		engine->update();
	}
	
	system("cls");
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
	system("cls");
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
	
	bool started = true;
	while(true)
	{
		run_common_choosing_list(text,min,max,current,choosen_option);
		started = false;
		if(choosen_option == 1) run_list_albums(L"",artist);
		if(choosen_option == 2) run_list_titles(L"",artist,L"");
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
wstring App::get_path_to_title(const wstring& artist, 
							   const wstring& album,
							   const wstring& title)
{
	wstring path;
	for(auto& m:music)
	{
		if(m->artist == artist &&
		   m->album  == album  &&
		   m->title  == title) path = m->path;
	}
	return path;
}
wstring App::get_genre_of_title(const wstring& artist, 
							    const wstring& album,
							    const wstring& title)
{
	wstring str;
	for(auto& m:music)
	{
		if(m->artist == artist &&
		   m->album  == album  &&
		   m->title  == title) str = m->genre;
	}
	if(str.empty()) str = L"None";
	return str;
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
wspvector App::get_title_data_from_music(const wstring& artist, const wstring& genre)
{
	wspvector data;
	for(auto& m:music)
	{
		wspair _new(m->title,m->path);
		bool is_new = find(data.begin(),data.end(),_new) == data.end();
		if(is_new)
		{
			
			if(artist.empty()      && genre.empty())       data.push_back(wspair(m->title,m->path));
			if(genre == m->genre   && artist.empty())      data.push_back(wspair(m->title,m->path));
			if(genre.empty()       && artist== m->artist)  data.push_back(wspair(m->title,m->path));
			if(genre == m->genre   && artist == m->artist) data.push_back(wspair(m->title,m->path));
		}
	}
	return data;
}
wspvector App::get_title_data_from_music(const wstring& album)
{
	wspvector data;
	for(auto& m:music)
	{
		wspair _new(m->title,m->path);
		bool is_new = find(data.begin(),data.end(),_new) == data.end();
		if(is_new)
		{
			if(album  == m->album) data.push_back(wspair(m->title,m->path));
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
PlayTime* App::compute_time(irrklang::ik_u32 time)
{
	if(time == 0) return nullptr;
	
	int second = 1000; // 1 sec = 1000 miliseconds
	int minute = 60;   // 1 min = 60 seconds
	int hour   = 60;   // 1 hour= 60 minute
	
	double total_seconds = (double)time/second;
	double total_minutes = total_seconds/60;
	double total_hours   = total_minutes/60;
	
	
	unsigned int secs  = total_seconds  > 1.0f? (int)total_seconds%60 : 0;
	unsigned int mins  = total_minutes  > 1.0f? (int)total_minutes : 0;
	unsigned int hours = 0;
	if(total_hours > 1.0f)
	{
		hours = (int) total_hours;
		mins  = (int) modf(total_hours,NULL); 
	}
	
	return new PlayTime(hours,mins,secs);	
}