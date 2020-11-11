/*
 * Bubblegum is script programming language.
 * Steps of programm processing:
 *   1) load programm as vector of lists
 *   2) iterate over vector, computing lists recursivly
 *  Every list has head, that tells which command must be used
 *  List can contains another lists.
 *
 */
#ifndef VirtualMachine_H
#define VirtualMachine_H
#include<algorithm>
#include<utility>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<map>
#include<typeinfo>
#include<limits>
#include<iomanip>
#include"StrFunction.h"
#include"TypeChecker.h"
#include"ErrorPrinter.h"
#include"Function.h"
#include"Module.h"

using namespace std;
using namespace Bubblegum::Special;
typedef vector<string> svector;

namespace Bubblegum
{
namespace _VirtualMachine
{
typedef map<string,Function*>* fn_map_ptr;

class VirtualMachine:public ErrorPrinter,
					 public TypeChecker
{
private:
	enum HeadType
	{
		var,
		fn,
		lambda,
		def_var,
		def_fn,
		print,
		printl,
		enter,
		init_var,
		_if,
		_loop,	
		add,
		substract,
		divide,
		multiplicate,
		_and,
		_or,
		_ex_or,
		_not,
		equal,
		more,
		less,
		more_equal,
		less_equal,
		get_elem,
		_break,
		_continue,
		_concat_list,
		_set,
		_return,
		_import,
		list_size_without_computation,
        list_size_with_computation,
        add_to_list_end,
        add_to_list_begin,
		_exit,
	};
	enum ExprHeadType
    {
	    get_var_val,
	    dont_process_list,
    };

	Memory* current_memory;
    fn_map_ptr current_function_map;
    fn_map_ptr current_lambda_map;

	map<string,Module*>   modules;

	bool function_computation_on;
	map<string,Memory*> sub_functions_memory;
	string current_sub_function;
public:
	VirtualMachine();
	~VirtualMachine();
	
	void run(const svector& script, const string& module_name);
	void run_interactive_mode();
	Module* get_module(const string& name);
    svector load_script(const string& path);
    void check_braces_equality(const svector& script);
private:
	//loading
	svector read_input(ifstream& file);
	void import_modules(const svector& args);
	//.
	
	//running
	string compute(const string& list);
	string process_list(const string& head,
					    const svector& args);
	string compute_expr(const string& val);

	int get_expr_type(const string&val);
	int get_head_type(const string& head);
	bool is_computable(const string& list);
	//.
	
	//computations
	pair<svector, vector<int> > get_vals_and_types(const svector& args);
	
	vector<int>   get_integer_val_vec(const svector& vals);
	vector<float> get_float_val_vec(const svector& vals);
	vector<bool>  get_bool_val_vec(const svector& vals);
	
	template<class T>
	bool is_val_too_low(T& val);
	
	template<class T>
	bool is_val_too_high(T& val);
	
	string compute_add(const svector& args);
	template<class T>
	string add_vals(const svector& vals);
	string add_strings(const svector& vals);
	void add_val_to_list_end(const svector& vals);
	void add_val_to_list_begin(const svector& vals);
	string get_var_name_for_adding(const svector& vals);
	
	string compute_substract(const svector& args);
	template<class T>
	string distract_vals(const svector& vals);
	
	string compute_multiplication(const svector& args);
	template<class T>
	string multiplicate_vals(const svector& vals);
	
	string compute_dividing(const svector& args);
	template<class T>
	string divide_vals(const svector& vals);
	
	string compute_and(const svector& args);
	string compute_or(const svector& args);
	string compute_xor(const svector& args);
	string compute_not(const svector& args);
	
	string compute_equality(const svector& args);
	string compute_more(const svector& args);
	string compute_more_equal(const svector& args);
	template<typename T>
	string is_a_more_than_b(const svector& args);
	
	string compute_print(const svector& args, bool move_to_next_line);
	void compute_enter(const svector& args);
	
	string compute_if(const svector& args);
	
	void compute_loop(const svector& args);
		
	string get_list_elem(const string& head,const svector& args);
	
	void create_variable(const svector& args);
	void create_function(const svector& args);
	
	string concat_list(const svector& args);
	void set_val_to_list(const svector& args);

	string compute_list_size_without_computation(const svector& args);
    string compute_list_size_with_computation(const svector& args);

    string compute_function(const string& head, const svector& _args,Function* fn);
    string compute_sub_function(const string& head, const svector& _args, Function* fn,string& fn_name);
    string process_variable(const string& head,const svector& args);

    string get_variable_value(const string& head);
    string change_memory(const string& head);
	
	string compute_return(const svector& args);
	//.

	//head types
	bool is_var_calling_head(const string& head);
	int process_fn_to_call(const string& head);
	//.
	
	//common
	template<class T>
	bool does_vec_has_equal_elems(const vector<T>& vec);
	void is_computable_data_type_correct(const vector<int>& vec,
										 const svector& vals);

	Memory* get_current_memory();
    fn_map_ptr get_current_lambda_map();
	
	bool does_list_has_equal_braces_number(const string& list);
	//.
	
	//interactive mode specific methods
	string get_input();
	//.
	
};
};	
};
#endif //VirtualMachine_H
