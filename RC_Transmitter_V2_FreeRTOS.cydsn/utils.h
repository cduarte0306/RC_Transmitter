/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef UTILS_H
#define UTILS_H
    
#include "cytypes.h"
    

#define TRUE    1
#define FALSE   0
    
#ifndef msb 
    #define msb(x) ((uint16)x & 0xFF00) >> 8
#endif    

#ifndef lsb
    #define lsb(x) ((uint16)x & 0x00FF)
#endif

#ifndef min
    #define min(a, b) (a <= b) ? b : a
#endif

#ifndef max
    #define max(a, b) (a >= b) ? b : a
#endif

#ifndef bound
    #define bound(a,b,c) (b <= a) ? a : (b >= c) ? c : b
#endif

#endif

/* [] END OF FILE */
