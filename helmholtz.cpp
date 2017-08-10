
//Functions that control the Helmholtz coils

#include "field.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <unistd.h>

// Include the Sensoray drivers

extern "C" {
#include "DAQ/s626drv.h"
#include "DAQ/App626.h"
#include "DAQ/s626mod.h"
#include "DAQ/s626core.h"
#include "DAQ/s626api.h"
}

// Hardware defaults
#define max_current 8.0
#define max_volt 8191

void initialize_sensoray() {

    S626_OpenBoard(0, 0, NULL, 1);

    DWORD ErrCode = S626_GetErrors(0);

    if (ErrCode) {
        std::cout << "HELMHOLTZ COIL: Error(s) opening the s626 DAQ card.\n";

        // List errors.
        if (ErrCode & 0x00000001)		std::cout << " * Failed to open kernel-mode driver.\n";
        if (ErrCode & 0x00000002)		std::cout << " * Can't detect/register board.\n";
        if (ErrCode & 0x00000004)		std::cout << " * Memory allocation error.\n";
        if (ErrCode & 0x00000008)		std::cout << " * Can't lock DMA buffer.\n";
        if (ErrCode & 0x00000010)		std::cout << " * Can't launch interrupt thread.\n";
        if (ErrCode & 0x00000020)		std::cout << " * Can't enable IRQ.\n";
        if (ErrCode & 0x00000040)		std::cout << " * Missed interrupt.\n";
        if (ErrCode & 0x00000080)		std::cout << " * Can't instantiate board object.\n";
        if (ErrCode & 0x00000100)		std::cout << " * Unsupported kernel-mode driver version.\n";
        if (ErrCode & 0x00010000)		std::cout << " * D/A communication timeout.\n";
        if (ErrCode & 0x00020000)		std::cout << " * Illegal counter parameter.\n";

    }

    std::cout << "HELMHOLTZ COIL: Succeeded opening the s626 DAQ card.\n";
}

void setXcurrent(float current) {

    if (current > max_current)
        current = max_current;
    if (current < -max_current)
        current = -max_current;

    int volts = (int)(current*(max_volt / max_current));
    //std::cout<<"volt_x: "<<volts<<"\n";
    S626_WriteDAC(0, 0, volts); // set the x voltage (pin 42)

    return;
}

void setYcurrent(float current) {

    if (current > max_current)
        current = max_current;
    if (current < -max_current)
        current = -max_current;

    int volts = (int)(current*(max_volt / max_current));
    //std::cout<<"volt_y: "<<volts<<"\n";
    S626_WriteDAC(0, 1, volts); // set the y voltage (pin 44)

    return;
}

void setZcurrent(float current) {

    if (current > max_current)
        current = max_current;
    if (current < -max_current)
        current = -max_current;

    int volts = (int)(current*(max_volt / max_current));
    //std::cout<<"volt_z: "<<volts<<"\n";
    S626_WriteDAC(0, 2, volts); // set the z voltage (pin 46)

    return;
}

void setCoilsOff() {

    S626_WriteDAC(0, 0, 0); // set the x voltage to 0v (pin 42)
    S626_WriteDAC(0, 1, 0); // set the y voltage to 0v (pin 44)
    S626_WriteDAC(0, 2, 0); // set the z voltage to 0V (pin 46)
    std::cout << "HELMHOLTZ COIL: Setting all coils OFF\n";

    return;
}

void setInhibitOff() {

    S626_WriteDAC(0, 3, max_volt); // set the inhibit voltage to 0 (pin 48)

    setCoilsOff();

    std::cout << "HELMHOLTZ COIL: Set inhibit OFF.\n";

    return;
}


void setInhibitOn() {

    S626_WriteDAC(0, 3, 0); // set the inhibit voltage to 0 (pin 48)

    setCoilsOff();

    std::cout << "HELMHOLTZ COIL: Set inhibit ON.\n";

    return;
}
void turn_on() {
    initialize_sensoray();
    setInhibitOff();
    std::cout << "Process complete\n";
}

void turn_off() {
    setCoilsOff();
    setInhibitOn();
    S626_CloseBoard(0);
    std::cout << "Process complete\n";
}
