#include <bits/stdc++.h>
using namespace std;

// Factores primos de un numero a.
typedef pair<int, int> Factor;
typedef long long Long;

vector<Factor> FactoresPrimos(int a) {
    int conteo = 0;
    vector<Factor> factores;
    while (!(a & 1)) ++conteo, a >>= 1;
    if (conteo) factores.push_back(
        Factor(2, conteo)), conteo = 0;

    int raiz = sqrt(a);
    for (int i = 3; i <= raiz; i += 2) {
        while (!(a % i)) ++conteo, a /= i;
        if (conteo) factores.push_back(
            Factor(i, conteo)), conteo = 0;
    }
    if (a > 1) factores.push_back(
        Factor(a, 1));
    return factores;
}

// Criba de Eratostenes de 1 a n.
vector<int> Criba(int n) {
    int raiz = sqrt(n); vector<int> criba(n + 1);
    for (int i = 4; i <= n; i += 2) criba[i] = 2;
    for (int i = 3; i <= raiz; i += 2) if (!criba[i])
        for (int j = i * i; j <= n; j += i)
            if (!criba[j]) criba[j] = i;
    return criba;
}

Long get_powers(Long n, Long p) {
    Long res = 0;
    while (n != 0) {
        res += n/p;
        n = n/p;
    }
    return res;
}

// Factores primos de n factorial (n!).
// El vector de primos debe estar ordenado.

vector<Factor> FactoresFactorial(
    int n, const vector<int>& primos) {

    vector<Factor> factores;
    for (int i = 0; i < primos.size(); ++i) {
        if (n < primos[i]) break; int p = primos[i];
        factores.push_back(Factor(primos[i], get_powers(n, p)));
    }
    return factores;
}

// Exponenciacion binaria a^n mod m.

typedef long long Long;

Long Exponenciar(Long a, Long n, Long m) {
    Long resultado = 1;
    for (; n; n >>= 1) {
        if (n & 1) resultado =
            (resultado * a) % m;
        a = (a * a) % m;
    }
    return resultado;
}

// Multiplicacion binaria a*b mod m.

Long Multiplicar(Long a, Long b, Long m) {
    Long resultado = 0;
    for (; b; b >>= 1) {
        if (b & 1) resultado =
            (resultado + a) % m;
        a = (a + a) % m;
    }
    return resultado;
}

// Algoritmo de Euclides extendido entre a y b.
// Además de devolver el gcd(a, b), resuelve la
// identidad de Bezout con el par (x, y). Si el
// parametro mod no es especificado, se resuelve
// con aritmetica normal; si mod se especifica,
// la identidad se resuelve modulo mod.

Long Euclides(Long a, Long b,
    Long& x, Long& y, Long mod = 0) {
    if (!b) { x = 1, y = 0; return a; }
    Long gcd = Euclides(b, a % b, x, y, mod);

    x = !mod? x - y * (a / b): (mod +
        x - (y * (a / b)) % mod) % mod;
    swap(x, y); return gcd;
}

// Tipo de dato para operar fracciones.

struct Fraccion {
    Long num, den;
    Fraccion() : num(0), den(1) {}
    Fraccion(Long n, Long d) {
        if (d < 0) n = -n, d = -d;
        Long gcd = __gcd(abs(n), abs(d));
        num = n / gcd, den = d / gcd;
    }

    Fraccion operator-() const {
        return Fraccion(-num, den);
    }

    Fraccion operator+(const Fraccion& f) {
        Long gcd = __gcd(den, f.den);
        return Fraccion(
            num * (f.den / gcd) +
            f.num * (den / gcd),
            den * (f.den / gcd)
        );
    }

    Fraccion operator-(const Fraccion& f) {
        return *this + -f; // a - b = a + (-b)
    }

    Fraccion operator*(const Fraccion& f) {
        return Fraccion(num * f.num, den * f.den);
    }

    Fraccion operator/(const Fraccion& f) {
        return Fraccion(num * f.den, den * f.num);
    }

    bool operator<(const Fraccion& cmp) {
        Long gcd = __gcd(den, cmp.den);
        return num * (cmp.den / gcd) <
               cmp.num * (den / gcd);
    }

    bool operator==(const Fraccion& cmp) {
        Long gcd = __gcd(den, cmp.den);
        return num * (cmp.den / gcd) ==
               cmp.num * (den / gcd);
    }
};

// Eliminacion Gaussiana de matrices.
// Definiciones iniciales para Gauss-Jordan.

typedef vector<double> Vector;
typedef vector<Vector> Matriz;

// Para eliminacion con fracciones.

Fraccion fabs(const Fraccion& f) {
    return Fraccion(abs(f.num), f.den);
}

bool EsCero(const Fraccion& f) {
    return f.num == 0;
}

// Para eliminacion con flotantes.

const double ERROR = 1e-9;

bool EsCero(double a) {
    return fabs(a) < ERROR;
}

// En caso de no permitir el pivoteo (eg. cuando
// requieran sacar una matriz inversa) simplemente
// comenten o borren la seccion <comment>.

int solved = 0;
void Gauss(Matriz& m) {
    int columna = 0;
    for (int i = 0; i < m.size(); ++i) {
        if (columna >= m[0].size()) break;
        bool col_cero = true; // Toda la columna tiene ceros
        
        for (int j = i; j < m.size() && col_cero; ++j)
            if (!EsCero(m[j][columna])) col_cero = false;

        if (col_cero) {
            columna++, i--;
            continue;
        }

        int fila_mayor = i;
        for (int j = i; j < m.size(); ++j)
            if (fabs(m[fila_mayor][columna]) <
                fabs(m[j][columna])) fila_mayor = j;
            swap(m[i], m[fila_mayor]);

        if (EsCero(m[i][columna])) continue;
        solved++;
        for (int j = m[i].size() - 1; j >= i; --j)
            m[i][j] = m[i][j] / m[i][columna];
        for (int j = 0; j < m.size(); ++j) {
            if (i == j || EsCero(m[j][columna])) continue;
            for (int k = m[j].size() - 1; k >= i; --k)
                m[j][k] = m[j][k] - m[i][k] * m[j][columna];
        }
        columna++;
    }
}

typedef vector<long double> Vector;
typedef vector<Vector> Matriz;

long double Determinante(Matriz& m) {
    int conta = 0;
    for (int i = 0; i < m.size(); ++i) {
        for (int j = i + 1; j < m.size(); ++j) {
            long double alfa = m[j][i] / m[i][i];
            for (int k = m[j].size() - 1; k >= i; --k)
                m[j][k] = m[j][k] - (m[i][k] * alfa);
        }
    }
    long double deter = 1;
    for (int i = 0; i < m.size(); ++i)
        deter = deter * m[i][i];
    return deter;
}
// Check if AxB = C  O(n^2)
bool Freivalds(Matriz &a, Matriz &b, Matriz &c) {
    int n = a.size();
    vector<Long> abr(n), cr(n), r(n), br(n);
    int T = 10;
    bool flag = true;
    while (T-- && flag) {
        for (int i = 0; i < n; ++i)
            r[i] = rand();
        
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                br[i] += r[j] * b[i][j];
                cr[i] += r[j] * c[i][j];
            }
        
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                abr[i] += br[j] * a[i][j];

        for (int i = 0; i < n; i++)
            if (cr[i] != abr[i])
                flag = false;
    }
    return flag;
}


// Tipo de dato para operar numeros complejos.
struct Complejo {
    double real, imag;
    Complejo() : real(), imag() {}
    Complejo(double r, double i) : real(r), imag(i) {}

    Complejo operator+(const Complejo& c) {
        return Complejo(real + c.real, imag + c.imag);
    }
    Complejo operator-(const Complejo& c) {
        return Complejo(real - c.real, imag - c.imag);
    }
    Complejo operator*(const Complejo& c) {
        return Complejo(real * c.real - imag * c.imag,
                        real * c.imag + imag * c.real);
    }
};

// Transformada rapida de Fourier.
// Se tiene que garantizar que el numero de
// elementos en el vector sea una potencia de 2.

const double M_2PI = 2 * M_PI;

vector<Complejo> FastAndFourier(
    const vector<Complejo>& a, int k = 1) {

    int n = a.size();
    if (n == 1) return a;
    vector<Complejo> par, impar;
    for (int i = 0; i < n; ++i)
        if (i & 1) par.push_back(a[i]);
        else impar.push_back(a[i]);
    impar = FastAndFourier(impar, k);
    par = FastAndFourier(par, k);

    vector<Complejo> fourier(n);
    Complejo w(1, 0), wn(cos(-k * M_2PI/n),
                         sin(-k * M_2PI/n));
    for (int i = 0; i < n/2; w = w * wn, ++i) {
        fourier[i + n/2] = impar[i] - w * par[i];
        fourier[i] = impar[i] + w * par[i];
    }
    return fourier;
}

// Transformada inversa de Fourier.
// Se tiene que garantizar que el numero de
// elementos en el vector sea una potencia de 2.

vector<Complejo> InvFastAndFourier(
    const vector<Complejo>& a) {

    vector<Complejo> ifft = FastAndFourier(a, -1);
    for (int i = 0; i < ifft.size(); ++i)
        ifft[i].real /= ifft.size(),
        ifft[i].imag /= ifft.size();
    return ifft;
}

// Convolucion discreta de dos vectores usando
// transformada rapida de Fourier O(n log n).
// Multiplica eficientemente dos polinomios.

Vector ConvolucionDiscreta(
    const Vector& a, const Vector& b) {

    int n = a.size() + b.size() - 1;
    int p = 1; while (p < n) p <<= 1;

    vector<Complejo> A(p), B(p), C(p);
    for (int i = 0; i < a.size(); ++i)
        A[i] = Complejo(a[i], 0);
    for (int i = 0; i < b.size(); ++i)
        B[i] = Complejo(b[i], 0);

    A = FastAndFourier(A);
    B = FastAndFourier(B);
    for (int i = 0; i < p; ++i)
        C[i] = A[i] * B[i];
    C = InvFastAndFourier(C);

    Vector convolucion(n);
    for (int i = 0; i < n; ++i)
        convolucion[i] = C[i].real;
    return convolucion;
}

int main() {
    return 0;
}
