#ifndef PLAYER_H
#define PLAYER_H
#include"CommonMenu.h"
#include "MusicData.h"
#include "PlayTime.h"
#include "irrKlang.h"

using namespace irrklang;
namespace Dwarf
{
	
typedef pair<wstring,wstring> wspair;
typedef vector<wspair> wspvector;	

class Player : public virtual CommonMenu
{
protected:
	ISoundEngine* engine;
	svector loaded_music;
	
	bool play_next = false;
	
	wspvector current_play_list;
	int current_play_list_pos;
	irrklang::ik_f32 volume_cash;
public:
	Player();
	virtual ~Player();
	
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
	PlayTime* compute_time(irrklang::ik_u32 time);
};
};
#endif //PLAYER_H