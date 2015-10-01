#include <iostream>
#include <LuaEngine/LuaEngine.h>

using namespace SM;
using namespace std;

int main(int argc, char *argv[]) {

    LuaEngine* LUA = LuaEngine::getInstance();

<<<<<<< HEAD
    LUA->registerVariable("var1", "string");
    LUA->registerVariable("var2", 10);
    LUA->registerVariable("var3", 100.5f);
    LUA->registerVariable("var4", true);


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

    for(int i = 0; i < mTimes.size(); i++) {
        cout << mTimes.at(i) << ", " << mFrames.at(i) << endl;
    }

    cout << endl << endl;

    std::vector<std::string> keys =  LUA->getTableKeys("player");

    for(int i =0; i < keys.size(); i++) {
        cout << keys.at(i) << endl;
    }

    return 0;
=======
    LUA->registerVariable("width", 100);
    LUA->registerVariable("height", 50);

    LUA->executeCode("print(width)");
    LUA->executeCode("print(height)");


  return 0;
>>>>>>> origin/master
}
