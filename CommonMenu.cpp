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
	
	draw();
	Sleep(40);
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