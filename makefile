#libraries
TAGLIB_INCS = -I"E:/taglib-1.10/taglib/" -I"E:/taglib-1.10/taglib/toolkit/" -I"E:/taglib-1.10/build/"
taglib      = E:/taglib-1.10/build/taglib/libtag.dll.a

libzplay_incs = -I"E:/libzplay/C++/"
libzplay_lib  = E:/libzplay/C++/libzplay.a


#common compiler data
INCS     = -I"E:/cpp_projects/DwarfAudioPlayer" $(TAGLIB_INCS) $(libzplay_incs)
CPP      = g++.exe
FLAG     = $(INCS) -std=c++17 -g3 


#object files
objects 		  = main.o app.o console.o CommonMenu.o PlayerMenu.o PathManager.o ConfigLoader.o AlbumMenu.o ArtistMenu.o GenreMenu.o SearchMenu.o
bublegum_objects  = ErrorPrinter.o TypeChecker.o Memory.o VirtualMachine.o


dwarf : $(objects) $(bublegum_objects) 
	$(CPP) -o build/dwarf $(objects) $(bublegum_objects) $(taglib) $(libzplay_lib)

#main programm

ArtistMenu.o : ArtistMenu.cpp
	$(CPP) -c ArtistMenu.cpp ArtistMenu.h AlbumMenu.h $(FLAG)

AlbumMenu.o : AlbumMenu.cpp
	$(CPP) -c AlbumMenu.cpp AlbumMenu.h PlayerMenu.h $(FLAG)

CommonMenu.o : CommonMenu.cpp
	$(CPP) -c CommonMenu.cpp CommonMenu.h $(FLAG)

GenreMenu.o : GenreMenu.cpp
	$(CPP) -c GenreMenu.cpp GenreMenu.h ArtistMenu.h $(FLAG)

SearchMenu.o : SearchMenu.cpp
	$(CPP) -c SearchMenu.cpp SearchMenu.h GenreMenu.h $(FLAG)

PlayerMenu.o : PlayerMenu.cpp
	$(CPP) -c PlayerMenu.cpp PlayerMenu.h CommonMenu.h $(FLAG)

ConfigLoader.o : ConfigLoader.cpp
	$(CPP) -c ConfigLoader.cpp ConfigLoader.h Pos.h ErrorProcessor.h SearchMenu.h VirtualMachine/VirtualMachine.h $(FLAG)

PathManager.o : PathManager.cpp
	$(CPP) -c PathManager.cpp PathManager.h ErrorProcessor.h $(FLAG)

app.o : app.cpp 
	$(CPP) -c app.cpp app.h SearchMenu.h ConfigLoader.h PathManager.h $(FLAG)

console.o : console.cpp
	$(CPP) -c console.cpp console.h Pos.h $(FLAG)

main.o : main.cpp
	$(CPP) -c main.cpp app.h ConfigLoader.h $(FLAG)


#Bubblegum
path = VirtualMachine/
ErrorPrinter.o : $(path)ErrorPrinter.cpp
	$(CPP) -c $(path)ErrorPrinter.cpp $(path)ErrorPrinter.h $(FLAG)
	
TypeChecker.o : $(path)TypeChecker.cpp
	$(CPP) -c $(path)TypeChecker.cpp $(path)TypeChecker.h $(path)StrFunction.h $(FLAG)
	
Memory.o : $(path)Memory.cpp
	$(CPP) -c $(path)Memory.cpp $(path)Memory.h $(path)Variable.h $(path)ErrorPrinter.h $(path)TypeChecker.h $(FLAG)

VirtualMachine.o : $(path)VirtualMachine.cpp
	$(CPP) -c $(path)VirtualMachine.cpp $(path)VirtualMachine.h $(path)ErrorPrinter.h $(path)TypeChecker.h $(path)Function.h $(path)Module.h $(path)StrFunction.h $(FLAG)