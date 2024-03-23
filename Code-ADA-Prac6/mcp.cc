#include <iostream> 
#include <limits>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream> 

#define SENTINEL -1

using namespace std;


struct mcp_t {
    std::vector<std::vector<int>> mcp;
    int n = 0;
    int m = 0;
    /*
    bool tables = false;
    bool ignoreNaive = false;
    bool path = false;
    */

    std::vector<std::vector<int>> tablaMemoization;
    std::vector<std::vector<int>> tablaIterativa;
};

void showUsage() {
    cout << "Usage:" << endl 
         << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

void argumentsChecking(int argc, char* argv[], bool &t, 
                        bool &p2D, bool &ignoreNaive,
                        string &fileName) {
    if (argc == 1 && string(argv[0]) != "mcp") {
        showUsage();
        exit(EXIT_SUCCESS);
    }

    for (unsigned int i = 1;  i < (unsigned int) argc; i++){
        string arg = argv[i];
        if(arg == "-t") {
            t = true;
        }
        else if (arg == "--p2D") {
            p2D = true;
        }
        else if (arg == "--ignore-naive") {
            ignoreNaive = true;
        }
        else if (arg == "-f") {
            i++;
            if (i >= (unsigned int) argc) {
                cerr << "ERROR: missing filename." << endl;
                showUsage();
                exit(EXIT_FAILURE);
            }
            fileName = argv[i];
            if (fileName.empty()) {
                cerr << "ERROR: missing filename." << endl;
                showUsage();
                exit(EXIT_FAILURE);
            }
            ifstream is(fileName);

            if(!is) {
                cerr << "ERROR: can’t open file: " + fileName + "." << endl ;
                showUsage();
                exit (EXIT_FAILURE);
            }
        }
        else {
            cerr << "ERROR: unknown option " + arg << endl;
            showUsage();
            exit(EXIT_FAILURE);
        }
    }
}

int sumatorio(const std::vector<size_t>& numeros) {
    size_t suma = 0;
    for (size_t numero : numeros) {
        suma += numero;
    }
    return suma;
}

size_t mcp_naive(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m, size_t i, size_t j) {
    size_t result = 0;

    if (m == 1) {
        for (size_t k = i; k < n; k++) {
            result += mcp[k][0];
        }
        return result;
    }

    else if (n == 1) {
        for (size_t k = j; k < m; k++) {
            result += mcp[0][k];
        }
        return result;
    }

    else {
        result = min(mcp_naive(mcp, n, m-1, i, j+1) + mcp[i][j + 1],
                     mcp_naive(mcp, n-1, m-1, i+1, j+1) + mcp[i + 1][j + 1],
                     mcp_naive(mcp, n-1, m, i+1, j) + mcp[i + 1][j]);
        return result;
    }

}

void setRowsColumns (size_t &n, size_t &m, ifstream &f) {
    f >> n >> m;
}

void SetMcp(std::vector<std::vector<size_t>>& mcp, std::ifstream &f) {
    std::string line;
    size_t row = 0;

    // Leer el resto del archivo línea por línea
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        size_t value, col = 0;
        // Leer cada valor en la línea actual y asignarlo a mcp
        while (iss >> value) {
            mcp[row][col] = value;
            ++col;
        }
        ++row;
    }
}


int main (int argc, char* argv[]) {
    bool t = false;
    bool p2D = false;
    bool ignoreNaive = false;
    string fileName;
    size_t n = -1;
    size_t m = -1;
    argumentsChecking(argc,argv,t,p2D,ignoreNaive,fileName);
    /*
    cout << "Evereything Okay :D" << endl;
    cout << "t: " << t << endl
         << "p2D: " << p2D << endl
         << "ignoreNaive: " << ignoreNaive << endl
         << "fileName: " << fileName << endl;
    */
   
   ifstream f(fileName);
   setRowsColumns(n,m,f);
   return 0;
}

