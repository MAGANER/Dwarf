#ifndef GROUP_H
#define GROUP_H
#include<vector>
#include<wstring>
namespace Dwarf
{
struct Group
{
	Group(const wstring& path,
		  const wstring& name,
		  vector<wstring>& values)
	{
		this->path = path;
		this->name = name;
		this->values= values;
	}
	~Group(){}
	
	wstring path, name;
	vector<wstring> values;
};
};
#endif //GROUP_H