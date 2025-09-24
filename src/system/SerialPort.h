///////////////////////////////////////////////////////////////////////////////
// Name               : SerialPort.h
// Purpose            : A serial port class for Linux and Windows
// Thread Safe        : No
// Platform dependent : Yes
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 08.09.2002
// Copyright          : (C) 2002 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_SERIALPORT_H
#define SYSTEM_SERIALPORT_H

/*!\class SerialPort
 * \brief Serial port class for Linux and Windows
 *
 * Encapsulates the intricacies of the serial port on different systems.
 *
 * \todo Implement functions to find and enumerate available serial ports.
 */

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#define __WIN
#elif defined(linux) || defined(__linux)
#define __LINUX
#else
#error "Neither a Linux nor a Windows system was found!"
#endif

#ifdef __LINUX
#include <termios.h>
#include <stdlib.h>
#endif

#include <string>

class SerialPort {

public:
	SerialPort();

	/**\brief Destructor of serial port object.
	 *
	 * \note The destructor also closes any open connection.
	 */
	virtual ~SerialPort();

	/**\brief Open a serial connection by its number
	 *
	 * For Linux systems this is /dev/ttyS#.
	 *
	 * For Windows systems this is COM#.
	 *
	 * The numbers are not shifted. (Windows starts counting at 1, Linux starts
	 * counting at 0.)
	 *
	 * \param nPort Number of the port
	 * \param nBaud Baudrate (default: 19200)
	 */
	bool Open(int nPort, int nBaud = 19200);

	/**\brief Open a serial port by its name
	 *
	 * On Windows a device name like 'COM1', 'COM2', 'COM3', ...
	 * is expected.
	 *
	 * On Linux systems a path to the dev-folder is expected like
	 * '/dev/ttyS0', '/dev/ttyS1', ... or  '/dev/ttyUSB0', ...
	 *
	 * \todo Check if the given portname is a valid port.
	 *
	 * \param Port std::string with the name
	 * \param nBaud Baudrate (default: 19200)
	 */
	bool Open(const std::string &Port, int nBaud = 19200);

	/**\brief Returns if the port has been opened successfully
	 *
	 * Return the boolean variable, that is set to true after the Open()
	 * function successfully has opened the port. This function has little to
	 * no runtime impact.
	 *
	 * \return bool, if serial port is open
	 */
	inline bool IsOpen() const {
		return (Opened);
	}

	/**\brief Close the serial port
	 *
	 * This function is also called on the destruction of the serial port
	 * object.
	 */
	bool Close();

	/**\brief Read data available on the port into a given char buffer
	 *
	 * \param buffer Pointer to a buffer of char
	 * \param limit Size of available bytes in the buffer
	 * \return Number of bytes read
	 */
	size_t ReadData(char *buffer, size_t limit);

	size_t SendData(const char *buffer, size_t size);
	size_t SendData(std::string data);

	size_t ReadDataWaiting();
	void FlushData();
	void WaitTXFinish();

	std::string GetName() const;
	int GetHandle() const;

	/**\brief Set the DTR line on the serial port
	 *
	 * This function is needed, if more complicated devices are connected to
	 * the serial port, like programming or JTAG adapters.
	 *
	 * \param activate New state of the DTR line
	 */
	void SetDTR(bool activate);

	/**\brief Set the RTS line on the serial port
	 *
	 * This function is needed, if more complicated devices are connected to
	 * the serial port, like programming or JTAG adapters.
	 *
	 * \param activate New state of the RTS line
	 */
	void SetRTS(bool activate);

private:
	bool Opened;
	std::string szPort; ///< Name of the open port.

private:
#ifdef __WIN
	bool WriteCommByte(unsigned char);
#endif
#ifdef __LINUX
	static const size_t BUFFER_LEN = 2100;
	int fd;
	int res;
	size_t buffer_RD;
	size_t buffer_WR;
	char m_buffer[BUFFER_LEN];
	struct termios oldtio;
	struct termios newtio;
#endif
};

#endif // SYSTEM_SERIALPORT_H
