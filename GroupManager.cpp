#include"GroupManager.h"
using namespace Dwarf;

GroupManager::GroupManager()
{}

GroupManager::~GroupManager()
{}

void GroupManager::show_groups()
{	
	int max_counter    = groups.size() < visible_range? groups.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	bool started = true;
	
	while(true)
	{
		if(kbhit() || started)
		{
			run_common_list(groups,
							choosen_option,
							"there is no any group!",
							current_elem,
							start_counter,
							max_counter,
							"groups:");
			started = false;
		}
		if(choosen_option == -2) break;
		if(choosen_option == current_elem)
		{		
			system("cls");
			started = true;
			choosen_option = -1;
		}
	}
	system("cls");
}
void GroupManager::run_group_menu()
{
	COORD text_pos = {20,8};
	while(true)
	{
		draw_string("Group Menu",red_label,main_label_pos);
		draw_string("press n to create new group",green_label,help1_pos);
		draw_string("press a to add anything in list menu",green_label,help2_pos);
		draw_string("use arrows to move",green_label,help3_pos);
		draw_string("press space to see groups",green_label,help4_pos);
		
		svector text = {
			"Group is not a primitive playlist made of songs.",
			"For sure, you can add song, but also whole album",
			"artist or even a genre. That is a group."
					   };				   
		for(size_t i = 0;i<text.size();++i)
		{	
			draw_string(text[i],standart,text_pos);
			++text_pos.Y;
		}
		text_pos.Y = 8;
		
		int input = getch();
		
		if(input == ESCAPE) break;
		if(input == N)
		{
			system("cls");
			
			cout<<"enter group name:";
			string name = get_input();
			
			wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
			wstring _name = converter.from_bytes(name);
			
			bool does_added = find(groups.begin(),groups.end(),_name) != groups.end();
			if(!does_added) groups.push_back(_name);
			system("cls");
		}
		if(input == SPACE) show_groups();
		
	}
}
string GroupManager::get_input()
{
	string path;
	const int size = sizeof(istream)/sizeof(char);
	char* buffer = new char[size];
	cin.getline(buffer,size);
	path = buffer;
	delete buffer;
	return path;
}