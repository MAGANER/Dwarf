#include"ErrorPrinter.h"
#include"StrFunction.h"
#include"Variable.h"
#include"TypeChecker.h"
#include<vector>
#include<map>
#include<string>
#include <algorithm>
using namespace std;
using namespace Bubblegum::Special;

namespace Bubblegum
{
namespace _VirtualMachine
{
typedef Variable<int>*    iVar;
typedef Variable<float>*  fVar;
typedef Variable<string>* sVar;
typedef Variable<bool>*   bVar;
typedef Variable<vector<string> >* listVar;

typedef map<string, iVar> IM;
typedef map<string, fVar> FM;
typedef map<string, bVar> BM;
typedef map<string, sVar> SM;
typedef map<string, listVar> LM;	
	
class Memory: public ErrorPrinter,
		 	  public TypeChecker
{
private:
	IM int_memory;
	FM float_memory;
	BM bool_memory;
	SM string_memory;
	LM list_memory;

	svector none_variables;
public:
	Memory();
	~Memory();
	
	void set_value(const string& val, const string& name);
	void change_variable(const string& val, const string& name);
	string get_value(const string& var_name, int type);
	void destroy_variable(const string& name);

	int does_exist(const string& var_name);
	void clear();

	void print_memory();
private:
	vector<string> separate_simple_list(const string& list);
	;
};	
};
};
