#ifndef HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Timer_hpp_ALREADY_INCLUDED

namespace gubg { namespace arduino { 

    template <typename T>
    class RelativeTimer
    {
    public:
        //These functions can be called during process()-ing as well.
        void start_timer(T value) { value_ = value; }
        void add_to_timer(T value) { value_ += value; }
        T timer() const {return value_;}

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

    template <typename T>
    class AbsoluteTimer
    {
    public:
        //Keep processing
        template <typename Ftor>
        bool process(const T time, Ftor &&ftor)
        {
            const T elapsed = time-prev_;
            prev_ = time;
            return false;
        }

    private:
        T prev_ = 0;
    };

} } 

#endif
