/*
	Class provides abilities to play music.
	Also it renderes the menu.
*/

#ifndef PlayerMenu_H
#define PlayerMenu_H

#include"CommonMenu.h"
#include "MusicData.h"
#include "PlayTime.h"

#include<cstring> 
#include<filesystem>
#include<cmath>

#include"libzplay.h"

namespace fs = std::filesystem;
using namespace libZPlay;
namespace Dwarf
{	
typedef pair<wstring,wstring> wspair;
typedef vector<wspair> wspvector;	

class PlayerMenu : public virtual CommonMenu
{
private:

	///
//* Data is used by plaing function	
	//labels' positions
	COORD help1 = {5,18};
	COORD help2 = {5,19};
	COORD help3 = {5,20};
	COORD help4 = {5,21};
	COORD help5 = {5,22};
	
	wstring label = L"Playing:";
	COORD label_pos = {2,0};
	
	COORD title_pos   ={4,2};
	COORD artist_pos  ={4,3};
	COORD album_pos   ={4,4};
	COORD genre_pos   ={4,5};
	COORD volume_pos  ={4,7};
	COORD length_pos  ={4,8};
	COORD pos_label_pos={4,9};
	COORD curr_pos_pos={4,10};
	COORD next_pos    ={4,11};
	
	COORD play_next_pos = {4,13};
	COORD repeat_pos    = {4,14};
	COORD pause_pos     = {4,15};
	//.
	
	wstring current_title, current_genre, current_artist, current_album;
	
	//variables to manage playing
	bool repeat = false;
	bool play_next_after_finishing = true;
	bool play = false;
	bool pause= false;
	bool space_isnt_pressed = true;
	bool n_isnt_pressed     = true;
	bool r_isnt_pressed     = true;
	bool stop = false;
	bool can_play_next;
	void set_managing_vars();
	
	bool break_playing = false;
	//.
	
	TStreamInfo info;
	TStreamStatus status;
	
	//composition time
	int hr, min, sec;
	//.
	
	void load_composition(const string& path);
	void process_input();
	void process_playing_cycle();
	void draw_position(const COORD& label_pos);
	void draw_help_info();
	void draw_length();
	void draw_composition_status(bool raw_music);
	
	void draw_there_is_no_ability_to_play();
	
private:
	ZPlay *player;
	bool audio_init = true;
protected:
	svector loaded_music;
	
	bool play_next = false;
	
	wspvector current_play_list;
	int current_play_list_pos;
	int volume = 100;	
public:
	PlayerMenu();
	virtual ~PlayerMenu();
	
	void run_list_titles(const vector<MusicData*>& data,
						 const wstring& genre,
						 const wstring& artist, 
						 const wstring& album);
	void run_playing_composition(const vector<MusicData*>& _music,
								 const wstring& artist, 
								 const wstring& album,
								 const wspair& title);
	void play_raw_music(const wstring& output_path,
						const wstring& real_path);
private:
	wspvector get_title_data_from_music(const vector<MusicData*>& music,
										const wstring& artist, 
										const wstring& genre);
	wspvector get_title_data_from_music(const vector<MusicData*>& music, const wstring& album);
	wstring get_genre_of_title(const vector<MusicData*>& music,
							   const wstring& artist, 
							   const wstring& album,
							   const wstring& title);
	PlayTime* compute_time(int time);	
	void set_song_position(int hrs,int mins,int secs);
	int to_seconds(int hrs,int mins,int secs);
};
};

#endif //PlayerMenu_H