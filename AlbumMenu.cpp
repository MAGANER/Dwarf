#include"AlbumMenu.h"
using namespace Dwarf;
AlbumMenu::AlbumMenu()
{
}
AlbumMenu::~AlbumMenu()
{
}

void AlbumMenu::run_list_albums(const vector<MusicData*>& music,
								const wstring& genre,
								const wstring& artist)
{
	wsvector albums = get_album_data_from_music(music,artist,genre);
	sort(albums.begin(),albums.end());
	
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
			run_list_titles(music,genre,artist,albums[current_elem]);
			choosen_option = -1;
		}
	}
	clear();
}
wsvector AlbumMenu::get_album_data_from_music(const vector<MusicData*>& music,
											  const wstring& artist, 
											  const wstring& genre)
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