#include"ConfigLoader.h"
using namespace Dwarf;

ConfigLoader::ConfigLoader(){}
ConfigLoader::~ConfigLoader(){}


void ConfigLoader::load_config()
{
	svector script = machine.load_script("data/config.bg");
	machine.check_braces_equality(script);
	machine.run(script,"main");
}
void ConfigLoader::load_config(const string& custom_path)
{
	svector script = machine.load_script(custom_path.c_str());
	machine.check_braces_equality(script);
	machine.run(script,"main");
}
Pos ConfigLoader::get_terminal_size()
{
	bg::Module* main = machine.get_module("main");
	string swidth = main->memory.get_value("width",bg::TypeChecker::Integer);
	string sheight= main->memory.get_value("height",bg::TypeChecker::Integer);
	return Pos(atoi(swidth.c_str()),atoi(sheight.c_str()));
}
bool ConfigLoader::is_smart_sort_enabled()
{
	bg::Module* main = machine.get_module("main");
	std::string option= main->memory.get_value("smart_sort_enabled",bg::TypeChecker::Bool);
	return option=="true"?true:false;
}
int ConfigLoader::get_max_path_char_number()
{
	bg::Module* main = machine.get_module("main");
	std::string size = main->memory.get_value("path_char_max",bg::TypeChecker::Integer);
	return atoi(size.c_str());
}
void ConfigLoader::set_terminal_size()
{
	HANDLE terminal = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize = {75,450};
	SetConsoleScreenBufferSize(terminal,bufferSize);
	HWND hwnd = FindWindow(NULL,"Dwarf");
	MoveWindow(hwnd,100,100,size.x,size.y,false);
}