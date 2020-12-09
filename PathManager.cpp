#include"PathManager.h"
using namespace Dwarf;


PathManager::PathManager()
{
	able_extensions = {L"mp3"};
}
PathManager::~PathManager(){}



svector PathManager::get_searching_paths()
{
	std::ifstream file;
	file.open("data/paths.txt");
	if(!file)
	{
		process_error("can not open data/paths.txt!");
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
void PathManager::add_new_search_paths(const std::string& value)
{
	svector existing_paths = get_searching_paths();
	bool exist = std::find(existing_paths.begin(),existing_paths.end(),value) != existing_paths.end();
	if(!exist)
	{
		std::ofstream file("data/paths.txt",std::ios::out | std::ios::app);
		if(file)
		{
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
void PathManager::get_music_files()
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
					TagLib::FileRef f(str.c_str());
					TagLib::Tag *tag = f.tag();
					std::wstring artist = tag->artist().toWString();
					std::wstring title  = tag->title().toWString();
					std::wstring album  = tag->album().toWString();
					std::wstring genre  = tag->genre().toWString();
					unsigned int year= tag->year();
					
					bool empty = artist.empty() || 
								 title.empty()  ||
								 album.empty()  ||
								 genre.empty();
					if(empty)
					{
						raw_music.push_back(str);
					}
					else
					{
						MusicData* data = new MusicData(artist,title,album,genre,str,year);
						music.push_back(data);
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