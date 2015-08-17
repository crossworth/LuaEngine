#include <iostream>
#include <LuaEngine/LuaEngine.h>

using namespace SM;
using namespace std;

int main(int argc, char *argv[]) {

    string file = "test.lua";
    LuaEngine* LUA = LuaEngine::getInstance();

//    LUA->registerVariable("x", "string");
//    LUA->registerVariable("p", 10);
//    LUA->registerVariable("t", 100.5f);
//    LUA->registerVariable("o", true);

    LUA->registerVariable("window.size.width", 100);
    LUA->registerVariable("window.size.height", 50);


    if (!LUA->loadFile(file)) {
        cout << "Arquivo não foi carregado" << endl;
        cout << LUA->getError() << endl;
    }

    return 0;

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


//    class player {
//    public:
//        int hp;
//        int getHP();
//    };

//    LUA->getInstance()->registerFunction()



  return 0;
}
