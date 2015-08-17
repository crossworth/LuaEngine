#include <iostream>
#include <LuaEngine/LuaEngine.h>

using namespace SM;
using namespace std;

int main(int argc, char *argv[]) {

    LuaEngine* LUA = LuaEngine::getInstance();

    LUA->executeCode("print(10)");


  return 0;
}
