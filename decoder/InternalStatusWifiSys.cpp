/*
 * INTERNALSTATUSWIFISYS.cpp
 *
 *  Created on: 07.06.2019
 *      Author: sven
 */

#include "InternalStatusWifiSys.h"
#ifdef ESP8266
        #include <ESP8266WiFi.h>
#else
        #include <WiFi.h>
#endif
#include "Utils.h"
#include <Arduino.h>
#include "Logger.h"

InternalStatusWifiSys::InternalStatusWifiSys() {
	// TODO Auto-generated constructor stub

}

InternalStatusWifiSys::~InternalStatusWifiSys() {
	// TODO Auto-generated destructor stub
}

void InternalStatusWifiSys::getInternalStatus(IInternalStatusCallback* cb, String modul, String key) {
	if (modul.equals("wifi") || modul.equals("*")) {
		if (key.equals("status") || key.equals("*")) {
		String status = "";
		int wifiStatus = 0;
		#ifdef ESP8266
			wifiStatus = wifi_get_opmode();
		#else
			wifiStatus = WiFi.getMode();
		#endif

		switch (wifiStatus) {
			case 0: 
				status = "Status unbekannt";
				break;
			case 1: 
				status = Utils::wifi2String(WiFi.status());
	  			if (status == "Connected") {
					  	status = WiFi.localIP().toString();
	  			}
				break;
			case 2: 
				status = "AP: ";
				status += WiFi.softAPIP().toString();
				break;
			case 3: 
				status = "STA: ";
				String tmp =  Utils::wifi2String(WiFi.status());
	  			if (tmp == "Connected") {
					  	tmp = WiFi.localIP().toString();
	  			}
				status += tmp;
				status += " / AP: ";
				status += WiFi.softAPIP().toString();
				break;
		}
		cb->send("wifi", "status", status);
		}
		if (key.equals("ip sta") || key.equals("*")) {
			cb->send("wifi", "sta ip", WiFi.localIP().toString());
		}
		if (key.equals("ip ap") || key.equals("*")) {
			cb->send("wifi", "ap ip", WiFi.softAPIP().toString());
		}
		if (key.equals("modus") || key.equals("*")) {
			#ifdef ESP8266
			uint8_t midx = wifi_get_opmode();
			#else
			uint8_t midx = WiFi.getMode();
			#endif

    		const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };
			cb->send("wifi", "modus", (midx >= 0 && midx <=3)? modes[midx]: "?");
		}
	}
	if (modul.equals("sys") || modul.equals("*")) {
		
		if (key.equals("compile_date") || key.equals("*")) {
			cb->send("sys", "compile_date", compile_date);
		}
		// if (key.equals("lwip") || key.equals("*")) {
		// 	cb->send("sys", "lwip", String(LWIP_VERSION_MAJOR) + "." + String(LWIP_VERSION_MINOR) + "." + String(LWIP_VERSION_REVISION) + "." + String(LWIP_VERSION_RC));
		// }
		if (key.equals("startfreememory") || key.equals("*")) {
			cb->send("sys", "startfreememory", String(Logger::getInstance()->startmemory));
		}
		if (key.equals("startfreeheap") || key.equals("*")) {
			cb->send("sys", "startfreeheap", String(ESP.getFreeHeap()));
		}
		if (key.equals("freesketchspace") || key.equals("*")) {
			cb->send("sys", "freesketchspace", String(ESP.getFreeSketchSpace()));
		}
		if (key.equals("loggermemory") || key.equals("*")) {
			cb->send("sys", "loggermemory", String(Logger::getInstance()->getMemUsage()));
		}
		if (key.equals("flashsizereal") || key.equals("*")) {
			uint32_t realSize = -1;
			#ifdef ESP8266
			realSize = ESP.getFlashChipRealSize();
			#endif
			cb->send("sys", "flashsizereal", String(realSize));
		}
		if (key.equals("flashsizeconfig") || key.equals("*")) {
			cb->send("sys", "flashsizeconfig", String(ESP.getFlashChipSize()));
		}
		if (key.equals("flashchipmode") || key.equals("*")) {
			FlashMode_t ideMode = ESP.getFlashChipMode();
			cb->send("sys", "flashchipmode", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
		}
		if (key.equals("flashchipspeed") || key.equals("*")) {
			cb->send("sys", "flashchipspeed", String(ESP.getFlashChipSpeed()));
		}
	}
	if (modul.equals("log") || modul.equals("*")) {
		LinkedList<Logger::logdata*>* l = Logger::getInstance()->getLogs();
		for (int i = 0; i <  l->size(); i++) {
			//Logger::logdata* ll = (*l)[i];
      Logger::logdata* ll = l->get(i);
			cb->send("log", String(i), ll->msg);
		}
	}

}
