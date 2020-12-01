#ifndef GENRE_MENU_H
#define GENRE_MENU_H
#include"ArtistMenu.h"
namespace Dwarf
{
class GenreMenu : public ArtistMenu
{
public:
	GenreMenu();
	~GenreMenu();
	
	void run_list_genres(const vector<MusicData*>& music);
	void choose_what_to_run_from_genre_menu(const vector<MusicData*>& music,
											const wstring& genre_name);

private:
	wsvector get_genre_data_from_music(const vector<MusicData*>& music);
};
};
#endif //GENRE_MENU_H