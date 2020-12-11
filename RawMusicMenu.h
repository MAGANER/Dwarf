#ifndef RAW_MUSIC_MENU_H
#define RAW_MUSIC_MENU_H
#include"PathManager.h"
#include"PlayerMenu.h"
namespace Dwarf
{
class RawMusicMenu : public virtual PlayerMenu, public PathManager
{
public:
	RawMusicMenu();
	virtual ~RawMusicMenu();
	
	void show_raw_music_list();
private:
	wsvector make_printable_data();
};
};
#endif //RAW_MUSIC_MENU_H