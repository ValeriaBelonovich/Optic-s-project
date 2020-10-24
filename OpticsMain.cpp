// dllmain.cpp : Defines the entry point for the DLL application.
#include "OpticsClasses.hpp"

namespace Opticsproject
{

    extern "C" OPTICSPROJECT_API std::string system_run(std::string system)
    {
        std::cout << "Programm started" << std::endl;
        std::string a = system;
        std::cout << "Parse started" << std::endl;
        json j = json::parse(a);
        std::cout << "Parse ended" << std::endl;
        System sys(j);
        std::cout << "Programm inited, not run" << std::endl;
        sys.run();
        std::cout << "Programm ended: " << sys.get_result().dump()<< std::endl;
        return sys.get_result().dump();
        /*char* t = new char[sys.get_result().length() + 1];
        strcpy(t,sys.get_result().c_str());
        return t;*/
    }

}

