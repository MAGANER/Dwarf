#include"TagReader.h"
using namespace TAG_READER;

TagReader::TagReader(const string& file)
{
	ok = true;
	mp3_tag_size = 128;
	buffer = new char[mp3_tag_size+1];
	audio_file.open(file);
	tag_data = {0};
	
	audio_file.seekg(-(mp3_tag_size),ios::end);
	if(!audio_file.fail())
	{
		audio_file.read(reinterpret_cast<char*>(&tag_data),mp3_tag_size);
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