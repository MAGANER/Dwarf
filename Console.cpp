#include"Console.h"
using namespace Dwarf;

Color::Color(int back, int foreground)
{
	background = back;
	this->foreground = foreground;
}
Color::Color()
{
	background = 0;
	foreground = 0;
}
Color::~Color()
{
}

bool Color::operator==(const Color& color)
{
	return this->background == color.background &&
		   this->foreground == color.foreground;
}
bool Color::operator!=(const Color& color)
{
	return !(*this == color);
}
void Color::operator=(const Color& color)
{
	this->background = color.background;
	this->foreground = color.foreground;
}

Pos::Pos(int x, int y)
{
	this->x = x;
	this->y = y;
}
Pos::Pos()
{
}
Pos::~Pos()
{
}

Console::Console()
{
	
}
Console::~Console()
{
	
}

void Console::draw_string(const string& str, const Color& color, const COORD& pos)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (WORD)((color.background << 4) | color.foreground ));
	SetConsoleCursorPosition(console,pos);
	
	PDWORD written = 0;
	WriteConsoleA(console,str.c_str(),strlen(str.c_str()),written,NULL);
}






