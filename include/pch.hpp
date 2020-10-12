#pragma once
#include <inttypes.h>
#include <memory>

inline bool CheckBit(uint8_t val, unsigned bit) { return ((val >> bit) & 0x01) == 0x01; }
inline bool CheckBit(uint16_t val, unsigned bit) { return ((val >> bit) & 0x01) == 0x01; }

inline void SetBit(uint8_t& val, unsigned bit) { (val = val | (1 << bit)); }
inline void SetBit(uint16_t& val, unsigned bit) { (val = val | (1 << bit)); }

inline void ClearBit(uint8_t& val, unsigned bit) { (val = val & ~(1 << bit)); }
inline void ClearBit(uint16_t& val, unsigned bit) { (val = val & ~(1 << bit)); }

inline bool CheckCarry(uint8_t a, uint8_t b) { return ((((uint16_t)a + (uint16_t)b) & 0x0100) == 0x0100); }
inline bool CheckBorrow(uint8_t a, uint8_t b) { return ((((uint16_t)a - (uint16_t)b) & 0x0100) == 0x0100); }
inline bool CheckPageCrossed(uint16_t a, uint8_t b) { return ((((a & 0x00ff) + (uint16_t)b) & 0x0100) == 0x0100); }
inline bool CheckPageCrossed(uint16_t a, int8_t b) { return ((((a & 0x00ff) + (int16_t)b) & 0x0100) == 0x0100); }

inline bool CheckNegative(uint8_t a) { return (a & 0x80) >> 7; }
																	
inline bool CheckOverflow(uint8_t a, uint8_t b, uint8_t s) { return ((!CheckNegative(a) && !CheckNegative(b) && CheckNegative(s)) /*pos pos neg*/
																	|| (CheckNegative(a) && CheckNegative(b) && !CheckNegative(s))) /*neg neg pos*/;};

#define ARRAYSIZE(a) (sizeof a / sizeof a[0])
