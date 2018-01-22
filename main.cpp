#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

//estructura para comparar el heap y dejar el menor al principio
struct comparedis {
    bool operator()(vector<int> i, vector<int> j) {
        return i > j;
    }
};

priority_queue<int,vector<vector<int> >, comparedis > vect;
int iMat[21][21];
int minimo = 20000;

//funcion que saca valor posible de un ciclo incompleto
int bound( vector<int> va,int n){
    int sum = 0;
    vector<bool> bo(n,0);

    //vector boleano para ver cuales ya son parte del ciclo
    for(int i=1;i<va.size()-1;i++){
        bo[va[i] - 1] = 1;
        bo[va[i + 1] - 1] = 1;
        sum += iMat[va[i] - 1][va[i + 1] - 1];
    }

    vector<bool> bi(n,0),bj(n,0);

    //dos vectores boleanos en 0, bi tiene los constrains de i y bj tiene los constrains en j
    for(int i=0;i<va.size()-2;i++){
        bi[va[i + 1] -1 ] = 1;
        bj[va[i + 2] -1 ] = 1;
    }

    //ciclo para atravesar los renglones de la matriz
    for(int i=1;i<n;i++){
        int mi = 20000;
        bool bolea=0;

        //si hay un constrain en un renglon se salta al siguiente
        if(bi[i]==1 && i!=n-1)
            i++;

        //ciclo para atravesar las columnas de la matriz
        for(int j=0;j<n;j++){
            //si hay un constrain en una columna se salta al siguiente
            if(bj[j]==1 && j!=n-1)
                j++;

            //si no hay constrain y no es la diagonal principal va buscando el menor por renglon
            if(bi[i] == 0 && bj[j] == 0 && i!=j){
                mi = min(mi , iMat[i][j]);
                if(mi ==20000) bolea=1;
            }
        }

        //si un minimo queda como 20000 regresa el resultado de 20000, pues es el maximo
        if(mi == 20000 && bolea==1)
            return 20000;

        //control para no sumar 20000 de mas
        if(mi!= 20000)
            sum+=mi;
    }
    // regresa resultado
    return sum;
}

//funcion para sacar el minimo valor de un ciclo hamiltoniano
int desmadre(int n){
    //ciclo que hace hasta que haya un costo posible menor al minimo actual
    do{
        vector<int> v = vect.top();
        vect.pop();
        vector<bool> bul (n,0);
        //vector boleano para ver cuales ya estan en el ciclo
        for(int i = 1;i < v.size();i++){
            bul[v[i] - 1] = 1;
        }

        for(int i = v.size();i <= n; i++){
            vector<int> aux = v;
            int ite=0;

            //buscar el primero en el vector booleano que no este en el ciclo
            while(bul[ite])
                ite++;

            //darle valor de ya visitado
            bul[ite] = 1;

            //meter ese valor como parte del ciclo
            aux.push_back(ite + 1);

            //checar si ya solo queda uno para el ciclo
            if(aux.size() == n){
                minimo = min( minimo,bound(aux,n) );
            }

            else{
                aux[0] = bound(aux,n);
                vect.push(aux);
            }
        }

    }while(vect.top()[0] <= minimo);

    return minimo;
}

int main()
{
    int n,m;
    cin>> n >> m;

    //inicializar la matriz con numero grande y 0 en la diagonal principal
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            iMat[i][j]=20000;
            if(i == j)
                iMat[i][j]=0;
        }
    }

    //meter los valores de los arcos
    char l,r;
    for(int i=0;i<m;i++){
        cin>>l>>r;
        cin>>iMat[l-65][r-65];
        iMat[r-65][l-65]=iMat[l-65][r-65];
    }

    vector<int> v;

    //primer vector a meter
    v.push_back(10);
    v.push_back(1);

    vect.push(v);

    //llamar al resultado del minimo ciclio hamiltoniano
    int re=desmadre(n);

    //control por si no hay ciclos
    if(re>=20000){
        cout<<"INF";
    }
    else
        cout<<re;

    return 0;
}
