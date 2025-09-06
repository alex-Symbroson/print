#include<iostream>
#include <stdio.h>
using namespace std;

int printUInt(char* rbuf, uint32_t n) {
    int i = 0;
    do rbuf[i--] = '0' + n % 10;
    while (n /= 10);
    return -i;
}

char* printFloat(float f, int precision = 5, int len = 7) {
    static char buffer[32];
    memset(buffer, ' ', 32);
    buffer[31] = 0;
    char* p = buffer + 30;

    int power = 1;
    while (precision--) power *= 10;
    uint32_t n = abs(f * power);
    p -= printUInt(p, n % power);
    *p-- = '.';
    p -= printUInt(p, abs(f));
    if (f < 0) *p-- = '-';

    const int curLen = buffer+30-p;
    if (len && len > curLen) p -= len - curLen;
    return p + 1;
}

int main1()
{
    printf("%s\n", printFloat(0.0, 2));
    printf("%s\n", printFloat(0.12, 2));
    printf("%s\n", printFloat(12.34, 2));
    printf("%s\n", printFloat(-12.34, 2));
    cout << "Hello World!" << endl;
    return 0;
}

#define CELLCOUNT 11
#define cid2cell(id) ((id) % CELLCOUNT)
#define cid2stack(id) ((id) / CELLCOUNT)
#define cid2pair(id) cid2stack(id), cid2cell(id)
#define cell2cid(stack, id) ((stack) * CELLCOUNT + (id))
#include <array>
using namespace std;

int lvTestLoop = 0;
array<array<int, 11>, 10> sendarr;
int TOTAL_CELLS=10*11;
void sendC() {
    for (uint8_t i = 0; i < TOTAL_CELLS; i += 2) {
        const uint8_t stack1 = cid2stack(i);
        const uint8_t cells1 = cid2cell(i);
        const uint8_t stack2 = cid2stack(i+1);
        const uint8_t cells2 = cid2cell(i+1);

        if (lvTestLoop && (lvTestLoop % 3 != stack1 % 3 && lvTestLoop % 3 != stack2 % 3)) continue;

//printf("(%i,%i)",stack1,cells1);
        //printf("(%i,%i)",stack2,cells2);
        sendarr[stack1][cells1]++;
        sendarr[stack2][cells2]++;

    }
}

int main2() {
    for(int i=0; i<100; i++)sendC(),++lvTestLoop;
    for(int i=0; i<10; i++) {
        for(int j=0; j<11; j++) {
            if(j==5)printf(" .. ");
            if(j>3&&j<8)continue;
            printf("%3i ",sendarr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

#include <unistd.h>

uint32_t getTimeMs() {
    return clock();
}
bool chpow = false, chbal = false;
// STAT  OK  TRNS BLNC TRNS  OK
// CHRG 1111 0000 0000 0000 1111  // power stage
// BLNC 0000 0000 1111 0000 0000  // CB mode
// B_BG 0000 1111 1110 0000 0000  // CB begin: checkChargeBalancing
// B_DN 0000 0000 0000 1111 0000  // CB  done: checkChargeBalanced
// TIME  ->   ->   ->   ->   ->
void setChargeBalancing(bool enbl) {
    const int32_t time = getTimeMs();
    static uint32_t switchTime = 0;

    if (enbl && chbal) goto printInfo;
    if (!enbl && chpow) goto printInfo;

    if (enbl) chpow = (false);
    else chbal = false;

    if (!switchTime) switchTime = time + 2000;

    // transition TRNS: wait for 2 seconds
    if (time < switchTime) goto printInfo;
    switchTime = 0;

    if (enbl) chbal = true;
    else chpow = (true);

printInfo:
    printf("CHRG Bal (e:%i,b:%i,c:%i): %i\n", enbl, chbal, chpow, switchTime ? switchTime - time : -1);
}

int main() {
    chpow = !true;
    bool bal = false;

    for(int i = 0; i < 200; i++) {
        if(i&&i%10==0||i%30==3)bal=!bal;
        setChargeBalancing(bal);
        usleep(100000);
    }
}