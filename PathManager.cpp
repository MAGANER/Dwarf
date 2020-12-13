#include"PathManager.h"
using namespace Dwarf;


PathManager::PathManager()
{
	able_extensions = {L"mp3"};
}
PathManager::~PathManager(){}



svector PathManager::get_searching_paths(const string& path)
{
	std::ifstream file;
	file.open(path);
	if(!file)
	{
		process_error("can not open "+path+"!");
	}
	
	svector paths;
	while(file)
	{
		string line;
		file>>line;
		if(!line.empty())paths.push_back(line);
	}
	file.close();
	return paths;
}
void PathManager::add_new_search_paths(const std::string& value, const string& file)
{
	svector existing_paths = get_searching_paths(file);
	bool exist = std::find(existing_paths.begin(),existing_paths.end(),value) != existing_paths.end();
	if(!exist)
	{
		if(fs::exists("data/paths.txt"))
		{
		        std::ofstream file("data/paths.txt",std::ios::out | std::ios::app);
			file<<endl;
			file<<value;
			file.close();
		}
		else if(fs::exists("C:/dwarf_data/paths.txt"))
		{
                        std::ofstream file("C:/dwarf_data/paths.txt",std::ios::out | std::ios::app);
			file<<endl;
			file<<value;
			file.close();
		}
		else
		{
			process_error("cannot open data/paths.txt!");
		}
	}
}
void PathManager::get_music_files(bool smart_sort)
{
	for(auto& spath: searching_paths)
	{
		if(fs::exists(spath))
		{
		for(auto& data: fs::recursive_directory_iterator(spath))
		{
			std::wstring str = fix_path_slash(data.path().wstring());
			int point = str.find_last_of(L".");
			if(point != wstring::npos)
			{
				std::wstring extension = get_substr(str,point+1,str.size());
				if(is_extension_able(extension))
				{
					MPEG::File f(str.c_str());
					ID3v1::Tag *id3v1tag = f.ID3v1Tag();
					if(id3v1tag)
					{
						std::wstring artist = id3v1tag->artist().toWString();
					
						std::wstring title  = id3v1tag->title().toWString();
						std::wstring album  = id3v1tag->album().toWString();
						std::wstring genre  = id3v1tag->genre().toWString();
						unsigned int year= id3v1tag->year();
						
						MusicData* data = new MusicData(artist,title,album,genre,str,year);
						music.push_back(data);	
					}
					else
					{
						raw_music.push_back(str);
					}
				}
			}
		}
		}
	}
}
bool PathManager::is_extension_able(const std::wstring& extension)
{
	return std::find(able_extensions.begin(),able_extensions.end(),extension) != able_extensions.end();
}
wstring PathManager::fix_path_slash(const std::wstring& path)
{
	//change \ to /
	std::wstring new_;
	for(int i = 0;i<path.size();++i)
	{
		auto curr = path[i];
		if(static_cast<int>(curr) != 92)new_+=curr;
		else new_+=L"/";
	}
	return new_;
}
wspair PathManager::does_val_exist(const std::wstring& value)
{
	using namespace std;
	wstring val = clear_str(value);
	for(auto& file:music)
	{
		wstring artist = clear_str(file->artist);
		wstring album  = clear_str(file->album);
		wstring genre  = clear_str(file->genre);
		wstring title  = clear_str(file->title);
		
		if(val == artist) return wspair(L"artist",val);
		if(val == album)  return wspair(L"album", val);
		if(val == genre)  return wspair(L"genre", val);
		if(val == title)  return wspair(L"title", val);
	}
	return wspair(L"",L"");
}	
std::wstring PathManager::clear_str(const std::wstring& str)
{
	//delete spaces and set all chars to lower case
	std::wstring new_str;
	for(auto& ch:str) if(!isspace(ch)) new_str.push_back(tolower(ch));
	return new_str;
}
