#ifndef LABIRENT_HPP
#define LABIRENT_HPP

#include "Konum.hpp"
#include "Yigit.hpp"
#include <string>

typedef enum {
    ASAGI, SOL, YUKARI, SAG
} Yon;

class Labirent {
private:
    char** harita;          
    bool** ziyaretEdildi;   // ZİYARET KONTROLÜ İÇİN YENİ EKLENDİ
    int yukseklik;          
    int genislik;           
    
    Konum baslangic;        
    Konum cikis;            
    
    Yigit<Konum> yolYigiti; 

public:
    Labirent(std::string dosyaAdi);
    ~Labirent();

    void haritaOku(std::string dosyaAdi);
    void haritaCiz();
    void coz();
    bool gidilebilirMi(int x, int y);
};

#endif