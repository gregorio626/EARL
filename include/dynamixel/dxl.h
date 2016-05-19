#ifndef _DYNAMIXEL_H_
#define _DYNAMIXEL_H_

#include <vector>
#include <iostream>
#ifdef _cplusplus
extern "C" {
#endif

namespace EARL{
	namespace Dynamixel{


#define INSTRUCTION_PING      (0x01) //No execution. It is used when controller is ready to receive Status Packet
#define INSTRUCTION_READ      (0x02) //This command reads data from Dynamixel
#define INSTRUCTION_WRITE     (0x03) //This command writes data to Dynamixel
#define INSTRUCTION_REG_WRITE (0x04) //It is similar to WRITE, but it remains in the standby state without being executed until the ACTION command arrives.
#define INSTRUCTION_ACTION    (0x05) //This command initiates motions registered with REG WRITE
#define INSTRUCTION_RESET     (0x06) //This command restores the state of Dynamixel to the factory default setting.

#define HEADER_VALUE     	  (0xFF)


#define INST_SYNC_WRITE		(131)
#define INST_SYNC_READ		(132)
#define MAXNUM_TXPARAM		(150)
#define MAXNUM_RXPARAM		(225)
#define BROADCAST_ID		(254)
#define ERRBIT_VOLTAGE		(1)
#define ERRBIT_ANGLE		(2)
#define ERRBIT_OVERHEAT		(4)
#define ERRBIT_RANGE		(8)
#define ERRBIT_CHECKSUM		(16)
#define ERRBIT_OVERLOAD		(32)
#define ERRBIT_INSTRUCTION	(64)
#define	COMM_TXSUCCESS		(0)
#define COMM_RXSUCCESS		(1)
#define COMM_TXFAIL			(2)
#define COMM_RXFAIL			(3)
#define COMM_TXERROR		(4)
#define COMM_RXWAITING		(5)
#define COMM_RXTIMEOUT		(6)
#define COMM_RXCORRUPT		(7)

///////////// device control methods ////////////////////////
int connect(int iDeviceIndex, int iBaudnum);
void disconnect();

class Packet {

private:



	void beginTxPacket(int iID);//Creates the header of an instruction packet
	void endTxPacket();//the foot of the packet

	void setTxPacketID(int iID);//Sets the dynamixel ID within the packet
	void setTxPacketInstruction(unsigned char ucInstruction);//sets the instruction value within the packet
	void setTxPacketReg(int iRegister);
	void setTxPacketValue(int iValue);
	void setTxPacketLength(int iLength);

	int getRxPacketError(int iErrbit);
	int getRxPacketLength(void);
	int RxPacketParameter(int iIndex);
	int makeWord(int iLowByte, int iHighByte);

	unsigned char getLowByte(int iWord);
	unsigned char getHighByte(int iWord);

	////////// packet communication methods ///////////////////////
	void wrtiePacket();
	void readPacket();
	void txrx_packet();

	int getResult();


	

public:
	std::vector<unsigned char> packet;//the data packet to be sent/received

	Packet(){
	}
	std::vector<unsigned char>& ping(int iID);
	std::vector<unsigned char>& readByte(int iID, int iRegister);//length read = 1
	std::vector<unsigned char>& readWord(int iID, int iRegister);//length read = 2
	std::vector<unsigned char>& write(int iID, int iRegister, int ivalue);//length written = 1
	std::vector<unsigned char>& reset(int iID);



	void viewPacket();



protected:

};


	class SerialInterface
	{
	public:

		/*If any new board are added to the framework, you must define a name for the board here, create a class that uses 
		the virtual members from EARL::Dynamixel::SerialInterface, and create a new .cpp file for it.
		*/
		typedef enum {
			usb2ax
		}Board;


		typedef enum {
			ERR_NONE = 0,/*No error occured*/
			ERR_CMD_UNSOPPORTED = -1,/*The attempted comand is unsupported*/
			ERR_DISCONNECTED = -2,/*The port is disconnected*/
			ERR_PACKET_TOO_SHORT = -3,/*The current packet is not long enough*/
			ERR_BAD_INCOMING_CHECKSUM = -4,/*The incoming checksum is bad*/
			ERR_PACKET_EMPTY = -5 /*The packet is empty; the packet is NULL*/

		}Status;

	typedef enum {
		STATUS_NOERROR = 0, /*!< No Error occurred. */
		STATUS_RESULT_FAIL, /*!< Failed to process instruction packet. */
		STATUS_INSTRUCTION, /*!< Undefined instruction or Action without Reg Write. */
		STATUS_CRC, /*!< CRC or Checksum mismatch. */
		STATUS_DATA_RANGE, /*!< Value is outside of the register's range. */
		STATUS_DATA_LENGTH, /*!< Data length shorter than the required length, i.e. writing 3 bytes into a 4 byte register. */
		STATUS_DATA_LIMIT, /*!< Data length longer tan the required length of the register. */
		STATUS_ACCESS /*!< Writing to read only, reading from write only, or Torque Enable ROM lock. */
	} StatusPacketError;

	
		SerialInterface(int baud);//The constructor;

		virtual ~SerialInterface() = 0;

		/*used to open the UART communication port
		*/
		virtual bool openPort(const char * portName) = 0;

		/*Closes the UART port
		*/
		virtual void closePort() = 0;

		static SerialInterface * createPortInterface(int currentBoard, int baud);

		bool isOpen() {
			return bCurrentlyOpen;
		}


	protected:


		int board;

		bool bCurrentlyOpen;

		const char * portName;

		int baud;//the baudrate(bps)

	};

	class USB2AX : public SerialInterface {

	private:
		int fd;

	public:

		USB2AX(int baud) : SerialInterface(baud), fd(-1) {
		}
		bool openPort(const char * portName);//used to open the UART communication port
	
		void closePort();

	};

	class PortHandler {

private:
	SerialInterface * interface;

	bool isBusy;


protected:
	int baud;//the baudrate

public:


	PortHandler();//Constructor

	~PortHandler();//Destructor

	SerialInterface::Status initialize(int board, int baudNumber);//used to initialize the port; opens the port; sets the baudrate

	bool busy();


};







/*
class Communication{

void ping(int iID);
int dxl_read_byte(int iID, int iAddress);
void dxl_write_byte(int iID, int iAddress, int iValue);
int dxl_read_word(int iID, int iAddress);
void dxl_write_word(int iID, int iAddress, int iValue);

};

//////////// Synchroneous communication methods ///////////////////////
void dxl_sync_write_start(int iAddress, int iDataLength);
void dxl_sync_write_push_id(int iID);
void dxl_sync_write_push_byte(int iValue);
void dxl_sync_write_push_word(int iValue);
void dxl_sync_write_send();

void dxl_sync_read_start(int iAddress, int iDataLength);
void dxl_sync_read_push_id(int iID);
void dxl_sync_read_send();
int dxl_sync_read_pop_byte();
int dxl_sync_read_pop_word();
*/
unsigned char getChecksum(const unsigned char*);
}//namespace Dynamixel
}//namespace EARL

#ifdef _cplusplus
}
#endif

#endif
