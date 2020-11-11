#include"TypeChecker.h"
using namespace Bubblegum::_VirtualMachine;

TypeChecker::TypeChecker()
{
}
TypeChecker::~TypeChecker()
{
}


bool TypeChecker::is_val_bool(const string& val)
{
	return val == "true" || val == "false";
}

bool TypeChecker::is_val_negative_number(const string& val)
{
	return val[0] == '-';
}
bool TypeChecker::is_val_integer(const string& val)
{
	string _val = is_val_negative_number(val)?
					get_substr(val,1,val.size()):val;
	for(auto ch: _val)
	{
		if(!isdigit(ch))
			return false;
	}
	return true;
}
bool TypeChecker::is_val_float(const string& val)
{
	bool has_point = false;
	
	string _val = is_val_negative_number(val)?
					get_substr(val,1,val.size()):val;
	for(auto ch: _val)
	{
		if(!isdigit(ch))
		{
			//must be point
			if(ch != '.')
				return false;
			else
				has_point = true;
		}
	}
	return true && has_point;
}
bool TypeChecker::is_val_string(const string& val)
{
	bool first_symbol = val[0] == '"';
	bool last_symbol  = *--val.end() == '"';
		
	return first_symbol && last_symbol;
}
bool TypeChecker::is_val_list(const string& val)
{
	bool first_symbol = val[0] == '(';
	bool last_symbol  = *--val.end() == ')';

	return first_symbol && last_symbol;
}
bool TypeChecker::is_val_expression(const string& val)
{
    bool first_case = *val.begin() == '$';
    return first_case;
}
bool TypeChecker::is_val_none(const string &val)
{
    return val == "None";
}
int TypeChecker::get_type(const string& val)
{
	if(is_val_bool(val))
		return Bool;
	if(is_val_integer(val))
		return Integer;
	if(is_val_float(val))
		return Float;
	if(is_val_string(val))
		return String;
	if(is_val_list(val))
		return List;
	if(is_val_none(val))
	    return None;
	if(is_val_expression(val))
	    return Expr;
		
	return -1;
}


