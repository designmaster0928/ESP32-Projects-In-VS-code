#ifndef FakeSPIFFS_h
#define FakeSPIFFS_h

#include <Arduino.h>
#include <vector>
#include <map>

#define FS_BUF_SIZE 512

class FakeFile;

class FakeFS {

protected:
    _dara;

public:
  FakeFS();
  ~FakeFS();

  bool begin();
  void end();

  bool format();
  bool exists(const char* path);
  size_t getFileSize(const char* path);
  uint8_t* getFileData(const char* path);
  bool remove(const char* path);
  bool rename(const char* pathFrom, const char* pathTo);
  bool mkdir(const char* path);
  bool rmdir(const char* path);
  bool opendir(const char* path);
  void rewinddir();
  bool nextdir();
  const char* direntName();
  bool isdir();

private:
  std::map<std::string, std::vector<uint8_t>> _files;
  std::vector<std::string> _dirs;
  std::vector<std::string>::iterator _dirIterator;
  bool _isDir;
};

class FakeSPIFFSClass {
public:
  File open(const char* path, const char* mode);
};

extern FakeSPIFFSClass SPIFFS;

#endif