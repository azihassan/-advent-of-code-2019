#include <iostream>

using namespace std;

constexpr long compute_fuel(long mass)
{
    long total = 0;
    while(true)
    {
        long fuel = (mass / 3) - 2;
        if(fuel <= 0)
        {
            break;
        }
        total += fuel;
        mass = fuel;
    }
    return total;
}

int main()
{
    static_assert(compute_fuel(14) == 2, "Test 1 failed");
    static_assert(compute_fuel(1969) == 966, "Test 2 failed");
    static_assert(compute_fuel(100756) == 50346, "Test 3 failed");

    string line;
    long total = 0;

    while(getline(cin, line))
    {
        long module = stol(line);
        total += compute_fuel(module);
    }

    cout << total << endl;
}
