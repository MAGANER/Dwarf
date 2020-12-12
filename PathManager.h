#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H


#include "tbytevector.h"

#include "mpeg/mpegfile.h"

#include "mpeg/id3v2/id3v2tag.h"
#include "mpeg/id3v2/id3v2frame.h"
#include "mpeg/id3v2/id3v2header.h"

#include "mpeg/id3v1/id3v1tag.h"

#include "ape/apetag.h"

#include"MusicData.h"
#include"VirtualMachine/StrFunction.h"
#include"ErrorProcessor.h"
#include<fstream>
#include<vector>
#include<string>
#include<filesystem>

namespace fs = std::filesystem;
using namespace TagLib;

typedef std::vector<string> svector;
typedef std::vector<wstring> wsvector;
typedef std::pair<wstring, wstring> wspair;
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
	void get_music_files(bool smart_sort);
	bool is_extension_able(const std::wstring& extension);
	wstring fix_path_slash(const std::wstring& path);
	void add_new_search_paths(const std::string& value);
private:
	wspair does_val_exist(const std::wstring& value);
	std::wstring clear_str(const std::wstring& str);
};
};
#endif //PATH_MANAGER_H