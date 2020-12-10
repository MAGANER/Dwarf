#include"GenreMenu.h"
using namespace Dwarf;

GenreMenu::GenreMenu()
{}
GenreMenu::~GenreMenu()
{}

void GenreMenu::run_list_genres(const vector<MusicData*>& music)
{
	wsvector genres = get_genre_data_from_music(music);
	sort(genres.begin(),genres.end());

	int choosen_option = -1;
		
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = genres.size()< visible_range?genres.size():visible_range;
	
	
	while(true)
	{
		run_common_list(genres,
						choosen_option,
						"there is no any genre data",
						current_elem,
						start_counter,
						max_counter,
						"Genres:");
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{
			clear();
			choose_what_to_run_from_genre_menu(music,genres[choosen_option]);
			choosen_option = -1;
		}
	}
	clear();
}
void GenreMenu::choose_what_to_run_from_genre_menu(const vector<MusicData*>& music,
												   const wstring& genre_name)
{
	wsvector text = 
	{
		L"What you want to see from "+genre_name+L":",
		L"Artists",
		L"Albums",
		L"Compositions",
	};
	int choosen_option = -1;
	
	int min = 1;
	int max = 3;
	int current = 1;
	while(true)
	{
		run_common_choosing_list(text,min,max,current,choosen_option);
		if(choosen_option == 1) run_list_artists(music,genre_name);
		if(choosen_option == 2) run_list_albums(music,genre_name,L"");
		if(choosen_option == 3) run_list_titles(music,genre_name,L"",L"");
		if(choosen_option == -2) break;
	}
	clear();
}
wsvector GenreMenu::get_genre_data_from_music(const vector<MusicData*>& music)
{
	wsvector data;
	for(auto& m:music)
	{
		bool already_added = find(data.begin(),data.end(),m->genre) != data.end();
		if(!already_added) data.push_back(m->genre);
	}
	if(!data.empty())data.erase(--data.end());
	return data;
}