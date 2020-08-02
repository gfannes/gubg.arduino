#ifndef HEADER_gubg_arduino_Clock_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Clock_hpp_ALREADY_INCLUDED

#include <Arduino.h>

namespace gubg { namespace arduino { 

    class MillisClock
    {
    public:
        using TimePoint = unsigned long;

        static TimePoint now() {return millis();}
    private:
    };

} } 

#endif
