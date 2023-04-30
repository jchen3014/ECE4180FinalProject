#include "Mutex.h"
#include "cmsis_os.h"
#include "Thread.h"
#include "mbed.h"
#include "rtos.h" //might need rtos though

Serial pc(USBTX, USBRX);
RawSerial bt(p28, p27);
RawSerial pi(p9, p10);

DigitalOut solenoid(p30);

//solenoid mutex
Mutex solenoid_mutex;


//might need to mutex these as they are both serial, but it might be fine because reading
void bt_recv() {
    while(bt.readable()) {
        char btchar = bt.getc();
        if (false) {
            unlock();
        }
    }
}

//need to come up with some sort of communication standard for Pi communication
void pi_recv() {
    while (pi.readable()) {
        char pichar = pi.getc();
        if (false) {
            unlock();
        }
    }
}

void unlock() {
    solenoid_mutex.lock();
    solenoid = 1;
    Thread::wait(5000);
    solenoid = 0;
    Thread::wait(5000); //forced solenoid cooldown so it doesn't burn out
    solenoid_mutex.unlock();
}

int main() {
    bt.baud(9600);
    bt.attach(&bt_recv, Serial::RxIrq);

    pi.baud(9600); //this might need to be different
    pi.attach(&pi_recv, Serial::RxIrq);

    solenoid = 0;

    
    while(1) {
        Thread::wait(1000);
    }
}
