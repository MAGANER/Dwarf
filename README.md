# Dwarf
## simple, but extendable audio player for windows, written in C++.

First of all, you should know it's free. The reason to make it free is simple. It is my little pet-project.

But it is still a good player, so i want to make it grow.

Current Version is 1.0.

### TODO list:
Here i show you what is done and what is going to be done.
- [x] Make basical player application(sorting, loading, playing).
- [x] Make searching.
- [ ] Make groups(extended playlists).
- [ ] Make GUI.
- [ ] Make full support for Unicode in Console.
- [x] Make ability to see 'raw' music(that has no tags)
- [ ] Make it able to play music, passed as command line argument
- [ ] (Optional)Make smart sorting. For example Alterac and alterac are the same names.

### Dependencies:
In this project i used:

1. TDM-GCC and C++17
2. GNU/Make to build project
3. TagLib v 1.10 (link:https://taglib.org/) to obtain tag data
4. LibZplay v 2.02 (link:http://libzplay.sourceforge.net/) to play audio
5. WinApi to work with windows terminal

### About Makefile

There you can find several variables, containing data about include paths and libraries.

So you should change it for yourself.

Then you can run make command `make`.

### Commands

To scroll the list use arrow UP/DOWN.
To change the current choosen element, use +/-.
