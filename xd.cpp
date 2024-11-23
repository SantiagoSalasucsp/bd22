#include <iostream>
#include <vector>
using namespace std;

struct disco;
struct superficie;
struct pista;
struct sector;

static vector<sector*> todos_sectores;
static vector<pista*> todas_pistas;
static vector<superficie*> todas_superficies;
static vector<disco*> todos_discos;

struct sector {
    int tamano;
    sector* nextsector;
    vector<int> objeto_ids;
    
    sector(int tam) {
        tamano = tam;
        nextsector = nullptr;
        todos_sectores.push_back(this);
        
    }
};

struct pista {
    vector<sector> sectores;
    pista* nextpista;
    vector<int> objeto_ids;
    
    pista(int numSectores, int tamSector) {
        nextpista = nullptr;
        todas_pistas.push_back(this);
        
        for(int i = 0; i < numSectores; i++) {
            sector xd(tamSector);
            sectores.push_back(xd);
        }
    }
};

struct superficie {
    vector<pista> pistas;
    superficie* nextsuperficie;
    vector<int> objeto_ids;
    
    superficie(int numPistas, int numSectores, int tamSector) {
        nextsuperficie = nullptr;
        todas_superficies.push_back(this);
        
        for(int i = 0; i < numPistas; i++) {
            pista xd(numSectores, tamSector);
            pistas.push_back(xd);
        }
    }
};

struct disco {
    vector<superficie> superficies;
    disco* nextdisco;
    vector<int> objeto_ids;
    
    disco(int numPistas, int numSectores, int tamSector) {
        nextdisco = nullptr;
        todos_discos.push_back(this);
        
        for(int i = 0; i < 2; i++) {
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
        numPistas = nPistas;
        numSectores = nSectores;
        tamSector = tSector;
        
        cout << "Creando dicodu con " << numDiscos << " discos" << endl;
        for(int i = 0; i < numDiscos; i++) {
            disco xd(numPistas, numSectores, tamSector);
            discos.push_back(xd);
        }
        
      
        enlazar_elementos();
    }

    

    // Este método lo puedes usar después para actualizar los punteros
    void actualizar_ultimo_usado(sector* s, pista* p, superficie* sup, disco* d) {
        if(s) ultimo_sector_usado = s;
        if(p) ultima_pista_usada = p;
        if(sup) ultima_superficie_usada = sup;
        if(d) ultimo_disco_usado = d;
    }
};

int main() {
    dicodu d(2, 3, 4, 10);
     
    return 0;
}
