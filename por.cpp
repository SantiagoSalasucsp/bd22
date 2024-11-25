#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
//#include <bits/stdc++.h>
using namespace std;




static int id_sec = 1;
static int id_pis = 1;
static int id_sup = 1;
static int id_dis = 1;

struct discodu;
struct disco;
struct superficie;
struct pista;
struct sector;

static vector<sector*> todos_sectores;
static vector<pista*> todas_pistas;
static vector<superficie*> todas_superficies;
static vector<disco*> todos_discos;

struct tipo {
    int tam;
    int objeto_id;
    string name;
    tipo(int t, int id, string n){
        tam = t;
        objeto_id = id;
        name = n;
    }
    virtual void getvalor() = 0;
};

struct INTEGER: public tipo {
    int val;
    INTEGER(int val, int ID, int t, string n): tipo(t, ID, n) {
        this->val = val;
    }
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};

struct VARCHAR: public tipo {
    string val;
    int longitud_maxima;
    
    VARCHAR(string val, int ID, int max, string n): tipo(val.length(), ID, n) {
        this->longitud_maxima = max;
        if(val.length() <= max) {
            this->val = val;
        } else {
            cout << "no se pudo ingresar" << endl;
        }
    }
    
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};

struct DECIMAL: public tipo {
    double val;
    int precision;
    int escala;
    
    DECIMAL(double val, int ID, int precision, int escala, string n): tipo(8, ID, n) {
        this->val = val;
        this->precision = precision;
        this->escala = escala;
    }
    
    void getvalor() override {
        cout << "el valor es:" << val << endl;
    }
};




struct sector {
    int tamano;
    sector* next_sector;
    pista* pista_padre;
    vector<int> objeto_ids;
    int ID_sector;

    sector(int tam, pista* padre) {
        tamano = tam;
        next_sector = nullptr;
        pista_padre = padre;
        ID_sector = id_sec++;
        todos_sectores.push_back(this);
    }
};

struct pista {
    vector<sector*> sectores;
    pista* next_pista;
    superficie* superficie_padre;
    vector<int> objeto_ids;
    int ID_pista;

    pista(int numSectores, int tamSector, superficie* padre) {
        next_pista = nullptr;
        superficie_padre = padre;
        ID_pista = id_pis++;
        todas_pistas.push_back(this);

        for (int i = 0; i < numSectores; i++) {
            sector* nuevo_sector = new sector(tamSector, this);
            sectores.push_back(nuevo_sector);
        }
    }
};

struct superficie {
    vector<pista*> pistas;
    superficie* next_superficie;
    disco* disco_padre;
    vector<int> objeto_ids;
    int ID_superficie;

    superficie(int numPistas, int numSectores, int tamSector, disco* padre) {
        next_superficie = nullptr;
        disco_padre = padre;
        ID_superficie = id_sup++;
        todas_superficies.push_back(this);

        for (int i = 0; i < numPistas; i++) {
            pista* nueva_pista = new pista(numSectores, tamSector, this);
            pistas.push_back(nueva_pista);
        }
    }
};

struct disco {
    vector<superficie*> superficies;
    disco* next_disco;
    discodu* dicodu_padre;
    vector<int> objeto_ids;
    int ID_disco;

    disco(int numPistas, int numSectores, int tamSector, discodu* padre) {
        next_disco = nullptr;
        dicodu_padre = padre;
        ID_disco = id_dis++;
        todos_discos.push_back(this);

        for (int i = 0; i < 2; i++) {
            superficie* nueva_superficie = new superficie(numPistas, numSectores, tamSector, this);
            superficies.push_back(nueva_superficie);
        }
    }
};

struct discodu {
    vector<disco*> discos;
    vector<int> objeto_ids;
    int numDiscos;
    int numSuperficies;
    int numPistas;
    int numSectores;
    int tamSector;

    sector* ultimo_sector_usado;
    pista* ultima_pista_usada;
    superficie* ultima_superficie_usada;
    disco* ultimo_disco_usado;

    void enlazar_elementos() {
        if (!todos_sectores.empty()) ultimo_sector_usado = todos_sectores[0];
        if (!todas_pistas.empty()) ultima_pista_usada = todas_pistas[0];
        if (!todas_superficies.empty()) ultima_superficie_usada = todas_superficies[0];
        if (!todos_discos.empty()) ultimo_disco_usado = todos_discos[0];
        
        for(size_t i = 0; i < todos_sectores.size() - 1; i++) {
            todos_sectores[i]->next_sector = todos_sectores[i + 1];
        }
        if (!todos_sectores.empty()) {
            todos_sectores.back()->next_sector = nullptr;
        }

        for(size_t i = 0; i < todas_pistas.size() - 1; i++) {
            todas_pistas[i]->next_pista = todas_pistas[i + 1];
        }
        if (!todas_pistas.empty()) {
            todas_pistas.back()->next_pista = nullptr;
        }

        for(size_t i = 0; i < todas_superficies.size() - 1; i++) {
            todas_superficies[i]->next_superficie = todas_superficies[i + 1];
        }
        if (!todas_superficies.empty()) {
            todas_superficies.back()->next_superficie = nullptr;
        }

        for(size_t i = 0; i < todos_discos.size() - 1; i++) {
            todos_discos[i]->next_disco = todos_discos[i + 1];
        }
        if (!todos_discos.empty()) {
            todos_discos.back()->next_disco = nullptr;
        }
    }

    discodu(int nDiscos, int nPistas, int nSectores, int tSector) {
        todos_sectores.clear();
        todas_pistas.clear();
        todas_superficies.clear();
        todos_discos.clear();

        numDiscos = nDiscos;
        numSuperficies = 2;
        numPistas = nPistas;
        numSectores = nSectores;
        tamSector = tSector;

        cout << "Creando dicodu con " << numDiscos << " discos" << endl;
        
        for (int i = 0; i < numDiscos; i++) {
            disco* nuevo_disco = new disco(numPistas, numSectores, tamSector, this);
            discos.push_back(nuevo_disco);
        }

        enlazar_elementos();

        todos_sectores.clear();
        todas_pistas.clear();
        todas_superficies.clear();
        todos_discos.clear();
    }

    ~discodu() {
        for (auto disco : discos) {
            for (auto superficie : disco->superficies) {
                for (auto pista : superficie->pistas) {
                    for (auto sector : pista->sectores) {
                        delete sector;
                    }
                    delete pista;
                }
                delete superficie;
            }
            delete disco;
        }
    }
};





class tabla {
private:
    vector<string> tokens;

public:
    tabla() {
        ifstream archivo("estructura.txt");
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo estructura.txt" << endl;
            return;
        }

        string linea;
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            if (linea == ");") {
                break;
            }

            if (!linea.empty() && linea != ")") {
                if (linea[linea.length() - 1] == ',') {
                    linea = linea.substr(0, linea.length() - 1);
                }

                stringstream ss(linea);
                string palabra;

                while (ss >> palabra) {
                    if (palabra == "PRIMARY" || palabra == "KEY" || palabra == "NOT" || palabra == "NULL") {
                        continue;
                    }

                    if (palabra == ");") {
                        break;
                    }

                    size_t pos = palabra.find("(");
                    if (pos != string::npos) {
                        string tipo = palabra.substr(0, pos);
                        string numero = palabra.substr(pos + 1);

                        if (!numero.empty() && (numero[numero.length() - 1] == ')' || numero[numero.length() - 1] == ',')) {
                            numero = numero.substr(0, numero.length() - 1);
                        }

                        if (!tipo.empty()) tokens.push_back(tipo);
                        if (!numero.empty()) tokens.push_back(numero);
                        continue;
                    }

                    if (palabra[palabra.length() - 1] == ')' || palabra[palabra.length() - 1] == ',') {
                        palabra = palabra.substr(0, palabra.length() - 1);
                    }

                    if (!palabra.empty()) {
                        tokens.push_back(palabra);
                    }
                }
            }
        }
        archivo.close();

        cout << "Tokens encontrados:" << endl;
        for (const auto& token : tokens) {
            cout << token << endl;
        }

        cout << "\nTokens en formato array:" << endl;
        cout << "{";
        for (size_t i = 0; i < tokens.size(); i++) {
            cout << "\"" << tokens[i] << "\"";
            if (i < tokens.size() - 1) cout << ", ";
        }
        cout << "}" << endl;
    }

    vector<string> getTokens() {
        return tokens;
    }
};





int main() {
    discodu d(2, 3, 4, 10);

    
    cout << "IDs asignados:\n";
    cout << "Último sector: " << id_sec - 1 << endl;
    cout << "Última pista: " << id_pis - 1 << endl;
    cout << "Última superficie: " << id_sup - 1 << endl;
    cout << "Último disco: " << id_dis - 1 << endl;
    tabla xd;

    return 0;
}


