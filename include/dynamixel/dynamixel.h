/*
*
*   Author: EARL Technologies
*
*   Created: April 16, 2016
*
*/

#ifndef _DYNAMIXEL_H_
#define _DYNAMIXEL_H_
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

namespace EARL
{
    namespace Dynamixel
    {

        class Interface{//UART Interface
        public:
            enum
            {
               USB2AX,
               PRO
            };
            typedef enum
            {
                ERR_NONE                 = 0,//There is no error
                ERR_COMMAND_UNSUPPORTED = -1,//The command is unsupported
                ERR_PORT_DISCONNECTED   = -2,//The port is disconnected
                ERR_PACKET_LENGTH       = -3,//The packet is too short
                ERR_PACKET_CHECKSUM     = -4,//Bad incoming checksum
                ERR_PACKET_NULL         = -5,//The packet is empty/NULL
                ERR_BAD_DEVICE_NAME     = -6,//There is no such device type(see com::open(...))
                ERR_DEVICE_OPEN         = -7,//Device open error(for usb2ax code)
                ERR_PORT_IS_CLOSED      = -8,//The communcation port is not open
                ERR_UNKNOWN             = -9 //Unknown error
            }StatusError;

        public:

            Interface(int baudrate) : isCurrentlyOpen(false), byteTransferTime(0){ baud = baudrate;}
            
            bool isOpen() { return isCurrentlyOpen; }
            
            static Interface* create(int type, int baud);

            /*VIRTUAL*/
            virtual ~Interface() = 0;

            virtual StatusError openPort(const char * devPort) = 0;

            virtual int closePort() = 0;

            float BAUD();

        protected:

            int baud;

            bool isCurrentlyOpen;

            float byteTransferTime;

        private:

        };
        class USB2AX : public Interface {
        private:

            int _fd;

        public:

            USB2AX(int baudrate) : Interface(baudrate), _fd(-1){
            }

            StatusError openPort(const char * devPort);//'devPort' = 'devicePort'
            
            int closePort();
        };
        class Pro : public Interface
        {
        private:

            int _fd;

        public:

            Pro(int baudrate) : Interface(baudrate), _fd(-1){
            }

            StatusError openPort(const char * devPort);

            int closePort();

        protected:

        };

        class Handler {
        private:

            Interface * interface;

            bool isBusy;

        public:

            Handler();

            ~Handler();

            Interface::StatusError initInterface(int interfaceType, int baudrate, const char * devPort);
           
            bool busy();
           
            void setInterface(Interface * interface);
        
        protected:
        
            int baud;

        };
    }
}

#ifdef __cplusplus
}
#endif

#endif



