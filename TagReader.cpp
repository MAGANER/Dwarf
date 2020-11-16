#include"TagReader.h"
using namespace TAG_READER;

TagReader::TagReader(const string& file)
{
	ok = true;
	mp3_tag_size = 128;
	audio_file.open(file);
	
	audio_file.seekg(-(mp3_tag_size-3),ios::end);
	if(!audio_file.fail())
	{
		tag_data.title = read_data(audio_file);
		tag_data.artist= read_data(audio_file);
		tag_data.album = read_data(audio_file);
		tag_data.year  = read_data(audio_file);
		if(audio_file.fail())
		{
			ok = false;
			cout<<file<<" is unable to be readed!"<<endl;
		}
	}
	else ok = false;
	audio_file.close();
}
TagReader::~TagReader()
{
}
TagData TagReader::get_tag_info()
{
	return tag_data;
}
bool TagReader::is_ok()
{
	return ok;
}
string TagReader::read_data(ifstream& file)
{
	string data;
	char ch;
	for(int i = 0;i<30;++i)
	{
		audio_file.get(ch);
		data.push_back(ch);
	}
	return data;
}