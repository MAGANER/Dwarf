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
	
	printf(str.c_str());
}
void Console::draw_string(const wstring& str, const Color& color, const COORD& pos)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, (WORD)((color.background << 4) | color.foreground ));
	SetConsoleCursorPosition(console,pos);
	
	wprintf(str.c_str());
}
void Console::clear() 
{
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}




