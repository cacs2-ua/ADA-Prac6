#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

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


int main (int argc, char* argv[]) {

    
    return 0;
}

size_t mcp_memo(std::vector<std::vector<size_t>>& storage,const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m, size_t i, size_t j) {
    if (storage[n-1][m-1] != SENTINEL)
        return storage[n-1][m-1];

    if (i == n-1) {
        for (size_t k = j; k < m; k++) {
            if (j == 0 && i != 0) { //if it's not the initial square
              return storage[n-1][k] = storage[n-2][k] + mcp[n-1][k];
            }
            else
              return storage[n-1][k] = storage[n-1][k-1] + mcp[n-1][k];
        }
    }

    else if (j == m-1) {
        for (size_t k = i; k < n; k++) { 
            if (i == 0 && j != 0)   
              return storage[k][m-1] = storage[k][m-2] + mcp[k][m-1];
            else
               return storage[k][m-1] = storage[k-1][m-1] + mcp[k][m-1];
        }
    }

    else {
     return storage[i][j] = min({
        mcp[i][j] + mcp_naive(mcp,n,m,i,j+1),
        mcp[i][j] + mcp_naive(mcp,n,m,i+1,j+1),
        mcp[i][j] + mcp_naive(mcp,n,m,i+1,j)
        });
    }
}

size_t mcp_memo(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m, size_t i, size_t j) {
    vector<vector<size_t>>storage(n, vector<size_t>(m, SENTINEL));
    return mcp_memo(storage,mcp,n,m,i,j);
}

void test1(int argc, char* argv[]) {
    bool t = false;
    bool p2D = false;
    bool ignoreNaive = false;
    string fileName;
    argumentsChecking(argc,argv,t,p2D,ignoreNaive,fileName);
    cout << "Evereything Okay :D" << endl;
    cout << "t: " << t << endl
         << "p2D: " << p2D << endl
         << "ignoreNaive: " << ignoreNaive << endl
         << "fileName: " << fileName << endl;
}

