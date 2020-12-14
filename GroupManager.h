#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H
#include"CommonMenu.h"
#include<codecvt>
#include<locale>
#include<filesystem>

namespace fs=std::filesystem;
namespace Dwarf
{
class GroupManager : public virtual CommonMenu
{
private:
	COORD main_label_pos = {35, 5};
	COORD help1_pos = {25,15};
	COORD help2_pos = {25,16};
	COORD help3_pos = {25,17};
	COORD help4_pos = {25,18};
protected:
	wsvector groups;
public:
	GroupManager();
	virtual ~GroupManager();
	
	void show_groups();
	void run_group_menu();
	
	string get_input();
private:
	void load_groups();
	void save_groups();

};
};
#endif //GROUP_MANAGER_H