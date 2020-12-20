#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H
#include"CommonMenu.h"
#include"VirtualMachine/StrFunction.h"
#include<codecvt>
#include<locale>
#include<filesystem>
#include<fstream>

namespace fs=std::filesystem;
namespace Dwarf
{
class GroupManager : public virtual CommonMenu
{
protected:
	bool play_group_elem = false;
	wstring current_group_elem;
private:
	COORD main_label_pos = {35, 5};
	COORD help1_pos = {25,15};
	COORD help2_pos = {25,16};
	COORD help3_pos = {25,17};
	COORD help4_pos = {25,18};
	
public:
	GroupManager();
	virtual ~GroupManager();
	
	void show_groups();
	void run_group_menu();
	string get_input();
	
	void load_groups();
	void save_groups();
private:
	wstring save_group_elements(const group_pair& group);
	void show_group_elements(const wsvector& elems,const wstring& group_name);
	wstring clear_braces(const wstring& wstr);
};
};
#endif //GROUP_MANAGER_H