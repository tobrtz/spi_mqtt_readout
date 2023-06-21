//
//  main.c
//  mqtt_publisher
//
//  Created by Niall Cooling on 23/12/2011.
//  Copyright 2012 Feabhas Limited. All rights reserved.
//  
/*

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

// Current Assumes
// QoS 0
// All messages < 127 bytes
//
// ./mqttpub -c <client name> -i <ip address> -p <port> -t <topic> -n <loop count>
//
// e.g.
// ./mqttpub -i 192.168.1.38 -t mbed/fishtank -c MacBook -n 5
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"
#include <unistd.h> // for sleep function
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define	SPI_CHAN			0
#define	NUM_TIMES		   	100
#define	MAX_SIZE		      	(1024*1024)
#define  READ_PARAM_NOS    		6
#define  IU				5
#define  IV				4
#define  VDCRAW				3
#define  RANGLE				2
#define  VALPHA			        1
#define  VBETA				0

const char *client_name = "default_pub"; 	// -c
const char *ip_addr     = "127.0.0.1";		// -i
uint32_t    port        = 1883;			// -p
const char *topic       = "iMotion/spi/data";	// -t
uint32_t    count       = 10;			// -n

void parse_options(int argc, char** argv);
#define	TRUE	(1==1)
#define	FALSE	(!TRUE)

static int myFd ;

unsigned int microsecond = 1000000;
unsigned int param_nos = READ_PARAM_NOS;

void spiSetup (int speed)
{
  if ((myFd = wiringPiSPISetup (SPI_CHAN, speed)) < 0)
  {
    exit (EXIT_FAILURE) ;
  }
}

int main (int argc, char** argv)
{   
   int speed, times, size ;
   unsigned int idata =0xABCD;
   int spiFail ;
   unsigned char *myData ;
   double timePerTransaction, perfectTimePerTransaction, dataSpeed ;

   if ((myData = (unsigned char*) malloc (MAX_SIZE)) == NULL)
   {
     exit (EXIT_FAILURE) ;
   }

   wiringPiSetup () ;

   spiSetup (1000000) ;

    puts("MQTT PUB Test Code");

//  mqtt_broker_handle_t *broker = mqtt_connect("default_pub","127.0.0.1", 1883);
    mqtt_broker_handle_t *broker = mqtt_connect(client_name, ip_addr, port);

    
    if(broker == 0) {
        puts("Failed to connect");
        exit(1);
    }
 
    char msg[128];
  
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("|             iMOTION SPI ReadOut - RPi               |\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;
   printf ("|   Iu   |   Iv   | VdcRaw | RAngle | Valpha | Vbeta  |\n") ;
   printf ("+--------+--------+--------+--------+--------+--------+\n") ;   
   size = sizeof(idata);
   //myData = (unsigned char*)&idata;
   
   while(1)
   {
      spiFail = FALSE ;
      param_nos = READ_PARAM_NOS;
      while(param_nos--)
      {
         idata =0xABCD;
         if (wiringPiSPIDataRW (SPI_CHAN, (unsigned char*)&idata, 2) == -1)
         {
            spiFail = TRUE ;
            break ;
         }
	 if (param_nos == IU)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "Iu = 0x%04x", idata);
	 }
	 else if (param_nos == IV)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "Iv = 0x%04x", idata);
	 }
	 else if (param_nos == VDCRAW)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "VdcRaw = 0x%04x", idata);
	 }
	 else if (param_nos == RANGLE)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "RAngle = 0x%04x", idata);
	 }
	 else if (param_nos == VALPHA)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "Valpha = 0x%04x", idata);
	 }
	 else if (param_nos == VBETA)
	 {	 
         printf ("| 0x%04x ", idata) ;
	 sprintf(msg, "Vbeta = 0x%04x", idata);
	 }
	      
	if(mqtt_publish(broker, topic, msg, QoS1) == -1) {
            printf("publish failed\n");
        }
   
      }//end of sub while
      
      if (spiFail)
      {
         break ;
      }
      printf ("|\n") ;
      sleep(1);//sleeps for 1 second
   } //end of main while
   
   printf("Break with SPI fail!!\n");
   close (myFd) ;
   mqtt_disconnect(broker);
   return 0 ;
}


