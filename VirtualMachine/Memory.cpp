#include"Memory.h"
using namespace Bubblegum::_VirtualMachine;

Memory::Memory()
{
}
Memory::~Memory()
{
}
void Memory::set_value(const string& val, const string& name)
{
	int type = get_type(val);
	switch(type)
	{
		case Integer:
		{
			iVar var = new Variable<int>(type,atoi(val.c_str()));
			int_memory[name] = var;
		}
		break;
		case Float:
		{
			fVar var = new Variable<float>(type,atof(val.c_str()));
			float_memory[name] = var;
		}
		break;
		case Bool:
		{
			bool _val = val == "true"?true:false;
			bVar var = new Variable<bool>(type,_val);
			bool_memory[name] = var;
		}
		break;
		case List:
		{
			vector<string> _val = separate_simple_list(val);
			listVar list = new Variable<vector<string>>(type,_val);
			list_memory[name] = list;
		}
		break;
		case String:
		{
			sVar var = new Variable<string>(type,val);
			string_memory[name] = var;
		}
		break;
	    case None:
        {
            none_variables.push_back(name);
        }
		break;
	}
}
void Memory::change_variable(const string& val, const string& name)
{
	int type = does_exist(name);
	if(type == -1)
	{
		vector<string> adds = {name};
		print_error(ErrorPrinter::VarDoesntExist,adds);
	}
	switch(type)
	{
		case Integer:
		{
			int _val = atoi(val.c_str());
			int_memory[name]->set_value(_val);
		}
		break;
		case Float:
		{
			float _val = atof(val.c_str());
			float_memory[name]->set_value(_val);
		}
		break;
		case String:
			string_memory[name]->set_value(val);
		break;
		case Bool:
		{
			bool _val = val == "true"?true:false;
			bool_memory[name]->set_value(_val);
		}
		break;
		case List:
		{
			vector<string> _val = separate_simple_list(val);
			list_memory[name]->set_value(_val);
		}
		break;
	}	
}
string Memory::get_value(const string& var_name, int type)
{
	bool exist = does_exist(var_name) != -1;
	if(!exist)
	{
		vector<string> adds = {var_name};
		print_error(ErrorPrinter::VarDoesntExist,adds);
	}
	switch(type)
	{
		case Integer:
		{
			int val = int_memory[var_name]->get_value();
			return to_string(val);
		}
		break;
		case Float:
		{
			float val = float_memory[var_name]->get_value();
			return to_string(val);
		}
		break;
		case String:
			return string_memory[var_name]->get_value();
		break;
		case Bool:
			return bool_memory[var_name]->get_value()==true?"true":"false";
		break;
		case List:
		{
			vector<string> list = list_memory[var_name]->get_value();
			string val = unite_list(list);
			return val;
		}
		break;
	    case None: return "None";
		default:
		{
			svector adds = {"passed code type doesn't exist!"};
			print_error(ErrorPrinter::IncorrectType,adds);
		};
	}
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
void Memory::destroy_variable(const string& name)
{
	int type = does_exist(name);
	if(type == -1)
	{
		svector adds={name};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	switch(type)
	{
		case Integer:
			int_memory.erase(name);
		break;
		case Float:
			float_memory.erase(name);
		break;
		case String:
			string_memory.erase(name);
		break;
		case Bool:
			bool_memory.erase(name);
		break;
		case List:
			list_memory.erase(name);
		break;
	    case None:
        {
            auto pos = find(none_variables.begin(),none_variables.end(),name);
            none_variables.erase(pos);
        }
        break;
	}
}
int Memory::does_exist(const string& var_name)
{
	bool is_int = int_memory.find(var_name) != int_memory.end();
	if(is_int) return Integer;
	
	bool is_float = float_memory.find(var_name) != float_memory.end();
	if(is_float) return Float;
	
	bool is_bool = bool_memory.find(var_name) != bool_memory.end();
	if(is_bool) return Bool;
	
	bool is_string = string_memory.find(var_name) != string_memory.end();
	if(is_string) return String;
	
	bool is_list = list_memory.find(var_name) != list_memory.end();
	if(is_list) return List;

	bool is_none = find(none_variables.begin(),none_variables.end(),var_name) != none_variables.end();
	if(is_none) return None;
	
	return -1;
}
vector<string> Memory::separate_simple_list(const string& list)
{
	string sub_list = get_substr(list,1,list.size()-1)+' ';
	
	vector<string> elems;
	string elem;
	for(auto ch:sub_list)
	{
		if(!isspace(ch))
			elem+=ch;
		else
		{
			elems.push_back(elem);
			elem.clear();
		}
	}
	return elems;
}
void Memory::clear()
{
	int_memory.clear();
	float_memory.clear();
	bool_memory.clear();
	string_memory.clear();
	list_memory.clear();
}
void Memory::print_memory()
{
    cout<<"INTEGERS"<<endl;
    for(auto val:int_memory)
    {
        cout<<val.first<<endl;
    }
}