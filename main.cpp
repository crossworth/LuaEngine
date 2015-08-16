#include <iostream>
#include <LuaEngine/LuaEngine.h>

using namespace SM;
using namespace std;

int main(int argc, char *argv[]) {

    string file = "test.lua";
    LuaEngine* LUA = LuaEngine::getInstance();

    if (!LUA->loadFile(file)) {
        cout << "Arquivo não foi carregado" << endl;
        cout << LUA->getError() << endl;
    }

    cout << LUA->get<int>("player.sprite.running.frameCounter") << endl;
    cout << LUA->get<int>("player.sprite.running.frame1.time") << endl;
    cout << LUA->get<std::string>("player.sprite.running.frame1.texture") << endl;


  return 0;
}
