#include <Servo.h>

class AW {

  private:
    // static members
    static const unsigned int _sweepPosition_90;
    static unsigned int _sweepTime;
    static unsigned int _sweepAngle;
    static unsigned int _sweepDelay;
    static unsigned int _sweepPosition_0;
    static unsigned int _sweepPosition_180;
    unsigned int _sweepPosition;

    // (static) functions
    static void initSweep(void);

    // other variables
    bool _KAW = false;
    int _relayPort;
    unsigned long _timeNow;
    unsigned long _timeOld;
    unsigned long _timeDelta;
    Servo _servo;

  public:
    // constructor - destructor
    AW();
    ~AW();

    // (static) functions)
    static void setSweepTime(unsigned int);
    static void setSweepAngle(unsigned int);
    void setPorts(int, int, bool);
    bool set_AW_0();
    bool set_AW_180();
};
