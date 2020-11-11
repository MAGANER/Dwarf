#ifndef ERROR_PRINTER_H
#define ERROR_PRINTER_H
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<cstdlib>
namespace Bubblegum
{
using namespace std;
typedef vector<string> svector;

namespace Special
{
class ErrorPrinter
{
private:
	map<int,string> error_table;
public:
	enum ErrorType
	{
		IncorrectBraces,
		NoFile,	
		IncorrectHead,
		IncorrectType,
		DifferentTypes,
		IncorrectArgs,
		VarDoesntExist,
		VarAlreadyExist,
		FnAlreadyExist,
		NoModule,
		FnDoesntExist,
		EmptyList,
		IncorrectExpression,
		OutOfLimit,
	};
	
	ErrorPrinter();
	~ErrorPrinter();
	void print_error(int type,const svector& adds);
};	
};	
};
#endif //ERROR_PRINTER_H
