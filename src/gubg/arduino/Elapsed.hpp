#ifndef HEADER_gubg_arduino_Elapsed_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Elapsed_hpp_ALREADY_INCLUDED

namespace gubg { namespace arduino { 

    template <typename T>
    class Elapsed
    {
    public:
        T operator()() const {return elapsed_;}

        T process(T v)
        {
            elapsed_ = v - prev_;
            prev_ = v;
            return elapsed_;
        }

    private:
        T prev_ = 0;
        T elapsed_ = 0;
    };

} } 

#endif
