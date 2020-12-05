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
#include <cmath>
#include <windows.h>

#include "audiere.h"

namespace fs = std::filesystem;
using namespace audiere; 
namespace Dwarf
{	
typedef pair<wstring,wstring> wspair;
typedef vector<wspair> wspvector;	

class PlayerMenu : public virtual CommonMenu
{
protected:
	svector loaded_music;
	
	bool play_next = false;
	
	wspvector current_play_list;
	int current_play_list_pos;
	float volume       = 1.0f;
	int volume_percent = 100;	
public:
	PlayerMenu();
	virtual ~PlayerMenu();
	
	void run_list_titles(const vector<MusicData*>& data,
						 const wstring& genre,
						 const wstring& artist, 
						 const wstring& album);
	void run_playing_composition(const vector<MusicData*>& _music,
								 bool& play_next,
								 const wstring& artist, 
								 const wstring& album,
								 const wspair& title);
private:
	wspvector get_title_data_from_music(const vector<MusicData*>& music,
										const wstring& artist, 
										const wstring& genre);
	wspvector get_title_data_from_music(const vector<MusicData*>& music, const wstring& album);
	wstring get_genre_of_title(const vector<MusicData*>& music,
							   const wstring& artist, 
							   const wstring& album,
							   const wstring& title);
	PlayTime* compute_time(const string& path_to_file);
};
};

#endif //PlayerMenu_H