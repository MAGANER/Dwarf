#include"GroupManager.h"
using namespace Dwarf;

GroupManager::GroupManager()
{
	load_groups();
}

GroupManager::~GroupManager()
{
}

void GroupManager::show_groups()
{	
	wsvector groups = get_groups_name();
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
			show_group_elements(this->groups[current_elem].second,this->groups[current_elem].first);
			started = true;
			choosen_option = -1;
		}
	}
	system("cls");
}
void GroupManager::show_group_elements(const wsvector& elems, const wstring& group_name)
{
	int max_counter    = elems.size() < visible_range? elems.size():visible_range;
	int choosen_option = -1;
	
	int start_counter = 0;
	int current_elem  = 0;
	bool started = true;
	
	while(true)
	{
		if(kbhit() || started)
		{
			run_common_list(elems,
							choosen_option,
							"there is no any group element!",
							current_elem,
							start_counter,
							max_counter,
							convert_wstring_to_std(group_name));
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
			
			vector<wstring> groups_names = get_groups_name();
			bool does_added = find(groups_names.begin(),groups_names.end(),_name) != groups_names.end();
			if(!does_added) groups.push_back(group_pair(_name,wsvector(0)));
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
wstring GroupManager::save_group_elements(const group_pair& group)
{
	wstring elements = L"(";
	for(auto& elem:group.second)
	{
		elements+=L"("+elem+L") ";
	}
	elements+=L")";
	return elements;
}
void GroupManager::save_groups()
{
	auto save =[&](const string& path)
	{
		ofstream file(path+"groups.txt");
		file.clear();
		for(auto& group:groups)
		{
			wstring name= group.first;
			wstring val =L"="+save_group_elements(group);
			
			string to_save = convert_wstring_to_std(name)+convert_wstring_to_std(val);
			file<<to_save;
			file<<endl;
		}
		file.close();		
	};
	save("data/");
	save("C:/dwarf_data/");
}
void GroupManager::load_groups()
{
	ifstream file;
	if(fs::exists("data/groups.txt")) file.open("data/groups.txt");
	else if(fs::exists("C:/dwarf_data/groups.txt"))file.open("C:/dwarf_data/groups.txt");
	
	
	while(file)
	{
		string text;
		getline(file,text);
		if(!is_spacefull(text))
		{
			auto eq_pos = text.find("=");
			string name = get_substr(text,0,eq_pos);
			string val  = get_substr(text,eq_pos+1,text.size());
			svector vals = separate_by_space(val);

			wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
			wstring _name = converter.from_bytes(name);
			
			wsvector _vals;
			for(auto& __val: vals) _vals.push_back(converter.from_bytes(__val));
				
			group_pair _pair(_name,_vals);
			groups.push_back(_pair);
		}
	}
}