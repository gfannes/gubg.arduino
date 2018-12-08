#ifndef HEADER_gubg_arduino_rs485_Endpoint_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_rs485_Endpoint_hpp_ALREADY_INCLUDED

#include "gubg/std/cassert.hpp"
#include "gubg/std/cstddef.hpp"
#include "gubg/std/cstdint.hpp"
#include "gubg/std/algorithm.hpp"
#include "gubg/arduino/Timer.hpp"
#include "gubg/mss.hpp"
#include "HardwareSerial.h"

namespace gubg { namespace arduino { namespace rs485 { 

    class Endpoint
    {
    public:
        void init(HardwareSerial &hws, unsigned int tx_enable_pin, unsigned long baud_rate, std::uint8_t config)
        {
            hws_ = &hws;
            tx_enable_pin_ = tx_enable_pin;

            pinMode(tx_enable_pin_, OUTPUT);
            digitalWrite(tx_enable_pin_, is_sending_);

            hws_->begin(baud_rate, config);

            {
                const auto nr_bits = nr_bits_(config);
                char_duration_us_ = nr_bits*(1000000ul/baud_rate+1);
            }
        }

        bool process(unsigned long elapse)
        {
            MSS_BEGIN(bool);
            MSS(!!hws_);
            send_timer_.process(elapse, [&](){
                    is_sending_ = false;
                    digitalWrite(tx_enable_pin_, is_sending_);
                    });
            MSS_END();
        }

        bool is_sending() const { return is_sending_; }

        template <typename Byte>
        bool send(size_t &offset, const Byte *buffer, size_t size)
        {
            static_assert(sizeof(Byte) == sizeof(std::byte), "Can only send byte-like elements");
            return send_(offset, (const std::byte *)buffer, size);
        }

    private:
        bool send_(size_t &offset, const std::byte *buffer, size_t size)
        {
            MSS_BEGIN(bool);
            MSS(!!hws_);
            if (!is_sending_)
            {
                is_sending_ = true;
                digitalWrite(tx_enable_pin_, is_sending_);
                send_timer_.start_timer(char_duration_us_);
            }
            const auto nr_to_write = std::min<unsigned int>(size-offset, hws_->availableForWrite());
            hws_->write((const std::uint8_t *)buffer, nr_to_write);
            send_timer_.add_to_timer(char_duration_us_*nr_to_write);
            offset += nr_to_write;
            MSS_END();
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
        HardwareSerial *hws_ = nullptr;
        RelativeTimer<unsigned long> send_timer_;
        bool is_sending_ = false;
        unsigned int tx_enable_pin_;
        unsigned long char_duration_us_;
    };

} } } 

#endif
