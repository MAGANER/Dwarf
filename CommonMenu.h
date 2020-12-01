#ifndef COMMON_MENU_H
#define COMMON_MENU_H
#include<functional>
#include<cstdlib>
#include<windows.h>
#include<algorithm>
#include<cwchar>
#include"Console.h"
#include"VirtualMachine/StrFunction.h"

using namespace std;
namespace Dwarf
{
typedef vector<wstring> wsvector;	

class CommonMenu: public Console
{
protected:
	Color standart, green_label, red_label, empty, magenta_label;
	const int visible_range = 28;
protected:
	bool can_wstring_be_converted_to_std(const wstring& str);
	string convert_wstring_to_std(const wstring& str);
public:
	CommonMenu();
	~CommonMenu();

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
};
};
#endif //COMMON_MENU_H