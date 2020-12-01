#libraries
IRK_KLANG_INCS = -I"E:/irrKlang/include/"
irk_klang_lib  = E:/irrKlang/lib/Win32-gcc/libirrKlang.a

TAGLIB_INCS = -I"E:/taglib-1.10/taglib/" -I"E:/taglib-1.10/taglib/toolkit/" -I"E:/taglib-1.10/build/"
taglib      = E:/taglib-1.10/build/taglib/libtag.dll.a


#common compiler data
INCS     = -I"E:/cpp_projects/DwarfAudioPlayer" $(TAGLIB_INCS) $(IRK_KLANG_INCS)
CPP      = g++.exe
FLAG     = $(INCS) -std=c++17 -pthread -g3 


#object files
objects 		  = main.o app.o console.o CommonMenu.o Player.o AlbumMenu.o ArtistMenu.o GenreMenu.o SearchMenu.o
bublegum_objects  = ErrorPrinter.o TypeChecker.o Memory.o VirtualMachine.o


dwarf : $(objects) $(bublegum_objects) 
	$(CPP) -o build/dwarf $(objects) $(bublegum_objects) $(taglib) $(irk_klang_lib)

#main programm

ArtistMenu.o : ArtistMenu.cpp
	$(CPP) -c ArtistMenu.cpp ArtistMenu.h $(FLAG)

AlbumMenu.o : AlbumMenu.cpp
	$(CPP) -c AlbumMenu.cpp AlbumMenu.h $(FLAG)

CommonMenu.o : CommonMenu.cpp
	$(CPP) -c CommonMenu.cpp CommonMenu.h $(FLAG)

GenreMenu.o : GenreMenu.cpp
	$(CPP) -c GenreMenu.cpp GenreMenu.h $(FLAG)

SearchMenu.o : SearchMenu.cpp
	$(CPP) -c SearchMenu.cpp SearchMenu.h $(FLAG)

Player.o : Player.cpp
	$(CPP) -c Player.cpp Player.h $(FLAG)

headers = app.h console.h MusicData.h CommonMenu.h SearchMenu.h GenreMenu.h ArtistMenu.h PlayTime.h VirtualMachine/VirtualMachine.h  Player.h  AlbumMenu.h
		  
app.o : app.cpp 
	$(CPP) -c app.cpp $(headers) $(FLAG)

console.o : console.cpp
	$(CPP) -c console.cpp console.h $(FLAG)

main.o : main.cpp
	$(CPP) -c main.cpp app.h $(FLAG)


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