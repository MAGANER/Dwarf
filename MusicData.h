#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H
#include<string>
using namespace std;
namespace Dwarf
{
struct MusicData
{
	wstring artist, title, album, genre, path;
	unsigned int year;
	MusicData(const wstring& artist,
			  const wstring& title,
			  const wstring& album,
			  const wstring& genre,
			  const wstring& path,
			  unsigned int year)
	{
		this->artist = artist;
		this->title  = title;
		this->album  = album;
		this->genre  = genre;
		this->year   = year;
		this->path   = path;
	}
	~MusicData(){}
};
};
#endif //MUSIC_DATA_H