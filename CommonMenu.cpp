#include"CommonMenu.h"
using namespace Dwarf;

CommonMenu::CommonMenu()
{
	standart    = Color(ColorCode::Black,ColorCode::White);
	green_label = Color(ColorCode::Black,ColorCode::Green);
	red_label   = Color(ColorCode::Black,ColorCode::Red);
	magenta_label=Color(ColorCode::Black,ColorCode::Magenta);
	empty       = Color(ColorCode::Black,ColorCode::Black);
}
CommonMenu::~CommonMenu()
{}

void CommonMenu::run_common_list(const wsvector& data,
								 int& choosen_option,
								 const string& no_elems_text,
								 int& current_elem,
								 int& start_counter,
								 int& max_counter,
								 const string& title)
{
	clear();
	COORD title_pos{0,0};
	COORD elem_pos{2,1};
	
	auto draw = [&]() 
	{
		draw_string(title,red_label,title_pos);
		for(int i = start_counter;i<max_counter;++i)
		{
			auto curr = data[i];
			if(curr.size() > 73) curr = get_substr(curr,0,73)+L"...";
			if(!can_wstring_be_converted_to_std(curr))
			{
				if(current_elem == i)
					draw_string(curr,green_label,elem_pos);
				else draw_string(curr,standart,elem_pos);
			}
			else
			{
				string text = convert_wstring_to_std(curr);
				if(current_elem == i)
					draw_string(text,green_label,elem_pos);
				else draw_string(text,standart,elem_pos);					
			}
				
			++elem_pos.Y;
		}
		elem_pos.Y = 1;
	};
	
	if(data.empty())
	{
		COORD pos{10,5};
		draw_string(no_elems_text,standart,pos);
	}
	else
	{
		draw();
	}
	int input = _getch();
	clear();
	
	if(input == ESCAPE) choosen_option = -2;
	if(input == MINUS && current_elem != 0) --current_elem;
	if(input == PLUS  && current_elem < max_counter-1)++current_elem;
	if(input == DOWN  && max_counter != data.size())
	{
		++max_counter;
		++start_counter;
	}
	if(input == UP    && start_counter != 0)
	{
		--max_counter;
		--start_counter;
	}
	if(input == ENTER) choosen_option = current_elem;
	if(input == A)
	{
		clear();
		wstring elem_to_add = data[current_elem];
		
		wsvector names = get_groups_name();
		wstring group_to_add= show_groups_to_choose(names);
		
		if(!group_to_add.empty())
			add_elem_to_group(elem_to_add,group_to_add);
	}
	
	draw();
	Sleep(20);
}
wstring CommonMenu::show_groups_to_choose(const wsvector& groups_names)
{
	while(true)
	{
		if(!groups_names.empty())
		{
			wsvector _groups_names = {L"groups:"};
			copy(groups_names.begin(),groups_names.end(),back_inserter(_groups_names));
			
			int min = 1;
			int max = _groups_names.size();
			int current = 1;
			int current_elem = -1;
			int choosen_option = -1;
			
			run_common_choosing_list(_groups_names,min,max,current,choosen_option);
			if(current == choosen_option) 
			{
				return _groups_names[current];
			}
			if(choosen_option == -2) return L"";
		}
		else
		{
			cout<<"no groups can be used to store objects!"<<endl;
			getch();
			return L"";
		}
	}
}
void CommonMenu::run_common_choosing_list(const wsvector& text,
										  int& min,
										  int& max,
										  int& current,
										  int& choosen_option)
{
	COORD pos{10,4};
	while(true)
	{
		for(size_t i = 0;i<text.size();++i)
		{
			auto curr = text[i];
			if(i == 1)
			{
				pos.X = 25;
				pos.Y = 10;
			}
			if(!can_wstring_be_converted_to_std(curr))
			{
				if(current == i)
					draw_string(curr,green_label,pos);
				else draw_string(curr,standart,pos);
			}
			else
			{
				string text = convert_wstring_to_std(curr);
				if(current == i)
					draw_string(text,green_label,pos);
				else draw_string(text,standart,pos);					
			}
				
			++pos.Y;
		}
		
		int input = _getch();
		if(input == UP &&  current != min)
		{
			--current;
		}
		if(input == DOWN && current != max)
		{ 
			++current;
		}
		if(input == ESCAPE) 
		{
			choosen_option = -2;
			break;
		}
		if(input == ENTER)
		{
			system("cls");
			choosen_option = current;
			break;
		}
		
		pos.Y = 4;
		pos.X = 10;
	}	
}
bool CommonMenu::can_wstring_be_converted_to_std(const wstring& str)
{
	for(auto& ch:str)
	{
		if(wctob(ch) == EOF) return false;
	}
	return true;
}
string CommonMenu::convert_wstring_to_std(const wstring& str)
{
	string new_str;
	for(auto& ch:str)
	{
		char new_ch = wctob(ch);
		new_str+=new_ch;
	}
	return new_str;
}
vector<wstring> CommonMenu::get_groups_name()
{
	vector<wstring> names; 
	for(auto& name:groups)
	{
		auto exists = find(names.begin(),names.end(),name.first) != names.end();
		if(!exists) names.push_back(name.first);
	}
	return names;
}
void CommonMenu::add_elem_to_group(const wstring& name, const wstring& group)
{
	if(is_group_existing(group))
	{
		groups[get_group_id(group)].second.push_back(name);
	}
}
bool CommonMenu::is_group_existing(const wstring& name)
{
	for(auto& _name:groups)
	{
		if(_name.first == name) return true;
	}
	return false;	
}
int CommonMenu::get_group_id(const wstring& group)
{
	for(auto i=0;i<groups.size();++i)
	{
		auto curr = groups[i];
		if(curr.first == group) return i; 
	}
	return -1;
}