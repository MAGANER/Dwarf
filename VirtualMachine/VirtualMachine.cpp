#include"VirtualMachine.h"
using namespace Bubblegum::_VirtualMachine;

VirtualMachine::VirtualMachine()
{
    Module* main = new Module();
    modules["main"] = main;


	current_memory = &modules["main"]->memory;
    current_function_map = &modules["main"]->functions;

    function_computation_on = false; 
}
VirtualMachine::~VirtualMachine()
{
    current_function_map = nullptr;
    current_memory = nullptr;
}
svector VirtualMachine::load_script(const string& path)
{
	//load script by strings

	ifstream file;
	file.open(path.c_str());
	
	if(file.fail())
	{
		svector adds = {path};
		print_error(ErrorPrinter::NoFile,adds);
	}
	
	svector script = read_input(file);
	file.close();
	return script;
}
svector VirtualMachine::read_input(ifstream& file)
{
    /*
     read file by list
     when number of ( equals to number of )
     list is completed, so save it
     */
	string list;
	svector script;
	int open_counter  = 0;
	int close_counter = 0;

	int comment_counter = 0;
	while(file)
	{
		char ch;
		file.get(ch);

		if(ch == '#')comment_counter++;
		if(ch == '(')open_counter++;
		if(ch == ')')close_counter++;

		if((open_counter != 0 || close_counter != 0) && comment_counter == 0)
			list.push_back(ch);
		if(comment_counter == 2)
		    comment_counter = 0;

		if(open_counter == close_counter)
		{
			script.push_back(list);
			list.clear();
			open_counter  = 0;
			close_counter = 0;
		}
	}
	return script;
}
Module* VirtualMachine::get_module(const string &name)
{
    bool exist = modules.find(name) != modules.end();
    if(exist)
        return modules[name];
    else
        return nullptr;
}
void VirtualMachine::check_braces_equality(const svector& script)
{
	//number of open braces must be equal to close ones' number
	for(auto& list:script)
	{
		bool equal = does_list_has_equal_braces_number(list); 
		if(!equal)
		{
			svector adds ={"not equal number of braces!",list};
			print_error(ErrorPrinter::IncorrectBraces,adds);
		}
	}
}

void VirtualMachine::run(const svector& script,const string& module_name)
{
    Module* module = get_module(module_name);
    if(module == nullptr)
    {
        svector adds = {"module can not be loaded!it doesn't exist",module_name};
        print_error(ErrorPrinter::NoModule,adds);
    }
    current_memory = &module->memory;
    current_function_map =&module->functions;

	for(auto list:script)
	{
		if(is_computable(list))
		{
			string result = compute(list);
		}
	}
}
bool VirtualMachine::is_computable(const string& list)
{
	int open = count(list.begin(),list.end(),'(');
	int close= count(list.begin(),list.end(),')');
	return open != 0 && close != 0;
}
string VirtualMachine::compute(const string& list)
{
    if(get_type(list) == Expr) return compute_expr(list);

	svector _list = separate_by_space(list);
	string head = _list[0];

	svector args = svector(++_list.begin(),_list.end());

	bool not_break    = head != "break";
	bool not_continue = head != "continue";
	bool not_cycle_management = not_break && not_continue;
	bool not_exit = head != "exit";

	bool not_curr_var =  get_current_memory()->does_exist(head) == -1;
	bool not_other_var = head.find("~") == string::npos;
	bool not_var = not_curr_var && not_other_var;

	if(args.empty() && not_var && not_cycle_management && not_exit)
    {
		svector adds={"it can not get zero arguments!",head,list};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}

	return process_list(head,args);	
}
string VirtualMachine::compute_expr(const string& val)
{
    int type = get_expr_type(val);
    switch(type)
    {
        case ExprHeadType::dont_process_list: return get_substr(val,2,val.size());
        case ExprHeadType::get_var_val: return get_variable_value(val);
		default:
		{
			svector adds = {"type of expression is incorrect!:"+to_string(type)};
			print_error(ErrorPrinter::IncorrectType,adds);
		};
    };
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
int VirtualMachine::get_expr_type(const string& val)
{
    if(val[1] == '&') return ExprHeadType::dont_process_list;
    if(*val.begin() == '$')return ExprHeadType::get_var_val;
	
	return -1;
}
string VirtualMachine::process_list(const string& head,
				          		    const svector& args)
{
    //below kind of map head -> command

	int head_type = get_head_type(head);
	if(head_type == -1)
	{
		svector adds ={head};
		print_error(ErrorPrinter::IncorrectHead,adds);
	}
	switch(head_type)
	{
	    //change value, get value
		case HeadType::var:
		    return process_variable(head,args);
		    break;
		//compute functions
	    case HeadType::lambda:
        {
            string fn_name = head.substr(1, head.size());
            Function *fn = (*current_lambda_map)[fn_name];
            if(!function_computation_on)
                return compute_function(head, args, fn);
            else
                return compute_sub_function(head,args,fn,fn_name);
        }
        break;
		case HeadType::fn:
		{
            string fn_name = head.substr(0, head.size() - 1);
            int mod_pos = fn_name.find("~");
            if (mod_pos != string::npos) {
                fn_name = get_substr(fn_name, mod_pos + 1, fn_name.size());
            }
            Function *fn = (*current_function_map)[fn_name];
		    if(!function_computation_on)
                return compute_function(head, args, fn);
		    else
                return compute_sub_function(head,args,fn,fn_name);
		}
		break;
		case HeadType::def_var:{
            create_variable(args);
            return "";
        }
		break;
		case HeadType::def_fn: {
            create_function(args);
            return "";
        }
		break;
		case HeadType::print:
			return compute_print(args,false);
		break;
		case HeadType::printl:
			return compute_print(args,true);
		break;
		case HeadType::enter:
			compute_enter(args);
			return "";   
		break;
		case HeadType::_if:
			return compute_if(args);     
		break;
		case HeadType::_loop:
		{
			compute_loop(args);
			return "";
		}
		break;
		case HeadType::add: 
			return compute_add(args); 
		break;
		case HeadType::substract:
			return compute_substract(args);
		break;
		case HeadType::divide:
			return compute_dividing(args);   
		break;
		case HeadType::multiplicate:
			return compute_multiplication(args); 
		break;
		case HeadType::_and:
			return compute_and(args);         
		break;
		case HeadType::_or:
			return compute_or(args);          
		break;
		case HeadType::_ex_or:
			return compute_xor(args);       
		break;
		case HeadType::_not:
			return compute_not(args);         
		break;
		case HeadType::equal:
			return compute_equality(args);        
		break;
		case HeadType::more:
			return compute_more(args);
		break;
		case HeadType::less:
		{
			string res = compute_more(args);
			return res=="true"?"false":"true";
		}
		break;
		case HeadType::more_equal:
			return compute_more_equal(args);
		break;
		case HeadType::less_equal:
		{
			string res = compute_more_equal(args);
			return res=="true"?"false":"true";
		}
		break;
		case HeadType::get_elem:
			return get_list_elem(head,args);
		break;
		case HeadType::_break:
		{
			if(!args.empty())
			{
				svector adds = {head,"can not take any argument!"};
				print_error(ErrorPrinter::IncorrectArgs,adds);
			}
			return "break";
		}
		break;
		case HeadType::_continue:
		{
			if(!args.empty())
			{
				svector adds = {head,"can not take any argument!"};
				print_error(ErrorPrinter::IncorrectArgs,adds);	
			}
			return "continue";
		}
		break;
		case HeadType::_concat_list:
		{
			return concat_list(args);
		}
		break;
		case HeadType::_set:
		{
			set_val_to_list(args);
			return "";
		}
		break;
		/*
		 if called inside of function, breaks it, then pass computed value
		 else just return the value
		 */
		case HeadType::_return:
		{
			return compute_return(args);
		}
		break;
	    case HeadType::_import:
	    {
            import_modules(args);
            return "";
	    }
	    break;
	    case HeadType::list_size_without_computation:
            return compute_list_size_without_computation(args);
        break;
	    case HeadType::list_size_with_computation:
	        return compute_list_size_with_computation(args);
	    break;
	    case HeadType::add_to_list_end:
        {
            add_val_to_list_end(args);
            return "";
        }
        break;
	    case HeadType::add_to_list_begin:
        {
            add_val_to_list_begin(args);
            return "";
        }
        break;
		case HeadType::_exit:
		{

			string return_code = args.empty()?"0":args[0];
			int type = get_type(return_code);
			if(type != Integer) return_code = "0";
			
			int code = atoi(return_code.c_str()); 
			exit(code);
		}
		break;
	}
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
int VirtualMachine::get_head_type(const string& head)
{
	if(head == "defvar")return HeadType::def_var;
	if(head == "defn")  return HeadType::def_fn;
	if(head == "print") return HeadType::print;
	if(head == "printl")return HeadType::printl;
	if(head == "enter") return HeadType::enter;
	if(head == "if")    return HeadType::_if;
	if(head == "loop")  return HeadType::_loop;
	if(head == "+")     return HeadType::add;
	if(head == "-")     return HeadType::substract;
	if(head == "/")     return HeadType::divide;
	if(head == "*")     return HeadType::multiplicate;
	if(head == "and")   return HeadType::_and;
	if(head == "or")    return HeadType::_or;
	if(head == "xor")   return HeadType::_ex_or;
	if(head == "not")   return HeadType::_not;
	if(head == "=")     return HeadType::equal; 
	if(head == ">")     return HeadType::more;
	if(head == "<")     return HeadType::less;
	if(head == ">=")    return HeadType::more_equal;
	if(head == "<=")    return HeadType::less_equal;
	if(head == "get") 	return HeadType::get_elem;
	if(head == "break") return HeadType::_break;
	if(head == "continue") return HeadType::_continue;
	if(head == "concat")return HeadType::_concat_list;
	if(head == "set")   return HeadType::_set;
	if(head == "return")return HeadType::_return;
	if(is_var_calling_head(head)) return HeadType::var;
	if(head == "adde" ) return HeadType::add_to_list_end;
    if(head == "addb")  return HeadType::add_to_list_begin;

	//call fn
	if(*(--head.end()) == '!')
	{
        int head_type = process_fn_to_call(head);
        if(head_type != -1) return HeadType::fn;
	}
	if(*head.begin() == '!')
    {
	    return HeadType::lambda;
    }
	if(head == "import")    return HeadType::_import;
	if(head == "size")      return HeadType::list_size_without_computation;
	if(head == "sizec")     return HeadType::list_size_with_computation;
	if(head == "get")       return HeadType::get_elem;
	if(head == "exit")      return HeadType::_exit;
	return -1;
}
bool VirtualMachine::is_var_calling_head(const string& head)
{
    bool existing = get_current_memory()->does_exist(head) != -1;
    bool module_calling = head.find("~") != string::npos;
    bool not_fn = (*(--head.end())) != '!';
    return (existing || module_calling) && not_fn;
}
int VirtualMachine::process_fn_to_call(const string& head)
{
    //fn_name! -> call function, named fn_name

    string fn_name = head.substr(0,head.size()-1);
    int mod_pos = fn_name.find("~");
    if(mod_pos != string::npos)
    {
        string module = get_substr(fn_name,0,mod_pos);
        fn_name = get_substr(fn_name,mod_pos+1,fn_name.size());
        current_function_map = &modules[module]->functions;
    }

    auto exist = (*current_function_map).find(fn_name);
    if(exist != (*current_function_map).end())
        return HeadType::fn;

    return -1;
}
pair<svector, vector<int> > VirtualMachine::get_vals_and_types(const svector& args)
{
	svector vals;
	vector<int> types;
	
	//compute vals and types
	for(auto arg: args)
	{
		int type = get_type(arg);
        if(type == -1)
        {
            svector adds={arg};
            print_error(ErrorPrinter::IncorrectType,adds);
        }
		if(type == List || type == Expr)
		{
			string val = compute(arg);
			int _type = get_type(val);
            if(type == -1)
            {
                svector adds={val};
                print_error(ErrorPrinter::IncorrectType,adds);
            }
			types.push_back(_type);
			vals.push_back(val);
		}
		else
		{
			vals.push_back(arg);
			types.push_back(type);
		}
	}
	return pair<svector, vector<int> >(vals,types);	
}
vector<int> VirtualMachine::get_integer_val_vec(const svector& vals)
{
	vector<int> _vals;
	for(auto val:vals)
		_vals.push_back(atoi(val.c_str()));
	return _vals;
}
vector<float> VirtualMachine::get_float_val_vec(const svector& vals)
{
	vector<float> _vals;
	for(auto val:vals)
		_vals.push_back(atof(val.c_str()));
	return _vals;
}
vector<bool>  VirtualMachine::get_bool_val_vec(const svector& vals)
{
	vector<bool> _vals;
	for(auto val:vals)
		_vals.push_back(val=="true"?true:false);
	return _vals;
}
template<class T>
bool VirtualMachine::is_val_too_low(T& val)
{
	if(typeid(T) == typeid(int))
		return val < numeric_limits<int>::min();
	if(typeid(T) == typeid(float))
		return val < numeric_limits<float>::min();
}
template<class T>
bool VirtualMachine::is_val_too_high(T& val)
{
	if(typeid(T) == typeid(int))
		return val > numeric_limits<int>::max();
	if(typeid(T) == typeid(float))
		return val > numeric_limits<float>::max();	
}
string VirtualMachine::compute_add(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);

	switch(types[0])
	{
		case Integer: 
			return add_vals<int>(vals);  
		break;
		case Float:   
			return add_vals<float>(vals);
		break;
		case String:
			return add_strings(vals);
		default: 
		print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
template<class T>
string VirtualMachine::add_vals(const svector& vals)
{
	if(typeid(T) == typeid(int))
	{
		vector<int> _vals = get_integer_val_vec(vals);
		int summ = 0;
		for(auto val:_vals)
		{	
			bool inside_the_limit = is_val_too_low<int>(val) || is_val_too_high<int>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Integer",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			summ+=val;
		}
		return to_string(summ);
	}
	else if(typeid(T) == typeid(float))
	{
		vector<float> _vals = get_float_val_vec(vals);
		float summ = 0;
		for(auto val:_vals)
		{
			bool inside_the_limit = is_val_too_low<float>(val) || is_val_too_high<float>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Float",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			summ+=val;
		}
		return to_string(summ);
	}
	//TODO::return default value
}
string VirtualMachine::add_strings(const svector& vals)
{
	string summ;
	for(auto val:vals)
		summ+=get_substr(val,1,val.size()-1);
	
	return '"'+summ+'"';
}
string VirtualMachine::compute_substract(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	switch(types[0])
	{
		case Integer:
			return distract_vals<int>(vals);
		break;
		case Float:
			return distract_vals<float>(vals);
		break;
		default: print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 	
	}
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
template<class T>
string VirtualMachine::distract_vals(const svector& vals)
{
	if(typeid(T) == typeid(int))
	{
		vector<int> _vals = get_integer_val_vec(vals);
		int dist = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<int>(val) || is_val_too_high<int>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Integer",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			dist-=val;
		}
		return to_string(dist);
	}
	else if(typeid(T) == typeid(float))
	{
		vector<float> _vals = get_float_val_vec(vals);
		float dist = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<float>(val) || is_val_too_high<float>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Float",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			dist-=val;
		}
		return to_string(dist);
	}
	//TODO:: add default return
}

string VirtualMachine::compute_multiplication(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
		
	switch(types[0])
	{
		case Integer:
			return multiplicate_vals<int>(vals);
		break;
		case Float:
			return multiplicate_vals<float>(vals);
		break;
		default:
			print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
	//About TDM-GCC warning note:
	/*
		default statement breaks the programm
		execution, so control will never reach
		the end of this function
	*/
}
template<class T>
string VirtualMachine::multiplicate_vals(const svector& vals)
{
	if(typeid(T) == typeid(int))
	{
		vector<int> _vals = get_integer_val_vec(vals);
		int mult = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<int>(val) || is_val_too_high<int>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Integer",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			mult*=val;
		}
		return to_string(mult);
	}
	if(typeid(T) == typeid(float))
	{
		vector<float> _vals = get_float_val_vec(vals);
		float mult = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<float>(val) || is_val_too_high<float>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Float",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			mult-=val;
		}
		return to_string(mult);
	}
}
string VirtualMachine::compute_dividing(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
		
	switch(types[0])
	{
		case Integer:
			return divide_vals<int>(vals);
		break;
		case Float:
			return divide_vals<float>(vals);
		break;
		default:
			print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
}
template<class T>
string VirtualMachine::divide_vals(const svector& vals)
{
	if(typeid(T) == typeid(int))
	{
		vector<int> _vals = get_integer_val_vec(vals);
		int div = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<int>(val) || is_val_too_high<int>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Integer",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			div/=val;
		}
		return to_string(div);
	}
	else if(typeid(T) == typeid(float))
	{
		vector<float> _vals = get_float_val_vec(vals);
		float div = _vals[0];
		for(int i =1;i<_vals.size();++i)
		{
			auto val = _vals[i];
			bool inside_the_limit = is_val_too_low<float>(val) || is_val_too_high<float>(val);
			if(inside_the_limit)
			{
				svector adds = {"type:Float",to_string(val)};
				print_error(ErrorPrinter::OutOfLimit,adds);
			}
			div/=val;
		}
		return to_string(div);
	}
}
string VirtualMachine::compute_and(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	if(types[0] == Bool)
	{
		vector<bool> _vals = get_bool_val_vec(vals);
		bool res = _vals[0];
		for(size_t i =1;i<_vals.size();++i)
			res=res&&_vals[i];
		return res==true?"true":"false";
	}
	else
	{
		print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
	}
}
string VirtualMachine::compute_or(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	if(types[0] == Bool)
	{
		vector<bool> _vals = get_bool_val_vec(vals);
		bool res = _vals[0];
		for(size_t i =1;i<_vals.size();++i)
			res=res||_vals[i];
		return res==true?"true":"false";
	}
	else
	{
		print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
	}
}
string VirtualMachine::compute_xor(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	if(types[0] == Bool)
	{
		vector<bool> _vals = get_bool_val_vec(vals);
		bool res = _vals[0];
		for(size_t i =1;i<_vals.size();++i)
			res=res^_vals[i];
		return res==true?"true":"false";
	}
	else
	{
		print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
	}	
}
string VirtualMachine::compute_not(const svector& args)
{
	if(args.size()>1)
	{
		svector adds ={"not can not take more than 1!"};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	if(types[0] != Bool)
	{
		svector adds={"must be Bool!"};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	
	return vals[0]=="true"?"false":"true";
}
string VirtualMachine::compute_equality(const svector& args)
{
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
		
	switch(types[0])
	{
		case Integer:
		{
			vector<int> _vals = get_integer_val_vec(vals);
			bool res = 	does_vec_has_equal_elems<int>(_vals);
			return res==true?"true":"false";
		}
		break;
		case Float:
		{
			vector<float> _vals = get_float_val_vec(vals);
			bool res = 	does_vec_has_equal_elems<float>(_vals);
			return res==true?"true":"false";	
		}
		break;
		case String:
		{
			bool res = 	does_vec_has_equal_elems<string>(vals);
			return res==true?"true":"false";	
		}
		break;
		case Bool:
		{
			vector<bool> _vals = get_bool_val_vec(vals);
			bool res = 	does_vec_has_equal_elems<bool>(_vals);
			return res==true?"true":"false";		
		}
		break;
		default:
			print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
}
string VirtualMachine::compute_more(const svector& args)
{
	if(args.size()>2)
	{
		svector adds={"can not take more than 2!"};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	switch(types[0])
	{
		case Integer:
			return is_a_more_than_b<int>(vals);
		break;
		case Float:
			return is_a_more_than_b<float>(vals);
		break;
		default:
			print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
}
string VirtualMachine::compute_more_equal(const svector& args)
{
	if(args.size()>2)
	{
		svector adds={"can not take more than 2!"};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	
	pair<svector, vector<int> > data = get_vals_and_types(args);
	svector vals = data.first;
	vector<int> types = data.second;
	
	is_computable_data_type_correct(types,vals);
	
	switch(types[0])
	{
		case Integer:
		{
			string equals = compute_equality(vals);
			string more = is_a_more_than_b<int>(vals);
			bool res1 = equals == "true"?true:false;
			bool res2 = more == "true"?true:false;
			bool res  = res1 || res2;
			return res == true?"true":"false";
		}	
		break;
		case Float:
		{
			string equals = compute_equality(vals);
			string more = is_a_more_than_b<float>(vals);
			bool res1 = equals == "true"?true:false;
			bool res2 = more == "true"?true:false;
			bool res  = res1 || res2;
			return res == true?"true":"false";
		}	
		break;
		default:
			print_error(ErrorPrinter::IncorrectType,svector(vals.begin(),vals.end())); 
		break;
	}
}

template<typename T>
string VirtualMachine::is_a_more_than_b(const svector& args)
{
	if(typeid(T) == typeid(int))
	{
		vector<int> vals = get_integer_val_vec(args);
		bool res = vals[0]>vals[1];
		return res==true?"true":"false";
	}
	if(typeid(T) == typeid(float))
	{
		vector<float> vals = get_float_val_vec(args);
		bool res = vals[0]>vals[1];
		return res==true?"true":"false";	
	}
}
string VirtualMachine::compute_print(const svector& args, bool move_to_next_line)
{
	if(args.empty())
	{
		svector adds={"nothing to print!"};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	svector data;
	for(auto arg:args)
	{
		int type = get_type(arg);
		if(type == List || type == Expr)
			data.push_back(compute(arg));
		else if(type != -1)
		{
			string _arg = arg;
			if(type == String)
			{
				_arg = get_substr(arg,1,arg.size()-1);
			}
			data.push_back(_arg);
		}
		else
		{
			svector adds = {arg};
			print_error(ErrorPrinter::IncorrectArgs,adds);
		}
		
	}
	
	for(auto elem:data)
	{
		if(move_to_next_line)
			cout<<elem<<endl;
		else
			cout<<elem;
	}

	return "";
}
void VirtualMachine::compute_enter(const svector& args)
{
	if(args.size() != 1)
	{
		svector adds(args.begin(),args.end());
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	string value;
	cin>>value;
	int val_type = get_type(value);
	int var_type = get_current_memory()->does_exist(args[0]);
	if(val_type != var_type)
	{
		svector adds={"entered value type differs from storage type!"};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	if(var_type == -1)
	{
		svector adds={args[0]};
		print_error(ErrorPrinter::VarDoesntExist,adds);
	}

    get_current_memory()->change_variable(value,args[0]);
}
string VirtualMachine::compute_if(const svector& args)
{
	if(args.size() > 3 && args.size() < 2)
	{
		svector adds(args.begin(),args.end());
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	
	string cond = args[0];
	int cond_type = get_type(cond);
	if(cond_type == List || cond_type == Expr)
	{
		cond = compute(cond);
		cond_type = get_type(cond);	
	}
	if(cond_type != Bool)
	{
		svector adds={"condtion type must be bool!"};
		print_error(ErrorPrinter::IncorrectType,adds);
	}

	bool computable_body1 = get_type(args[1]) == List || get_type(args[1]) == Expr;
	if(cond == "true" && computable_body1)
		return compute(args[1]);			
	else if(cond == "true")
		return args[1];



	bool has_third = args.size() == 3;
	if(cond != "true" && has_third)
    {
        bool computable_body2 = get_type(args[2]) == List || get_type(args[2]) == Expr;
        if(computable_body2) return compute(args[2]);
        else return args[2];
    }

	return "";
}
void VirtualMachine::compute_loop(const svector& args)
{
	string cond = args[0];
	string computed_cond;
	int cond_type = get_type(cond);
	if(cond_type != List)
	{
		svector adds = {cond,"loop condition must be list!"};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	else
	{
		computed_cond = compute(cond);
		cond_type = get_type(computed_cond);
	}
	
	if(cond_type != Bool)
	{
		svector adds = {cond,"condition must be bool!"};
		print_error(ErrorPrinter::IncorrectType,adds);	
	}
	
	bool exit = false;
	while(computed_cond == "true" && !exit)
	{
		for(int i = 1;i<args.size();++i)
		{
			auto arg = args[i];
			if(is_computable(arg))
			{
				string sub_result = compute(arg);
				if(sub_result == "break")exit = true;
				if(sub_result == "continue")i = 1;
			}
		}
		computed_cond = compute(cond);
	}
}
string VirtualMachine::get_list_elem(const string& head,const svector& args)
{
    string list = args[0];
    string pos  = args[1];

    svector vals;
    int list_type = get_type(list);
    if(list_type == List)
    {
        svector elems = separate_by_space(list);
        string head = elems[0];
        int head_type = get_head_type(head);
        //if it's variable
        if(head_type != -1 && head_type == HeadType::var)
        {
            int var_type = get_current_memory()->does_exist(head);
            if(var_type != -1)
            {
                string var_value = get_current_memory()->get_value(head,var_type);
                vals = separate_by_space(var_value);
            }
            else
            {
                svector adds = {head,"variable must be a list!"};
                print_error(ErrorPrinter::IncorrectType,adds);
            }
        }
        //if it's just a list
        else if(head_type == -1)
        {
            vals = elems;
        }
    }
    else if(list_type == Expr)
    {
        int expr_type = get_expr_type(list);
        if(expr_type == ExprHeadType::get_var_val)
        {
            string variable_val = compute_expr(list);
            vals = separate_by_space(variable_val);
        }
        else
        {
            svector adds = {list,"expression must be to get a variable value!"};
            print_error(ErrorPrinter::IncorrectExpression,adds);
        }
    }
    else
    {
        svector adds = {list,"it should be a list or expression!"};
        print_error(ErrorPrinter::IncorrectType,adds);
    }

    int pos_type = get_type(pos);
    if(pos_type == List || pos_type == Expr)
    {
        pos = compute(pos);
    }
    pos_type = get_type(pos);
    if(pos_type != Integer)
    {
        svector adds = {pos,"position of list element must be of Integer type!"};
        print_error(ErrorPrinter::IncorrectType,adds);
    }

    int pos_val = atoi(pos.c_str());
    if(pos_val < 0 || pos_val > vals.size())
    {
        svector adds = {pos,"position is out of list range!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    return vals[pos_val];
}
string VirtualMachine::concat_list(const svector& args)
{
	svector _args;
	for(auto arg:args)
	{
		int type = get_type(arg);
		if(type == Expr)
        {
		    string val = compute(arg);
		    int val_type = get_type(val);
		    if(val_type == List)
            {
		        svector sub = separate_by_space(val);
		        copy(sub.begin(),sub.end(),back_inserter(_args));
            }
        }
		else if(type == List)
		{
			string res;
			svector subs;
			svector list = separate_by_space(arg);
			if(get_head_type(list[0]) == -1)
			    res = arg;
			else
				res = compute(arg);

			if(is_computable(res))
			{
				subs = separate_by_space(res);
				copy(subs.begin(),subs.end(),back_inserter(_args));
			}
		}
		else
        {
		    svector adds = {arg};
		    print_error(ErrorPrinter::IncorrectArgs,adds);
        }
	}
	string result = "(";
	for(auto arg:_args)
	{
		result+=arg;
		result+=" ";
	}
	result+= ")";
	return result;
}
string VirtualMachine::compute_list_size_without_computation(const svector& args)
{
    if(args.size() != 1)
    {
        svector adds = args;
        adds.push_back("takes 1 argument only!");
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }

    string val = args[0];
    int type = get_type(val);
    if(type != List)
    {
        svector adds = {val,"size operation can take list only!"};
        print_error(ErrorPrinter::IncorrectType,adds);
    }
    svector list = separate_by_space(val);

    int size = list.size();
    return to_string(size);
}
string VirtualMachine::compute_list_size_with_computation(const svector& args)
{
    if(args.size() != 1)
    {
        svector adds = args;
        adds.push_back("takes 1 argument only!");
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }

    string val = args[0];
    int type = get_type(val);
    if(type != List && type != Expr)
    {
        svector adds = {val,"size operation can take list only!"};
        print_error(ErrorPrinter::IncorrectType,adds);
    }
    val = compute(val);
    if(get_type(val) == List)
    {
        svector list = separate_by_space(val);
        return to_string(list.size());
    }
    else
    {
        svector adds = {val,"size operation can take list only!"};
        print_error(ErrorPrinter::IncorrectType,adds);
    }
}
void VirtualMachine::set_val_to_list(const svector& args)
{
	if(args.size() != 3)
	{
		svector adds(args.begin(),args.end());
		adds.push_back("incorrect arguments for setting value of list element!");
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	string place = args[0];
	int place_type = get_type(place);
	
	string val   = args[1];
	int val_type = get_type(val);
	
	string list  = args[2];
	int list_type = get_type(list);

	if(place_type == List || place_type == Expr)
		place = compute(place);
		
	place_type = get_type(place);
	if(place_type != Integer)
	{
		svector adds = {to_string(place_type),"element position must be Integer!"}	;
		print_error(ErrorPrinter::IncorrectType,adds);
	}	
	int _place = atoi(place.c_str());
		
		
	if(val_type == List || val_type == Expr)
		val = compute(val);
	
	string list_name;
	if(list_type == List)
	{
		svector vals = separate_by_space(list);
		int head_type = get_head_type(vals[0]);
		if(head_type == HeadType::var)
		{
			list_name = vals[0];
		}
		else
		{
			svector adds = {vals[0],"must be a list variable!"};
			print_error(ErrorPrinter::IncorrectArgs,adds);
		}
	}
	else if(list_type == Expr)
    {
	    if(*list.begin() == '$')
        {
	        string _list_name = get_substr(list,1,list.size());
	        int exist = get_current_memory()->does_exist(_list_name);
	        if(exist == -1)
            {
	            svector adds = {_list_name};
	            print_error(ErrorPrinter::VarDoesntExist,adds);
            }
	        list_name = _list_name;
        }
	    else
        {
	        svector adds = {list};
	        print_error(ErrorPrinter::IncorrectArgs,adds);
        }
    }
	else
	{
		list_name = list;
	}

	string _val = get_current_memory()->get_value(list_name,List);
    if(list_is_empty(_val))
    {
        svector adds = {"var name:"+list_name,"value:"+_val};
        print_error(ErrorPrinter::EmptyList,adds);
    }
	svector vals = separate_by_space(_val);
	if(_place < 0 || _place > vals.size())
	{
		svector adds = {to_string(_place),"element of list is out of range!"};
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	vals[_place] = val;
	string new_list = unite_list(vals);
    get_current_memory()->set_value(new_list,list_name);
			
}
void VirtualMachine::create_variable(const svector& args)
{
	if(args.size() != 2)
	{
		svector adds(args.begin(),args.end());
		adds.push_back("incorrect number of elements for var creation!");
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	string val = args[1];
	int type = get_type(val);
	if(type == -1)
	{
		svector adds = {val};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	bool is_name_already_created = get_current_memory()->does_exist(args[0]) != -1;
	if(!is_name_already_created)
	{
	    int type = get_type(val);
		if(type == List && !list_is_empty(val))
        {
		    svector list = separate_by_space(val);
		    int type = get_head_type(list[0]);
		    if(type != -1)
		        val = compute(val);
        }
        get_current_memory()->set_value(val,args[0]);
	}
	else
	{
		svector adds ={args[0]};
		print_error(ErrorPrinter::VarAlreadyExist,adds);
	}
}
void VirtualMachine::create_function(const svector& args)
{
	string name = args[0];
	auto same = (*current_function_map).find(name);
	if(same != (*current_function_map).end())
	{
		svector adds = {name};
		print_error(ErrorPrinter::FnAlreadyExist,adds);
	}
	
	
	string fn_args = args[1];
	int fn_args_type = get_type(fn_args);
	if(fn_args_type != List)
	{
		svector adds = {fn_args};
		print_error(ErrorPrinter::IncorrectType,adds);
	}
	
	svector parsed_args = separate_by_space(fn_args);
	svector body;
	for(int i = 2; i< args.size();++i)
	{
		auto curr = args[i];
		int curr_type = get_type(curr);
		if(curr_type != List)
		{
			svector adds = {curr};
			print_error(ErrorPrinter::IncorrectType,adds);
		}
		body.push_back(curr);
	}
	
	Function* fn = new Function(parsed_args,body,name);
    (*current_function_map)[name] = fn;
}
void VirtualMachine::import_modules(const svector& args)
{
    //importing means module running
    for(auto module:args)
    {
        string name = module;
        string path = module;
        auto slash = module.find_last_of("/");
        if(slash != string::npos)
        {
            name = get_substr(module,slash+1,module.size());
        }
        auto point = module.find(".");
        if(point != string::npos)
        {
            name = get_substr(name,0,point);
        }
        else
        {
            path+=".bg";
        }

        svector module_script = load_script(path);
        check_braces_equality(module_script);

        Module* _module = new Module;
        modules[name] = _module;

        current_memory = &modules[name]->memory;
        current_function_map = &modules[name]->functions;
        run(module_script,name);
    }
    current_memory = &modules["main"]->memory;
    current_function_map = &modules["main"]->functions;
}
template<class T>
bool VirtualMachine::does_vec_has_equal_elems(const vector<T>& vec)
{
    //check all elements of vector are the same
	T prev = vec[0];
	for(auto elem: vec)
	{
		if(prev != elem)
			return false;
		else
			prev = elem;
	}
	return true;
}
void VirtualMachine::is_computable_data_type_correct(const vector<int>& vec,
													 const svector& vals)
{
	//values types must be equal to each other
	bool types_equal =  does_vec_has_equal_elems<int>(vec);
	if(!types_equal)
	{
		print_error(ErrorPrinter::DifferentTypes,vals);
	}
}
string VirtualMachine::compute_function(const string& head, const svector& _args,Function* fn)
{
    function_computation_on = true;
    current_lambda_map = &fn->lambdas;
    if(_args.size() != 1)
    {
        svector adds(_args.begin(),_args.end());
        adds.push_back("function can take arguments list only!");
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    string arg_list = _args[0];
    if(get_type(arg_list) != List)
    {
        svector adds = {arg_list,"must be a List!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }

    svector args = separate_by_space(arg_list);
    bool equal_args_number = args.size() == fn->get_args().size();
    if(!equal_args_number)
    {
        svector adds = {"incorrect number of arguments!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    //compute arguments if they are functions/variables
    int curr_arg_number = 0;
    for(auto& arg:args)
    {
        int arg_type = get_type(arg);
        if(arg_type == List)
        {
            svector list = separate_by_space(arg);
            int type = get_head_type(list[0]);
            if (type == HeadType::var || type == HeadType::fn)
            {
                arg = compute(arg);
            }
        }
        else if(arg_type == Expr)arg = compute(arg);
        else if(arg_type == -1)
        {
			
			
            //it can be function
            auto is_fn = current_function_map->find(arg);
			int mod = arg.find("~");
			bool is_from_module = mod != string::npos; 
            if(is_fn != current_function_map->end())
            {
                //add it to lambdas
                string lambda_name = fn->get_args()[curr_arg_number];
                (*current_lambda_map)[lambda_name] = (*is_fn).second;
            }
			else if(is_from_module)
			{
				string mod_name = get_substr(arg,0,mod);
				string fn_name  = get_substr(arg,mod+1,arg.size());
				Module* module = get_module(mod_name);
				auto is_mod_fn = module->functions.find(fn_name);
				bool exist = is_mod_fn != module->functions.end();
				if(exist)
				{
					string lambda_name = fn->get_args()[curr_arg_number];
					(*current_lambda_map)[lambda_name] = (*is_mod_fn).second;
				}
				else
				{
					svector adds = {arg};
					print_error(ErrorPrinter::FnDoesntExist,adds);
				}
			}
            else
            {
                svector adds = {arg};
                print_error(ErrorPrinter::FnDoesntExist,adds);
            }
        }
        curr_arg_number++;
    }

    //pass arguments to local memory

    current_memory = &fn->local_memory;
    if(args.size() != 0)
    {
        for(size_t i = 0;i<args.size();++i)
        {
            string val = args[i];
            string name= fn->get_args()[i];
            current_memory->set_value(val,name);
        }
    }

    string res;
    for(auto& list:fn->get_body())
    {
        res = compute(list);
    }
    current_memory->clear();
    sub_functions_memory.clear();
    current_memory = &modules["main"]->memory;
    current_function_map = &modules["main"]->functions;
    current_lambda_map = nullptr;
    function_computation_on = false;
    return res;
}
string VirtualMachine::compute_sub_function(const string& head, const svector& _args, Function* fn,string& fn_name)
{
    if(_args.size() != 1)
    {
        svector adds(_args.begin(),_args.end());
        adds.push_back("function can take arguments list only!");
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    string arg_list = _args[0];
    if(get_type(arg_list) != List)
    {
        svector adds = {arg_list,"must be a List!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }

    svector args = separate_by_space(arg_list);
    bool equal_args_number = args.size() == fn->get_args().size();
    if(!equal_args_number)
    {
        svector adds = {"incorrect number of arguments!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    //compute arguments if they are functions/variables
    int curr_arg_number = 0;
    for(auto& arg:args)
    {
        int arg_type = get_type(arg);
        if(arg_type == List)
        {
            svector list = separate_by_space(arg);
            int type = get_head_type(list[0]);
            if (type == HeadType::var || type == HeadType::fn)
            {
                arg = compute(arg);
            }
        }
        else if(arg_type == Expr)arg = compute(arg);
        else if(arg_type == -1)
        {
            //it can be function
            auto is_fn = current_function_map->find(arg);
			int mod = arg.find("~");
			bool is_from_module = mod != string::npos; 
            if(is_fn != current_function_map->end())
            {
                //add it to lambdas
                string lambda_name = fn->get_args()[curr_arg_number];
                (*current_lambda_map)[lambda_name] = (*is_fn).second;
            }
			else if(is_from_module)
			{
				string mod_name = get_substr(arg,0,mod);
				string fn_name  = get_substr(arg,mod+1,arg.size());
				Module* module = get_module(mod_name);
				auto is_mod_fn = module->functions.find(fn_name);
				bool exist = is_mod_fn != module->functions.end();
				if(exist)
				{
					string lambda_name = fn->get_args()[curr_arg_number];
					(*current_lambda_map)[lambda_name] = (*is_mod_fn).second;
				}
				else
				{
					svector adds = {arg};
					print_error(ErrorPrinter::FnDoesntExist,adds);
				}
			}
            else
            {
                svector adds = {arg};
                print_error(ErrorPrinter::FnDoesntExist,adds);
            }
        }
        curr_arg_number++;
    }

    //pass arguments to local memory

    sub_functions_memory[fn_name] = &fn->local_memory;
    current_sub_function = fn_name;
    if(args.size() != 0)
    {
        for(size_t i = 0;i<args.size();++i)
        {
            string val = args[i];
            string name= fn->get_args()[i];
            sub_functions_memory[fn_name]->set_value(val,name);
        }
    }

    string res;
    for(auto& list:fn->get_body())
    {
        res = compute(list);
    }
    current_sub_function.clear();
    sub_functions_memory[fn_name]->clear();
    return res;
}

string VirtualMachine::change_memory(const string& head)
{
    string var_name = head;
    if(*var_name.begin() == '$')
        var_name = get_substr(var_name,1,var_name.size());

    //change memory
    int mod_pos = head.find("~");
    if(mod_pos != string::npos)
    {
        string module_name = get_substr(head,0,mod_pos);
        current_memory = &modules[module_name]->memory;

        var_name = get_substr(head,mod_pos+1,head.size());
    }
    return var_name;
}
string VirtualMachine::process_variable(const string& head, const svector& args)
{
    string var_name = change_memory(head);
    //if there is no args
    //just get value
    if(args.empty())
    {
        int type = get_current_memory()->does_exist(var_name);
        return get_current_memory()->get_value(var_name,type);
    }
    else
    {
        //set new value
        //and get value
        if(args.size() > 1)
        {
            svector adds(args.begin(),args.end());
            print_error(ErrorPrinter::IncorrectArgs,adds);
        }
        string val;
        int val_type = get_type(args[0]);
        if(val_type == List || val_type == Expr)
        {
            val = compute(args[0]);
            val_type = get_type(val);
        }
        else
            val = args[0];

        int type = get_current_memory()->does_exist(var_name);
        if(type != val_type && type != None)
        {
            svector adds={args[0],"value type differs from variable's one"};
            print_error(ErrorPrinter::IncorrectType,adds);
        }
        if(type != None)
        {
            get_current_memory()->change_variable(val,var_name);
        }
        else
        {
            //destroy none var
            get_current_memory()->destroy_variable(var_name);
            get_current_memory()->set_value(val,var_name);
        }
        return "";
    }
}
string VirtualMachine::get_variable_value(const string& head)
{
    string var_name = change_memory(head);
    int type = get_current_memory()->does_exist(var_name);
    return get_current_memory()->get_value(var_name,type);
}
string VirtualMachine::get_var_name_for_adding(const svector& vals)
{
    string variable_name;

    string list_name = vals[0];
    int type = get_type(list_name);
    if(type == List)
    {
        svector sub = separate_by_space(list_name);
        int head_type = get_head_type(sub[0]);
        if(head_type == var)
        {
            int type = get_current_memory()->does_exist(sub[0]);
            if(type == -1)
            {
                svector adds = {sub[0]};
                print_error(ErrorPrinter::VarDoesntExist,adds);
            }
            variable_name = sub[0];
        }
    }
    else if(type == Expr)
    {
        string name = get_substr(list_name,1,list_name.size());
        int type = get_current_memory()->does_exist(name);
        if(type == -1)
        {
            svector adds ={name};
            print_error(ErrorPrinter::VarDoesntExist,adds);
        }
        variable_name = name;
    }
    else
    {
        svector adds = {list_name,"it must be variable calling!"};
        print_error(ErrorPrinter::IncorrectArgs,adds);
    }
    return variable_name;
}
void VirtualMachine::add_val_to_list_end(const svector& vals)
{
    string variable_name = get_var_name_for_adding(vals);

    string val = get_current_memory()->get_value(variable_name,List);
    svector elems = separate_by_space(val);
    for(int i = 1;i<vals.size();++i)
    {
        auto curr = vals[i];
        int value_type = get_type(curr);
        if(value_type == List)
            curr = compute(curr);
        if(value_type == Expr)
            curr = compute_expr(curr);

        elems.push_back(curr);
    }
    string new_val = unite_list(elems);
    get_current_memory()->set_value(new_val,variable_name);
}
void VirtualMachine::add_val_to_list_begin(const svector& vals)
{
    string variable_name = get_var_name_for_adding(vals);

    string val = get_current_memory()->get_value(variable_name,List);
    svector elems;
    for(int i = 1;i<vals.size();++i)
    {
        auto curr = vals[i];
        int value_type = get_type(curr);
        if(value_type == List)
            curr = compute(curr);
        if(value_type == Expr)
            curr = compute_expr(curr);

        elems.push_back(curr);
    }
    svector old_vals = separate_by_space(val);
    for(auto val:old_vals)
    {
        elems.push_back(val);
    }
    string new_val = unite_list(elems);
    get_current_memory()->set_value(new_val,variable_name);
}
Memory* VirtualMachine::get_current_memory()
{
    if(current_sub_function.empty())
            return current_memory;
    else
    {
        return sub_functions_memory[current_sub_function];
    }
}
string VirtualMachine::compute_return(const svector& args)
{
	if(args.size() != 1)
	{
		svector adds(args.begin(),args.end());
		adds.push_back("can not return more than 1 argument!");
		print_error(ErrorPrinter::IncorrectArgs,adds);
	}
	string val = args[0];
	int type = get_type(val);
	if(type == Expr) return compute_expr(val);
	if(type == List)
	{
		svector elems = separate_by_space(val);
		string head = elems[0];
		int head_type = get_head_type(head);
				
		if(head_type == -1)
			return val;
		else
			return compute(val);
	}
			
	return val;	
}
void VirtualMachine::run_interactive_mode()
{
	//it runs untill there is no exit command
	string command;
	while(true)
	{
		cout<<">>";
		string input = get_input();
		command+=input;
		if(does_list_has_equal_braces_number(command))
		{
			string result = compute(command);
			cout<<result<<endl;
		}
	}
}
string VirtualMachine::get_input()
{
	string command;
	const int size = sizeof(istream)/sizeof(char);
	char* buffer = new char[size];
	cin.getline(buffer,size);
	command = buffer;
	delete buffer;
	return command;
}
bool VirtualMachine::does_list_has_equal_braces_number(const string& list)
{
	int open_number  = count(list.begin(),list.end(),'(');
	int close_number = count(list.begin(),list.end(),')');
	return open_number == close_number;
}