#ifndef HEADER_gubg_arduino_ppm_Pin11_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_ppm_Pin11_hpp_ALREADY_INCLUDED

#include <gubg/arduino/ppm/Pin01.hpp>

namespace gubg { namespace arduino { namespace ppm { 

    template <unsigned int PinId, unsigned int Min, unsigned int Max>
    class Pin11: public Pin01<PinId, Min, Max>
    {
    private:
        using Base = Pin01<PinId, Min, Max>;

    public:
        float as_float() const { return Base::as_float()*2.0f-1.0f; }
    };

} } } 

#endif
