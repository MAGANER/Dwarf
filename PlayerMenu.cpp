#include"PlayerMenu.h"
using namespace Dwarf;

PlayerMenu::PlayerMenu()
{
	player = CreateZPlay();
	if(player == 0)
	{
		audio_init = false;
		cout<<"can not create audio player object!"<<endl;
		getch();
	}
	
	int version = player->GetVersion();
	if(version < 190)
	{
		audio_init = false;
		cout<<"need libZplay 1.9 or above!"<<endl;
		getch();
	}
}
PlayerMenu::~PlayerMenu()
{
	player->Release();
}


void PlayerMenu::set_managing_vars()
{
	repeat = false;
	play_next_after_finishing = true;
	play = false;
	pause= false;
	space_isnt_pressed = true;
	n_isnt_pressed     = true;
	r_isnt_pressed     = true;
	stop = false;
	break_playing = false;
}
void PlayerMenu::run_playing_composition(const vector<MusicData*>& _music,
										 const wstring& artist,
								         const wstring& album,
								         const wspair&  title)
{	
	set_managing_vars();
	clear();

	current_artist = artist;
	current_album  = album;
	current_title  = title.first;
	current_genre  = get_genre_of_title(_music,artist,album,title.first);
	
	wstring _path  = title.second;
	string path    = convert_wstring_to_std(_path); 
				
	
	can_play_next = current_play_list_pos+1 < current_play_list.size();
	draw_there_is_no_ability_to_play();
	while(!stop && audio_init)
	{
		bool not_out_of_playlist = current_play_list_pos < current_play_list.size(); 
		if(not_out_of_playlist){
		player->SetMasterVolume(volume,volume);
		
		draw_composition_status(false);
		
		load_composition(path);
		
		draw_length();
		draw_position(pos_label_pos);
		draw_help_info();
		
		process_playing_cycle();
		process_input();
		Sleep(10);
		
		if(break_playing) break;
		
		}else break;
	}
	
	clear();
}
void PlayerMenu::play_raw_music(const wstring& output_path,
								const wstring& real_path)
{
	set_managing_vars();
	clear();
	
	current_title  = output_path;

	can_play_next = current_play_list_pos+1 < current_play_list.size();
	draw_there_is_no_ability_to_play();
	while(!stop && audio_init)
	{
		player->SetMasterVolume(volume,volume);
		
		draw_string(current_title,standart,title_pos);
		
		load_composition(convert_wstring_to_std(real_path));
		
		draw_length();
		draw_position(pos_label_pos);
		draw_help_info();
		draw_composition_status(true);
		
		
		process_playing_cycle();
		process_input();
		Sleep(10);
		
		if(break_playing) break;
	}
	
	clear();	
}
void PlayerMenu::draw_there_is_no_ability_to_play()
{
	if(!audio_init)
	{
		cout<<"It is not unable to play music!"<<endl;
		system("pause");
	}
}
void PlayerMenu::draw_composition_status(bool raw_music)
{
	string vol_val = to_string(volume);	
	if(volume < 100) vol_val = get_substr(vol_val,0,2);
	if(volume < 10)   vol_val= get_substr(vol_val,0,1);
	vol_val+="%% ";
	string volume_str = "volume:"+vol_val;

	draw_string(label,red_label,label_pos);
	
	if(!raw_music)
	{
		draw_string(L"Title:" + current_title,  standart, title_pos);
		draw_string(L"Artist:"+ current_artist, standart, artist_pos);
		draw_string(L"Album:" + current_album,  standart, album_pos);
		draw_string(L"Genre:" + current_genre,  standart, genre_pos);
	}
	draw_string(volume_str, standart, volume_pos);
			
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
}
void PlayerMenu::load_composition(const string& path)
{
	if(!play)
	{
		if(player->OpenFile(path.c_str(),sfAutodetect) == 0)
		{
			cout<<"can not open "+path<<endl;
			getch();
			break_playing = true;
		}
		else
		{			
		if(player->Play() == 0)
		{
			cout<<"can not play "+path+"!"<<endl;
			getch();
			break_playing = true;
		}
			player->GetStreamInfo(&info);
			
			PlayTime* time = compute_time(info.Length.ms);
			hr = time->hour;
			min= time->minutes;
			sec= time->secs;
			delete time;
			
			play = true;
		}
	}	
}
void PlayerMenu::draw_length()
{
	string length="length="+to_string(hr)+":"+to_string(min)+":"+to_string(sec);
	draw_string(length,standart,length_pos);
}
void PlayerMenu::process_playing_cycle()
{
	player->GetStatus(&status);
	bool not_playing = status.fPlay == 0 && !pause;
		
	if(not_playing && !repeat && !can_play_next) stop = true;
	if(not_playing && repeat) player->Play();
	if(not_playing && !repeat && play_next_after_finishing && can_play_next)
	{
		play_next = true;
		stop = true;
	}	
}
void PlayerMenu::process_input()
{
	if(kbhit())
	{
		int input = _getch();
		
		if(input == ESCAPE) 
		{
			player->Close();
			play_next = false;
			break_playing = true;
		}
			
		//change volume
		if(input == PLUS && volume < 100)
		{
			++volume;
			player->SetMasterVolume(volume,volume);
		}
		if(input == MINUS && volume > 0)
		{
			--volume;
			player->SetMasterVolume(volume,volume);
		}
			
		//pause 
		if(input == SPACE && !pause && space_isnt_pressed) 
		{
			player->Pause();
			pause = true;
			space_isnt_pressed = false;
		}
		if(input == SPACE &&  pause && space_isnt_pressed) 
		{
			player->Resume();
			pause = false;
			space_isnt_pressed = false;
		}
			
		//repeat
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
			
		//set new song position
		if(input == C)
		{
			clear();
			set_song_position(hr,min,sec);
		}
			
		n_isnt_pressed      = true;
		r_isnt_pressed      = true; 
		space_isnt_pressed  = true;
	}
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
PlayTime* PlayerMenu::compute_time(int time)
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
void PlayerMenu::draw_position(const COORD& label_pos)
{
	TStreamTime time;
	
	player->GetPosition(&time);
	PlayTime* _time = compute_time(time.ms);
	if(_time != nullptr)
	{
		int hr = _time->hour;
		int min= _time->minutes;
		int sec= _time->secs;
	
		string pos = "position="+to_string(hr)+":"+to_string(min)+":"+to_string(sec)+" ";
		draw_string(pos,standart,label_pos);
	}
	delete _time;
}
void PlayerMenu::draw_help_info()
{
	draw_string("+/- to change volume",      green_label, help1);
	draw_string("SPACE to pause/unpause",    green_label, help2);
	draw_string("r to set repeat on/off",    green_label, help3);
	draw_string("n to play/(not) next song", green_label, help4);
	draw_string("c to set new song position",green_label, help5);	
}
void PlayerMenu::set_song_position(int hrs,int mins,int secs)
{
	COORD label_pos = {4,10};
	string label    = "new_pos=";
	
	COORD help1 = {4,15};
	COORD help2 = {4,16};
	
	int hour   = 0;
	int minute = 0;
	int second = 0;
	
	
	bool dir = true;
	int current= 0;
	while(true)
	{
		string hour_str = to_string(hour);
		short hour_x = 4+label.size();
		
		string minute_str = to_string(minute);
		short minute_x = hour_x+2;
		
		string second_str = to_string(second);
		short second_x = minute_x+2;
		
		draw_string("space to move",green_label,help1);
		draw_string("+/- to change value",green_label,help2);
		
		draw_string(label,standart,label_pos);
		if(current == 0) draw_string(hour_str,red_label,COORD{hour_x,10});
		else draw_string(hour_str,standart,COORD{hour_x,10});
		
		if(current == 1) draw_string(minute_str,red_label,COORD{minute_x,10});
		else draw_string(minute_str,standart,COORD{minute_x,10});

		if(current == 2) draw_string(second_str,red_label,COORD{second_x,10});
		else draw_string(second_str,standart,COORD{second_x,10});
		
		short separator_pos1 = hour_x+1;
		short separator_pos2 = minute_x+1;
		
		draw_string(":",standart,COORD{separator_pos1,10});
		draw_string(":",standart,COORD{separator_pos2,10});
		
		int input = getch();
		if(input == ESCAPE) break;
		if(input == SPACE)
		{
			if(current == 2) dir = false;
			if(current == 0) dir = true;
			if(dir)current++;
			else current--;
		}
		
		if(input == PLUS)
		{
			if(current == 0 && hour)
			{
				hour++;
			}
			if(current == 1 && minute < 59)
			{
				minute++;
			}
			if(current == 2 && second < 59)
			{
				second++;
			}
		}
		if(input == MINUS)
		{
			if(current == 0 && hour != 0)
			{
				hour--;
			}
			if(current == 1 && minute != 0)
			{
				minute--;
			}
			if(current == 2 && second != 0)
			{
				second--;
			}			
		}	
	}
	clear();
	
	TStreamTime time;
	time.sec = to_seconds(hour,minute,second);
	
	if(time.sec <= info.Length.ms) player->Seek(tfSecond,&time,smFromBeginning);
	
	if(pause)player->Pause();
}
int PlayerMenu::to_seconds(int hrs,int mins,int secs)
{
	return secs+(mins*60)+(hrs*3600);
}