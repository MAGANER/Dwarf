/*
 This header contains simple,
 but most used string functions
*/

#ifndef STR_FUNCTION
#define STR_FUNCTION
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;

typedef string::size_type f_pos;
typedef vector<string> svector;
namespace
{
bool is_string_digit(const string& str)
{
	//check every character 
	for(auto& ch:str)
	{
		if(!isdigit(ch))
			return false;
	}
	return true;
}	
string get_substr(const string& str,int begin, int end)
{
	/*i hate standart substr method, cos it takes
	  start and the len of substring. it's awful
	*/
	string sub;
	for(size_t i = begin;i<end;++i)
	{
		sub.push_back(str[i]);
	}
	return sub;
}
f_pos find_substr(const string& str, const string& sub)
{
	//actualy, it can be deleted
	//cos standart way is ok
	size_t sub_size = sub.size();
	for(size_t n=0;n<str.size();++n)
	{
		bool not_out_of_range = n+sub_size < str.size();
		if(not_out_of_range)
		{
			
			string variant = get_substr(str,n,n+sub_size);
			if(variant == sub)
			{
				return n;
			}
		}
	}
	return -1;
}
f_pos find_first_not_space_symb(const string& str)
{
	for(size_t i= 0;i<str.size();++i)
	{
		if(!isspace(str[i]))
			return i;
	}
	return -1;
}
string clear_spaces(const string& str)
{
	//recopy string without space symbol
	string cleared_str;
	for(size_t i =0;i<str.size();++i)
	{
		if(!isspace(str[i]))
			cleared_str.push_back(str[i]);
	}
	return cleared_str;
}
bool is_spacefull(const string& line)
{
	int space_counter = 0;
	for(auto ch:line)
	{
		if(isspace(ch))
			space_counter++;
	}
	
	return space_counter == line.size();
}
void delete_empty_lines(svector& vec)
{
	for(size_t i =0;i<vec.size();++i)
	{
		auto line = vec[i];
		bool spaces_only = is_spacefull(line);	
		if(spaces_only)
			vec.erase(vec.begin()+i);
			
	}
}
svector separate_by_space(const string& list)
{
	string sub_list = get_substr(list,1,list.size()-1);
	
	svector elems;
	string elem;
	
	int open_counter = 0;
	int close_counter= 0;
	
	int str_open_counter = 0;
	int str_close_counter= 0;
	
	bool str_open_counted = false;
	bool str_close_counted= false;
	for(auto i=0;i<sub_list.size();++i)
	{		
		auto ch = sub_list[i];
		if(ch == '(') ++open_counter;
		if(ch == ')') ++close_counter;
		
		if(ch != '(' && open_counter == 0)
		{
			if(ch == '"' && !str_open_counted)
			{
				++str_open_counter;	
				str_open_counted = true;
			}
			else if(ch == '"' && str_open_counter != 0 && !str_close_counted)
			{
				++str_close_counter;
				str_close_counted = true;
			}
				
			if(str_open_counter == str_close_counter)
			{
				str_open_counter = 0;
				str_close_counter= 0;
				str_close_counted = false;
				str_open_counted  = false;
			}
			
			bool is_str = str_open_counter != 0 && isspace(ch);
							 
			if(!isspace(ch) || is_str)
				elem+=ch;
			else
			{
				if(!elem.empty())
					elems.push_back(elem);
				elem.clear();
				str_open_counter = 0;
				str_close_counter= 0;
			}
			
			if(i+1 == sub_list.size())
			{
				elem+=ch;
				string _elem(elem.begin(),--elem.end());
				if(!elem.empty())
					elems.push_back(_elem);
			}
		}
		else
		{
			if(open_counter != close_counter)
				elem+=ch;
			else
			{
				elems.push_back(elem+')');
				elem.clear();
				open_counter = 0;
				close_counter =0;
			}
		}
	}
	
	delete_empty_lines(elems);
	return elems;
}
string unite_list(const vector<string>& list)
{
	string res="(";
	for(auto elem:list)
		res+=elem+" ";
	res+=")";
	return res;
}
bool list_is_empty(const string& list)
{
    int open_number = std::count(list.begin(),list.end(),'(');
    int close_number= std::count(list.begin(),list.end(),')');
    int spaces = std::count(list.begin(),list.end(),' ');
    return (open_number+close_number+spaces) == list.size();
}

};
#endif //STR_FUNCTION
