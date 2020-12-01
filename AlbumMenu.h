#ifndef ALBUM_MENU_H
#define ALBUM_MENU_H
#include"Player.h"

namespace Dwarf
{
class AlbumMenu : public virtual Player
{
public:
	AlbumMenu();
	virtual ~AlbumMenu();
	
	void run_list_albums(const vector<MusicData*>& music,
						 const wstring& genre,
						 const wstring& artist);
private:
	wsvector get_album_data_from_music(const vector<MusicData*>& music,
									   const wstring& artist, 
									   const wstring& genre);
};
};
#endif //ALBUM_MENU_H