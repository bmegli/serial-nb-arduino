/*
 * serial-nb-arduino library header
 *
 * Copyright 2018 (C) Bartosz Meglicki <meglickib@gmail.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. 
 *
 */

/*
 * Arduino/Teensy library for non-blocking serial (UART/USB) communication
 * 
*/

#ifndef SERIAL_NB_H
#define SERIAL_NB_H

#include "Arduino.h"

#include <stdint.h>

template<class SERIAL_TYPE>
class SerialNB
{
	enum {SerialNBBufferSize=512};	
public:
	SerialNB(SERIAL_TYPE &serial):
		m_serial(serial),
		m_buffer_bytes(0)
	{}

	inline int free_bytes()
	{
		return SerialNBBufferSize-m_buffer_bytes;
	}

	int send()
	{
		int available;
		int written=0;

		while( (available=m_serial.availableForWrite()) && m_buffer_bytes-written > 0 )
		{		
			int to_write=min(available, m_buffer_bytes-written);
					
			written+=m_serial.write(m_buffer+written, to_write);
		}
		
		//move the rest of the buffer to the beginning, later change to circular buffer
		if(written && (m_buffer_bytes-written) )
			memmove(m_buffer, m_buffer+written, m_buffer_bytes-written);
		m_buffer_bytes-=written;
		
		return written;
	}

	void push(const uint8_t *data, int size)
	{
		memcpy(m_buffer+m_buffer_bytes, data, size);
		m_buffer_bytes += size;
	}

	template <class T>
	void push(T val)
	{
		memcpy(m_buffer+m_buffer_bytes, &val, sizeof(T));
		m_buffer_bytes += sizeof(T);
	}

private:
	SERIAL_TYPE &m_serial;
	uint8_t m_buffer[SerialNBBufferSize];
	int m_buffer_bytes=0;	
};

#endif
