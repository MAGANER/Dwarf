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
	char tag[3];
	char title[30];
	char artist[30];
	char album [30];
	char year[4];
	char comment[30];
	int genre;
	
};	
class TagReader
{
private:
	int mp3_tag_size;
	char* buffer;
	ifstream audio_file;
	TagData tag_data;
public:
	TagReader(const string& file);
	~TagReader();
	TagData get_tag_info();
};

};
#endif //TAG_READER_H