#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H
#include<string>
using namespace std;
namespace Dwarf
{
struct MusicData
{
	string title, artist,
		   album, year;
	
	MusicData(char* title, char* artist,
			  char* album, char* year)
	{
		this->title = title;
		this->artist= artist;
		this->album = album;
		this->year  = year;
	}
	~MusicData()
	{
	}
				  
};
};
#endif //MUSIC_DATA_H