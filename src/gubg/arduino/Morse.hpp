#ifndef HEADER_gubg_arduino_Morse_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Morse_hpp_ALREADY_INCLUDED

#include "gubg/std/cstdint.hpp"
#include "gubg/std/array.hpp"
#include "gubg/mss.hpp"

namespace gubg { namespace arduino { 

    template <size_t BufferSize>
    class Morse
    {
    public:
        bool empty() const { return nr_ == 0 && it_ == end_; }

        bool set_message(const char *msg)
        {
            MSS_BEGIN(bool);

            it_ = 0;
            end_ = 0;
            nr_ = 0;

            size_t ix = 0;
            for (; !!*msg; ++msg, ++ix)
            {
                MSS(ix < buffer_.size());
                const auto ch = *msg;
                MSS(is_alpha_(ch) || is_digit_(ch) || is_space_(ch));
                buffer_[ix] = ch;
            }
            end_ = ix;

            MSS_END();
        }

        //Returns true if signal should be emmitted, false for silence
        bool process()
        {
            if (nr_ == 0)
                prepare_char_();
            if (nr_ == 0)
                return false;

            const bool signal = (mask_ & 0x01);
            mask_ >>= 1;
            --nr_;
            return signal;
        }

    private:
        static bool is_alpha_(char ch) {return 'a' <= ch && ch <= 'z';}
        static bool is_digit_(char ch) {return '0' <= ch && ch <= '9';}
        static bool is_space_(char ch) {return ' ' == ch;}

        void prepare_char_()
        {
            if (it_ >= end_)
                return;
            const auto ch = buffer_[it_];
            switch (ch)
            {
                case 'a': push_code_(".-"); break;
                case 'b': push_code_("-..."); break;
                case 'c': push_code_("-.-."); break;
                case 'd': push_code_("-.."); break;
                case 'e': push_code_("."); break;
                case 'f': push_code_("..-."); break;
                case 'g': push_code_("--."); break;
                case 'h': push_code_("...."); break;
                case 'i': push_code_(".."); break;
                case 'j': push_code_(".---"); break;
                case 'k': push_code_("-.-"); break;
                case 'l': push_code_(".-.."); break;
                case 'm': push_code_("--"); break;
                case 'n': push_code_("-."); break;
                case 'o': push_code_("---"); break;
                case 'p': push_code_(".--."); break;
                case 'q': push_code_("--.-"); break;
                case 'r': push_code_(".-."); break;
                case 's': push_code_("..."); break;
                case 't': push_code_("-"); break;
                case 'u': push_code_("..-"); break;
                case 'v': push_code_("...-"); break;
                case 'w': push_code_(".--"); break;
                case 'x': push_code_("-..-"); break;
                case 'y': push_code_("-.--"); break;
                case 'z': push_code_("--.."); break;
                case '0': push_code_("-----"); break;
                case '1': push_code_(".----"); break;
                case '2': push_code_("..---"); break;
                case '3': push_code_("...--"); break;
                case '4': push_code_("....-"); break;
                case '5': push_code_("....."); break;
                case '6': push_code_("-...."); break;
                case '7': push_code_("--..."); break;
                case '8': push_code_("---.."); break;
                case '9': push_code_("----."); break;

                case ' ': push_code_(" "); break;
            }
            if (it_ > 0)
                push_separator_(buffer_[it_-1], ch);
            ++it_;
        }

        void add_(const char ch)
        {
            std::uint16_t mask = 0;
            size_t nr = 0;
            switch (ch)
            {
                case '.':
                    nr += 1;
                    mask <<= 1;
                    mask |= 0b1;
                    break;
                case '-':
                    nr += 3;
                    mask <<= 3;
                    mask |= 0b111;
                    break;
                case ' ':
                    nr += 7;
                    break;
            }

            //Trailing space
            mask <<= 1;
            ++nr;

            mask <<= nr_;
            mask_ |= mask;
            nr_ += nr;
        }
        void push_code_(const char *str)
        {
            mask_ = 0;
            nr_ = 0;
            for (; !!*str; ++str)
                add_(*str);
        }
        void push_separator_(char prv, char cur)
        {
            if (is_space_(prv) || is_space_(cur))
                return;
            mask_ <<= 3;
            nr_ += 3;
        }

        std::uint16_t mask_ = 0;
        std::uint8_t nr_ = 0;
        std::array<char, BufferSize> buffer_;
        size_t it_ = 0;
        size_t end_ = 0;
    };

} } 

#endif
