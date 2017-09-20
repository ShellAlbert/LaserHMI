#ifndef CRC16_H
#define CRC16_H
extern unsigned short ccitt_table[256];
extern unsigned short crc16_ccitt(unsigned char *q, int len);
#endif // CRC16_H
