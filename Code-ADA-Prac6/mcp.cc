//CRISTIAN ANDRÉS CÓRDOBA SILVESTRE DNI: 05988721G

#include <iostream> 
#include <limits>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream> 

#define SENTINEL std::numeric_limits<size_t>::max()

using namespace std;

void showUsage() {
    cout << "Usage:" << endl 
         << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

void showUsageError() {
    cerr << "Usage:" << endl 
         << "mcp [--p2D] [-t] [--ignore-naive] -f file" << endl;
}

void argumentsChecking(int argc, char* argv[], bool &t, 
                        bool &p2D, bool &ignoreNaive,
                        string &fileName) {
    if (argc == 1 && string(argv[0]) == "./mcp") {
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
                showUsageError();
                exit(EXIT_FAILURE);
            }
            fileName = argv[i];
            if (fileName.empty()) {
                cerr << "ERROR: missing filename." << endl;
                showUsageError();
                exit(EXIT_FAILURE);
            }
            ifstream is(fileName);

            if(!is) {
                cerr << "ERROR: can’t open file: " + fileName + "." << endl ;
                showUsageError();
                exit (EXIT_FAILURE);
            }
        }
        else {
            cerr << "ERROR: unknown option " + arg << endl;
            showUsageError();
            exit(EXIT_FAILURE);
        }
    }
}

void setRowsColumns (size_t &n, size_t &m, ifstream &f) {
    f >> n >> m;

    f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void SetMcp(std::vector<std::vector<size_t>>& mcp, std::ifstream &f) {
    std::string line;
    size_t row = 0;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        size_t value, col = 0;
        while (iss >> value) {
            mcp[row][col] = value;
            col++;
        }
        row++;
    }
}

size_t mcp_naive(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m, size_t i, size_t j) {
    size_t result = 0;

    if (i == n-1) {
        for (size_t k = j; k < m; k++) {
            result += mcp[n-1][k];
        }
        return result;
    }

    else if (j == m-1) {
        for (size_t k = i; k < n; k++) {
            result += mcp[k][m-1];
        }
        return result;
    }

    else {
        result = min({
    mcp[i][j] + mcp_naive(mcp,n,m,i,j+1),
    mcp[i][j] + mcp_naive(mcp,n,m,i+1,j+1),
    mcp[i][j] + mcp_naive(mcp,n,m,i+1,j)
});
        return result;
    }

}

size_t mcp_memo(std::vector<std::vector<size_t>>& storage, const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m, size_t i, size_t j) {
    if (storage[i][j] != SENTINEL) {
        return storage[i][j];
    }

    if (i == n - 1 && j == m - 1) {
        return storage[i][j] = mcp[i][j];
    }
    if (i == n - 1) { 
        return storage[i][j] = mcp[i][j] + mcp_memo(storage, mcp, n, m, i, j + 1);
    }
    if (j == m - 1) { 
        return storage[i][j] = mcp[i][j] + mcp_memo(storage, mcp, n, m, i + 1, j);
    }

    size_t right = mcp_memo(storage, mcp, n, m, i, j + 1);
    size_t down = mcp_memo(storage, mcp, n, m, i + 1, j);
    size_t diagonal = mcp_memo(storage, mcp, n, m, i + 1, j + 1);

    storage[i][j] = mcp[i][j] + std::min({right, down, diagonal});

    return storage[i][j];
}

size_t mcp_memo(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m) {
    std::vector<std::vector<size_t>> storage(n, std::vector<size_t>(m, SENTINEL));
    return mcp_memo(storage, mcp, n, m, 0, 0);
}

void assignDimensionsToStorage(const std::vector<std::vector<size_t>>& mcp, 
                                std::vector<std::vector<char>>& storage) {
    storage.resize(mcp.size()); 

    for (size_t i = 0; i < storage.size(); ++i) {
        storage[i].resize(mcp[i].size());
    }
}

size_t mcp_it_matrix(std::vector<std::vector<size_t>>& storage, const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m) {
    for (size_t i = 0; i < storage.size(); ++i) { 
        for (size_t j = 0; j < storage[i].size(); ++j) {
            if (i == 0 && j == 0) {
                storage[i][j] = mcp[0][0];
                continue;
            }

            if (i == 0) {
                storage[i][j] = storage[i][j - 1] + mcp[i][j];
                continue;
            }

            if (j == 0){
                storage[i][j] = storage[i - 1][j] + mcp[i][j];
                continue;
            }
            storage[i][j] = mcp[i][j] + std::min({storage[i][j - 1], storage[i - 1][j], storage[i - 1][j - 1]});
        }
    }

    return storage[n - 1][m - 1];    
}

void swap(std::vector<size_t> &v0, std::vector<size_t> &v1) {
    v1.swap(v0);
}

size_t mcp_it_vector(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m) {
    std::vector<size_t> v0(m,SENTINEL);
    std::vector<size_t> v1(m,SENTINEL);
    for (size_t i = 0; i < n; ++i) { 
        for (size_t j = 0; j < m; ++j) {
            if (i == 0 && j == 0) {
                v1[j] = mcp[0][0];
                continue;
            }

            if (i == 0) {
                v1[j] = v1[j - 1] + mcp[i][j];
                continue;
            }

            if (j == 0){
                v1[j] = v0[j] + mcp[i][j];
                continue;
            }

            v1[j] = mcp[i][j] + std::min({v0[j], v0[j - 1], v1[j - 1]});
        }
        swap(v0,v1);
    }

    return v0[m - 1]; 
}

void printStorage(const std::vector<std::vector<size_t>>& storage) {
    for (size_t i = 0; i < storage.size(); ++i) {
        for (size_t j = 0; j < storage[i].size(); ++j) {
            // Asegura que cada número se imprima con un espacio fijo, en este caso, asumiendo un máximo de 2 dígitos
            std::cout << storage[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printFinalResults
(size_t naiveResult, size_t memoResult, 
size_t iterResult, size_t iterEconomizedResult,
const std::vector<std::vector<size_t>>& storage,
bool ignoreNaive, bool p2D, bool t) {
    if (ignoreNaive == true) {
        cout << "- " << memoResult << " "  << iterResult << " " << iterEconomizedResult << endl;
        if (p2D == true) 
            cout << "?" << endl;
        if (t == true)
            printStorage(storage);
    }

    else {
        cout << naiveResult << " " << memoResult << " "  << iterResult << " " << iterEconomizedResult << endl;
        if (p2D == true) 
            cout << "?" << endl;
        if (t == true)
            printStorage(storage);
    }
}

int main (int argc, char* argv[]) {
    bool t = false;
    bool p2D = false;
    bool ignoreNaive = false;
    size_t n = 0;
    size_t m = 0;
    string fileName;
    argumentsChecking(argc,argv,t,p2D,ignoreNaive,fileName);
    ifstream f(fileName);
    if (f.is_open()) {
    setRowsColumns(n,m,f);
    std::vector<std::vector<size_t>> mcp(n, std::vector<size_t>(m));
    SetMcp(mcp, f);
    size_t naiveResult = SENTINEL;
    if (ignoreNaive == false) {
        naiveResult = mcp_naive(mcp, n, m, 0, 0);
    }
    size_t memoResult = mcp_memo(mcp,n,m);
    std::vector<std::vector<size_t>> iterStorage(n, std::vector<size_t>(m, SENTINEL));
    size_t iterResult = mcp_it_matrix(iterStorage,mcp,n,m);
    size_t iterEconomizedResult = mcp_it_vector(mcp,n,m);
    printFinalResults(naiveResult,memoResult,iterResult,iterEconomizedResult,iterStorage,ignoreNaive,p2D,t);
    f.close();
    }

    return 0;
}