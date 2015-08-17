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
    void executeCode(const std::string &code);

    void cleanState();

    void registerVariable(const std::string &variableName, const char* value);
    void registerVariable(const std::string &variableName, const int &value);
    void registerVariable(const std::string &variableName, const float &value);
    void registerVariable(const std::string &variableName, const bool &value);

    void printStack();
    std::string getError();

    template<typename T>
    std::vector<T> getVector(const std::string &variableName);

    std::vector<std::string> getTableKeys(const std::string &variableName);

    void cleanStack();

    template<typename T>
    T get(const std::string &variableName);

    ~LuaEngine();

private:
    void setTable(const unsigned int &ref, const std::vector<std::string> &elements, const int &value);
    void setField(const char* index, const int &value);
    bool isStateEnable(const char* funcName);

    bool loadToStack(const std::string &variableName);

    template<typename T>
    T getLastFromStack(const std::string &variablename);

    std::vector<std::string> stringExplode(const std::string  &string, char delimiter);

    lua_State *L;
    std::string mError;
    int currentLevel;
    bool _openLibs;

};

template<typename T>
T LuaEngine::get(const std::string &variableName) {
    if (!isStateEnable(__func__)){
        return T();
    }

    currentLevel = 0;
    if (loadToStack(variableName) || typeid(T).name() == typeid(std::string).name()) {
        T result = getLastFromStack<T>(variableName);
        cleanStack();
        return result;
    }

    cleanStack();
    return 0;
}

template<>
inline std::vector<bool> LuaEngine::getVector<bool>(const std::string &variableName) {
    std::vector<bool> result;

    if (!isStateEnable(__func__)){
        return result;
    }

    if (loadToStack(variableName) && !lua_isnil(L, -1)) {
        lua_pushnil(L);

        while(lua_next(L, -2)) {
            result.push_back((static_cast<bool>(lua_toboolean(L, -1))));
            lua_pop(L, 1);
        }
    }

    cleanStack();
    return result;
}

template<>
inline std::vector<float> LuaEngine::getVector<float>(const std::string &variableName) {
    std::vector<float> result;

    if (!isStateEnable(__func__)){
        return result;
    }

    if (loadToStack(variableName) && !lua_isnil(L, -1)) {
        lua_pushnil(L);

        while(lua_next(L, -2)) {
            result.push_back((static_cast<float>(lua_tonumber(L, -1))));
            lua_pop(L, 1);
        }
    }

    cleanStack();
    return result;
}

template<>
inline std::vector<int> LuaEngine::getVector<int>(const std::string &variableName) {
    std::vector<int> result;

    if (!isStateEnable(__func__)){
        return result;
    }

    if (loadToStack(variableName) && !lua_isnil(L, -1)) {
        lua_pushnil(L);

        while(lua_next(L, -2)) {
            result.push_back((static_cast<int>(lua_tointeger(L, -1))));
            lua_pop(L, 1);
        }
    }

    cleanStack();
    return result;
}

template<>
inline std::vector<std::string> LuaEngine::getVector<std::string>(const std::string &variableName) {
    std::vector<std::string> result;

    if (!isStateEnable(__func__)){
        return result;
    }

    if (loadToStack(variableName) && !lua_isnil(L, -1)) {
        lua_pushnil(L);

        while(lua_next(L, -2)) {
            result.push_back((std::string(lua_tostring(L, -1))));
            lua_pop(L, 1);
        }
    }

    cleanStack();
    return result;
}

template<typename T>
T LuaEngine::getLastFromStack(const std::string &variablename) {
    isStateEnable(__func__);
    return 0;
}

template<>
inline bool LuaEngine::getLastFromStack<bool>(const std::string &variablename) {
    if (!isStateEnable(__func__)){
        return false;
    }

    if (!lua_isboolean(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<bool>] Variable " + variablename + " is not a boolean" << std::endl;
        return false;
    }
    return static_cast<bool>(lua_toboolean(L, -1));
}

template<>
inline int LuaEngine::getLastFromStack<int>(const std::string &variablename) {
    if (!isStateEnable(__func__)){
        return 0;
    }

    if (!lua_isinteger(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<int>] Variable " + variablename + " is not a integer" << std::endl;
        return 0;
    }
    return static_cast<int>(lua_tointeger(L, -1));
}

template<>
inline float LuaEngine::getLastFromStack<float>(const std::string &variablename) {
    if (!isStateEnable(__func__)){
        return 0.0f;
    }

    if (!lua_isnumber(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<float>] Variable " + variablename + " is not a float" << std::endl;
        return 0.0f;
    }
    return static_cast<float>(lua_tonumber(L, -1));
}

template<>
inline std::string LuaEngine::getLastFromStack<std::string>(const std::string &variablename) {
    if (!isStateEnable(__func__)){
        return "null";
    }

    if (!lua_isstring(L, -1)) {
        std::cout <<  "[LuaEngine::getLastFromStack<std::string>] Variable " + variablename + " is not a std::string" << std::endl;
        return "null";
    }

    return std::string(lua_tostring(L, -1));
}

}

#endif // LUAENGINE_H
