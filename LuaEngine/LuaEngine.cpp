#include "LuaEngine.h"

using namespace SM;

LuaEngine::LuaEngine(bool openLibs) {
    L = luaL_newstate();

    if (openLibs == true) {
        luaL_openlibs(L);
    }
}

LuaEngine* LuaEngine::mInstance = nullptr;

LuaEngine* LuaEngine::getInstance(bool openLibs) {
    if (!mInstance) {
        mInstance = new LuaEngine(openLibs);
    }
    return mInstance;
}

bool LuaEngine::loadFile(const std::string& fileName) {
    if (luaL_dofile(L, fileName.c_str())) {
        if (lua_isstring(L, -1)) {
            mError = lua_tostring(L, -1);
        }
        return false;
    }
    return true;
}

/**
 * @brief LuaEngine::printStack Print the LuaStack
 * Based on: https://szpg1108.wordpress.com/2013/08/08/understanding-the-lua-stack-pt-2-viewing-the-stack/
 */
void LuaEngine::printStack() {
    std::cout << "-- LuaEngine:printStack --" << std::endl;

    int i   = 0;
    int top = lua_gettop(L);

    std::cout << "Stack size: " << top << std::endl;

    for (i = top; i >= 1; i--) {
      int type = lua_type(L, i);
      switch (type) {
        case LUA_TSTRING:
          std::cout << i << " ---- " << lua_tostring(L, i) << std::endl;
          break;
        case LUA_TBOOLEAN:
          std::cout << i << " ---- " << (lua_toboolean(L, i) ? "true" : "false") << std::endl;
          break;
        case LUA_TNUMBER:
          std::cout << i << " ---- " << lua_tonumber(L, i) << std::endl;
          break;
        default:
          std::cout << i << " ---- " << lua_typename(L, type) << std::endl;
          break;
      }
    }
    std::cout << "--------------------------" << std::endl;
}

std::string LuaEngine::getError() {
    return mError;
}

/**
 * @brief LuaEngine::stringExplode
 * @param string
 * @param delimiter
 * @return std::vector<std::string>
 *
 * Based on: http://stackoverflow.com/a/12967010
 */
std::vector<std::string> LuaEngine::stringExplode(const std::string &string, char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(string);

    for (std::string token; std::getline(iss, token, delimiter); ) {
        result.push_back(std::move(token));
    }

    return result;
}

LuaEngine::~LuaEngine() {
    if (L) {
        lua_close(L);
    }
}

bool LuaEngine::loadToStack(const std::string& variableName) {
    std::vector<std::string> variables = stringExplode(variableName, '.');

    for (unsigned int i = 0; i < variables.size(); i++) {
        if (currentLevel == 0) {
            lua_getglobal(L, variables.at(i).c_str());
        } else {
            lua_getfield(L, -1, variables.at(i).c_str());
        }

        if (lua_isnil(L, lua_gettop(L))) {
            std::cout << "[LuaEngine::loadToStack] Variable " << variables.at(i) << " is not defined (" + variableName + ")" << std::endl;
            break;
        }

        currentLevel++;
    }

    if (lua_isnil(L, lua_gettop(L))) {
        return false;
    }
    return true;
}
