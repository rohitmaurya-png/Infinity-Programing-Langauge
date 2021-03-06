
echo "building compiler..."
g++ main.cpp -DMAKE_COMPILER lib/grammar/*.cpp lib/runtime/Opcode.cpp lib/grammar/optimizer/*.cpp lib/grammar/generator/*.cpp lib/grammar/frontend/*.cpp lib/grammar/frontend/*/*.cpp lib/grammar/backend/*.cpp lib/grammar/backend/*/*.cpp lib/util/*.cpp lib/util/zip/*.cpp -o bin/infinityc -std=c++11 -pthread -lstdc++ -O3 -m32

echo "building runtime engine..."
g++ main.cpp lib/runtime/*.cpp lib/Modules/*/*.cpp lib/runtime/*/*.cpp lib/util/*.cpp lib/util/zip/*.cpp lib/util/fmt/src/*.cc -o bin/infinity -std=c++11 -pthread -lstdc++ -lm -lrt -lstdc++ -O3 -ldl -m32

echo "building runtime engine profiler..."
# g++ main.cpp -Dinfinity_PROF_ lib/runtime/*.cpp lib/Modules/*/*.cpp lib/runtime/*/*.cpp lib/util/*.cpp lib/util/zip/*.cpp lib/util/fmt/src/*.cc lib/util/jit/asmjit/src/asmjit/x86/*.cpp lib/util/jit/asmjit/src/asmjit/base/*.cpp -o bin/tanto -std=c++11 -pthread -lm -lrt -lstdc++ -O3 -ldl

cp bin/infinity /bin
cp bin/infinityc /bin
echo "infinity build finished!"
