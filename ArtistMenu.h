#ifndef ARTIST_MENU_H
#define ARTIST_MENU_H
#include"AlbumMenu.h"

namespace Dwarf
{
class ArtistMenu : public virtual AlbumMenu
{
public:
	ArtistMenu();
	virtual ~ArtistMenu();
	
	void run_list_artists(const vector<MusicData*>& music,
						  const wstring& genre);
	void choose_what_to_run_from_artist_menu(const vector<MusicData*>& music,
											 const wstring& artist, 
											 const wstring& genre);
private:
	wsvector get_artists_data_from_music(const vector<MusicData*>& music,
										 const wstring& genre);
};
};
#endif //ARTIST_MENU_H