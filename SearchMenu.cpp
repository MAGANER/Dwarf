#include"SearchMenu.h"
using namespace Dwarf;

SearchMenu::SearchMenu(){}
SearchMenu::~SearchMenu(){}

void SearchMenu::run_search(const vector<MusicData*>& music)
{
	COORD pos = {5,10};
	draw_string("look for:",standart, pos);
	wstring input = get_wpath();
	
	vector<SearchResultData*> results = search(music,input);
	wsvector  text_only;
	for(auto& s:results)
	{
		if(s->type == SearchResultData::SearchDataType::Genre)
		{
			text_only.push_back(L"genre:"+s->data->genre);
		}
		if(s->type == SearchResultData::SearchDataType::Artist)
		{
			text_only.push_back(L"artist:"+s->data->artist);
		}
		if(s->type == SearchResultData::SearchDataType::Album)
		{
			text_only.push_back(L"album:"+s->data->album);
		}
		if(s->type == SearchResultData::SearchDataType::Title)
		{
			text_only.push_back(L"title:"+s->data->title);
		}
	}
	
	int choosen_option = -1;
	int current_elem = 0;
	int start_counter = 0;
	int max_counter   = results.size()< visible_range? results.size():visible_range;
	while(true && !results.empty())
	{
		run_common_list(text_only,
						choosen_option,
						"there is no any similar!",
						current_elem,
						start_counter,
						max_counter,
						"Found:");
		
		auto curr = results[current_elem];
		auto type = curr->type;
		bool list_item = choosen_option == current_elem;
		
		if(choosen_option == -2) break;
		if(list_item && type == SearchResultData::SearchDataType::Genre)
		{
			clear();
			
			choose_what_to_run_from_genre_menu(music,curr->data->genre);
			choosen_option = -1;
		}
		if(list_item && type == SearchResultData::SearchDataType::Title)
		{
			clear();
		
			wspair data(curr->data->title,curr->data->path);
			run_playing_composition(music,curr->data->artist,curr->data->album,data);
			choosen_option = -1;
		}
		if(list_item && type == SearchResultData::SearchDataType::Album)
		{
			clear();
			
			run_list_titles(music,curr->data->genre,curr->data->artist,curr->data->album);
			choosen_option = -1;
		}
		if(list_item && type == SearchResultData::SearchDataType::Artist)
		{
			clear();
			
			choose_what_to_run_from_artist_menu(music,curr->data->artist,curr->data->genre);
			choosen_option = -1;
		}
	}

	clear();
}
vector<SearchResultData*> SearchMenu::search(const vector<MusicData*>& music,
					     const wstring& source)
{
	vector<SearchResultData*> results;
	wspvector cash;
	for(auto& file:music)
	{
		bool _artist = file->artist.find(source) != wstring::npos;
		bool _album  = file->album.find(source)  != wstring::npos;
		bool _genre  = file->genre.find(source)  != wstring::npos;
		bool _title  = file->title.find(source)  != wstring::npos;
		
		string type;
		if(_artist) type+="1";
		if(_album)  type+="2";
		if(_genre)  type+="3";
		if(_title)  type+="4";
		
		int end = string::npos;
		bool is_artist = type.find('1') != end;
		bool is_album  = type.find('2') != end;
		bool is_genre  = type.find('3') != end;
		bool is_title  = type.find('4') != end;
		
		if(is_artist)
		{
			SearchResultData* val = new SearchResultData(SearchResultData::SearchDataType::Artist,file);
			wspair added(L"1",file->artist);
			bool not_added = find(cash.begin(),cash.end(),added) == cash.end();
			if(not_added)
			{
				cash.push_back(added);
				results.push_back(val);
			}
		}
		if(is_album)
		{
			SearchResultData* val = new SearchResultData(SearchResultData::SearchDataType::Album,file);
			wspair added(L"2",file->album);
			bool not_added = find(cash.begin(),cash.end(),added) == cash.end();
			if(not_added)
			{
				cash.push_back(added);
				results.push_back(val);
			}
		}
		if(is_genre)
		{
			SearchResultData* val = new SearchResultData(SearchResultData::SearchDataType::Genre,file);
			wspair added(L"3",file->genre);
			bool not_added = find(cash.begin(),cash.end(),added) == cash.end();
			if(not_added)
			{
				cash.push_back(added);
				results.push_back(val);
			}
		}
		
		if(is_title)
		{
			SearchResultData* val = new SearchResultData(SearchResultData::SearchDataType::Title,file);
			wspair added(L"4",file->title);
			bool not_added = find(cash.begin(),cash.end(),added) == cash.end();
			if(not_added)
			{
				cash.push_back(added);
				results.push_back(val);
			}
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
								 
