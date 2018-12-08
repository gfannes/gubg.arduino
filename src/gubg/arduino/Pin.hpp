#ifndef HEADER_gubg_arduino_Pin_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Pin_hpp_ALREADY_INCLUDED

#include "gubg/std/cstdint.hpp"

namespace gubg { namespace arduino { 

    class Pin
    {
    private:
        enum class State : std::uint8_t {
            NoPin,
            HasPin,
            Input0,
            Input1,
            Output0,
            Output1,
        };
    public:
        Pin(){}
        Pin(std::uint8_t pin): state_(State::HasPin), pin_(pin) {}

        void set_pin(std::uint8_t pin)
        {
            pin_ = pin;
            state_ = State::HasPin;
        }

        bool has_pin() const {return state_ != State::NoPin;}
        bool is_output() const {return state_ == State::Output0 || state_ == State::Output1;}
        bool is_output(bool on) const {return state_ == (on ? State::Output1 : State::Output0);}
        bool is_input() const {return  state_ == State::Input0  || state_ == State::Input1;}

        bool set_output(bool on)
        {
            if (!has_pin())
                return false;
            if (!is_output())
                pinMode(pin_, OUTPUT);
            state_ = (on ? State::Output1 : State::Output0);
            digitalWrite(pin_, state_ == State::Output1);
            return true;
        }

        bool toggle()
        {
            return set_output(state_ == State::Output0);
        }

    private:
        State state_ = State::NoPin;
        std::uint8_t pin_;
    };

} } 

#endif
