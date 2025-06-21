#include "common.h"

using namespace std;

class Utils {
    public:
        template<typename T>
        static vector<T> readVectorFromFile(const string& parentFolder, const string& filename)
        {
            string fullPath = "data/" + parentFolder + "/" + filename;
            ifstream in(fullPath);
            if (!in.is_open())
                throw runtime_error("Error: could not open file '" + fullPath + "'!\n");
    
            vector<T> data;
            T value;
            while (in >> value)
                data.push_back(value);
    
            if (data.empty())
                throw runtime_error("Error: file '" + fullPath + "' is empty or has invalid format!\n");
    
            return data;
        }
    
        template<typename T>
        static void writeVectorToFile(const string& parentFolder, const string& filename, const vector<T>& data)
        {
            string fullPath = "data/" + parentFolder + "/" + filename;
            ofstream out(fullPath);
            if (!out.is_open())
                throw runtime_error("Error: could not open file '" + fullPath + "' for writing!\n");
    
            for (int i = 0; i < data.size(); ++i)
                out << data[i] << '\n';
    
            if (!out)
                throw runtime_error("Error: failed while writing to file '" + fullPath + "'!\n");
        }
};