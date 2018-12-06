/****************************************************************
 * Header file for the 'OneMemoryWord' class to contain one
 * word of memory, which could be interpreted in several ways.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Cassidy Bradley
 * Date: 17 August 2018
**/

#ifndef ONEMEMORYWORD_H
#define ONEMEMORYWORD_H

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

// #include "../../Utilities/scanner.h"
// #include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "dabnamespace.h"

class OneMemoryWord {
 public:
  OneMemoryWord();
  OneMemoryWord(string thestring);
  virtual ~OneMemoryWord();

  string GetAddressBits() const;
  string GetBitPattern() const;
  string GetIndirectFlag() const;
  string GetMnemonicBits() const;
  string GetMnemonicCode() const;

  void SetBitPattern(string what);
  string ToString() const;

 private:
  string bit_pattern_;

  void Initialize(string thestring);
};
#endif
