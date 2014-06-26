main: main.o Vehicle.o Utility.o Lane.o Censor.o Demand.o
	clang++ -o main main.o Vehicle.o Utility.o Lane.o Censor.o Demand.o
Utility.o: Utility.h Utility.cpp 
	clang++ -c   Utility.cpp -std=c++1y
Vehicle.o: Vehicle.h Utility.h Macro.h Lane.h Vehicle.cpp
	clang++ -c   Vehicle.cpp -std=c++1y
Lane.o: Lane.h Utility.h Macro.h Vehicle.h Censor.h Demand.h Lane.cpp
	clang++ -c   Lane.cpp -std=c++1y
Censor.o: Censor.h Vehicle.h Censor.cpp
	clang++ -c   Censor.cpp -std=c++1y
Demand.o: Demand.h Demand.cpp
	clang++ -c  Demand.cpp -std=c++1y
main.o: Lane.h Vehicle.h Macro.h Utility.h Censor.h Demand.h main.cpp
	clang++ -c   main.cpp -std=c++1y
