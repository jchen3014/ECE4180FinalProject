#include "mbed.h"
#include <string>

RawSerial  pc(USBTX, USBRX);
RawSerial  pi(p9, p10);
RawSerial  bt(p28,p27);
DigitalOut solenoid(p30);
DigitalOut led1(LED1);
DigitalOut led4(LED4);

volatile bool unlock = false;
string btstring = "";
string pistring = "";

string password = "cordicupdate";

void bt_recv()
{
    while(bt.readable()) {
        char btchar = bt.getc();
        if (btchar == '\n') {
            if ((btstring.rfind(password, 0) == 0 && btstring.rfind("unlock") != -1) || btstring.rfind("1") != -1) {
                unlock = true;
                pi.puts("BT TRIG UNLOCK");
            }
            btstring = "";
        } else {
            btstring += btchar;
        }
    }
}

void pi_recv()
{
    while(pi.readable()) {
        char pichar = pi.getc();
        if (pichar == '\n') {
            pc.puts(pistring.c_str());
            if ((pistring.rfind(password, 0) != -1 && pistring.rfind("unlock") != -1) || pistring.rfind("1") != -1) {
                unlock = true;
            }
            pistring = "";
        } else {
            pistring += pichar;
        }
    }
}

int main()
{
    pi.baud(9600);
    bt.baud(9600);
    pc.baud(9600);

    pi.attach(&pi_recv, Serial::RxIrq);
    bt.attach(&bt_recv, Serial::RxIrq);

    while(1) {
        led1 = unlock;
        if (unlock) {
            pc.printf("Unlocking...\n");
            solenoid = 1;
            wait(5);
            solenoid = 0;
            unlock = false;
            led1 = unlock;
            wait(5);
        }
        sleep();
    }
}
