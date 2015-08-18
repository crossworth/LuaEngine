#include <iostream>
#include <LuaEngine/LuaEngine.h>

using namespace SM;
using namespace std;

int main(int argc, char *argv[]) {

    LuaEngine* LUA = LuaEngine::getInstance();

    LUA->registerVariable("width", 100);
    LUA->registerVariable("height", 50);

    LUA->executeCode("print(width)");
    LUA->executeCode("print(height)");


  return 0;
}
