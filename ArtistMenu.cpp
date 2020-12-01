#include"ArtistMenu.h"
using namespace Dwarf;

ArtistMenu::ArtistMenu()
{}
ArtistMenu::~ArtistMenu()
{}

void ArtistMenu::run_list_artists(const vector<MusicData*>& music,const wstring& genre)
{
	wsvector artists   = get_artists_data_from_music(music,genre);
	sort(artists.begin(),artists.end());
	
	int max_counter    = artists.size() < visible_range? artists.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	bool started = true;
	
	while(true)
	{
		if(kbhit() || started)
		{
			run_common_list(artists,
							choosen_option,
							"there is no any artist!",
							current_elem,
							start_counter,
							max_counter,
							"Artists:");
			started = false;
		}
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{		
			system("cls");
			choose_what_to_run_from_artist_menu(music,artists[current_elem],genre);
			started = true;
			choosen_option = -1;
		}
	}
	system("cls");
}
void ArtistMenu::choose_what_to_run_from_artist_menu(const vector<MusicData*>& music,const wstring& artist,const wstring& genre)
{
	system("cls");
	wsvector text = 
	{
		L"What you want to see from "+artist+L":",
		L"Albums",
		L"All compositions",
	};
	int choosen_option = -1;
	
	int min = 1;
	int max = 2;
	int current = 1;
	
	bool started = true;
	while(true)
	{
		run_common_choosing_list(text,min,max,current,choosen_option);
		started = false;
		if(choosen_option == 1) run_list_albums(music,L"",artist);
		if(choosen_option == 2) run_list_titles(music,L"",artist,L"");
		if(choosen_option == -2) break;
	}
	system("cls");	
}
wsvector ArtistMenu::get_artists_data_from_music(const vector<MusicData*>& music,
												 const wstring& genre)
{
	wsvector data;
	for(auto& m:music)
	{
		bool already_added = find(data.begin(),data.end(),m->artist) != data.end();
		if(!already_added && genre.empty()) data.push_back(m->artist);
		else if(!already_added && genre == m->genre) data.push_back(m->artist);
	}
	return data;
}