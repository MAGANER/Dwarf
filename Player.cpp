#include"Player.h"
using namespace Dwarf;

Player::Player()
{
	volume_cash = 1.0f;
}
Player::~Player()
{
	
}
void Player::run_playing_composition(const vector<MusicData*>& _music,
									 bool& play_next,
								     const wstring& artist,
								     const wstring& album,
								     const wspair& title)
{	
	engine = createIrrKlangDevice();
	engine->setSoundVolume(volume_cash);
	system("cls");

	wstring _genre = get_genre_of_title(_music,artist,album,title.first);
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
		string volume_str = "volume:"+to_string(volume_output)+"%% ";

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


		bool nothing_to_play = !engine->isCurrentlyPlaying(music); 
		if(nothing_to_play && repeat) engine->play2D(music);
		if(nothing_to_play && !repeat && play_next_after_finishing && can_play_next)
		{
			play_next = true;
			break;
		}
		if(nothing_to_play && !can_play_next) break;
		
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
void Player::run_list_titles(const vector<MusicData*>& data,
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
		system("cls");
		run_playing_composition(data,play_next,artist,album,titles[current_elem]);
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
wspvector Player::get_title_data_from_music(const vector<MusicData*>& music,
											const wstring& artist, 
											const wstring& genre)
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
wspvector Player::get_title_data_from_music(const vector<MusicData*>& music,const wstring& album)
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
wstring Player::get_genre_of_title(const vector<MusicData*>& music,
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
PlayTime* Player::compute_time(irrklang::ik_u32 time)
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