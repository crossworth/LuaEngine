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

    cout << endl << endl << endl;

    std::vector<int> mTimes          = LUA->getVector<int>("player.sprite2.running.times");
    std::vector<std::string> mFrames = LUA->getVector<std::string>("player.sprite2.running.frames");

    cout << mTimes.size() << endl;

    for(int i =0; i < mTimes.size(); i++) {
        cout << mTimes.at(i) << ", " << mFrames.at(i) << endl;
    }

  return 0;
}
