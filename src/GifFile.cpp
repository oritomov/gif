/**
 * @(#)giffile.cpp
 *
 * @version     1.0, 1.04.2020
 * @author      <a href="mailto:oritomov@yahoo.com">Orlin Tomov</a>
 */

#include "GifFile.h"

//---------------------------------------------------------------------------
GifFile::GifFile(const char* filename, const char* mode = FILE_READ) {
  open(filename, mode);
}

//---------------------------------------------------------------------------
GifFile::~GifFile() {
  close();
}

//---------------------------------------------------------------------------
void GifFile::open(const char* filename, const char* mode = FILE_READ) {
  printf("rfile: %s\n", filename);
#if defined(ARDUINO)
  file = SPIFFS.open(filename, mode);
#elif defined(__BORLANDC__)
  file = fopen(filename, mode);
#endif // defined
  if (!file)
#if defined(ARDUINO)
    GifCommon::runtime_error("err: opening file!\n");
#elif defined(__BORLANDC__)
    throw std::runtime_error("err: opening file!\n");
#endif // defined
}

//---------------------------------------------------------------------------
void GifFile::close() {
  if (file)
#if defined(ARDUINO)
    file.close();
#elif defined(__BORLANDC__)
    fclose(file);
#endif // defined
}

//---------------------------------------------------------------------------
void GifFile::flush() {
#if defined(ARDUINO)
  file.flush();
#elif defined(__BORLANDC__)
  fflush(file);
#endif // defined
}

//---------------------------------------------------------------------------
size_t GifFile::read(void* ptr, size_t size, size_t nmemb) {
#if defined(ARDUINO)
  register char *cp = (char *)ptr;
  register int c;
  size_t ndone = 0;
  register size_t s;

  if (size)
    while ( ndone < nmemb ) {
      s = size;
      do {
        if ((c = file.read()) != EOF)
          *cp++ = (char)c;
        else
          return ndone;
      } while (--s);
      ndone++;
    }

  return ndone;
#elif defined(__BORLANDC__)
  return fread(&ptr, size, nmemb, file);
#endif // defined
}

//---------------------------------------------------------------------------
size_t GifFile::write(void* ptr, size_t size, size_t nmemb) {
#if defined(ARDUINO)
  register char *cp = (char *)ptr;
  register char c;
  size_t ndone = 0;
  register size_t s;

  if (size)
    while ( ndone < nmemb ) {
      s = size;
      do {
        c = *cp++;
        if ((file.write(c)) == EOF)
          return ndone;
      } while (--s);
      ndone++;
    }

  return ndone;
#elif defined(__BORLANDC__)
  return fwrite(&ptr, size, nmemb, file);
#endif // defined
}
