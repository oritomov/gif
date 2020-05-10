/**
 * @(#)gifcommon.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GifCommonH
#define GifCommonH

#if defined(ARDUINO)
#include <Arduino.h>
#include <HardwareSerial.h>

#define DEBUG    

//===========================================================================
namespace GifCommon {
//---------------------------------------------------------------------------
class runtime_error {
	public:
		explicit runtime_error(const char* buff);
};
//---------------------------------------------------------------------------
} // namespace GifCommon
//===========================================================================

#elif defined(__BORLANDC__)
#include <windows.h>
#include <stdio.h>
#include <stdexcept>
#define byte BYTE
#define word WORD
#endif // defined
#endif // CommonH
