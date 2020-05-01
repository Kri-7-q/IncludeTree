#include <string>
#include <vector>
#include <iostream>
#include <fstream>


void parseFile(const std::string &filepath, const std::string &filename, const std::vector<std::string> &patternList, int depth);

int main(int argc, char *argv[])
{
    // --------------------------------------------
    // Program arguments
    if (argc < 2)
    {
        std::cerr << "Sie müssen eine Datei angeben!\n";
        return 0;
    }
    std::vector<std::string> patternList;
    std::string filepath(argv[1]);
    if (filepath.at(0) != '/')
    {
        std::string appPath(argv[0]);
        size_t pos = appPath.rfind('/');
        filepath = appPath.substr(0, pos) + filepath;
    }
    size_t index = filepath.rfind('/');
    std::string filename = filepath.substr(index+1);
    filepath = filepath.substr(0, index);
    // Such pattern
    for (int index=1; index<argc; ++index)
    {
        patternList.push_back( std::string(argv[index]) );
    }
    try {
        parseFile(filepath, filename, patternList, 0);
    } catch (const std::exception &e) {
        std::cerr << "Fehler: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

/*!
 * \brief parseFile
 * \param filepath
 * \param patternList
 * \param depth
 * \param out
 */
void parseFile(const std::string &filepath, const std::string &filename, const std::vector<std::string> &patternList, int depth)
{
    // print filename
    std::cerr << "\033[34m" << depth << ": " << std::string(depth*2, ' ') << filename << "\033[0m\n";
    // Search in file
    std::fstream inStream;
    std::string path = filepath + '/' + filename;
    inStream.open(path.c_str());
    if (! inStream.is_open())
    {
        throw std::runtime_error("Konnte Datei '" + path + "' nicht öffnen!");
    }
    std::vector<std::string> content;
    while (!inStream.eof())
    {
        char lineBuff[1024];
        inStream.getline(lineBuff, 1024);
        std::string line(lineBuff);
        size_t index = line.find('#');
        if (index == 0)
            continue;
        if (index > 0 && index < line.length()-1)
            line = line.substr(0, index);
        // Search for include
        size_t pos = line.find("@INCLUDE");
        if (pos != std::string::npos)
        {
            pos+=8;
            pos = line.find('(', pos);
            int end = line.find(")", pos);
            if (line.at(pos+1) == '\"') pos+=1;
            if (line.at(end-1) == '\"') end-=1;
            std::string filename = line.substr(pos+1, end-pos-1);
            parseFile(filepath, filename, patternList, depth+1);
        }
        for (const std::string &pattern : patternList)
        {
            if (line.find(pattern) != std::string::npos)
            {
                size_t index = line.find_first_not_of(' ');
                size_t endIndex = 0;
                for (endIndex=line.length()-1; endIndex>index; --endIndex)
                    if (line[endIndex] != ' ') break;
                std::cerr << depth << ": " << std::string(depth*2, ' ') << line.substr(index, endIndex+1) << '\n';
            }
        }
    }
    inStream.close();
}
