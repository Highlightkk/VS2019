#include <iostream>
#include <bitset>
using namespace std;

#define ACK 170
#define pin_ON_OFF 6
#define pin_SS 7
#define NtarMax 5
#define f0Min 5
#define f0Max 195
#define f0Max_CW 245
#define BWMin 50
#define BWMax 240
#define NsMin 50
#define NsMax 200
#define RmaxMax 100
#define VmaxMax 63

int main()
{
    uint8_t _configuration[8];
    uint8_t mode, f0, BW, Ns, Ntar, Rmax, MTI, Mth; // variable definition
    uint16_t I_Buffer[200], Q_Buffer[200];
    bool distance_true, velocity_true, SNR_true, I_true, Q_true, movement_true;

    mode = 2;   // sawtooth mode
    f0 = 5;     // starting at 24.005 GHz
    BW = 240;   // using all the BW available = 240 MHz
    Ns = 200;   // 200 samples
    Ntar = 3;   // 2 targets of interest
    Rmax = 100; // searching along the full distance range
    MTI = 0;    // MTI disabled because we want information of static and moving targets
    Mth = 0;    // parameter not used because "movement" is not requested

    distance_true = 1;
    velocity_true = 0;
    SNR_true = 1;
    I_true = 1;
    Q_true = 1;
    movement_true = 0;;

    // Check correct values
    if ((mode == 0) || (mode > 4)) {
        mode = 3;
    }
    if ((f0 > f0Max) && (mode != 1)) {
        f0 = f0Min;
    }
    else if ((f0 > f0Max_CW) && (mode == 1)) {
        f0 = f0Min;
    }
    else if (f0 < f0Min) {
        f0 = f0Min;
    }
    uint8_t BW_available = BWMax - f0 + f0Min;
    if ((BW < BWMin) || (BW > BW_available)) {
        BW = BW_available;
    }
    if ((Ns < NsMin) || (Ns > NsMax)) {
        Ns = NsMax;
    }
    if ((Ntar == 0) || (Ntar > NtarMax)) {
        Ntar = 1;
    }
    if ((mode != 1) && ((Rmax < 1) || (Rmax > RmaxMax))) {
        Rmax = RmaxMax;
    }
    else if ((mode == 1) && (Rmax > VmaxMax)) {
        Rmax = VmaxMax;
    }
    if (MTI > 1) {
        MTI = 0;
    }
    if ((Mth == 0) || (Mth > 4)) {
        Mth = 4;
    }
    Mth--;

    // Create _configuration register
    _configuration[0] = (mode << 5) + (f0 >> 3); // mode*32 + f0/8 = 64
    _configuration[1] = (f0 << 5) + (BW >> 3);   // f0*32 + BW/8 = 190
    _configuration[2] = (BW << 5) + (Ns >> 3);   // BW*32 + Ns/8 = 25
    _configuration[3] = (Ns << 5) + (Ntar << 2) + (Rmax >> 6); // Ns*32 + Ntar*4 + Rmax/64 = 13
    _configuration[4] = (Rmax << 2) + MTI;		 // Rmax*4 + MTI = 144
    _configuration[5] = (Mth << 6) + 0b00100000; // Mth*64 + 0b00100000 = 224
    _configuration[6] = 0; // sum(1-5) = 148

    if (distance_true != 0) {
        _configuration[6] = 0b10000000;
    }
    if (velocity_true != 0) {
        _configuration[6] = _configuration[6] + 0b01000000;
    }
    if (SNR_true != 0) {
        _configuration[6] = _configuration[6] + 0b00100000;
    }
    if (I_Buffer != 0) {
        _configuration[6] = _configuration[6] + 0b00010000;
    }
    if (Q_Buffer != 0) {
        _configuration[6] = _configuration[6] + 0b00001000;
    }
    if (movement_true != 0) {
        _configuration[6] = _configuration[6] + 0b00000100;
    }

    uint8_t CRC;
    CRC = (uint8_t)(_configuration[0] + _configuration[1] + _configuration[2] + _configuration[3] + _configuration[4] + _configuration[5] + _configuration[6]);
    _configuration[7] = CRC; // 148 + 184 = 76
    int i;
    for (i = 0; i < 8; i++) {
        cout << "_configuration[" << i << "] is£º" << bitset<sizeof(uint8_t) * 8>(_configuration[i]) << "." << endl;
    }

    uint8_t results[NtarMax * 3 * 4 + 2], mode_temp, buff_temp[4], Ntar_temp, temp;

    _configuration[5] = _configuration[5] & 0b11011111; // 192
    // results output array
    float distance[5], SNR[5];

    if (distance_true != 0 || velocity_true != 0 || SNR_true != 0 || movement_true != 0) {
        Ntar_temp = (_configuration[3] & 0b00011100) >> 2; // 3
        for (i = 0; i < Ntar_temp; i++) {
            if (distance_true != 0) {
                buff_temp[0] = 0;
                buff_temp[1] = 0;
                buff_temp[2] = 0;
                buff_temp[3] = 63;
                distance[i] = *((float*)(buff_temp));
                cout << "distance[" << i << "] is£º" << (distance[i]) << "." << endl;
            }
            // if (velocity != 0) {
            //     buff_temp[0] = results[NtarMax*4+i*4];
            //     buff_temp[1] = results[NtarMax*4+i*4+1];
            //     buff_temp[2] = results[NtarMax*4+i*4+2];
            //     buff_temp[3] = results[NtarMax*4+i*4+3];
            //     velocity[i] = *((float*)(buff_temp));
            // }
            // if (SNR != 0) {
            //     buff_temp[0] = results[NtarMax*8+i*4];
            //     buff_temp[1] = results[NtarMax*8+i*4+1];
            //     buff_temp[2] = results[NtarMax*8+i*4+2];
            //     buff_temp[3] = results[NtarMax*8+i*4+3];
            //     SNR[i] = *((float*)(buff_temp));
            // }
        }
        // if (movement != 0) {
        //     if (results[NtarMax*12] == 255) {
        //         movement[0] = true;
        //     } else {
        //         movement[0] = false;
        //     }
        // }
    }

    uint16_t Ns_temp;

    mode_temp = (_configuration[0] & 0b11100000) >> 5;
    Ns_temp = ((_configuration[2] & 0b00011111) << 3) + ((_configuration[3] & 0b11100000) >> 5);
    if (mode_temp == 3) {
        Ns_temp = 2 * Ns_temp;
    }
    else if (mode_temp == 4) {
        Ns_temp = 2 * Ns_temp + 2 * ceil(0.75 * Ns_temp);
    }

    uint16_t I_Bin = 0b10000100;
    float I_float = *((float*)&I_Bin);
    printf("I_float = %f.\n", I_float);
    return 0;
}