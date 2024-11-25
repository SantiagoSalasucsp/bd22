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
    sector* nextsector;
    vector<int> objeto_ids;
    int ID_sector;

    sector(int tam) {
        tamano = tam;
        nextsector = nullptr;
        ID_sector = id_sec++;
        todos_sectores.push_back(this);
    }
};

struct pista {
    vector<sector> sectores;
    pista* nextpista;
    vector<int> objeto_ids;
    int ID_pista;

    pista(int numSectores, int tamSector) {
        nextpista = nullptr;
        ID_pista = id_pis++;
        todas_pistas.push_back(this);

        for (int i = 0; i < numSectores; i++) {
            sector xd(tamSector);
            sectores.push_back(xd);
        }
    }
};

struct superficie {
    vector<pista> pistas;
    superficie* nextsuperficie;
    vector<int> objeto_ids;
    int ID_superficie;

    superficie(int numPistas, int numSectores, int tamSector) {
        nextsuperficie = nullptr;
        ID_superficie = id_sup++;
        todas_superficies.push_back(this);

        for (int i = 0; i < numPistas; i++) {
            pista xd(numSectores, tamSector);
            pistas.push_back(xd);
        }
    }
};

struct disco {
    vector<superficie> superficies;
    disco* nextdisco;
    vector<int> objeto_ids;
    int ID_disco;

    disco(int numPistas, int numSectores, int tamSector) {
        nextdisco = nullptr;
        ID_disco = id_dis++;
        todos_discos.push_back(this);

        for (int i = 0; i < 2; i++) {
            superficie xd(numPistas, numSectores, tamSector);
            superficies.push_back(xd);
        }
    }
};

struct dicodu {
    vector<disco> discos;
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
        ultimo_sector_usado = todos_sectores[0];
        ultima_pista_usada = todas_pistas[0];
        ultima_superficie_usada = todas_superficies[0];
        ultimo_disco_usado = todos_discos[0];
        
        for(size_t i = 0; i < todos_sectores.size() - 1; i++) {
            todos_sectores[i]->nextsector = todos_sectores[i + 1];
        }
        if (!todos_sectores.empty()) {
            todos_sectores.back()->nextsector = nullptr;
        }


        for(size_t i = 0; i < todas_pistas.size() - 1; i++) {
            todas_pistas[i]->nextpista = todas_pistas[i + 1];
        }
        if (!todas_pistas.empty()) {
            todas_pistas.back()->nextpista = nullptr;
        }


        for(size_t i = 0; i < todas_superficies.size() - 1; i++) {
            todas_superficies[i]->nextsuperficie = todas_superficies[i + 1];
        }
        if (!todas_superficies.empty()) {
            todas_superficies.back()->nextsuperficie = nullptr;
        }


        for(size_t i = 0; i < todos_discos.size() - 1; i++) {
            todos_discos[i]->nextdisco = todos_discos[i + 1];
        }
        if (!todos_discos.empty()) {
            todos_discos.back()->nextdisco = nullptr;
        }


        todos_sectores.clear();
        todas_pistas.clear();
        todas_superficies.clear();
        todos_discos.clear();
    }

    dicodu(int nDiscos, int nPistas, int nSectores, int tSector) {
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
            disco xd(numPistas, numSectores, tamSector);
            discos.push_back(xd);
        }

        enlazar_elementos();
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
    dicodu d(2, 3, 4, 10);

    
    cout << "IDs asignados:\n";
    cout << "Último sector: " << id_sec - 1 << endl;
    cout << "Última pista: " << id_pis - 1 << endl;
    cout << "Última superficie: " << id_sup - 1 << endl;
    cout << "Último disco: " << id_dis - 1 << endl;
    tabla xd;

    return 0;
}


