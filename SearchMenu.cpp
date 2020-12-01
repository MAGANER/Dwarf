#include"SearchMenu.h"
using namespace Dwarf;

SearchMenu::SearchMenu(){}
SearchMenu::~SearchMenu(){}

void SearchMenu::run_search(const vector<MusicData*>& music)
{
	COORD pos = {5,10};
	draw_string("look for:",standart, pos);
	wstring input = get_wpath();
	
	wspvector results = search(music,input);
	wsvector  text_only;
	for(auto& s:results)text_only.push_back(s.second);
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = results.size()< visible_range? results.size():visible_range;
	while(true)
	{
		run_common_list(text_only,
						choosen_option,
						"there is no any similar!",
						current_elem,
						start_counter,
						max_counter,
						"Found:");
		
		auto type = results[current_elem].first;
		bool list_item = choosen_option == current_elem;
		if(choosen_option == -2) break;
		if(list_item && type == L"genre")
		{
			clear();
			choose_what_to_run_from_genre_menu(music,results[current_elem].second);
		}
	}

	clear();
}
wspvector SearchMenu::search(const vector<MusicData*>& music,
							 const wstring& source)
{
	wspvector results;
	for(auto& file:music)
	{
		bool _artist = file->artist.find(source) != wstring::npos;
		bool _album  = file->album.find(source)  != wstring::npos;
		bool _genre  = file->genre.find(source)  != wstring::npos;
		bool _title  = file->title.find(source)  != wstring::npos;
		
		if(_artist)
		{
			auto val = wspair(L"artist",file->artist);
			bool not_added = find(results.begin(),results.end(),val) == results.end();
			if(not_added)results.push_back(val);
		}
		if(_album)
		{
			auto val = wspair(L"album",file->album);
			bool not_added = find(results.begin(),results.end(),val) == results.end();
			if(not_added)results.push_back(val);
		}
		if(_genre)
		{
			auto val = wspair(L"genre",file->genre);
			bool not_added = find(results.begin(),results.end(),val) == results.end();
			if(not_added)results.push_back(val);
		}
		if(_title) 
		{
			auto val = wspair(L"title",file->title);
			bool not_added = find(results.begin(),results.end(),val) == results.end();
			if(not_added)results.push_back(val);
		}
	}
	return results;		
}
wstring SearchMenu::get_wpath()
{
	wstring path;
	const int size = sizeof(istream)/sizeof(wchar_t);
	wchar_t* buffer = new wchar_t[size];
	wcin.getline(buffer,size);
	path = buffer;
	delete buffer;
	return path;
}
								 