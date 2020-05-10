/**
 * @(#)gifreader.h
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#ifndef GifFileH
#define GifFileH

#if defined(ARDUINO)
// Call up the SPIFFS FLASH filing system this is part of the ESP Core
#define FS_NO_GLOBALS
#include <FS.h>
#endif

#ifdef ESP32
  #include "SPIFFS.h"  // For ESP32 only
#endif

#include "GifCommon.h"

#if defined(ARDUINO)
#define FILE_READ       "r"
#define FILE_WRITE      "w"
#elif defined(__BORLANDC__)
#define FILE_READ       "rb"
#define FILE_WRITE      "wb"
#endif // defined

//---------------------------------------------------------------------------
class GifFile {

  public:
    GifFile(const char*, const char*);
    ~GifFile();

    size_t read(void*, size_t, size_t);
    size_t write(void*, size_t, size_t);
    void flush();

  protected:
    void open(const char*, const char*);
    void close();

  private:
#if defined(ARDUINO)
    fs::File file;
#elif defined(__BORLANDC__)
    FILE* file;
#endif // defined
};
//---------------------------------------------------------------------------
#endif // GifFileH
