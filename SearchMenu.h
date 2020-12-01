#ifndef SEARCH_MENU_H
#define SEARCH_MENU_H
#include"GenreMenu.h"
namespace Dwarf
{
class SearchMenu: public GenreMenu
{
public:
	SearchMenu();
	~SearchMenu();
	
	void run_search(const vector<MusicData*>& music);
protected:
	wstring get_wpath();
private:
	wspvector search(const vector<MusicData*>& music,
					 const wstring& source);
};
};
#endif //SEARCH_MENU_H