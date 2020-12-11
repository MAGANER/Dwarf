#include"RawMusicMenu.h"
using namespace Dwarf;

RawMusicMenu::RawMusicMenu(){}
RawMusicMenu::~RawMusicMenu(){}


void RawMusicMenu::show_raw_music_list()
{
	wsvector rmusic = make_printable_data();
	
	int max_counter    = rmusic.size() < visible_range? rmusic.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	bool started = true;
	
	while(true)
	{
		if(kbhit() || started)
		{
			run_common_list(rmusic,
							choosen_option,
							"there is no any raw music file!",
							current_elem,
							start_counter,
							max_counter,
							"Raw music:");
			started = false;
		}
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{		
			system("cls");
			
			wstring output_path = rmusic[current_elem];
			wstring real_path   = raw_music[current_elem];
			play_raw_music(output_path,real_path);
			
			started = true;
			choosen_option = -1;
		}
	}
	system("cls");
}
wsvector RawMusicMenu::make_printable_data()
{
	wsvector data;
	for(auto& m:raw_music)
	{
		auto last_slash = m.find_last_of(L"/");
		if(last_slash != wstring::npos) data.push_back(get_substr(m,last_slash+1,m.size()));
		else data.push_back(m);
	}
	return data;
}