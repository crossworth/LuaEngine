#include "LuaEngine.h"

using namespace SM;

LuaEngine::LuaEngine(bool openLibs) : _openLibs(openLibs) {
    L = luaL_newstate();

    if (_openLibs) {
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
    if (!isStateEnable(__func__)){
        return false;
    }

    if (luaL_dofile(L, fileName.c_str())) {
        if (lua_isstring(L, -1)) {
            mError = lua_tostring(L, -1);
        }
        return false;
    }
    return true;
}

void LuaEngine::executeCode(const std::string& code) {
    if (!isStateEnable(__func__)){
        return;
    }
    luaL_dostring(L, code.c_str());
}

void LuaEngine::cleanState() {
    if (L) {
        lua_close(L);
    }
    L = luaL_newstate();

    if (_openLibs) {
        luaL_openlibs(L);
    }
}

void LuaEngine::registerVariable(const std::string &variableName, const char* value) {
    if (!isStateEnable(__func__)){
        return;
    }
    lua_pushstring(L, value);
    lua_setglobal(L, variableName.c_str());
}

void LuaEngine::registerVariable(const std::string &variableName, const int &value) {
    if (!isStateEnable(__func__)){
        return;
    }

    if (variableName.find(".") != std::string::npos) {
        std::vector<std::string> variables = stringExplode(variableName, '.');

        lua_getglobal(L, variables.at(0).c_str());

        if (lua_isnil(L, -1)) {
            lua_pop(L, lua_gettop(L));
            lua_newtable(L);
        }

        if (variables.size() > 2) {
            setTable(1, variables, value);
        } else {
            setField(variables.at(1).c_str(), value);
        }

        lua_setglobal(L, variables.at(0).c_str());
        return;


         std::cout << "Registering: " << variableName << std::endl;

        // Try load the table on the stack
        lua_getglobal(L, variables.at(0).c_str());

        // If already defined it will be on top of stack
        // if it's not definied we just verify if it's nill and pop it out
        bool hasGlobalSet = false;

        if (lua_isnil(L, -1)) {
            lua_pop(L, lua_gettop(L));
        } else {
            hasGlobalSet = true;
            std::cout << "has global now" << std::endl;
        }
        printStack();

//        // A lot of work, I'll have to acess each member and set or reset it's value
//        for (unsigned int i = 1; i < variables.size(); i++) {

//            if (i == variables.size() - 1) {
//                lua_getfield(L, -1, variables.at(i).c_str());

//                if (lua_isnil(L, -1)) {
//                    std::cout << "nill" << std::endl;
//                    printStack();
//                    lua_pop(L, 1);
//                    lua_newtable(L);
//                } else {
//                    std::cout << "not nill" << std::endl;
//                    printStack();
//                }

//                lua_pushstring(L, variables.at(i).c_str());
//                lua_pushnumber(L, value);
//            } else {
//                if (i == 1 && hasGlobalSet) {
//                    lua_getfield(L, -1, variables.at(i).c_str());
//                } else {
//                    lua_newtable(L);
//                }
//                lua_pushstring(L, variables.at(i).c_str());
//            }
//        }

//        for (unsigned int i = 1; i < variables.size(); i++) {
//            lua_settable(L, -3);
//        }

        if (hasGlobalSet) {
            std::cout << "Get table if has global" << std::endl;
            printStack();
        } else {
            lua_newtable(L);
            std::cout << "Create table size" << std::endl;
            printStack();
        }

        lua_pushstring(L, variables.at(1).c_str());
        lua_gettable(L, -1);

        if (lua_isnil(L, -1)) {

            lua_pop(L, 1);
            lua_newtable(L);
            std::cout << "Create table height" << std::endl;
            printStack();
        }

//        lua_pushstring(L, variables.at(1).c_str());
//        lua_pushnumber(L, value);
//        lua_settable(L, -3);



        std::cout << "Push table into stack" << std::endl;
        printStack();
        setField(variables.at(2).c_str(), value);
        lua_settable(L, -3);

//        if (lua_isnil(L, -1)) {
//            std::cout << "is Nill" << std::endl;
//            lua_pop(L, 1);
//            lua_newtable(L);
//        }


        lua_setglobal(L, variables.at(0).c_str());

    } else {
        lua_pushinteger(L, value);
        lua_setglobal(L, variableName.c_str());
    }
}

void LuaEngine::registerVariable(const std::string &variableName, const float &value) {
    if (!isStateEnable(__func__)){
        return;
    }
    lua_pushnumber(L, value);
    lua_setglobal(L, variableName.c_str());
}

void LuaEngine::registerVariable(const std::string &variableName, const bool &value) {
    if (!isStateEnable(__func__)){
        return;
    }
    lua_pushboolean(L, value);
    lua_setglobal(L, variableName.c_str());
}

/**
 * @brief LuaEngine::printStack Print the LuaStack
 * Based on: https://szpg1108.wordpress.com/2013/08/08/understanding-the-lua-stack-pt-2-viewing-the-stack/
 */
void LuaEngine::printStack() {
    if (!isStateEnable(__func__)){
        return;
    }

    std::cout << "-- LuaEngine::printStack --" << std::endl;

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
    std::cout << "-------------------------" << std::endl;
}

std::string LuaEngine::getError() {
    return mError;
}

/**
 * @brief LuaEngine::getTableKeys
 * @param variableName
 * @return
 * Based on: https://eliasdaler.wordpress.com/2013/10/20/lua_and_cpp_pt2/
 */
std::vector<std::string> LuaEngine::getTableKeys(const std::string& variableName) {
    if (!isStateEnable(__func__)){
        return std::vector<std::string>();
    }

    std::vector<std::string> keys;
    std::string luaGetKeysScript = "function getKeys(name) s = \"\" for k, v in pairs(_G[name]) do s = s..k..\",\" end return s end";

    luaL_loadstring(L, luaGetKeysScript.c_str());
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys");
    lua_pushstring(L, variableName.c_str());
    lua_pcall(L, 1, 1, 0);

    if (lua_isstring(L, -1)) {
        std::string keysString = lua_tostring(L, -1);
        keys                   = stringExplode(keysString, ',');
    }

    return keys;
}

void LuaEngine::cleanStack() {
    if (!isStateEnable(__func__)){
        return;
    }

    int top = lua_gettop(L);
    lua_pop(L, top);
}

LuaEngine::~LuaEngine() {
    lua_close(L);
    L = nullptr;
    delete mInstance;
    mInstance = nullptr;
}

void LuaEngine::setTable(const unsigned int &ref, const std::vector<std::string> &elements, const int &value) {
    std::cout << "Elements size: " << elements.size() << " Ref: " << ref << std::endl;
    printStack();

    lua_pushstring(L, elements.at(ref).c_str());
    lua_gettable(L, -1);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        std::cout << "Created table" << std::endl;
    } else {
        std::cout << "Not created table" << std::endl;
    }
    printStack();


    std::cout << elements.at(ref) << std::endl;
    if (ref == elements.size() - 1) {
        std::cout << "Set field" << std::endl;
        setField(elements.at(ref).c_str(), value);
    } else {
        std::cout << "Set table" << std::endl;
        printStack();
        lua_newtable(L);
        lua_pushstring(L, elements.at(ref).c_str());
        setTable(ref + 1, elements, value);
        lua_settable(L, -3);
    }


}

void LuaEngine::setField(const char* index, const int &value) {
    lua_pushstring(L, index);
    lua_pushnumber(L, value);
    lua_settable(L, -3);
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
        result.push_back(token);
    }

    return result;
}

bool LuaEngine::isStateEnable(const char* funcName) {
    if (L == nullptr) {
        std::cout << "[LuaEngine::" + std::string(funcName) + "] Lua State not initialized" << std::endl;
        return false;
    }
    return true;
}

bool LuaEngine::loadToStack(const std::string& variableName) {
    if (!isStateEnable(__func__)){
        return false;
    }

    currentLevel = 0;

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
