#ifndef HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED

namespace gubg { namespace arduino { 

    template <typename T>
    class Timer
    {
    public:
        //These functions can be called during process()-ing as well.
        void start(T value) { value_ = value; }
        void add(T value) { value_ += value; }
        T value() const {return value_;}

        template <typename Ftor>
        void process(T elapsed, Ftor &&ftor)
        {
            while (true)
            {
                if (value_ == 0)
                {
                    //No timer value was actually set
                    return;
                }

                if (elapsed < value_)
                {
                    //Not enough time elapsed to make the timer fire
                    value_ -= elapsed;
                    return;
                }

                //Enough time elapsed to make the timer fire
                elapsed -= value_;
                value_ = 0;
                ftor();
            }
        }

    private:
        T value_ = 0;
    };

} } 

#endif
