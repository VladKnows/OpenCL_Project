#include "common.h"

using namespace std;

void Utils::generateRandomMatrixToFile(const string& folder, const string& filename, int rows, int cols)
{
    string fullPath = "data/" + folder + "/" + filename;
    ofstream out(fullPath);
    if (!out.is_open())
        throw runtime_error("Error: could not open file '" + fullPath + "' for writing");

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(-10.0f, 10.0f);

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            out << dist(gen);
            if (j < cols - 1)
                out << ' ';
        }
        out << '\n';
    }

    if (!out)
        throw runtime_error("Error: failed while writing to file '" + fullPath + "'");
}