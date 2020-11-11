#ifndef FUNCTION_H
#define FUNCTION_H
#include<string>
#include<vector>
#include"Memory.h"
typedef vector<string> svector;

namespace Bubblegum
{
namespace _VirtualMachine
{
class Function
{
private:
	svector args;
	svector body;
	string name;
public:
	Memory local_memory;
	map<string, Function*> lambdas;

	Function(const svector& args,
			 const svector& body,
			 const string& name)
	{
		this->args = args;
		this->body = body;
		this->name = name;
	};
	~Function()
	{
	};
	
	svector get_args(){return args;};
	svector get_body(){return body;};
};	
};
};
#endif //FUNCTION_H
