#include <cassert>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>

typedef uint8_t u8;

// i do this namespace enum thing to both contain the scope like enum-class but also keep implicit conversions to a number

// This is all of the different words that can show up in a number,
// stored as a number rather than a string.
// Notice how they are both in alphabetic order and their values match their order.
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

// These are the different type of words.
// They help determine which words can come after a word of type T
namespace EType {
enum {
	EON,
	Single, // A single digit. Can be the hundreds or one's digit of a group of three.
	Ten, // words that end with -ty, occupy the tens digit, and can be followed by a ones digit. Confusingly does not include 'Ten'.
	Teen, // words that occupy both the tens and ones, and cant be followed by a ones digit. Confusingly includes 'Ten'.
	Hundred, // Just the word hundred, because its use is unique
	Magnitude, // Billion, million, and thousand. 
};
}

// Textual representation of each word.
// For translating EWords (a number) to a string for writing output.
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

// basically just a key-value pair mapping a word to a type.
// I probably should have used a map for this really.
struct Word {
	u8 word;
	u8 type;

	operator u8() const { return word; }
};

// "Mapping" all the words to their type.
// Again, i should have just used a map.
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

// None of these arrays-by-type actually get used.
// I'll leave them here because its kindof nice to see the words grouped by type.
/*
const Word Numbers[]{Eight, Eighteen, Eighty, Eleven, Fifteen, Fifty, Five, Forty, Four, Fourteen,
					 Nine, Nineteen, Ninety, One, Seven, Seventeen, Seventy, Six, Sixteen, Sixty,
					 Ten, Thirteen, Three, Twelve, Twenty, Two};
const Word Singles[]{Eight, Five, Four, Nine, One, Seven, Six, Three, Two};
const Word Tens[]{Eighty, Fifty, Forty, Ninety, Seventy, Sixty, Thirty, Twenty};
const Word Teens[]{Eighteen, Eleven, Fifteen, Fourteen, Nineteen, Seventeen, Sixteen, Ten, Thirteen, Twelve}; */

// these are the lists of words that can follow each type.
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

// this is a representation of a number spelled out.
// instead of storing a string representation or list of strings,
// it stores a list of bytes representing each word.
struct Number {
	// the longest possible number has 16 words, so a list of 17 is needed to include the ending word.
	u8 words[17]{};
	// where in the list we should write added words to.
	u8 cursor : 5 = 0;
	// hundred can only be used once before each magnitude word.
	bool hundredUsed : 1 = false;
	// magnitude words can only be used once in the whole number.
	bool millionUsed : 1 = false;
	bool thousandUsed : 1 = false;

	// adds a word to this number.
	void append(u8 w) {
		assert(cursor <= 16);
		words[cursor] = w;
		cursor++;
	}

	// writes this number to the output file.
	void write() {
		for (int i = 0; i <= cursor; i++) {
			output << WordText[words[i]];
			if (words[i] != EWords::EON)
				output << ' ';
		}
		writeCount++;
		// occasionally print the progress and flush the output.
		// im not sure if the output flush is necessary.
		// I don't know how big the output buffer is, I don't want it to be gigabytes or something.
		if (writeCount % 1000000 == 0) {
			std::cout << writeCount << " lines written." << std::endl;
			output.flush();
		}
	}
};

// This method is unused, it was just for testing early on.
// It verifies the output line by line to make sure they're at least in alphabetical order.
// It can't tell you if a number is a valid number or if you're missing any or have duplicates.
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

// this is where the program really begins.

void generate() {
	Number n;
	n.append(Eight);
	n.write();
	magnitude(n, Billion);
}

// the current number we're working with and the magnitude to add.
void magnitude(Number n, u8 m) {
	if (m == EWords::Million) {
		// million can't be used if its already been used, and it can't come after thousand.
		if (n.millionUsed || n.thousandUsed) return;
		n.millionUsed = true;
	} else if (m == EWords::Thousand) {
		// thousand can't be used if it already has been.
		if (n.thousandUsed) return;
		n.thousandUsed = true;
	}
	// once a magnitude has been used, its okay to use hundred again.
	n.hundredUsed = false;

	// add the word and write to output.
	n.append(m);
	n.write();

	// iterate through all words that can follow a magnitude word, and call their respective functions.
	// these are the only valid types of words that can follow a magnitude. 
	// You can't have 'hundred' or another magnitude for example.
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

// the current number we're working with and the single digit name to add.
void single(Number n, u8 s) {
	// add the word and write to output.
	n.append(s);
	n.write();

	// iterate through all words that can follow a single digit.
	// Notice, the only options are nothing, a magnitude, or 'hundred'.
	// If thousand and hundred have been used,
	// This function won't be able to go deeper in the tree.
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

// the current number we're working with and the tens word to add.
void ten(Number n, u8 t) {
	// you can't have 'twenty hundred', so we invalidate the use of hundred until the next magnitude.
	n.hundredUsed = true;

	// add the word and write to output.
	n.append(t);
	n.write();

	// iterate through all valid words
	// you can see the type restrictions
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

// the current number we're working with and the teen to add.
void teen(Number n, u8 t) {
	// add the word and write to output.
	n.append(t);
	n.write();

	// iterature through all valid words.
	// The only thing that can follow a teen is a magnitude or the end.
	// eighteen hundred is invalid and eighteen twelve is invalid and eighteen five is invalid.
	for (int i = 0; i < _countof(FollowsTeen); i++) {
		switch (FollowsTeen[i].type) {
		case EType::EON: break;
		case EType::Magnitude:
			magnitude(n, FollowsTeen[i]);
			break;
		}
	}
}

// the current number we're working with
void hundred(Number n) {
	// skip this entire branch if we cant use hundred.
	if (n.hundredUsed) return;
	n.hundredUsed = true;

	// add theword and write to output.
	n.append(Hundred);
	n.write();

	// almost everthing is valid to follow 'Hundred', except hundred itself.
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