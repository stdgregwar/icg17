#include "ShaderBuilder.h"
#include <fstream>
#include <sstream>

namespace ShaderBuilder {

using namespace std;

string makeSource(const string& file) {
    const string begin = "#include ";
    size_t p = file.find_last_of("/\\");
    string baseDir = p != string::npos ? file.substr(0, p) : "";
    ifstream ifstr(file);
    if(!ifstr.is_open()) {
        throw runtime_error("Could not open file " + file);
    }
    ostringstream oss;
    string line;
    while(getline(ifstr,line), !ifstr.eof()) {
        if(!line.compare(0,begin.size(),begin)) {
            string iFile = line.substr(begin.size());
            line = makeSource(baseDir + iFile);
        }
        oss << line << endl;
    }
    return oss.str();
}

void printSource(const string& src) {
    istringstream str(src);
    string line;
    int i = 0;
    while(!getline(str,line).eof()) {
        cout << i++ << ". " << line << endl;
    }
}

GLuint makeShader(const string& vshader, const string& fshader, const string& gshader) {
    string vshadersrc = makeSource(vshader);
    string fshadersrc = makeSource(fshader);
    string gshadersrc;
    if(gshader!="") {
        gshadersrc = makeSource(gshader);
    }
    GLuint pid = icg_helper::CompileShaders(vshadersrc.c_str(),fshadersrc.c_str(), gshader != "" ? gshadersrc.c_str() : nullptr);
    if(!pid) {
        cout << "\n\nVSHAD" << endl;
        ShaderBuilder::printSource(vshadersrc);
        cout << "\n\nFSHAD" << endl;
        ShaderBuilder::printSource(fshadersrc);
    }
    return pid;
}

}
