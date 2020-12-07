/*
	contains basic abilities to draw any data
	to console, using the win32 functions.
	Also it can get the data about entered code.

*/
#ifndef CONSOLE_H
#define CONSOLE_H
#include<windows.h>
#include<conio.h>
#include<string>
#include<string.h>
#include<iostream>
#include"Pos.h"

namespace Dwarf
{
struct Color
{
	int background, foreground;
	Color(int back, int foreground);
	Color();
	~Color();
	void operator=(const Color& color);
	bool operator==(const Color& color);
	bool operator!=(const Color& color);
};
class Console
{
public:
	enum ColorCode 
	{
		Black = 0,
		Blue  = 1,
		Green = 2,
		Cyan  = 3,
		Red   = 4,
		Magenta = 5,
		Brown   = 6,
		LightGray = 7,
		DarkGray  = 8,
		LightBlue = 9,
		LightGreen= 10,
		LightCyan = 11,
		LightRed  = 12,
		LightMagenta= 13,
		Yellow = 14,
		White  = 15,
	};
	enum key_codes
	{
		UP   = 72,
		DOWN = 80,
		ESCAPE = 27,
		ENTER  = 13,
		MINUS  = 95,
		PLUS   = 43,
		SPACE  = 32,
		R      = 114,
		N      = 110,
	};
public:
	Console();
	virtual ~Console();
	
	void draw_string(const std::string& str, const Color& color, const COORD& pos);
	void draw_string(const std::wstring& str, const Color& color, const COORD& pos);
	void clear();
};
};
#endif //CONSOLE_H


