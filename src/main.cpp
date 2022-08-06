
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

extern "C" 
{
#include "FileLib.h" // C-Lib 
}

void process_file(std::string input_name, std::string output_name)
{
    FileLib::open_file_in(input_name.c_str());
    FileLib::open_file_out(output_name.c_str());

    for (char * line = FileLib::read_line(); line; line = FileLib::read_line())
    {
        std::string sline(line);
        free(line); // library wants this ¯\_(ツ)_/¯

        // read in vector data and compute length
        double x{0}, y{0}, z{0};
        std::istringstream ss(sline);
        ss >> x >> y >> z;
        double length = sqrt(x * x + y * y + z * z);

        // write result to file
        std::ostringstream out;
        out << length << "\n";
        std::string sout = out.str();
        FileLib::write_line(sout.c_str(), sout.size());
    }
    FileLib::close();
}

int main()
{
    std::thread t1(process_file, "data/vectors1_in.txt", "data/vectors1_out.txt");
    std::thread t2(process_file, "data/vectors2_in.txt", "data/vectors2_out.txt");
    std::thread t3(process_file, "data/vectors2_in.txt", "data/vectors3_out.txt");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}