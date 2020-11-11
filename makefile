CPP      = g++.exe -D__RELEASE__
INCS     = -I"E:/cpp_projects/DwarfAudioPlayer"
FLAG     = $(INCS) -std=c++17

objects = main.o app.o console.o
bublegum_objects  = ErrorPrinter.o TypeChecker.o Memory.o VirtualMachine.o

dwarf : $(objects) $(bublegum_objects)
	$(CPP) -o build/dwarf $(objects) $(bublegum_objects)

app.o : app.cpp 
	$(CPP) -c app.cpp app.h console.h VirtualMachine/VirtualMachine.h $(FLAG)

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