#include "pullet16assembler.h"

/***************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456
 * Class 'Assembler' for assembling code.
 *
 * Author/copyright:  Duncan A. Buell.  All rights reserved.
 * Used with permission and modified by: Cassidy Bradley, Anand Patel
 * Date: 17 August 2018
**/

/***************************************************************************
 * Constructor
**/
Assembler::Assembler() {
}
/***************************************************************************
 * Destructor
**/
Assembler::~Assembler() {
}

/***************************************************************************
 * Accessors and Mutators
**/

/***************************************************************************
 * General functions.
**/

/***************************************************************************
 * Function 'Assemble'.
 * This top level function assembles the code.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
 *   out_stream - the output stream to write to
**/
void Assembler::Assemble(Scanner& in_scanner, string binary_filename,
                         ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Assemble" << endl;
#endif

  //////////////////////////////////////////////////////////////////////////
  // Pass one
  // Produce the symbol table and detect errors in symbols.
  PassOne(in_scanner);
  PrintCodeLines();
  PrintSymbolTable();
  //////////////////////////////////////////////////////////////////////////
  // Pass two
  // Generate the machine code.
  PassTwo();
  PrintCodeLines();
  PrintSymbolTable();
  PrintMachineCode(binary_filename, out_stream);
  //////////////////////////////////////////////////////////////////////////
  // Dump the results.
  std::ifstream inputFile(binary_filename, std::ifstream::binary);
  if(inputFile){
    inputFile.seekg(0, inputFile.end);
    int len = inputFile.tellg();
    inputFile.seekg(0, inputFile.beg);

    char buf[2];
    for(int j=0; j<len/2; j++){
      inputFile.read(buf, 2);
      int16_t valRead = static_cast<int16_t>((buf[1]) | (buf[0] << 8));
      string binaryCon = DABnamespace::DecToBitString(valRead, 16);
      out_stream << binaryCon << endl;
    }
  }
  inputFile.close();
/**************************************************************************
 *  1. Read from the first file and push into a vector
 *  
 *  2. Use a for loop to stream the ASCII into the
 *  out stream and log stream
 *  
 *  3. Read back the binary using a binary input if
 *  stream 
 *   
 *  4. Convert the binary back into ascii and then 
 *  stream it into the out stream and log stream
 **/
  vector<string> meta;
  while (in_scanner.HasNext()) {
    string ascii = in_scanner.Next();
    meta.push_back(ascii);
  }
  for (unsigned int i = 0; i < meta.size(); ++i) {
    out_stream << "ASCII: " << meta.at(i) << endl;
    Utils::log_stream << "ASCII: " << meta.at(i) << endl;
  }
  std::ifstream bin_input(binary_filename, std::ios::binary);
  bin_input.seekg(0, bin_input.end);
  int length = bin_input.tellg();
  bin_input.seekg(0, bin_input.beg);
  char* binarybuffer = new char[2];
  for (int i = 0; i < length/2; i+=2) {
    bin_input.read(binarybuffer, 2);
    int16_t binaryread = *(reinterpret_cast<int16_t*>(binarybuffer));
    string converted = DABnamespace::DecToBitString(binaryread, 16);
    out_stream << "BINARY: " << converted << endl;
    Utils::log_stream << "BINARY: " << converted << endl;
  }
  bin_input.close();
#ifdef EBUG
  Utils::log_stream << "leave Assemble" << endl;
#endif
}

/***************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   symbol - the symbol that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, string symbol) {
  string returnvalue = "";
  return returnvalue;
}

/***************************************************************************
 * Function 'GetInvalidMessage'.
 * This creates a "value is invalid" error message.
 *
 * Parameters:
 *   leadingtext - the text of what it is that is invalid
 *   hex - the hex operand that is invalid
**/
string Assembler::GetInvalidMessage(string leadingtext, Hex hex) {
  string returnvalue = "";

  return returnvalue;
}

/***************************************************************************
 * Function 'GetUndefinedMessage'.
 * This creates a "symbol is undefined" error message.
 *
 * Parameters:
 *   badtext - the undefined symbol text
**/
string Assembler::GetUndefinedMessage(string badtext) {
  string returnvalue = "";
  return returnvalue;
}

/***************************************************************************
 * Function 'PassOne'.
 * Produce the symbol table and detect multiply defined symbols.
 *
 * CAVEAT: We have deliberately forced symbols and mnemonics to have
 *         blank spaces at the end and thus to be all the same length.
 *         Symbols are three characters, possibly with one or two blank at end.
 *         Mnemonics are three characters, possibly with one blank at end.
 *
 * Parameters:
 *   in_scanner - the input stream from which to read
 *   out-stream - the output stream to which to write
**/
void Assembler::PassOne(Scanner& in_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter PassOne" << endl;
#endif

Utils::log_stream << "Pass One" << endl;
  pc_in_assembler_ = 0;
  int lineNum = 0;
  CodeLine cLine;
  string nextLineRead;
  nextLineRead = in_scanner.NextLine();
  while(nextLineRead.length() > 0 && lineNum < 4096){
  	cLine = CodeLine();
  	string mnemonic, lbl, address, symbolOp, hexOp, comments, code;
  	if(nextLineRead.length() <= 20){
  		nextLineRead.append(21 - line.length(), ' ');
  	}
  	if(nextLineRead[0] == '*'){
  		cLine.setCommentsOnly(lineNum, nextLineRead);
  		code = "nullcode";
  		code.SetMachineCode(code);
  	}
  	else{
  		if(nextLineRead.substr(0, 3) != "   "){
  			lbl = nextLineRead.substr(0, 3);
  			if(symboltable_.count(lbl) == 0){
  				symboltable_.insert({lbl, Symbol(lbl, pc_in_assembler_)});
  			}
  			else{
  				symboltable_.at(lbl).SetMultiply();
  			}
  		}
  		mnemonic = nextLineRead.substr(4, 3);
  		address = nextLineRead.substr(8, 1);
  		if(nextLineRead.substr(10, 3) != "   "){
  			symbolOp = nextLineRead.substr(10, 3);
  		}
  		if(nextLineRead.substr(14, 5) != "     "){
  			hexOp = nextLineRead.substr(14, 5);
  		}
  		if(nextLineRead[20] == '*'){
  			comments = nextLineRead.substr(20);
  		}
  		cLine.SetCodeLine(lineNum, pc_in_assembler_, lbl, mnemonic, address, symbolOp, hexOp, comments, kDummyCodeA);
  		if(mnemonic == "DS"){
  			pc_in_assembler_ += cLine.GetHexObject().GetValue();
  		}
  		else if(mnemonic == "ORG"){
  			pc_in_assembler_ = cLine.GetHexObject().GetValue();
  		}
  		else if(mnemonic != "END"){
  			pc_in_assembler_ += 1;
  		}
  		if(mnemonic == "END"){
  			found_end_statement_ = true;
  		}
  	}
  	lineNum += 1;
  	codelines_.push_back(cLine);
  	nextLineRead = in_scanner.NextLine();
  }

#ifdef EBUG
  Utils::log_stream << "leave PassOne" << endl;
#endif
}

/***************************************************************************
 * Function 'PassTwo'.
 * This function does pass two of the assembly process.
**/
void Assembler::PassTwo() {
#ifdef EBUG
  Utils::log_stream << "enter PassTwo" << endl;
#endif
  
  Utils::log_stream << "Pass Two" << endl;
  string bitStr = "";
  for(auto iter = codelines_.begin(); iter != codelines_.end(); iter++){
  	if(!(*iter).IsAllComment()){
  		map<string, string>::iterator operIter = opcodes.find((*iter).GetMnemonic());
  		if(operIter != opcodes.end()){
  			bitStr = opcodes.find((*iter).GetMnemonic())->second;
  			if((*iter).GetAddr() != "*"){
  				bitStr += "0";
  			}
  			else{
  				bitStr += "1";
  			}
  			map<string, string>::iterator symIter = symboltable_.find((*iter).GetSymOperand());
  			Symbol sym;
  			if(symIter != symboltable_.end()){
  				sym = symboltable_.find((*iter).GetSymOperand())->second;
  			}
  			else{
  				if(bitStr.substr(0, 3) == "111"){
  					if((*iter).GetMnemonic() == "STP"){
  						bitStr += "000000000010";
  					}
  					else if((*iter).GetMnemonic() == "RD"){
  						bitStr += "000000000000";
  					}
  					else if((*iter).GetMnemonic() == "WRT"){
  						bitStr += "000000000011";
  					}
  				}
  				else{
  					bitStr += "ERROR0000000";
  					Utils::log_stream << "Error at " << (*iter).GetPC() << endl;
  				}
  			}
  		}
  		else if((*iter).GetMnemonic() == "HEX"){
  			bitStr = DABnamespace::DecToBitString((*iter).GetHexObject().GetValue(), 16);
  		}
  	}
  	if(bitStr.length() >= 16){
  		(*iter).SetMachineCode(bitStr);
  	}
  	else{
  		(*iter).SetMachineCode("0000000000000000");
  	}
  }

#ifdef EBUG
  Utils::log_stream << "leave PassTwo" << endl;
#endif
}

/***************************************************************************
 * Function 'PrintCodeLines'.
 * This function prints the code lines.
**/
void Assembler::PrintCodeLines() {
#ifdef EBUG
  Utils::log_stream << "enter PrintCodeLines" << endl;
#endif
  string s = "";

  for (auto iter = codelines_.begin(); iter != codelines_.end(); ++iter) {
    s += (*iter).ToString() + '\n';
  }

  if (!found_end_statement_) {
    s += "\n***** ERROR -- NO 'END' STATEMENT\n";
    has_an_error_ = true;
  }

#ifdef EBUG
  Utils::log_stream << "leave PrintCodeLines" << endl;
#endif
  Utils::log_stream << s << endl;
}

/***************************************************************************
 * Function 'PrintMachineCode'.
 * This function prints the machine code.
**/
void Assembler::PrintMachineCode(string binary_filename,
                                 ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter PrintMachineCode" << " "
                    << binary_filename << endl;
#endif
  string s = "";
  std::ofstream outFile(binary_filename, std::ofstream::binary);
  int cnt = 0;
  for(auto it = codelines_.begin(); it!=codelines_.end(); it++){
  	if(((*it).GetCode() != "nullcode") && ((*it).GetCode() != kDummyCodeA) && (*it).IsAllComment() && ((*it).GetMnemonic() != "END")){
  		s += Utils::Format(cnt, 4) + " ";
  		s += DABnamespace::DecToBitString(cnt, 12) + " ";
  		s += (*it).GetCode().substr(0, 4) + " " + (*it).GetCode().substr(4, 4) + " " + (*it).GetCode().substr(8, 4) + " " + (*it).GetCode().substr(12, 4) + "\n";
  		cnt++;
  		if(outFile){
  			int16_t ascii16 = DABnamespace::BitStringToDec((*it).GetCode());
  			char dataToWrite[4];
  			dataToWrite[0] = static_cast<char>(ascii16 >> 8);
  			dataToWrite[1] = static_cast<char>(ascii16);
  			outFile.write(dataToWrite, 2);
  		}
  	}
  }
  outFile.close();
  Utils::log_stream << s << endl;

#ifdef EBUG
  Utils::log_stream << "leave PrintMachineCode" << endl;
#endif
}

/******************************************************************************
 * Function 'PrintSymbolTable'.
 * This function prints the symbol table.
**/
void Assembler::PrintSymbolTable() {
#ifdef EBUG
  Utils::log_stream << "enter PrintSymbolTable" << endl;
#endif
  
  string emptyStr = "";
  Utils::log_stream << "SYMBOL TABLE\n 	SYM 	LOC 	FLAGS" << endl;
  for(auto std::pair<string, Symbol> iter : symboltable_){
  	Utils::log_stream << "SYM" << iter.second.ToString() << endl;
  }

#ifdef EBUG
  Utils::log_stream << "leave PrintSymbolTable" << endl;
#endif
  string s = "";
  Utils::log_stream << s << endl;
}

/******************************************************************************
 * Function 'SetNewPC'.
 * This function gets a new value for the program counter.
 *
 * No return value--this sets the class's PC variable.
 *
 * Parameters:
 *   codeline - the line of code from which to update
**/
void Assembler::SetNewPC(CodeLine codeline) {
#ifdef EBUG
  Utils::log_stream << "enter SetNewPC" << endl;
#endif

#ifdef EBUG
  Utils::log_stream << "leave SetNewPC" << endl;
#endif
}

/******************************************************************************
 * Function 'UpdateSymbolTable'.
 * This function updates the symbol table for a putative symbol.
 * Note that there is a hack here, in that the default value is 0
 * and that would mean we can't store a symbol at location zero.
 * So we add one, and then back that out after the full first pass is done.
**/
void Assembler::UpdateSymbolTable(int pc, string symboltext) {
#ifdef EBUG
  Utils::log_stream << "enter UpdateSymbolTable" << endl;
#endif

#ifdef EBUG
  Utils::log_stream << "leave UpdateSymbolTable" << endl;
#endif
}

void Assembler::WriteMemory(int pc, string code){
  for(int i=memory_.size(); i < pc; i++){
    memory_.push_back(OneMemoryWord(kDummyCodeA));
  }
  if((int)memory_.size() == pc){
    memory_.push_back(OneMemoryWord(code));
  }
  else{
    memory_.at(pc) = OneMemoryWord(code);
  }
}
