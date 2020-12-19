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
		if(add_elements_to_group)
		{
			wspvector whole_music = get_title_data_from_music(music,albums[current_elem]);
			wsvector titles;
			for(auto& m:whole_music) titles.push_back(m.first);

			system("cls");
			wsvector names = get_groups_name();
			wstring group_to_add= show_groups_to_choose(names);
			
			if(!group_to_add.empty())
				for(auto& t: titles) add_elem_to_group(t,group_to_add);
			
			add_elements_to_group = false;
			system("cls");
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