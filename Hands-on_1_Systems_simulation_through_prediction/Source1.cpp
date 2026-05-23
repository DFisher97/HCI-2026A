#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class LeastSquaredRegressor {
private:
    vector<vector<double>> X;
    vector<double> Y;
    vector<double> beta;

    // Transponer matriz
    vector<vector<double>> transpose(const vector<vector<double>>& mat) {
        int rows = mat.size();
        int cols = mat[0].size();
        vector<vector<double>> trans(cols, vector<double>(rows));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                trans[j][i] = mat[i][j];
            }
        }
        return trans;
    }

    // Multiplicar matriz x matriz
    vector<vector<double>> multiply(const vector<vector<double>>& A, const vector<vector<double>>& B) {
        int rowsA = A.size();
        int colsA = A[0].size();
        int colsB = B[0].size();
        vector<vector<double>> result(rowsA, vector<double>(colsB, 0.0));

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                for (int k = 0; k < colsA; ++k) {
                    result[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return result;
    }

    // Multiplicar matriz x vector
    vector<double> multiply(const vector<vector<double>>& A, const vector<double>& B) {
        int rows = A.size();
        int cols = A[0].size();
        vector<double> result(rows, 0.0);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result[i] += A[i][j] * B[j];
            }
        }
        return result;
    }

    // Inversa de matriz 3x3 (metodo de cofactores)
    vector<vector<double>> inverse3x3(const vector<vector<double>>& mat) {
        vector<vector<double>> inv(3, vector<double>(3));
        double det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
            mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
            mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

        if (det == 0) {
            cerr << "Error: Matriz singular, no tiene inversa." << endl;
            return inv;
        }

        double invDet = 1.0 / det;

        inv[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * invDet;
        inv[0][1] = -(mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) * invDet;
        inv[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invDet;
        inv[1][0] = -(mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) * invDet;
        inv[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invDet;
        inv[1][2] = -(mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]) * invDet;
        inv[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * invDet;
        inv[2][1] = -(mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]) * invDet;
        inv[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * invDet;

        return inv;
    }

public:
    // Constructor
    LeastSquaredRegressor() {
        // [1, Factor1(x1), Factor2(x2)]
        X = {
            {1, 41.9, 29.1}, {1, 43.4, 29.3}, {1, 43.9, 29.5}, {1, 44.5, 29.7},
            {1, 47.3, 29.9}, {1, 47.5, 30.3}, {1, 47.9, 30.5}, {1, 50.2, 30.7},
            {1, 52.8, 30.8}, {1, 53.2, 30.9}, {1, 56.7, 31.5}, {1, 57.0, 31.7},
            {1, 63.5, 31.9}, {1, 65.3, 32.0}, {1, 71.1, 32.1}, {1, 77.0, 32.5},
            {1, 77.8, 32.9}
        };

        // Yield (y)
        Y = {
            251.3, 251.3, 248.3, 267.5, 273.0, 276.5, 270.3, 274.9, 285.0,
            290.0, 297.0, 302.5, 304.5, 309.3, 321.7, 330.7, 349.0
        };
    }

    // Calcula B_0, B_1, y B_2
    void fit() {
        vector<vector<double>> X_T = transpose(X);               // X^T
        vector<vector<double>> X_T_X = multiply(X_T, X);         // X^T * X
        vector<vector<double>> X_T_X_inv = inverse3x3(X_T_X);    // (X^T * X)^-1
        vector<double> X_T_Y = multiply(X_T, Y);                 // X^T * Y

        beta = multiply(X_T_X_inv, X_T_Y);                       // beta = (X^T * X)^-1 * (X^T * Y)
    }

    // Imprime la Ecuación Resultante
    void printEquation() {
        cout << "=MODELO GENERADO=" << endl;
        cout << fixed << setprecision(4);
        cout << "Parametros calculados:" << endl;
        cout << "B_0 = " << beta[0] << endl;
        cout << "B_1 = " << beta[1] << endl;
        cout << "B_2 = " << beta[2] << endl;

        cout << "\nEcuacion de Regresion Multiple:" << endl;
        cout << "Yield = " << beta[0] << " + (" << beta[1] << " * Factor1) + ("
            << beta[2] << " * Factor2)\n" << endl;
    }

    // Predicción individual
    double predict(double x1, double x2) {
        return beta[0] + (beta[1] * x1) + (beta[2] * x2);
    }

    // Predice 5 experimentos con valores desconocidos
    void simulate() {
        cout << "=SIMULACION DE EXPERIMENTOS=" << endl;
        // Valores aleatoriamente (fuera del dataset)
        vector<pair<double, double>> experimentos = {
            {45.0, 30.0},
            {55.0, 31.0},
            {60.5, 31.8},
            {70.0, 32.5},
            {80.0, 33.5}
        };

        for (int i = 0; i < experimentos.size(); ++i) {
            double x1 = experimentos[i].first;
            double x2 = experimentos[i].second;
            double yield = predict(x1, x2);

            cout << "Experimento " << i + 1 << " -> Factor 1: " << x1
                << " | Factor 2: " << x2 << " ===> YIELD PREDICTIVO: "
                << yield << " unidades" << endl;
        }
    }
};

int main() {
    // Instanciar el objeto
    LeastSquaredRegressor regressor;

    // Ajustar el modelo
    regressor.fit();

    // Imprimir Ecuación y Parámetros
    regressor.printEquation();

    // Simular 5 experimentos
    regressor.simulate();

    return 0;
}