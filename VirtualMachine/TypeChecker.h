#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H
#include<string>
#include<cstring>
#include"StrFunction.h"
using namespace std;
namespace Bubblegum
{
namespace _VirtualMachine
{
class TypeChecker
{
public:
	TypeChecker();
	~TypeChecker();
	
	enum Type
	{
		Integer,
		Float,
		String,
		Bool,	
		List,
		None,
		Expr,
	};
	int get_type(const string& val);
private:
	bool is_val_negative_number(const string& val);
	bool is_val_integer(const string& val);
	bool is_val_float(const string& val);
	bool is_val_bool(const string& val);
	bool is_val_string(const string& val);
	bool is_val_list(const string& val);
	bool is_val_none(const string& val);
	bool is_val_expression(const string& val);
};
};	
};
#endif //TYPE_CHECKER_H
