#ifndef LUAENGINE_H
#define LUAENGINE_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <typeinfo>

extern "C" {
    # include "lua.h"
    # include "lauxlib.h"
    # include "lualib.h"
}

#ifndef nullptr
#define nullptr 0
#endif

namespace SM {

class LuaEngine {
private:
    static LuaEngine* mInstance;
    LuaEngine(bool openLibs);
public:
    static LuaEngine* getInstance(bool openLibs = true);
    bool loadFile(const std::string &fileName);
    void printStack();
    std::string getError();

    template<typename T>
    T get(const std::string &variableName);

    ~LuaEngine();

private:
    bool loadToStack(const std::string &variableName);

    template<typename T>
    T getLastFromStack(const std::string &variablename);

    std::vector<std::string> stringExplode(const std::string  &string, char delimiter);

    lua_State *L;
    std::string mError;
    int currentLevel;

};

template<typename T>
T LuaEngine::get(const std::string &variableName) {
    currentLevel = 0;
    if (loadToStack(variableName) || typeid(T).name() == typeid(std::string).name()) {
        return getLastFromStack<T>(variableName);
    }

    return 0;
}

template<typename T>
T LuaEngine::getLastFromStack(const std::string &variablename) {
    return 0;
}

template<>
inline bool LuaEngine::getLastFromStack<bool>(const std::string &variablename) {
    if (!lua_isboolean(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<bool>] Variable " + variablename + " is not a boolean" << std::endl;
        return false;
    }
    return static_cast<bool>(lua_toboolean(L, -1));
}

template<>
inline int LuaEngine::getLastFromStack<int>(const std::string &variablename) {
    if (!lua_isinteger(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<int>] Variable " + variablename + " is not a integer" << std::endl;
        return 0;
    }
    return static_cast<int>(lua_tointeger(L, -1));
}

template<>
inline float LuaEngine::getLastFromStack<float>(const std::string &variablename) {
    if (!lua_isnumber(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<float>] Variable " + variablename + " is not a float" << std::endl;
        return 0.0f;
    }
    return static_cast<float>(lua_tonumber(L, -1));
}

template<>
inline std::string LuaEngine::getLastFromStack<std::string>(const std::string &variablename) {
    if (!lua_isstring(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<std::string>] Variable " + variablename + " is not a std::string" << std::endl;
        return "null";
    }

    return std::string(lua_tostring(L, -1));
}

}

#endif // LUAENGINE_H
