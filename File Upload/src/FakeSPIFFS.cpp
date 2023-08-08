#include "FakeSPIFFS.h"

FakeFile::FakeFile() {
  _data = nullptr;
  _size = 0;
  _position = 0;
}

FakeFile::~FakeFile() {
  close();
}

bool FakeFile::open(FakeFS& fs, const char* path, const char* mode) {
  if (strcmp(mode, "r") == 0) {
    if (!fs.exists(path)) {
      return false;
    }

    _data = fs.getFileData(path);
    _size = fs.getFileSize(path);
    _position = 0;
    return true;
  }
  else if (strcmp(mode, "w") == 0 || strcmp(mode, "a") == 0) {
    _data = new uint8_t[FS_BUF_SIZE];
    _size = 0;
    _position = 0;
    return true;
  }
  else {
    return false;
  }
}

void FakeFile::close() {
  if (_data != nullptr) {
    delete[] _data;
    _data = nullptr;
  }

  _size = 0;
  _position = 0;
}

int FakeFile::read() {
  if (_data == nullptr) {
    return -1;
  }

  if (_position >= _size) {
    return -1;
  }

  int value = _data[_position];
  _position++;
  return value;
}

int FakeFile::read(uint8_t* buffer, size_t size) {
  if (_data == nullptr) {
    return -1;
  }

  size_t bytesToRead = min(size, _size - _position);
  memcpy(buffer, &_data[_position], bytesToRead);
  _position += bytesToRead;
  return bytesToRead;
}

int FakeFile::peek() {
  if (_data == nullptr) {
    return -1;
  }

  if (_position >= _size) {
    return -1;
  }

  return _data[_position];
}

void FakeFile::flush() {
}

size_t FakeFile::write(uint8_t data) {
  if (_data == nullptr) {
    return -1;
  }

  if (_size >= FS_BUF_SIZE) {
    return -1;
  }

  _data[_size] = data;
  _size++;
  
  return sizeof(data);
}

size_t FakeFile::write(const uint8_t* buffer, size_t size) {
  if (_data == nullptr) {
    return -1;
  }

  size_t bytesToWrite = min(size, FS_BUF_SIZE - _size);
  
  memcpy(&_data[_size], buffer, bytesToWrite);
  
  _size += bytesToWrite;

  return bytesToWrite;
}

FakeFS::FakeFS() {
}

FakeFS::~FakeFS() {
}

bool FakeFS::begin() {
  return true;
}

void FakeFS::end() {
}

bool FakeFS::format() {
  _files.clear();
  
  return true;
}

bool FakeFS::exists(const char* path) {
  return (_files.count(path) > 0);
}

size_t FakeFS::getFileSize(const char* path) {
  if (!exists(path)) {
    return -1;
  }

  std::vector<uint8_t>& data = _files[path];
  
  return data.size();
}

uint8_t* FakeFS::getFileData(const char* path) {
  if (!exists(path)) {
    return nullptr;
  }

  std::vector<uint8_t>& data = _files[path];
  
  return &data[0];
}

bool FakeFS::remove(const char* path) {
    if (!exists(path)) {
        return false;
    }
    
    _files.erase(path);

    return true;
}

bool FakeFS::rename(const char* pathFrom, const char* pathTo) {
    if (!exists(pathFrom)) {
        return false;
    }
    
    std::vector<uint8_t> data = _files[pathFrom];
    
    _files.erase(pathFrom);
    
    _files[pathTo] = data;

    return true;
}

bool FakeFS::mkdir(const char* path) {
    // Not implemented
    return false;
}

bool FakeFS::rmdir(const char* path) {
    // Not implemented
    return false;
}

bool FakeFS::opendir(const char* path) {
    // Not implemented
    return false;
}

void FakeFS::rewinddir() {
    // Not implemented
}

bool FakeFS::nextdir() {
    // Not implemented
    return false;
}

const char* FakeFS::direntName() {
    // Not implemented
    return nullptr;
}

bool FakeFS::isdir() {
    // Not implemented
    return false;
}

FakeSPIFFSClass SPIFFS;

File FakeSPIFFSClass::open(const char* path, const char* mode) {
    if (strcmp(mode, "r") == 0) {
        File file(new FakeFile());
        
        if (!file._file->open(SPIFFS._fs, path, mode)) {
            file.close();
            file._file = nullptr;
        }
        
        return file;        
    }
    
    // Not implemented for other modes
    
    return File();
}