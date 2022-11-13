g++ -std=c++17 ./src/assembler/assembler.cpp || { echo "Compilation Error: src/assembler/assembler.cpp"; exit -1; } && echo "Compilation Success: src/assembler/assembler.cpp";
g++ -std=c++17 ./main.cpp || { echo "Compilation Error: main.cpp"; exit -1; } && echo "Compilation Success: main.cpp";
