#include <iomanip>      // std::setprecision
#include <iostream>
#include <vector>

using namespace std;
using grafo = vector<vector<long long>>;

long long cant_subconjuntos_tomados_de_a_2(long long n){
    if(n < 2) return 0; // Si no hay ningún elemento en la CC, no hay ninguna forma de tomar un par de nodos

    return (n * (n - 1))/2;
}

long long NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2;

void dfs1(long long v, grafo &aristas, vector<long long> &parent, vector<long long> &estado, vector<vector<long long>> &treeEdges, vector<long long> &backConExtremoInferiorEn, vector<long long> &backConExtremoSuperiorEn) {
    estado[v] = EMPECE_A_VER;
   
    for (long long u : aristas[v]) {
        if (estado[u] == NO_LO_VI) {
            treeEdges[v].push_back(u);
            parent[u] = v;
            dfs1(u, aristas, parent, estado, treeEdges, backConExtremoInferiorEn, backConExtremoSuperiorEn);
        }    
        else if (u != parent[v] && estado[u] == EMPECE_A_VER) {
            backConExtremoInferiorEn[v]++;
            backConExtremoSuperiorEn[u]++; 
        }
    }
    estado[v] = TERMINE_DE_VER;
}

void dfs2(long long v, grafo &aristas, vector<long long> &componentes, long long num_componente, vector<long long> &cant_elem_por_CC, vector<long long> &estado) {
    estado[v] = EMPECE_A_VER;
    componentes[v] = num_componente;
    
    for (long long u : aristas[v]) {
        if (estado[u] == NO_LO_VI) {
            cant_elem_por_CC[num_componente]++;
            dfs2(u, aristas,componentes, num_componente, cant_elem_por_CC, estado);
        }    
    }
    estado[v] = TERMINE_DE_VER;
}

long long cubren(long long v, vector<long long> &parent, vector<long long> &backConExtremoInferiorEn, vector<long long> &backConExtremoSuperiorEn, vector<long long> &memo, vector<vector<long long>> &treeEdges) {
    
    if (memo[v] != -1) return memo[v];

    long long res = 0;
    res += backConExtremoInferiorEn[v];
    res -= backConExtremoSuperiorEn[v];
    for (long long hijo : treeEdges[v]){
        if (hijo != parent[v]) {
            parent[hijo] = v;
            res += cubren(hijo, parent, backConExtremoInferiorEn, backConExtremoSuperiorEn, memo, treeEdges);
        }
    }
    memo[v] = res;
    return res;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          

int main(){
    long long n, m;
    cin >> n >> m;

    grafo G(n + 1);

    for(long long i = 0; i < m; i++){
        long long v, w;
        cin >> v >> w;

        G[v].push_back(w);
        G[w].push_back(v);
    }

    vector<long long> parent(n + 1, -1);
    vector<long long> estado(n + 1, NO_LO_VI);
    vector<vector<long long>> treeEdges(n + 1);
    vector<long long> backConExtremoInferiorEn(n + 1); 
    vector<long long> backConExtremoSuperiorEn(n + 1); 
    
    for (int v = 1; v <= n; v++) {
        if (estado[v] == NO_LO_VI) {
            dfs1(v, G, parent, estado, treeEdges, backConExtremoInferiorEn, backConExtremoSuperiorEn);
        }
    }

    vector<long long> memo(n + 1, -1);
    vector<long long> puentes(n + 1, -1);

    for (int v = 1; v <= n; v++) {
        if(cubren(v, parent, backConExtremoInferiorEn, backConExtremoSuperiorEn, memo, treeEdges) == 0) {
            puentes[v] = parent[v];
        }
    }

    vector<vector<long long>> G_sin_puentes(n + 1);
    for(int v = 1; v <= n; v++){ 
        // Si la arista que conecta a v con su padre, NO es puente
        if(puentes[v] == -1){
            for(int j = 0; j < G[v].size(); j++){
                // Si el vecino de v que estoy mirando, no tiene a v como puente (su padre)
                if(puentes[G[v][j]] != v){
                    G_sin_puentes[v].push_back(G[v][j]);
                }
                 
            }
        } 
        else {
            for(int j = 0; j < G[v].size(); j++){
                if(G[v][j] != puentes[v] && puentes[G[v][j]] != v){
                    G_sin_puentes[v].push_back(G[v][j]);
                }
            }
        }
    }
    //          PREPARACIÓN NUEVO GRAFO          //
    fill(estado.begin(), estado.end(), NO_LO_VI); // Con esto ahorramos memoria al no crear un nuevo vector
    vector<long long> componentes(n + 1, 0);
    long long num_componente = 1;
    vector<long long> cant_elem_por_CC(n + 1, 0);
    

    for(int v = 1; v <= n; v++) {
        if (estado[v] == NO_LO_VI) {
            componentes[v] = num_componente;
            cant_elem_por_CC[num_componente]++;
            dfs2(v, G_sin_puentes, componentes, num_componente, cant_elem_por_CC, estado);
            num_componente++;
        }
    }
    /*
    Cantidad de formas de GANAR habiendo quitado los puentes:
    Para cada CC, hacer (#nodos_en_dicha_CC)  (NÚMERO COMBINATORIO)
                        (                  )
                        (        2         )
    */

    long long cant_formas_de_ganar = 0;
    for(int v = 1; v <= n; v++){
        cant_formas_de_ganar += cant_subconjuntos_tomados_de_a_2(cant_elem_por_CC[v]);
    }

    long long cant_formas_de_jugar = cant_subconjuntos_tomados_de_a_2(n);
    
    double res = 1 - ((double) cant_formas_de_ganar) / cant_formas_de_jugar;

    cout << fixed << setprecision(5) << res << "\n";

    return 0;
}