#include <cassert>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>

typedef uint8_t u8;

namespace EWords {
enum {
	EON, // End of Number
	Billion,
	Eight, Eighteen, Eighty,
	Eleven,
	Fifteen, Fifty, Five,
	Forty, Four, Fourteen,
	Hundred, Million,
	Nine, Nineteen, Ninety,
	One,
	Seven, Seventeen, Seventy,
	Six, Sixteen, Sixty,
	Ten,
	Thirteen, Thirty,
	Thousand,
	Three,
	Twelve, Twenty, Two,
};
}

const char* WordText[]{
	"\n",
	"Billion",
	"Eight", "Eighteen", "Eighty",
	"Eleven",
	"Fifteen", "Fifty", "Five",
	"Forty", "Four", "Fourteen",
	"Hundred", "Million",
	"Nine", "Nineteen", "Ninety",
	"One",
	"Seven", "Seventeen", "Seventy",
	"Six", "Sixteen", "Sixty",
	"Ten",
	"Thirteen", "Thirty",
	"Thousand",
	"Three",
	"Twelve", "Twenty", "Two"
};

namespace EType {
enum {
	EON,
	Single, Ten, Teen,
	Hundred,
	Magnitude,
};
}

struct Word {
	u8 word;
	u8 type;

	operator u8() const { return word; }
};

#pragma region Words

#define WORD(Name, Type) const Word Name{EWords::Name, EType::Type};

WORD(EON, EON)
WORD(Billion, Magnitude)
WORD(Eight, Single)
WORD(Eighteen, Teen)
WORD(Eighty, Ten)
WORD(Eleven, Teen)
WORD(Fifteen, Teen)
WORD(Fifty, Ten)
WORD(Five, Single)
WORD(Forty, Ten)
WORD(Four, Single)
WORD(Fourteen, Teen)
WORD(Hundred, Hundred)
WORD(Million, Magnitude)
WORD(Nine, Single)
WORD(Nineteen, Teen)
WORD(Ninety, Ten)
WORD(One, Single)
WORD(Seven, Single)
WORD(Seventeen, Teen)
WORD(Seventy, Ten)
WORD(Six, Single)
WORD(Sixteen, Teen)
WORD(Sixty, Ten)
WORD(Ten, Teen)
WORD(Thirteen, Teen)
WORD(Thirty, Ten)
WORD(Thousand, Magnitude)
WORD(Three, Single)
WORD(Twelve, Teen)
WORD(Twenty, Ten)
WORD(Two, Single)

#undef WORD

#pragma endregion

const Word Numbers[]{Eight, Eighteen, Eighty, Eleven, Fifteen, Fifty, Five, Forty, Four, Fourteen,
					 Nine, Nineteen, Ninety, One, Seven, Seventeen, Seventy, Six, Sixteen, Sixty,
					 Ten, Thirteen, Three, Twelve, Twenty, Two};
const Word Singles[]{Eight, Five, Four, Nine, One, Seven, Six, Three, Two};
const Word Tens[]{Eighty, Fifty, Forty, Ninety, Seventy, Sixty, Thirty, Twenty};
const Word Teens[]{Eighteen, Eleven, Fifteen, Fourteen, Nineteen, Seventeen, Sixteen, Ten, Thirteen, Twelve};

const Word FollowsSingle[]{EON, Hundred, Million, Thousand};
const Word FollowsTen[]{EON, Eight, Five, Four, Million, Nine, One, Seven, Six, Thousand, Three, Two};
const Word FollowsTeen[]{EON, Million, Thousand};
const Word FollowsHundred[]{EON, Eight, Eighteen, Eighty, Eleven, Fifteen, Fifty, Five, Forty, Four, Fourteen,
							Million,
							Nine, Nineteen, Ninety, One, Seven, Seventeen, Seventy, Six, Sixteen, Sixty,
							Ten, Thirteen, Thirty,
							Thousand,
							Three, Twelve, Twenty, Two};
const Word FollowsMagnitude[]{EON, Eight, Eighteen, Eighty, Eleven, Fifteen, Fifty, Five, Forty, Four, Fourteen,
							  Nine, Nineteen, Ninety, One, Seven, Seventeen, Seventy, Six, Sixteen, Sixty,
							  Ten, Thirteen, Thirty, Three, Twelve, Twenty, Two};

std::ofstream output;
uint64_t writeCount = 0;

void verify();

struct Number {
	u8 words[17]{};
	u8 cursor : 5 = 0;
	bool hundredUsed : 1 = false;
	bool millionUsed : 1 = false;
	bool thousandUsed : 1 = false;

	void append(u8 w) {
		assert(cursor <= 16);
		words[cursor] = w;
		cursor++;
	}

	void write() {
		for (int i = 0; i <= cursor; i++) {
			output << WordText[words[i]];
			if (words[i] != EWords::EON)
				output << ' ';
		}
		//output.write((char*)words, (std::streamsize)cursor+1);
		writeCount++;
		if (writeCount % 1000000 == 0) {
			std::cout << writeCount << " lines written." << std::endl;
			output.flush();
			/*output.close();
			output.open("output.txt", std::ios::out | std::ios::app);
			if (!output.is_open()) {
				std::cerr << "Failed to open 'output.txt' for writing." << std::endl;
				exit(-1);
			}*/
		}
		/*if (writeCount == 1000000) {
			output.close();
			verify();
			exit(1);
		}*/
	}
};

void verify() {
	std::ifstream input;
	input.open("output.txt", std::ios::in);
	if (!input.is_open()) {
		std::cerr << "Failed to open 'output.txt' for reading." << std::endl;
		return;
	}

	int lineNumber = 1;
	std::string line0;
	std::string line1;

	std::getline(input, line0);


	while (std::getline(input, line1)) {
		int result = line0.compare(line1);
		if (result > 0) {
			std::cout << "Lines " << lineNumber-1 << " and " << lineNumber << " are not in the right order." << std::endl;
		}
		lineNumber++;
		line0 = line1;
	}

	input.close();
}

void generate();
void single(Number n, u8 s);
void ten(Number n, u8 t);
void teen(Number n, u8 t);
void hundred(Number n);
void magnitude(Number n, u8 m);

int main() {
	output.open("output.txt", std::ios::out | std::ios::trunc);
	if (!output.is_open()) {
		std::cerr << "Failed to open 'output.txt' for writing." << std::endl;
		return -1;
	}
	generate();
	output.close();
	std::cout << writeCount << " lines written." << std::endl;
	//verify();
}

void generate() {
	Number n;
	n.append(Eight);
	n.write();
	magnitude(n, Billion);
}

void magnitude(Number n, u8 m) {
	if (m == EWords::Million) {
		if (n.millionUsed || n.thousandUsed) return;
		n.millionUsed = true;
	} else if (m == EWords::Thousand) {
		if (n.thousandUsed) return;
		n.thousandUsed = true;
	}
	n.hundredUsed = false;

	n.append(m);
	n.write();

	for (int i = 0; i < _countof(FollowsMagnitude); i++) {
		switch (FollowsMagnitude[i].type) {
		case EType::EON: break;
		case EType::Single:
			single(n, FollowsMagnitude[i]);
			break;
		case EType::Ten:
			ten(n, FollowsMagnitude[i]);
			break;
		case EType::Teen:
			teen(n, FollowsMagnitude[i]);
			break;
		}
	}
}

void single(Number n, u8 s) {
	n.append(s);
	n.write();

	for (int i = 0; i < _countof(FollowsSingle); i++) {
		switch (FollowsSingle[i].type) {
		case EType::EON: break;
		case EType::Hundred: 
			hundred(n);
			break;
		case EType::Magnitude:
			magnitude(n, FollowsSingle[i]);
			break;
		}
	}
}

void ten(Number n, u8 t) {
	n.hundredUsed = true;

	n.append(t);
	n.write();

	for (int i = 0; i < _countof(FollowsTen); i++) {
		switch (FollowsTen[i].type) {
		case EType::EON: break;
		case EType::Single: 
			single(n, FollowsTen[i]);
			break;
		case EType::Magnitude:
			magnitude(n, FollowsTen[i]);
			break;
		}
	}
}

void teen(Number n, u8 t) {
	n.append(t);
	n.write();

	for (int i = 0; i < _countof(FollowsTeen); i++) {
		switch (FollowsTeen[i].type) {
		case EType::EON: break;
		case EType::Magnitude:
			magnitude(n, FollowsTeen[i]);
			break;
		}
	}
}

void hundred(Number n) {
	if (n.hundredUsed) return;
	n.hundredUsed = true;

	n.append(Hundred);
	n.write();

	for (int i = 0; i < _countof(FollowsHundred); i++) {
		switch (FollowsHundred[i].type) {
		case EType::EON: break;
		case EType::Single:
			single(n, FollowsHundred[i]);
			break;
		case EType::Ten:
			ten(n, FollowsHundred[i]);
			break;
		case EType::Teen:
			teen(n, FollowsHundred[i]);
			break;
		case EType::Magnitude:
			magnitude(n, FollowsHundred[i]);
		}
	}
}