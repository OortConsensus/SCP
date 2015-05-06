#ifndef HASHING_H
#define HASHING_H
#include <openssl/sha.h>

#include <iostream>
#include <iomanip>

// Deprecated on OSX... figure out the proper thing with their internal lib later...
// Using technique from http://stackoverflow.com/questions/13784434/gcc-use-openssls-sha256-functions
inline std::string sha256(std::string str)
{
  unsigned char hbuf[SHA256_DIGEST_LENGTH];
  std::stringstream ss;
  SHA256_CTX h;
  SHA256_Init(&h);
  SHA256_Update(&h, str.c_str(), str.size());
  SHA256_Final(hbuf, &h);
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hbuf[i];
    }
  return ss.str();
};

#endif
