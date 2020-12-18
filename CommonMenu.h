#ifndef COMMON_MENU_H
#define COMMON_MENU_H
#include<functional>
#include<cstdlib>
#include<algorithm>
#include<cwchar>
#include"Console.h"
#include"VirtualMachine/StrFunction.h"

namespace Dwarf
{
typedef pair<wstring, vector<wstring> > group_pair; 
typedef std::vector<wstring> wsvector;	

class CommonMenu: public Console
{
protected:
	Color standart, green_label, red_label, empty, magenta_label;
	const int visible_range = 28;
	
	vector<group_pair> groups;
	vector<wstring> get_groups_name();
	void add_elem_to_group(const wstring& name, const wstring& group);
	bool is_group_existing(const wstring& name);
	int get_group_id(const wstring& group);
protected:
	bool can_wstring_be_converted_to_std(const wstring& str);
	string convert_wstring_to_std(const wstring& str);
public:
	CommonMenu();
	virtual ~CommonMenu();

	void run_common_list(const wsvector& data,
						 int& choosen_option,
						 const string& no_elems_text,
						 int& current_elem,
						 int& start_counter,
						 int& max_counter,
						 const string& title);
						 
	void run_common_choosing_list(const wsvector& text,
								  int& min,
								  int& max,
								  int& current,
								  int& choosen_option);
								  
	wstring show_groups_to_choose(const wsvector& groups_names); 
};
};
#endif //COMMON_MENU_H