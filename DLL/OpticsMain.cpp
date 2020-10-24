// dllmain.cpp : Defines the entry point for the DLL application.
#include "OpticsClasses.hpp"

namespace Opticsproject
{

    extern "C" OPTICSPROJECT_API std::string system_run(std::string system)
    {
        std::string a = system;
        json j = json::parse(a);
        System sys(j);
        sys.run();
        return sys.get_result().dump();
        /*char* t = new char[sys.get_result().length() + 1];
        strcpy(t,sys.get_result().c_str());
        return t;*/
    }

}

