#ifndef __NFC_H__
#define __NFC_H__

void configNFC(void *arg0);
bool readCardID(void *arg0,  char* buf);
//bool checkCardID(void *arg0);

#endif

