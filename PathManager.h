#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H
#include "fileref.h"
#include "tag.h"
#include"MusicData.h"
#include"VirtualMachine/StrFunction.h"
#include"ErrorProcessor.h"
#include<fstream>
#include<vector>
#include<string>
#include<filesystem>

namespace fs = std::filesystem;

typedef std::vector<string> svector;
typedef std::vector<wstring> wsvector;
namespace Dwarf
{
class PathManager
{	
protected:
	svector searching_paths;
	std::vector<MusicData*> music;
	wsvector raw_music;
	wsvector able_extensions;
public:
	PathManager();
	~PathManager();
	
	svector get_searching_paths();
	void get_music_files();
	void apply_smart_sort();
	bool is_extension_able(const std::wstring& extension);
	wstring fix_path_slash(const std::wstring& path);
	void add_new_search_paths(const std::string& value);

};
};
#endif //PATH_MANAGER_H