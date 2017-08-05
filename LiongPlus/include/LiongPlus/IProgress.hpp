// File: IProgress.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_IProgress
#define _L_IProgress
#include "Fundamental.hpp"

namespace LiongPlus
{   
    template<typename T>
    class IProgress
        : public Interface
    {
    public:
        virtual void Report(T value) = 0;
    };
}

#endif
