/**
 * @(#)gifcommon.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#if defined(ARDUINO)
#include "GifCommon.h"
//===========================================================================
namespace GifCommon {
//---------------------------------------------------------------------------
runtime_error::runtime_error(const char* buff) {
  printf(buff);
  while (1) yield(); // Stay here twiddling thumbs waiting
}
//---------------------------------------------------------------------------
}
//===========================================================================
#endif // ARDUINO
