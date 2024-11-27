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
struct tipo;


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


struct objeto {
    vector<tipo*> tipos;
    vector<tipo*> reserva;
    int tam;
    int objeto_id;
    
    
    objeto(vector<tipo*>& tipos_entrada) {
        
        if (!tipos_entrada.empty()) {
            objeto_id = tipos_entrada[0]->objeto_id;
            tam = 0;
            for (auto* tipo : tipos_entrada) {
                tam += tipo->tam;
            }
            tipos = tipos_entrada;
        }
    }
};





struct sector {
    int tamano;
    sector* next_sector;
    pista* pista_padre;
    vector<int> objeto_ids;
    vector<tipo*> almacenados;
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
    
    sector* main1;

    void enlazar_elementos() {
        if (!todos_sectores.empty()) ultimo_sector_usado = todos_sectores[0];
        if (!todas_pistas.empty()) ultima_pista_usada = todas_pistas[0];
        if (!todas_superficies.empty()) ultima_superficie_usada = todas_superficies[0];
        if (!todos_discos.empty()) ultimo_disco_usado = todos_discos[0];
        main1 = todos_sectores[0];
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
    
    void actualizar_ids_sin_redundancia(vector<int>& vector_ids, int nuevo_id) {
        bool encontrado = false;
        for (int i = 0; i < vector_ids.size(); i++) {
            if (vector_ids[i] == nuevo_id) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            vector_ids.push_back(nuevo_id);
        }
    }

    void insertar(objeto* obj) {
        if (main1 == nullptr) {
            cout << "Error: No hay sectores disponibles" << endl;
            return;
        }

        sector* sector_actual = main1;
        bool ver = true;

        while (!obj->tipos.empty()) {
            tipo* tipo_actual = obj->tipos[0];

            if (sector_actual->tamano >= tipo_actual->tam) {
                sector_actual->almacenados.push_back(tipo_actual);

                if (ver) {
                    actualizar_ids_sin_redundancia(sector_actual->objeto_ids, obj->objeto_id);

                    pista* pista_actual = sector_actual->pista_padre;
                    actualizar_ids_sin_redundancia(pista_actual->objeto_ids, obj->objeto_id);

                    superficie* superficie_actual = pista_actual->superficie_padre;
                    actualizar_ids_sin_redundancia(superficie_actual->objeto_ids, obj->objeto_id);

                    disco* disco_actual = superficie_actual->disco_padre;
                    actualizar_ids_sin_redundancia(disco_actual->objeto_ids, obj->objeto_id);

                    actualizar_ids_sin_redundancia(objeto_ids, obj->objeto_id);

                    ver = false;
                }

                sector_actual->tamano -= tipo_actual->tam;
                obj->tipos.erase(obj->tipos.begin());
            } else {
                obj->reserva.push_back(tipo_actual);
                obj->tipos.erase(obj->tipos.begin());
            }

            // Mover al siguiente sector si el actual se llena
            if (sector_actual->tamano < tipo_actual->tam) {
                if (sector_actual->next_sector != nullptr) {
                    sector_actual = sector_actual->next_sector;
                } else {
                    // Si no hay más sectores, salir del bucle
                    break;
                }
            }
        }

        cout << "Objeto insertado" << endl;
    }
    
    /*
    void actualizar_ids_sin_redundancia(vector<int>& vector_ids, int nuevo_id) {
        bool encontrado = 0;
        for (int i = 0; i < vector_ids.size(); i++) {
            if (vector_ids[i] == nuevo_id) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            vector_ids.push_back(nuevo_id);
        }
    }
    
    
    void insertar(objeto* obj) {
        bool ver=1;
        int x=0;
        if (main1 == nullptr) {
            cout << "Error: No hay sectores disponibles" << endl;
            return;
        }

        
        sector* sector_actual = main1;
        tipo* tipo_actual = obj->tipos[0];
        
        while (!obj->tipos.empty()) {
            

            //tipo* tipo_actual = obj->tipos[x];
            
            if (sector_actual->tamano >= tipo_actual->tam) {
                
                sector_actual->almacenados.push_back(tipo_actual);
                
                
                if(ver){
                    //sector_actual->objeto_ids.push_back(obj->objeto_id);
                    actualizar_ids_sin_redundancia(sector_actual->objeto_ids, obj->objeto_id);
                    
                    pista* pista_actual = sector_actual->pista_padre;
                    actualizar_ids_sin_redundancia(pista_actual->objeto_ids, obj->objeto_id);
                    
                
                    superficie* superficie_actual = pista_actual->superficie_padre;
                    actualizar_ids_sin_redundancia(superficie_actual->objeto_ids, obj->objeto_id);
                    
                    
                    disco* disco_actual = superficie_actual->disco_padre;
                    actualizar_ids_sin_redundancia(disco_actual->objeto_ids, obj->objeto_id);
                    
                    actualizar_ids_sin_redundancia(objeto_ids, obj->objeto_id);

         


                    
                    ver=0;
                }
                sector_actual->tamano-=tipo_actual->tam;
                //sector_actual->objeto_ids.push_back(tipo_actual->objeto_id);
                
                //x++;
                obj->tipos.erase(obj->tipos.begin());
                
                
            }
            else {
                obj->reserva.push_back(tipo_actual);
                obj->tipos.erase(obj->tipos.begin());
                //x++;
            }
            
        }
        objeto xd(obj->reserva);
        main1++;
        insertar(&xd);

        cout << "se ingreso" << endl;
    }*/
    
    
    
    /*
    void buscar(string nombre, int ID_objeto) {
        vector<int> camino;
        //camino.push_back(1);
        
        
        disco* disco_actual = this->discos[0];
        bool encontrado_en_disco = false;
        
        while (disco_actual != nullptr && !encontrado_en_disco) {
            
            for (int i = 0; i < disco_actual->objeto_ids.size(); i++) {
                if (disco_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_disco = 1;
                    break;
                }
            }
            if(encontrado_en_disco==0){
                disco_actual=disco_actual->next_disco;
            }
        }
        if (encontrado_en_disco)
            camino.push_back(disco_actual->ID_disco);
        
        
        
        superficie* superficie_actual = disco_actual->superficies[0];
        bool encontrado_en_superficie = false;
        
        while (superficie_actual != nullptr && !encontrado_en_superficie) {
            for (int i = 0; i < superficie_actual->objeto_ids.size(); i++) {
                if (superficie_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_superficie = true;
                    break;
                }
            }
            if(encontrado_en_superficie==0){
                superficie_actual=superficie_actual->next_superficie;
            }
        }
        
        if(encontrado_en_superficie)
            camino.push_back(superficie_actual->ID_superficie);
        
        
        pista* pista_actual = superficie_actual->pistas[0];
        bool encontrado_en_pista = false;
        
        while (pista_actual != nullptr && !encontrado_en_pista) {
            for (int i = 0; i < pista_actual->objeto_ids.size(); i++) {
                if (pista_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_pista = true;
                    break;
                }
            }
            if(encontrado_en_pista==0){
                pista_actual=pista_actual->next_pista;
            }
        }
        if(encontrado_en_pista)
            camino.push_back(pista_actual->ID_pista);
        
        
        sector* sector_actual = pista_actual->sectores[0];
        bool encontrado_en_sector = false;
        
        while (sector_actual != nullptr && !encontrado_en_sector) {
            for (int i = 0; i < sector_actual->objeto_ids.size(); i++) {
                if (sector_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_sector = true;
                    break;
                }
            }
            if(encontrado_en_sector==0){
                sector_actual=sector_actual->next_sector;
            }
        }
        if(encontrado_en_sector)
            camino.push_back(sector_actual->ID_sector);
        
        for (int i = 0; i < sector_actual->almacenados.size(); i++) {
            tipo* tipo_actual = sector_actual->almacenados[i];
            if (tipo_actual->name == nombre && tipo_actual->objeto_id == ID_objeto) {
                cout << "Objeto encontrado:" << endl;
                cout << "  Discodu: " << camino[0] << endl;
                cout << "  Disco: " << camino[1] << endl;
                cout << "  Superficie: " << camino[2] << endl;
                cout << "  Pista: " << camino[3] << endl;
                cout << "  Sector: " << camino[4] << endl;
                
                tipo_actual->getvalor();
                return;
            }
        }
    }
     
    */
    void buscar(string nombre, int ID_objeto) {
        vector<int> camino;

        disco* disco_actual = this->discos[0];
        bool encontrado_en_disco = false;

        while (disco_actual != nullptr && !encontrado_en_disco) {
            for (int i = 0; i < disco_actual->objeto_ids.size(); i++) {
                if (disco_actual->objeto_ids[i] == ID_objeto) {
                    encontrado_en_disco = true;
                    camino.push_back(disco_actual->ID_disco);
                    break;
                }
            }
            if (!encontrado_en_disco) {
                if (disco_actual->next_disco != nullptr) {
                    disco_actual = disco_actual->next_disco;
                } else {
                    break;
                }
            }
        }

        if (encontrado_en_disco) {
            superficie* superficie_actual = disco_actual->superficies[0];
            bool encontrado_en_superficie = false;

            while (superficie_actual != nullptr && !encontrado_en_superficie) {
                for (int i = 0; i < superficie_actual->objeto_ids.size(); i++) {
                    if (superficie_actual->objeto_ids[i] == ID_objeto) {
                        encontrado_en_superficie = true;
                        camino.push_back(superficie_actual->ID_superficie);
                        break;
                    }
                }
                if (!encontrado_en_superficie) {
                    if (superficie_actual->next_superficie != nullptr) {
                        superficie_actual = superficie_actual->next_superficie;
                    } else {
                        break;
                    }
                }
            }

            if (encontrado_en_superficie) {
                pista* pista_actual = superficie_actual->pistas[0];
                bool encontrado_en_pista = false;

                while (pista_actual != nullptr && !encontrado_en_pista) {
                    for (int i = 0; i < pista_actual->objeto_ids.size(); i++) {
                        if (pista_actual->objeto_ids[i] == ID_objeto) {
                            encontrado_en_pista = true;
                            camino.push_back(pista_actual->ID_pista);
                            break;
                        }
                    }
                    if (!encontrado_en_pista) {
                        if (pista_actual->next_pista != nullptr) {
                            pista_actual = pista_actual->next_pista;
                        } else {
                            break;
                        }
                    }
                }

                if (encontrado_en_pista) {
                    sector* sector_actual = pista_actual->sectores[0];
                    bool encontrado_en_sector = false;

                    while (sector_actual != nullptr && !encontrado_en_sector) {
                        for (int i = 0; i < sector_actual->objeto_ids.size(); i++) {
                            if (sector_actual->objeto_ids[i] == ID_objeto) {
                                encontrado_en_sector = true;
                                camino.push_back(sector_actual->ID_sector);
                                break;
                            }
                        }
                        if (!encontrado_en_sector) {
                            if (sector_actual->next_sector != nullptr) {
                                sector_actual = sector_actual->next_sector;
                            } else {
                                break;
                            }
                        }
                    }

                    if (encontrado_en_sector) {
                        for (int i = 0; i < sector_actual->almacenados.size(); i++) {
                            tipo* tipo_actual = sector_actual->almacenados[i];
                            if (tipo_actual->name == nombre && tipo_actual->objeto_id == ID_objeto) {
                                cout << "Objeto encontrado:" << endl;
                                //cout << "  Discodu: " << camino[0] << endl;
                                cout << "  Disco: " << camino[0] << endl;
                                cout << "  Superficie: " << camino[1] << endl;
                                cout << "  Pista: " << camino[2] << endl;
                                
                                cout << "  Sector: " << camino[3] << endl;
                                tipo_actual->getvalor();
                                return;
                            }
                        }
                    }
                }
            }
        }

        cout << "Objeto no encontrado" << endl;
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
    discodu d(2, 3, 4, 100);
    
    
    vector<tipo*> tipos;
    cout << "IDs asignados:\n";
    cout << "Último sector: " << id_sec - 1 << endl;
    cout << "Última pista: " << id_pis - 1 << endl;
    cout << "Última superficie: " << id_sup - 1 << endl;
    cout << "Último disco: " << id_dis - 1 << endl;
    //tabla xd;
    
    
    //fila 2
    INTEGER* index = new INTEGER(1, 1, 4, "Index");
    VARCHAR* item = new VARCHAR("Fruit of the Loom Gir", 1, 40, "Item");
    DECIMAL* cost = new DECIMAL(7.97, 1, 2, 2, "Cost");
    DECIMAL* tax = new DECIMAL(0.6, 1, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj1 = new objeto(tipos);
    d.insertar(obj1);
    tipos.clear();

    // Fila 3
    index = new INTEGER(2, 2, 4, "Index");
    item = new VARCHAR("Rawlings Little Leag", 2, 40, "Item");
    cost = new DECIMAL(2.97, 2, 2, 2, "Cost");
    tax = new DECIMAL(0.22, 2, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj2 = new objeto(tipos);
    d.insertar(obj2);
    tipos.clear();

    // Fila 4
    index = new INTEGER(3, 3, 4, "Index");
    item = new VARCHAR("Secret Antiperspirant", 3, 40, "Item");
    cost = new DECIMAL(1.29, 3, 2, 2, "Cost");
    tax = new DECIMAL(0.1, 3, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj3 = new objeto(tipos);
    d.insertar(obj3);
    tipos.clear();

    // Fila 5
    index = new INTEGER(4, 4, 4, "Index");
    item = new VARCHAR("Deadpool DVD", 4, 40, "Item");
    cost = new DECIMAL(14.96, 4, 2, 2, "Cost");
    tax = new DECIMAL(1.12, 4, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj4 = new objeto(tipos);
    d.insertar(obj4);
    tipos.clear();

    // Fila 6
    index = new INTEGER(5, 5, 4, "Index");
    item = new VARCHAR("Maxwell House Coffe", 5, 40, "Item");
    cost = new DECIMAL(7.28, 5, 2, 2, "Cost");
    tax = new DECIMAL(0.55, 5, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj5 = new objeto(tipos);
    d.insertar(obj5);
    tipos.clear();

    // Fila 7
    index = new INTEGER(6, 6, 4, "Index");
    item = new VARCHAR("Banana Boat Sunscr", 6, 40, "Item");
    cost = new DECIMAL(6.68, 6, 2, 2, "Cost");
    tax = new DECIMAL(0.5, 6, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj6 = new objeto(tipos);
    d.insertar(obj6);
    tipos.clear();

    // Fila 8
    index = new INTEGER(7, 7, 4, "Index");
    item = new VARCHAR("Wrench Set, 18 piec", 7, 20, "Item");
    cost = new DECIMAL(10.0, 7, 2, 2, "Cost");
    tax = new DECIMAL(0.75, 7, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj7 = new objeto(tipos);
    d.insertar(obj7);
    tipos.clear();

    // Fila 9
    index = new INTEGER(8, 8, 4, "Index");
    item = new VARCHAR("M and M, 42 oz", 8, 20, "Item");
    cost = new DECIMAL(8.98, 8, 2, 2, "Cost");
    tax = new DECIMAL(0.67, 8, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj8 = new objeto(tipos);
    d.insertar(obj8);
    tipos.clear();

    // Fila 10
    index = new INTEGER(9, 9, 4, "Index");
    item = new VARCHAR("Bertoli Alfredo Sauc", 9, 40, "Item");
    cost = new DECIMAL(2.12, 9, 2, 2, "Cost");
    tax = new DECIMAL(0.16, 9, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj9 = new objeto(tipos);
    d.insertar(obj9);
    tipos.clear();

    // Fila 11
    index = new INTEGER(10, 10, 4, "Index");
    item = new VARCHAR("Large Paperclips, 10", 10, 40, "Item");
    cost = new DECIMAL(6.19, 10, 2, 2, "Cost");
    tax = new DECIMAL(0.46, 10, 2, 2, "Tax");
    tipos.push_back(index);
    tipos.push_back(item);
    tipos.push_back(cost);
    tipos.push_back(tax);
    objeto* obj10 = new objeto(tipos);
    d.insertar(obj10);
    tipos.clear();

    // Búsqueda de un objeto
    d.buscar("Item", 1);

    return 0;
}









/*
 void buscar(string nombre, int ID_objeto) {
     vector<int> camino;
     
     // Buscar en discos
     disco* disco_actual = nullptr;
     for(auto disco : discos) {
         for(int id : disco->objeto_ids) {
             if(id == ID_objeto) {
                 disco_actual = disco;
                 camino.push_back(disco->ID_disco);
                 break;
             }
         }
         if(disco_actual) break;
     }
     
     if(!disco_actual) {
         cout << "Objeto no encontrado en discos" << endl;
         return;
     }
     
     // Buscar en superficies
     superficie* superficie_actual = nullptr;
     for(auto superficie : disco_actual->superficies) {
         for(int id : superficie->objeto_ids) {
             if(id == ID_objeto) {
                 superficie_actual = superficie;
                 camino.push_back(superficie->ID_superficie);
                 break;
             }
         }
         if(superficie_actual) break;
     }
     
     if(!superficie_actual) {
         cout << "Objeto no encontrado en superficies" << endl;
         return;
     }
     
     // Buscar en pistas
     pista* pista_actual = nullptr;
     for(auto pista : superficie_actual->pistas) {
         for(int id : pista->objeto_ids) {
             if(id == ID_objeto) {
                 pista_actual = pista;
                 camino.push_back(pista->ID_pista);
                 break;
             }
         }
         if(pista_actual) break;
     }
     
     if(!pista_actual) {
         cout << "Objeto no encontrado en pistas" << endl;
         return;
     }
     
     // Buscar en sectores
     sector* sector_actual = nullptr;
     for(auto sector : pista_actual->sectores) {
         for(int id : sector->objeto_ids) {
             if(id == ID_objeto) {
                 sector_actual = sector;
                 camino.push_back(sector->ID_sector);
                 break;
             }
         }
         if(sector_actual) break;
     }
     
     if(!sector_actual) {
         cout << "Objeto no encontrado en sectores" << endl;
         return;
     }
     
     // Buscar en almacenados
     for(auto tipo_actual : sector_actual->almacenados) {
         if(tipo_actual->name == nombre && tipo_actual->objeto_id == ID_objeto) {
             cout << "Objeto encontrado:" << endl;
             cout << "  Discodu: " << camino[0] << endl;
             cout << "  Disco: " << camino[1] << endl;
             cout << "  Superficie: " << camino[2] << endl;
             cout << "  Pista: " << camino[3] << endl;
             cout << "  Sector: " << camino[4] << endl;
             
             tipo_actual->getvalor();
             return;
         }
     }
     
     cout << "Objeto no encontrado en almacenados" << endl;
 }
 */

