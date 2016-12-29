#ifndef BMP_HPP
#define BMP_HPP

#include <windows.h>
#include <stdio.h>       // for memset


namespace BMP{

BYTE* LoadBMP ( int* width, int* height, long* size, LPCTSTR bmpfile );

};

#endif // BMP_HPP
