#include <iostream>
#include <vector>
#include <stack>

using namespace std;
using digrafo = vector<vector<int>>;

int NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2;

void dfs_digrafo_transpuesto(int v, digrafo &D_transpuesto, vector<int> &estado, stack<int> &finish) {
    estado[v] = EMPECE_A_VER;
    
    for (int u : D_transpuesto[v]) {
        if (estado[u] == NO_LO_VI) {
            dfs_digrafo_transpuesto(u, D_transpuesto, estado, finish);
        }    
    }
    estado[v] = TERMINE_DE_VER;
    finish.push(v);
}

void dfs_digrafo_original(int v, digrafo &D, vector<int> &estado, vector<int> &cfc, int num_cfc, vector<vector<int>> &conjunto_de_cfc) {
    estado[v] = EMPECE_A_VER;
    cfc[v] = num_cfc;

    conjunto_de_cfc[conjunto_de_cfc.size() - 1].push_back(v);
    for (int u : D[v]) {
        if (estado[u] == NO_LO_VI) {
            dfs_digrafo_original(u, D, estado, cfc, num_cfc, conjunto_de_cfc);
        }    
    }
    estado[v] = TERMINE_DE_VER;
}

void Kosaraju(digrafo &D, vector<int> &cfc, vector<vector<int>> &conjunto_de_cfc){

    // Construyo Digrafo Transpuesto = O(|V| + |E|)
    int n = D.size() - 1; // n
    digrafo D_transpuesto(n + 1);

    for(int v = 1; v <= n; v++){
        for(int j = 0; j < D[v].size(); j++){
            // La arista en D es (v, D[v][j])
            D_transpuesto[D[v][j]].push_back(v);
        }
    }
    // Fin construcción Digrafo Transpuesto // 

    vector<int> parent(n + 1, -1);       // O(|V|)
    vector<int> estado(n + 1, NO_LO_VI); // O(|V|)
    stack<int> finish;                   // O(1)

    // Hacemos los 2 DFS
    for(int v = 1; v <= n; v++){
        if(estado[v] == NO_LO_VI) dfs_digrafo_transpuesto(v, D_transpuesto, estado, finish);
    }
    
    // Hacemos esto para no crear nuevos vectores y ahorrar memoria. Sigue siendo O(n)
    fill(estado.begin(), estado.end(), NO_LO_VI);
    fill(parent.begin(), parent.end(), -1);

    int num_cfc = 1;

    while(!finish.empty()){
        int v = finish.top();
        finish.pop();

        if(estado[v] == NO_LO_VI){
            conjunto_de_cfc.push_back(vector<int>());
            cfc[v] = num_cfc;
            dfs_digrafo_original(v, D, estado, cfc, num_cfc, conjunto_de_cfc);
            num_cfc++;
        }
    }
}

int main(){
    int n, m;

    // n = #nodos    m = #aristas
    cin >> n >> m;
    
    digrafo D(n + 1);

    for(int i = 0; i < m; i++){
        int v, w;
        cin >> v >> w;
        D[v].push_back(w);
    }

    vector<int> cfc(n + 1, 0);
    vector<vector<int>> conjunto_de_cfc;

    Kosaraju(D, cfc, conjunto_de_cfc);

    /*
    La idea es que si tengo una CFC, y desde un nodo 'v 'puedo llegar a algún nodo de dicha
    CFC, entonces no me conviene tirar ningún nodo que esté en la CFC, sino que en princpio,
    me conviene tirar v (el razonamiento se repite para la CFC a la que pertenezca 'v', y así sucesivamente).
    De esta manera, al poner en 'false' los nodos que no nos conviene tirar,
    y esto hacerlo para todos los nodos (por ende para todas las CFCs),
    nos van a quedar en 'true' los nodos que empiezan las cascadas, y que por consiguiente son los que tiran
    la mayor cantidad de piezas.
    */
    vector<bool> piezas_candidatas(n + 1, true);
    for(int v = 1; v <= n; v++){
        for(int w : D[v]){
            if(cfc[v] != cfc[w]){
                piezas_candidatas[w] = false;
                /*
                Si w no es una pieza candidata, tampoco lo son los demás nodos que
                están en la misma CFC. Por lo tanto, recorro todos los nodos de
                dicha CFC, y los seteo en 'false'.
                */
                for(int i = 0; i < conjunto_de_cfc[cfc[w] - 1].size(); i++){
                    piezas_candidatas[conjunto_de_cfc[cfc[w] - 1][i]] = false;
                }
            }
        }
    }

    /*
    Ahora en 'piezas_candidatas', están en true los nodos a los cuales no podemos llegar mediante otros
    nodos que estén en otras CFCs. Sin embargo, puede ser que tengamos varios nodos candidatos, que estén
    en la misma CFC. En dado caso, sólo hace falta tirar uno de ellos, y como nos piden que el conjunto
    de piezas óptima a tirar esté ordenado lexicográficamente, nos conviene el que tenga el número más chico
    */

    vector<bool> min_piezas_a_tirar(n + 1, false);
    int tam_subconjunto_optimo = 0;

    for(auto componente : conjunto_de_cfc){
        int min_pieza = n + 1;

        // Me quedo con la mínima pieza candidata de la CFC actual. Esto lo haga con todas las CFCs
        for(int v = 0; v < componente.size(); v++){
            if(piezas_candidatas[componente[v]] && componente[v] < min_pieza) min_pieza = componente[v];
        }
        if(min_pieza != n + 1){
            min_piezas_a_tirar[min_pieza] = true;
            tam_subconjunto_optimo++;
        }
    }

    cout << tam_subconjunto_optimo << endl;
    for(int i = 0; i < min_piezas_a_tirar.size(); i++){
        if(min_piezas_a_tirar[i]) cout << i << " ";
    }
    cout << endl;

    return 0;
}
