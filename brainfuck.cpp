#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <stack>

using namespace std;

constexpr char INCREMENT = '+';
constexpr char DECREMENT = '-';
constexpr char RIGHT = '>';
constexpr char LEFT = '<';
constexpr char LOOP_START = '[';
constexpr char LOOP_END = ']';
constexpr char OUTPUT = '.';
constexpr char INPUT = ',';
constexpr int MEMORY_SIZE = 32; //byte


int main(int argc, char* argv[]){
	unsigned char memory[MEMORY_SIZE];
	unsigned int pointer = 0;
	unsigned int code_pointer = 0;
	unsigned int code_length = 0;
	stack<int> loops;

	memset(memory, 0, sizeof(memory));

	if (argc < 2){
		cerr << "Error: A Brainfuck code is not passed as a command-line argument." << endl;
        cerr << "Please pass an argument as the form, $ brainfuck [FILENAME]." << endl;
        return -1;
	}

	ifstream file(argv[1]);
	if (!file){
		cerr << "Error: The file, " << argv[1] << ", cannot be opened." << endl;
        return -1;
	}

	stringstream buffer;
	buffer << file.rdbuf();
	string code(buffer.str());
	code_length = code.size();

	while (code_pointer < code_length){
		switch (code[code_pointer]){
			case INCREMENT:
				memory[pointer]++;
				break;
			case DECREMENT:
				memory[pointer]--;
				break;
			case RIGHT:
				pointer = (pointer >= MEMORY_SIZE-1)? 0: pointer+1;
				break;
			case LEFT:
				pointer = (pointer <= 0)? MEMORY_SIZE-1: pointer-1;
				break;
			case LOOP_START:
				loops.push(code_pointer);
				if (memory[pointer] == 0){
					int depth = 1;
					while (depth > 0){
						code_pointer++;
						if (code_pointer >= code_length){
							cerr << "Error: Cannot find \"]\"." << endl;
							return -1;
						}
						if (code[code_pointer] == LOOP_START){
							depth++;
						}
						if (code[code_pointer] == LOOP_END){
							depth--;
						}
					}
					loops.pop();
				}
				break;
			case LOOP_END:
				if (loops.empty()){
					cerr << "Error: Loop start order, " << LOOP_START << ", is not found." <<endl;
					return -1;
				}
				code_pointer = loops.top() - 1;
				loops.pop();
				break;
			case OUTPUT:
				putchar(memory[pointer]);
				break;
			case INPUT:
				memory[pointer] = getchar();
				break;
			default:
				break;
		}
		code_pointer++;
	}
	cout << endl;

    return 0;
}
