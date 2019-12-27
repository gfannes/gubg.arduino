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

        Pin &set_pin(std::uint8_t pin)
        {
            pin_ = pin;
            state_ = State::HasPin;
        }

        bool has_pin() const {return state_ != State::NoPin;}
        bool is_output() const {return state_ == State::Output0 || state_ == State::Output1;}
        bool is_output(bool on) const {return state_ == (on ? State::Output1 : State::Output0);}
        bool is_input() const {return  state_ == State::Input0  || state_ == State::Input1;}
        bool is_input(bool on) const {return state_ == (on ? State::Input1 : State::Input0);}

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
        bool set_input(bool pullup)
        {
            if (!has_pin())
                return false;
            if (!is_input())
                pinMode(pin_, INPUT);
            digitalWrite(pin_, pullup);
            check_();
            return true;
        }
        Pin &check()
        {
            if (is_input())
                check_();
            return *this;
        }

        bool toggle()
        {
            return set_output(state_ == State::Output0);
        }
        bool toggled()
        {
            if (!is_input())
                return false;
            const auto orig_state = state_;
            check_();
            return orig_state != state_;
        }

    private:
        void check_()
        {
            state_ = (digitalRead(pin_) == HIGH ? State::Input1 : State::Input0);
        }
        State state_ = State::NoPin;
        std::uint8_t pin_;
    };

} } 

#endif
