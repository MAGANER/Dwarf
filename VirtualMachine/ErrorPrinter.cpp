#include"ErrorPrinter.h"
using namespace Bubblegum::Special;
ErrorPrinter::ErrorPrinter()
{
	error_table = 
	{
		{IncorrectBraces, "incorrect braces!"},
		{NoFile         , "no such file!"},
		{IncorrectHead  , "no such head!"},
		{IncorrectType  , "type of value is incorrect!"},
		{DifferentTypes , "different types of values!"},
		{IncorrectArgs  , "incorrect arguments!"},
		{VarDoesntExist , "variable does not exist!"},
		{VarAlreadyExist, "variable already exist!"},
		{FnAlreadyExist,  "function already exist!"},
        {NoModule,        "there is no correct module!"},
        {FnDoesntExist,   "function does not exist!"},
        {EmptyList,       "List variable is empty!"},
        {IncorrectExpression, "incorrect expression!"},
		{OutOfLimit,	  "the value is out of type limit!"},
	};
}
ErrorPrinter::~ErrorPrinter()
{
}

void ErrorPrinter::print_error(int type, const svector& adds)
{
	cout<<"Error#"<<type<<" "<<error_table[type]<<endl;
	for(auto add:adds)
		cout<<add<<endl;
	
	exit(-1);
}
