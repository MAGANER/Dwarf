#include"PlayerMenu.h"
using namespace Dwarf;

PlayerMenu::PlayerMenu()
{
}
PlayerMenu::~PlayerMenu()
{
}

void PlayerMenu::run_playing_composition(const vector<MusicData*>& _music,
										 const wstring& artist,
								         const wstring& album,
								         const wspair& title)
{	
	clear();
	AudioDevicePtr device  = OpenDevice();
	if(!device)
	{
		cout<<"something went wrong with output device...";
		exit(0);
	}
	
	wstring _genre = get_genre_of_title(_music,artist,album,title.first);
	wstring _path = title.second;
	string path  = convert_wstring_to_std(_path); 
	
	
	//label positions
	wstring label = L"Playing:";
	COORD label_pos = {2,0};
	
	COORD title_pos   ={4,2};
	COORD artist_pos  ={4,3};
	COORD album_pos   ={4,4};
	COORD genre_pos   ={4,5};
	COORD volume_pos  ={4,7};
	COORD length_pos  ={4,8};
	COORD curr_pos_pos={4,9};
	COORD next_pos    ={4,10};
	
	COORD play_next_pos = {4,12};
	COORD repeat_pos    = {4,13};
	COORD pause_pos     = {4,14};

	COORD help1 = {5,18};
	COORD help2 = {5,19};
	COORD help3 = {5,20};
	COORD help4 = {5,21};
	//
	
	
	//logical variables
	bool repeat = false;
	bool play_next_after_finishing = true;
	bool play = false;
	bool pause= false;
	bool space_isnt_pressed = true;
	bool n_isnt_pressed     = true;
	bool r_isnt_pressed      = true;
	bool stop = false;
	
	//time
	int minutes,seconds, hours;
	
	OutputStreamPtr sound = OpenSound(device , path.c_str() , true);
	if(!sound)
	{
		cout<<"something went wrong...";
		exit(-1);
	}
	
	
	while(!stop)
	{
		sound->setVolume(volume);
		string vol_val = to_string(volume_percent);
	
		if(volume_percent < 100) vol_val = get_substr(vol_val,0,2);
		if(volume_percent < 10)  vol_val = get_substr(vol_val,0,1);
		vol_val+="%% ";	
		
		string volume_str = "volume:"+vol_val;

		draw_string(label,red_label,label_pos);
		
		draw_string(L"Title:"+title.first,standart,title_pos);
		draw_string(L"Artist:"+artist,standart,artist_pos);
		draw_string(L"Album:"+album,standart,album_pos);
		draw_string(L"Genre:"+_genre,standart,genre_pos);
		draw_string(volume_str,standart,volume_pos);
		
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
			sound->reset();
			sound->play();
			play = true;
		}


		if(!sound->isPlaying() && !repeat && !can_play_next && !pause) stop = true;
		if(!sound->isPlaying() && repeat) sound->reset();
		if(!sound->isPlaying() && !repeat && play_next_after_finishing && can_play_next && !pause)
		{
			play_next = true;
			stop = true;
		}
		
		if(kbhit())
		{
			int input = _getch();
		
			if(input == ESCAPE) 
			{
				play_next = false;
				break;
			}
			
			//change volume
			if(input == PLUS && volume_percent < 100)
			{
				volume+=0.01f;
				volume_percent++;
				sound->setVolume(volume);
			}
			if(input == MINUS)
			{
				if(volume > 0.000001f)
				{
					volume-=0.01f;
					volume_percent--;
					sound->setVolume(volume);
				}
				if(volume < 0.000002f)
				{
					volume = 0.0f;
					volume_percent = 0;
					sound->setVolume(volume);
				}
			}
			
			//pause 
			if(input == SPACE && !pause && space_isnt_pressed) 
			{
				sound->stop();
				pause = true;
				space_isnt_pressed = false;
			}
			if(input == SPACE &&  pause && space_isnt_pressed) 
			{
				sound->play();
				pause = false;
				space_isnt_pressed = false;
			}
			
			//repeat
			if(input == R && !repeat && r_isnt_pressed)
			{
				sound->setRepeat(true);
				repeat = true;
				r_isnt_pressed = false;
			}
			if(input == R && repeat && r_isnt_pressed)
			{
				sound->setRepeat(false);
				repeat = false;
				r_isnt_pressed = false;
			}
		
			//play next song
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
	}
	
	clear();
}
void PlayerMenu::run_list_titles(const vector<MusicData*>& data,
							 const wstring& genre,
							 const wstring& artist, 
							 const wstring& album)
{
	wspvector titles;
	wsvector titles_only;

	if(album.empty()) titles = get_title_data_from_music(data,artist,genre);
	else titles = get_title_data_from_music(data,album);
	
	for(auto t:titles) titles_only.push_back(t.first);
	current_play_list = titles;
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = titles.size()< visible_range? titles.size():visible_range;
	
	
	auto play = [&]()
	{
		clear();
		run_playing_composition(data,artist,album,titles[current_elem]);
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
	clear();
}
wspvector PlayerMenu::get_title_data_from_music(const vector<MusicData*>& music,
											const wstring& artist, 
											const wstring& genre)
{
	wspvector data;
	wsvector added;
	for(auto& m:music)
	{
		bool is_new = find(added.begin(),added.end(),m->title) == added.end();
		if(is_new)
		{
			
			if(artist.empty()      && genre.empty())
			{
				data.push_back(wspair(m->title,m->path));
				added.push_back(m->title);
			}
			if(genre == m->genre   && artist.empty())     
			{
				data.push_back(wspair(m->title,m->path));
				added.push_back(m->title);
			}
			if(genre.empty()       && artist== m->artist) 
			{
				data.push_back(wspair(m->title,m->path));
				added.push_back(m->title);
			}
			if(genre == m->genre   && artist == m->artist)
			{
				data.push_back(wspair(m->title,m->path));
				added.push_back(m->title);
			}
		}
	}
	return data;
}
wspvector PlayerMenu::get_title_data_from_music(const vector<MusicData*>& music,const wstring& album)
{
	wspvector data;
	wsvector added;
	for(auto& m:music)
	{
		bool is_new = find(added.begin(),added.end(),m->title) == added.end();
		if(is_new)
		{
			if(album  == m->album)
			{
				data.push_back(wspair(m->title,m->path));
				added.push_back(m->title);
			}
		}
	}
	return data;	
}
wstring PlayerMenu::get_genre_of_title(const vector<MusicData*>& music,
								   const wstring& artist, 
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
PlayTime* PlayerMenu::compute_time(const string& path_to_file)
{
	fs::path _path(path_to_file);
	int size = fs::file_size(_path);
	
	return new PlayTime(0,0,0);	
}