#ifndef HEADER_gubg_arduino_rs485_Endpoint_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_rs485_Endpoint_hpp_ALREADY_INCLUDED

#include "gubg/std/cassert.hpp"
#include "gubg/std/cstddef.hpp"
#include "gubg/std/cstdint.hpp"
#include "gubg/std/algorithm.hpp"
#include "gubg/std/array.hpp"
#include "gubg/mss.hpp"
#include "HardwareSerial.h"

namespace gubg { namespace arduino { namespace rs485 { 

    class Endpoint: public Timer_crtp<unsigned long, Endpoint>
    {
    private:
        using Base = Timer_crtp<unsigned long, Endpoint>;

    public:
        void init(HardwareSerial &hws, unsigned int tx_enable_pin, unsigned long baud_rate, std::uint8_t config)
        {
            hws_ = &hws;
            tx_enable_pin_ = tx_enable_pin;

            pinMode(tx_enable_pin_, OUTPUT);

            hws_->begin(baud_rate, config);

            {
                const auto nr_bits = nr_bits_(config);
                char_duration_us_ = nr_bits*1000000ul/baud_rate;
            }

            change_state_(State::Listening);
        }

        bool process(unsigned long elapse)
        {
            MSS_BEGIN(bool);
            MSS(!!hws_);
            Base::process(elapse);
            MSS_END();
        }

        bool is_sending() const { return state_ == State::PrepareToSend || state_ == State::Sending || state_ == State::PrepareToListen; }

        template <typename Byte>
        bool send(size_t &offset, const Byte *buffer, size_t size)
        {
            static_assert(sizeof(Byte) == sizeof(std::byte), "Can only send byte-like elements");
            return send_(offset, (const std::byte *)buffer, size);
        }

        void timer_run()
        {
            switch (state_)
            {
                case State::PrepareToSend:
                    change_state_(State::Sending);
                    break;
                case State::Sending:
                    change_state_(State::PrepareToListen);
                    break;
                case State::PrepareToListen:
                    change_state_(State::Listening);
                    break;
            }
        }

    private:
        enum class State { Init, Listening, PrepareToSend, Sending, PrepareToListen, };

        bool send_(size_t &offset, const std::byte *buffer, size_t size)
        {
            MSS_BEGIN(bool);

            MSS(!!hws_);

            //Append to buffer
            for (auto nr_to_write = std::min<unsigned int>(size-offset, buffer_.size()-buffer_size_);
                    nr_to_write > 0;
                    --nr_to_write, ++buffer_size_, ++offset)
            {
                buffer_[buffer_size_] = (std::uint8_t)buffer[offset];
            }

            send_buffer_();

            MSS_END();
        }
        void send_buffer_()
        {
            switch (state_)
            {
                case State::PrepareToSend:
                    //We cannot send yet
                    break;
                case State::Sending:
                    {
                        if (buffer_size_ == 0)
                            return change_state_(State::PrepareToListen);
                        const auto nr_to_write = std::min<unsigned int>(buffer_size_, hws_->availableForWrite());
                        hws_->write(buffer_.data(), nr_to_write);
                        Base::add_to_timer(char_duration_us_*nr_to_write);
                        for (auto i = nr_to_write; i < buffer_size_; ++i)
                            buffer_[i-nr_to_write] = buffer_[i];
                        buffer_size_ -= nr_to_write;
                    }
                    break;
                default:
                    change_state_(State::PrepareToSend);
                    break;
            }
        }
        static unsigned int nr_bits_(std::uint8_t config)
        {
            switch (config)
            {
                case SERIAL_5N1: return 1+5+0+1;
                case SERIAL_6N1: return 1+6+0+1;
                case SERIAL_7N1: return 1+7+0+1;
                case SERIAL_8N1: return 1+8+0+1;
                case SERIAL_5N2: return 1+5+0+2;
                case SERIAL_6N2: return 1+6+0+2;
                case SERIAL_7N2: return 1+7+0+2;
                case SERIAL_8N2: return 1+8+0+2;
                case SERIAL_5E1: return 1+5+1+1;
                case SERIAL_6E1: return 1+6+1+1;
                case SERIAL_7E1: return 1+7+1+1;
                case SERIAL_8E1: return 1+8+1+1;
                case SERIAL_5E2: return 1+5+1+2;
                case SERIAL_6E2: return 1+6+1+2;
                case SERIAL_7E2: return 1+7+1+2;
                case SERIAL_8E2: return 1+8+1+2;
                case SERIAL_5O1: return 1+5+1+1;
                case SERIAL_6O1: return 1+6+1+1;
                case SERIAL_7O1: return 1+7+1+1;
                case SERIAL_8O1: return 1+8+1+1;
                case SERIAL_5O2: return 1+5+1+2;
                case SERIAL_6O2: return 1+6+1+2;
                case SERIAL_7O2: return 1+7+1+2;
                case SERIAL_8O2: return 1+8+1+2;
            }
            return 0;
        }

        void change_state_(State new_state)
        {
            if (new_state == state_)
                return;
            //Exit
            switch (state_)
            {
            }
            state_ = new_state;
            //Enter
            switch (state_)
            {
                case State::Listening:
                    digitalWrite(tx_enable_pin_, false);
                    break;
                case State::PrepareToSend:
                    digitalWrite(tx_enable_pin_, true);
                    Base::start_timer(char_duration_us_);
                    break;
                case State::Sending:
                    send_buffer_();
                    break;
                case State::PrepareToListen:
                    Base::start_timer(char_duration_us_);
                    break;
            }
        }
        State state_ = State::Init;
        std::array<std::uint8_t, 16> buffer_;
        unsigned int buffer_size_ = 0;
        HardwareSerial *hws_ = nullptr;
        unsigned int tx_enable_pin_;
        unsigned long char_duration_us_;
    };

} } } 

#endif
