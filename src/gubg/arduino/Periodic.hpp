#ifndef HEADER_gubg_arduino_Periodic_hpp_ALREADY_INCLUDED
#define HEADER_gubg_arduino_Periodic_hpp_ALREADY_INCLUDED

namespace gubg { namespace arduino { 

    template <typename Receiver, unsigned long Period>
    class Periodic_crtp
    {
    public:
        void process(unsigned long time)
        {
            if (remainder_ == Period)
            {
                //Do initialization
                prev_time_ = time;
                remainder_ = 0;
                receiver_().periodic_init();
            }

            //Call the run operation for as many times as needed
            for (remainder_ += (time - prev_time_);
                    remainder_ >= Period;
                    remainder_ -= Period)
                receiver_().periodic_run();

            prev_time_ = time;
        }

    private:
        Receiver &receiver_() {return *static_cast<Receiver*>(this);}

        unsigned long prev_time_;
        unsigned long remainder_ = Period;
    };

} } 

#endif
