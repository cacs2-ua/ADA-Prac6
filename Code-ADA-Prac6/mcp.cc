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

void setRowsColumns (size_t &n, size_t &m, ifstream &f) {
    f >> n >> m;

    f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            col++;
        }
        row++;
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
    // Verifica si el valor ya ha sido calculado y almacenado
    if (storage[i][j] != SENTINEL) {
        return storage[i][j];
    }

    // Casos base: llegada a los bordes de la matriz
    if (i == n - 1 && j == m - 1) {
        return storage[i][j] = mcp[i][j];
    }
    if (i == n - 1) { // Última fila, solo puede moverse hacia la derecha
        return storage[i][j] = mcp[i][j] + mcp_memo(storage, mcp, n, m, i, j + 1);
    }
    if (j == m - 1) { // Última columna, solo puede moverse hacia abajo
        return storage[i][j] = mcp[i][j] + mcp_memo(storage, mcp, n, m, i + 1, j);
    }

    // Calcula el mínimo costo de los tres posibles movimientos siguientes
    size_t right = mcp_memo(storage, mcp, n, m, i, j + 1);
    size_t down = mcp_memo(storage, mcp, n, m, i + 1, j);
    size_t diagonal = mcp_memo(storage, mcp, n, m, i + 1, j + 1);

    // Almacena el resultado en storage antes de retornar
    storage[i][j] = mcp[i][j] + std::min({right, down, diagonal});

    return storage[i][j];
}

size_t mcp_memo(const std::vector<std::vector<size_t>>& mcp, size_t n, size_t m) {
    std::vector<std::vector<size_t>> storage(n, std::vector<size_t>(m, SENTINEL));
    return mcp_memo(storage, mcp, n, m, 0, 0);
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
    if (ignoreNaive == false) {
        size_t result = mcp_naive(mcp, n, m, 0, 0);
        cout << result << endl;
    }
    cout << "memo solution" << endl;
    cout << mcp_memo(mcp,n,m);
    f.close();
    }

    return 0;
}

