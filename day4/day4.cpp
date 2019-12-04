#include <iostream>
#include <cassert>
#include <string>

using namespace std;

bool has_double(string input)
{
    int length = input.length();
    int i = 0;
    while(i < length - 1)
    {
        if(input[i] == input[i + 1])
        {
            if(i + 2 >= length) //123[44]
            {
                return true;
            }

            if(input[i] != input[i + 2]) //1[223]34
            {
                return true;
            }
            char current = input[i]; //1[222]34
            do
            {
                i++;
                if(i == length - 1) //123[444]
                {
                    return false;
                }
            }
            while(input[i] == current);
        }
        else
        {
            i++;
        }
    }
    return false;
}

bool is_increasing(string input)
{
    for(int i = 0; i < input.length() - 1; i++)
    {
        if(input[i] > input[i + 1])
        {
            return false;
        }
    }
    return true;
}

bool is_valid(string input)
{
    return is_increasing(input) && has_double(input);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Usage : " << argv[0] << " <min> <max>" << endl;
        return 0;
    }
    assert(is_valid("112233"));
    assert(is_valid("111122"));
    assert(!is_valid("123444"));

    int min = stoi(argv[1]);
    int max = stoi(argv[2]);
    int count = 0;

    for(int i = min; i <= max; i++)
    {
        string current = to_string(i);
        if(is_increasing(current) && has_double(current))
        {
            count++;
        }
    }

    cout << count << endl;
}
