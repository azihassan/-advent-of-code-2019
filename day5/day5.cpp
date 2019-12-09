#include <iostream>
#include <cmath>
#include <utility>
#include <fstream>
#include <vector>

using namespace std;

enum Addressing
{
    IMMEDIATE = 1,
    POSITION = 0
};

enum class Mnemonic
{
    ADD, MUL, LOAD, WRITE, JUMP_IF_TRUE, JUMP_IF_FALSE, LESS, EQ, HALT, ERROR
};

ostream& operator<<(ostream& out, const Mnemonic& m)
{
    switch(m)
    {
        case Mnemonic::ADD: return out << "ADD";
        case Mnemonic::MUL: return out << "MUL";
        case Mnemonic::LOAD: return out << "LOAD";
        case Mnemonic::WRITE: return out << "WRITE";
        case Mnemonic::JUMP_IF_TRUE: return out << "JUMP_IF_TRUE";
        case Mnemonic::JUMP_IF_FALSE: return out << "JUMP_IF_FALSE";
        case Mnemonic::LESS: return out << "LESS";
        case Mnemonic::EQ: return out << "EQ";
        case Mnemonic::HALT: return out << "HALT";
        case Mnemonic::ERROR: return out << "ERROR";
        default: return out << "UNKNOWN";
    }
}

struct Opcode
{
    Mnemonic value;
    vector<long> parameters;
};

int get_digit(long number, size_t index)
{
    return (number / (long) (pow(10, index))) % 10;
}

pair<int, Opcode> next_instruction(const vector<long>& program, size_t pc)
{
    Opcode opcode;
    switch(program[pc] % 100)
    {
        case 1:
            opcode.value = Mnemonic::ADD;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            opcode.parameters.push_back(program[pc + 3]);
            return make_pair(pc + 4, opcode);
        break;
        case 2:
            opcode.value = Mnemonic::MUL;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            opcode.parameters.push_back(program[pc + 3]);
            return make_pair(pc + 4, opcode);
        break;
        case 3:
            opcode.value = Mnemonic::LOAD;
            opcode.parameters.push_back(program[pc + 1]);
            return make_pair(pc + 2, opcode);
        break;
        case 4:
            opcode.value = Mnemonic::WRITE;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            //opcode.parameters.push_back(program[program[pc + 1]]);
            return make_pair(pc + 2, opcode);
        break;
        case 5:
            opcode.value = Mnemonic::JUMP_IF_TRUE;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            return make_pair(pc + 3, opcode);
        break;
        case 6:
            opcode.value = Mnemonic::JUMP_IF_FALSE;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            return make_pair(pc + 3, opcode);
        break;
        case 7:
            opcode.value = Mnemonic::LESS;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            opcode.parameters.push_back(program[pc + 3]);
            return make_pair(pc + 4, opcode);
        break;
        case 8:
            opcode.value = Mnemonic::EQ;
            opcode.parameters.push_back(get_digit(program[pc], 2) == IMMEDIATE ? program[pc + 1] : program[program[pc + 1]]);
            opcode.parameters.push_back(get_digit(program[pc], 3) == IMMEDIATE ? program[pc + 2] : program[program[pc + 2]]);
            opcode.parameters.push_back(program[pc + 3]);
            return make_pair(pc + 4, opcode);
        break;
        case 99:
            opcode.value = Mnemonic::HALT;
            return make_pair(-1, opcode);
        break;
        default:
            opcode.value = Mnemonic::ERROR;
            return make_pair(-2, opcode);
        break;
    }
}

bool run_program(vector<long>& program, bool debug)
{
    bool finished = false;
    bool failed = false;
    size_t pc = 0;
    long input;

    while(true)
    {
        if(pc > program.size())
        {
            cout << pc << " > " << program.size() << endl;
            return false;
        }
        pair<int, Opcode> next = next_instruction(program, pc);
        int next_pc = next.first;
        auto opcode = next.second.value;
        auto parameters = next.second.parameters;

        if(opcode == Mnemonic::HALT)
        {
            break;
        }
        if(opcode == Mnemonic::ERROR)
        {
            failed = true;
            break;
        }

        switch(opcode)
        {
            case Mnemonic::ADD:
                program[parameters[2]] = parameters[0] + parameters[1];
                pc = next_pc;
            break;
            case Mnemonic::MUL:
                program[parameters[2]] = parameters[0] * parameters[1];
                pc = next_pc;
            break;
            case Mnemonic::LOAD:
                cout << "> ";
                cin >> input;
                program[parameters[0]] = input;
                pc = next_pc;
            break;
            case Mnemonic::WRITE:
                cout <<  "< " << parameters[0] << endl;
                pc = next_pc;
            break;
            case Mnemonic::JUMP_IF_TRUE:
                pc = parameters[0] != 0 ? parameters[1] : next_pc;
            break;
            case Mnemonic::JUMP_IF_FALSE:
                pc = parameters[0] == 0 ? parameters[1] : next_pc;
            break;
            case Mnemonic::LESS:
                program[parameters[2]] = parameters[0] < parameters[1] ? 1 : 0;
                pc = next_pc;
            break;
            case Mnemonic::EQ:
                program[parameters[2]] = parameters[0] == parameters[1] ? 1 : 0;
                pc = next_pc;
            break;
        }
    }

    return !failed;
}

vector<long> load_program(string file)
{
    vector<long> program;
    ifstream fh(file);
    long integer;
    while(fh >> integer)
    {
        program.push_back(integer);
        if(fh.peek() == ',')
        {
            fh.ignore();
        }
    }
    return program;
}

int main(int argc, char *argv[])
{
    auto input = load_program("input");

    bool success = run_program(input, argc > 1);
    if(!success)
    {
        cout << "Program failed" << endl;
    }
    else
    {
        cout << "Program finished execution" << endl;
    }
}
