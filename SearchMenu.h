#ifndef SEARCH_MENU_H
#define SEARCH_MENU_H
#include"GenreMenu.h"
namespace Dwarf
{
class SearchResultData
{
public:
	enum SearchDataType
	{
		Artist,
		Album,
		Title,
		Genre
	};
	int type;
	
	MusicData* data;
	
	SearchResultData(int type, 
					 MusicData* data)
	{
		this->type = type;
		this->data = data;
	}
	~SearchResultData(){}
};
	
class SearchMenu: public GenreMenu
{
public:
	SearchMenu();
	~SearchMenu();
	
	void run_search(const vector<MusicData*>& music);
protected:
	wstring get_wpath();
private:
	vector<SearchResultData*> search(const vector<MusicData*>& music,
									 const wstring& source);
};
};

#endif //SEARCH_MENU_H