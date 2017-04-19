// pcap test.cpp : Defines the entry point for the console application.
//
// Author Liejian Chen 
// 2017.04.18 
// 

#include "stdafx.h"
#include "C:/develop/WpdPack/Include/pcap.h"
#include <cstring>
#include <sstream>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "C:/develop/WpdPack/Lib/wpcap.lib")

int main()
{
	pcap_if_t *alldevs, *d;
	std::string ifname;
	char errbuf[PCAP_ERRBUF_SIZE];

	//const char* match[] =ifname.c_str(); "{0F0AF3AA-2B90-4048-AB0D-ED15693DFB6E}";

	//
	// Find specified PCAP device
	//

	// List all PCAP devices
	if (pcap_findalldevs(&alldevs, errbuf) == -1) {
		std::cout << "Unable to obtain PCAP device list" << std::endl;
	}

	std::cout << "Listing PCAP device info:" << std::endl;

	// Identify selected device
	int pmatch = -1;
	char* pDevice = 0;
	int j = 0;
	for (d = alldevs; d; d = d->next) {
		char* substr = strchr(d->name, '{');
		if (strcmp(ifname.c_str(), substr) == 0) {
			pDevice = d->name;
			std::cout << "=>";
		}

		std::cout << "\tPCAP index: \t" << j << std::endl;
		std::cout << "\tAdapter Name: \t" << d->name << std::endl;
		std::cout << "\tAdapter Desc: \t" << d->description << std::endl;
		j++;
	}

	// Throw exception if no devices found
	if (j == 0) {
		std::cout << "PCAP device list is empty" << std::endl;
	}

	// Throw exception if specified device not found
	if (pDevice == 0) {
		std::cout << "Specified PCAP device not found" << std::endl;
	}
}

