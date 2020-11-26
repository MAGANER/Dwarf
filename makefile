CPP      = g++.exe
IRK_KLANG_INCS = -I"E:/irrKlang/include/"
TAGLIB_INCS = -I"E:/taglib-1.10/taglib/" -I"E:/taglib-1.10/taglib/toolkit/" -I"E:/taglib-1.10/build/"
INCS     = -I"E:/cpp_projects/DwarfAudioPlayer" $(TAGLIB_INCS) $(IRK_KLANG_INCS)
FLAG     = $(INCS) -std=c++17 -g3 

irk_klang_lib     = E:/irrKlang/lib/Win32-gcc/libirrKlang.a
taglib_path       = E:/taglib-1.10/build/taglib/libtag.dll.a
objects 		  = main.o app.o console.o
bublegum_objects  = ErrorPrinter.o TypeChecker.o Memory.o VirtualMachine.o

dwarf : $(objects) $(bublegum_objects)
	$(CPP) -o build/dwarf $(objects) $(bublegum_objects) $(taglib_path) $(irk_klang_lib)


app.o : app.cpp 
	$(CPP) -c app.cpp app.h console.h MusicData.h PlayTime.h VirtualMachine/VirtualMachine.h $(FLAG)

console.o : console.cpp
	$(CPP) -c console.cpp console.h $(FLAG)

path = VirtualMachine/


ErrorPrinter.o : $(path)ErrorPrinter.cpp
	$(CPP) -c $(path)ErrorPrinter.cpp $(path)ErrorPrinter.h $(FLAG)
	
TypeChecker.o : $(path)TypeChecker.cpp
	$(CPP) -c $(path)TypeChecker.cpp $(path)TypeChecker.h $(path)StrFunction.h $(FLAG)
	
Memory.o : $(path)Memory.cpp
	$(CPP) -c $(path)Memory.cpp $(path)Memory.h $(path)Variable.h $(path)ErrorPrinter.h $(path)TypeChecker.h $(FLAG)

VirtualMachine.o : $(path)VirtualMachine.cpp
	$(CPP) -c $(path)VirtualMachine.cpp $(path)VirtualMachine.h $(path)ErrorPrinter.h $(path)TypeChecker.h $(path)Function.h $(path)Module.h $(path)StrFunction.h $(FLAG)
	
window.o : TinyPDC/Window.cpp
	$(CPP) -c TinyPDC/Window.cpp TinyPDC/Window.h $(vector_path)Vector.h $(FLAG)  

main.o : main.cpp
	$(CPP) -c main.cpp app.h $(FLAG)
