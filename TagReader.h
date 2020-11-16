/*
	Simple class to read id3v1 tags
*/
#ifndef TAG_READER_H
#define TAG_READER_H
#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>
using namespace std;
namespace TAG_READER
{
struct TagData
{
	string title;
	string artist;
	string album;
	string year;
};	
class TagReader
{
private:
	bool ok;
	int mp3_tag_size;
	char* buffer;
	ifstream audio_file;
	TagData tag_data;
public:
	TagReader(const string& file);
	~TagReader();
	TagData get_tag_info();
	bool is_ok();
private:
	string read_data(ifstream& file);
};

};
#endif //TAG_READER_H