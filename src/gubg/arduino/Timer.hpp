#ifndef HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED

namespace gubg { namespace arduino { 

    template <typename T, typename Receiver>
    class Timer_crtp
    {
    public:
        void start_timer(T value) { value_ = value; }
        void add_to_timer(T value) { value_ += value; }
        T timer() const {return value_;}

        void process(T elapsed)
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
                receiver_().timer_run();
            }
        }

    private:
        Receiver &receiver_() {return *static_cast<Receiver*>(this);}

        T value_ = 0;
    };

} } 

#endif
