cd infinity

echo "getting latest..."
git pull

cd lib/grammar/

echo "installing infinity..."
g++ main.cpp -DMAKE_COMPILER lib/grammar/*.cpp lib/runtime/Opcode.cpp lib/grammar/optimizer/*.cpp lib/grammar/generator/*.cpp lib/grammar/frontend/*.cpp lib/grammar/frontend/*/*.cpp lib/grammar/backend/*.cpp lib/grammar/backend/*/*.cpp lib/util/*.cpp lib/util/zip/*.cpp -o bin/infinityc -std=c++11 -pthread -lstdc++ -O3 -Ofast

sudo cp infinityc /bin

echo "installing runtime support..."

cd ../runtime/
g++ main.cpp lib/runtime/*.cpp lib/Modules/*/*.cpp lib/runtime/*/*.cpp lib/util/*.cpp lib/util/jit/asmjit/src/asmjit/x86/*.cpp lib/util/jit/asmjit/src/asmjit/core/*.cpp lib/util/zip/*.cpp lib/util/fmt/src/*.cc -o bin/infinity -std=c++11 -pthread -lstdc++ -lm -lrt -lstdc++ -O3 -Ofast
sudo cp infinity /bin

echo "infinity installed!"
