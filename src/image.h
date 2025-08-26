#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

void bmp_write(const char* file, unsigned char* data)
{
    FILE* file = fopen(file, "wb");

    // fwrite()
}

#endif