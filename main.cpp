#include "App.h"
using namespace Dwarf;

int main(int argc, char **argv)
{
	App app;
	if(argc == 2)
	{
		string path = argv[1];
		app.set_custom_song_to_play(path);
	}
	
	app.run();
	return 0;
}
