#ifndef HEADER_gubg_arduino_Blinker_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Blinker_hpp_ALREADY_INCLUDED

#include "gubg/arduino/Pin.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/std/array.hpp"

namespace gubg { namespace arduino { 

    class Blinker
    {
    public:
        using Micros = decltype(micros());
        enum class Mode
        {
            SlowDark,
            SlowEven,
            SlowBright,
            FastDark,
            FastEven,
            FastBright,
            Dark,
            Bright,
        };

        Blinker(unsigned int pin): pin_(pin)
        {
            set_mode(Mode::SlowDark);
        }

        void set_mode(Mode mode)
        {
            switch (mode)
            {
                case Mode::SlowDark:
                    pin_.set_output(true);
                    duration_[0] = 450000u;
                    duration_[1] =  50000u;
                    break;
                case Mode::SlowEven:
                    pin_.set_output(true);
                    duration_[0] = 250000u;
                    duration_[1] = 250000u;
                    break;
                case Mode::SlowBright:
                    pin_.set_output(true);
                    duration_[0] =  50000u;
                    duration_[1] = 450000u;
                    break;

                case Mode::FastDark:
                    pin_.set_output(true);
                    duration_[0] = 45000u;
                    duration_[1] =  5000u;
                    break;
                case Mode::FastEven:
                    pin_.set_output(true);
                    duration_[0] = 25000u;
                    duration_[1] = 25000u;
                    break;
                case Mode::FastBright:
                    pin_.set_output(true);
                    duration_[0] =  5000u;
                    duration_[1] = 45000u;
                    break;

                case Mode::Dark:
                    pin_.set_output(false);
                    duration_[0] = 0u;
                    duration_[1] = 0u;
                    break;
                case Mode::Bright:
                    pin_.set_output(true);
                    duration_[0] = 0u;
                    duration_[1] = 0u;
                    break;
            }
            timer_.start(duration_[pin_.is_output(true)]);
        }

        void process(Micros elapsed_us)
        {
            timer_.process(elapsed_us, [&](){
                    pin_.toggle();
                    timer_.add(duration_[pin_.is_output(true)]);
            });
        }

    private:
        Pin pin_;
        Timer<Micros> timer_;
        std::array<Micros, 2> duration_;
    };

} } 

#endif
