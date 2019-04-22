#ifndef HEADER_gubg_arduino_ppm_Pin_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_ppm_Pin_hpp_ALREADY_INCLUDED

#include "Arduino.h"

namespace gubg { namespace arduino { namespace ppm { 

    template <unsigned int PinId, unsigned int Min, unsigned int Max>
    class Pin
    {
    public:
        void setup()
        {
            pinMode(PinId, INPUT);
            attachInterrupt(digitalPinToInterrupt(PinId), process_pin_change_, CHANGE);
            prev_ = 0;
            pulse_duration_ = 0;
        }

        unsigned int diff() const { return pulse_duration_; }
        float as_float() const { return float(pulse_duration_-Min)/float(Max-Min); }

    private:
        static void process_pin_change_()
        {
            const unsigned int now = micros();
            if (digitalRead(PinId) == HIGH)
                prev_ = now;
            else
            {
                pulse_duration_ = now-prev_;
                if (pulse_duration_ < Min)
                    pulse_duration_ = Min;
                else if (pulse_duration_ > Max)
                    pulse_duration_ = Max;
            }
        }

        static unsigned int prev_;
        static unsigned int pulse_duration_;
    };

    template <unsigned int PinId, unsigned int Min, unsigned int Max>
    unsigned int Pin<PinId, Min, Max>::prev_ = 0;
    template <unsigned int PinId, unsigned int Min, unsigned int Max>
    unsigned int Pin<PinId, Min, Max>::pulse_duration_ = 0;

} } } 

#endif
