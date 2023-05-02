#include "mbed.h"
#include "mpr121.h"
#include <string>

RawSerial  pc(USBTX, USBRX);
RawSerial  pi(p13, p14);
RawSerial  bt(p28,p27);
DigitalOut solenoid(p30);
DigitalOut led1(LED1);
DigitalOut led4(LED4);


// Create the interrupt receiver object on pin 26
InterruptIn interrupt(p11);
I2C i2c(p9, p10);
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);

volatile bool unlock = false;
string btstring = "";
string pistring = "";

string password = "cordicupdate";

volatile int memory[] = {0,0,0,0,0,0,0,0};
volatile int mem_index = 0;
int key_arr[] = {1,0, 1,0, 1,0, 1,0}; //note that these are keycodes and not numbers on the keypad

time_t keycode_start_time = 0;

time_t keycode_timeout = 3;

void fallInterrupt() {
    if (mem_index > 7)
    {
        mem_index = 0;
    } else if (mem_index == 0) {
        keycode_start_time = time(NULL);
    } else if (time(NULL) - keycode_start_time >= keycode_timeout) {
        mem_index = 0;
        // pc.printf("Keypad timeout\n");
        for (int i = 0; i<8; i++) {
            memory[i] = 0;
        }

    }

    int key_code=0;
    int i=0;
    int value=mpr121.read(0x00);
    value +=mpr121.read(0x01)<<8;
    
    memory[mem_index] = value;
    mem_index = mem_index + 1;
    if (value == 0 && mem_index % 2 == 1) { //impossible, means keycode array is out of sync
        mem_index = 0;
        keycode_start_time = time(NULL);
    } else if (mem_index >= 7) {
        bool correct_code = true;
        for (int i = 0; i<8; i++) {
            if (memory[i] != key_arr[i])
            {
                correct_code = false;
                break;
            }
        }
        if (correct_code) {
            unlock = true;
            pi.puts("keypad_unlock\n");
        }
        for (int i = 0; i<8; i++) {
            memory[i] = 0;
        }
        mem_index = 0;
        keycode_start_time = time(NULL);
    }
}

void bt_recv()
{
    while(bt.readable()) {
        char btchar = bt.getc();
        if (btchar == '\n') {
            if ((btstring.rfind(password, 0) == 0 && btstring.rfind("unlock") != -1) || btstring.rfind("1") != -1) { //remove second option for added security
                unlock = true;
                pi.puts("bluetooth_unlock\n");
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
        pc.putc(pichar);
        if (pichar == '\n') {
            pc.puts(pistring.c_str());
            if ((pistring.rfind(password, 0) != -1 && pistring.rfind("unlock") != -1) || pistring.rfind("1") != -1) { //remove second option for added security
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

    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);

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
        wait(1);
        // pc.puts(pistring.c_str());
        // wait(1);
        // pc.printf("Mem= ");
        // for (int i = 0; i <= 7; i++) {
        //     pc.printf("%x ", memory[i]);
        // }
        // pc.printf("\n");
        // pc.printf("Start time: %d\n", keycode_start_time);
        // pc.printf("Keypad Index: %d\n", mem_index);
    }
}
