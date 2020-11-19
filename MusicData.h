#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H
#include<string>
using namespace std;
namespace Dwarf
{
struct MusicData
{
	wstring artist, title, album;
	string genre;
	unsigned int year;
	MusicData(const wstring& artist,
			  const wstring& title,
			  const wstring& album,
			  const string& genre,
			  unsigned int year)
	{
		this->artist = artist;
		this->title  = title;
		this->album  = album;
		this->genre  = genre;
		this->year   = year;
	}
	~MusicData(){}
};
};
#endif //MUSIC_DATA_H