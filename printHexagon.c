#include <stdio.h>
#include <string.h>
#include <math.h>

#define W(n) (3 * ((n)-1))
#define H(n) (2 * (n)-1)

void bufHexagon(int n, int y, char *buf)
{
    // indent position
    int x = abs(n - y - 1);

    if (x + 1 == n)
        // Top / bottom caps
        memset(buf + x, '*', n);
    else
        // Left & right diagonal borders
        buf[x] = buf[W(n) - x] = '*';

    // Draw inner hexagons recursively
    if (y > 1 && y < H(n - 1))
        bufHexagon(n - 2, y - 2, buf + 3);
}

void printHexagon(int n)
{
    // Null terminated output buffer
    char buf[W(n) + 2];
    buf[W(n) + 1] = 0;

    // Iterate & print grid rows
    for (int y = 0; y < H(n); y++)
    {
        memset(buf, ' ', sizeof(buf) - 1);
        bufHexagon(n, y, buf);
        printf("%s\n", buf);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 10; i++)
        printHexagon(i + 1);
}