#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const int UTP = 0;
const int Fibra = 1;

double precioUTP = 0;
double precioFibra = 0;

int caso = 1;

struct Oficina {
    int x, y;
};

struct Arista{
    int u, v, tipoDeCable;
    double peso;
};

struct DSU{

    DSU(int n){
        padre = rank = vector<int>(n);
        for(int v = 0; v < n; v++) padre[v] = v;
    }

    int find(int v){
        if(v == padre[v]) return v;
        return padre[v] = find(padre[v]);
    }

    void unite(int u, int v){
        u = find(u), v = find(v);
        if(u == v) return;
        if(rank[u] < rank[v]) swap(u,v);
        padre[v] = padre[u];
        rank[u] = max(rank[u],rank[v]+1);
    }

    vector<int> padre;
    vector<int> rank;
};

bool compararPesos(const Arista& a, const Arista& b){
    return a.peso < b.peso;
}

double distancia(double x1, double y1, double x2, double y2){
    return sqrt(pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2));
}

vector<pair<double, int>> pesosArista(vector<Oficina> oficinas, int N, double U, double V, int R){
    vector<pair<double, int>> pesos;
    double d;
        for(int i = 0; i < N; i++){
            for(int j = i + 1; j < N; j++){
                d = distancia(oficinas[i].x, oficinas[i].y, oficinas[j].x, oficinas[j].y);
                if(d <= R){
                    double peso = d * U;
                    int tipoDeCable = UTP;
                    pesos.push_back(make_pair(peso, tipoDeCable));
                } else {
                    double peso = d * V;
                    int tipoDeCable = Fibra;
                    pesos.push_back(make_pair(peso, tipoDeCable));
                } 
            }
        }
    return pesos;
}

void kruskal(vector<Arista> aristas, int N, int W, int C){
    sort(aristas.begin(), aristas.end(), compararPesos);
    DSU dsu(N);
    int numComponentes = N; 
    vector<Arista> bosque;

    for(auto& arista : aristas){
        int u = arista.u;
        int v = arista.v;

        // Si (u,v) es arista segura
        if(dsu.find(u) != dsu.find(v)){
            // Agregar
            dsu.unite(u, v);
            bosque.push_back(arista);

            numComponentes--; 

            if(numComponentes == W){
                break;
            }
        }
    }

    for(int i = 0; i < bosque.size(); i++){
        if(bosque[i].tipoDeCable == UTP){
            precioUTP += bosque[i].peso;
        } else {
            precioFibra += bosque[i].peso;
        }
    }

    if(numComponentes == W){
        cout << fixed << setprecision(3) << "Caso #" << C << ": " << precioUTP << " " << precioFibra << endl;
    } else{
        cout << "IMPOSSIBLE\n";
    } 
}

int main(){
    int c;
    cin >> c;

    for(int caseNum = 1; caseNum <= c; caseNum++){
        int N, R, W, U, V;
        cin >> N >> R >> W >> U >> V;

        vector<Oficina> oficinas(N);
        for(int i = 0; i < N; i++){
            cin >> oficinas[i].x >> oficinas[i].y;
        }

        vector<pair<double, int>> pesos = pesosArista(oficinas, N, U, V, R);

        vector<Arista> aristas;
        for(int i = 0; i < N; i++){
            for(int j = i + 1; j < N; j++){
                Arista aristai;
                aristai.u = i;
                aristai.v = j;

                aristas.push_back(aristai);
            }
        }

        for(int i = 0; i < aristas.size(); i++){
            aristas[i].peso = pesos[i].first;
            aristas[i].tipoDeCable = pesos[i].second;
        }



        kruskal(aristas, N, W, caso);

        //Limpio todas las estructuras
        caso++;
        aristas.clear();
        pesos.clear();
        oficinas.clear();
        precioFibra = 0;
        precioUTP = 0;

    }

    return 0;
}
