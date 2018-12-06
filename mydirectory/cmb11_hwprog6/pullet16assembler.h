/****************************************************************
 * Header file for the Pullet16 assembler.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Cassidy Bradley
 * Date: 17 August 2018
**/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::ofstream;
using std::set;
using std::string;
using std::vector;

#include "../../Utilities/scanner.h"
#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "onememoryword.h"
#include "dabnamespace.h"
#include "codeline.h"
#include "hex.h"
#include "symbol.h"

class Assembler {
 public:
  Assembler();
  virtual ~Assembler();

  void Assemble(Scanner& in_scanner, string binary_filename,
                ofstream& out_stream);

 private:
  bool found_end_statement_;
  bool has_an_error_;

  const string kDummyCodeA = "1100110011001100";
  const string kDummyCodeB = "111100001111";
  const string kDummyCodeC = "1111000000000000";
  const string kDummyCodeD = "0000000011110000";

  int pc_in_assembler_;
  int maxpc_;
  vector<CodeLine> codelines_
  vector<OneMemoryWord> memory_;
  map<int, string> machinecode_;
  map<string, Symbol> symboltable_;
  map<string, string> opcodes = { {"BAN", "000"},
                    {"SUB", "001"},
                    {"STC", "010"},
                    {"AND", "011"},
                    {"ADD", "100"},
                    {"LD ", "101"},
                    {"BR ", "110"}
                  };
  set<string> mnemonics_;

  string GetInvalidMessage(string errortext);
  string GetUndefinedMessage(string errortext);
  string GetUndefinedMessage(string errortext);
  void PassOne(Scanner& in_scanner);
  void PassTwo();
  void PrintCodeLines();
  void PrintMachineCode(string binary_filename, ofstream& out_stream);
  void PrintSymbolTable();
  void SetNewPC(CodeLine codeline);
  void UpdateSymbolTable(int programcounter, string label
  void WriteMemory(int programcounter, string code);
};
#endif
