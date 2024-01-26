#ifndef OTA_H
#define OTA_H
#include <WebServer.h>


void OTAWebUpdaterD1(const char* ssid,const char* password,const char* host,const char* serverIndex,WebServer &server);

#endif  // OTA_H
