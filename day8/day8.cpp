#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void display(const vector<string>& layer)
{
    for(auto row: layer)
    {
        replace(row.begin(), row.end(), '0', ' ');
        cout << row << endl;
    }
    cout << endl;
}

vector<vector<string>> parse_layers(string input, size_t width, size_t height)
{
    size_t r = 0;
    size_t c = 0;
    string row;
    vector<string> layer;
    vector<vector<string>> layers;
    for(char digit: input)
    {
        row += digit;
        if(++c == width)
        {
            c = 0;
            layer.push_back(row);
            row = "";
            if(++r == height)
            {
                r = 0;
                layers.push_back(layer);
                layer.clear();
            }
        }
    }
    return layers;
}

int count_occurrences(const vector<string>& haystack, char needle)
{
    int cnt = 0;
    for(string row: haystack)
    {
        cnt += count(row.begin(), row.end(), needle);
    }
    return cnt;
}

vector<string> superpose(const vector<vector<string>>& layers)
{
    vector<string> image;
    const size_t width = layers[0][0].size();
    const size_t height = layers[0].size();

    for(size_t y = 0; y < height; y++)
    {
        image.push_back(string(width, '2'));
    }

    for(auto& layer: layers)
    {
        for(size_t y = 0; y < height; y++)
        {
            for(size_t x = 0; x < width; x++)
            {
                if(image[y][x] == '2')
                {
                    image[y][x] = layer[y][x];
                }
            }
        }
    }
    return image;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "Usage : " << argv[0] << " <width> <height>" << endl;
        return 1;
    }

    size_t width = stoi(argv[1]);
    size_t height = stoi(argv[2]);
    string input;
    getline(cin, input);

    auto layers = parse_layers(input, width, height);
    int min_zeroes = count_occurrences(layers[0], '0');
    size_t min_index = 0;
    for(size_t i = 1; i < layers.size(); i++)
    {
        int zeroes = count_occurrences(layers[i], '0');
        if(zeroes < min_zeroes)
        {
            min_zeroes = zeroes;
            min_index = i;
        }
    }

    cout << "min_zeroes = " << min_zeroes << endl;
    cout << "min_index = " << min_index << endl;
    cout << "answer = " << count_occurrences(layers[min_index], '1') * count_occurrences(layers[min_index], '2') << endl;

    display(superpose(layers));
}
