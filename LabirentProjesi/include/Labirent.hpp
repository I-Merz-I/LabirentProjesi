#ifndef LABIRENT_HPP
#define LABIRENT_HPP

#include "Konum.hpp"
#include "Yigit.hpp"
#include <string>

// Dokümanda belirtildiği gibi sabit boyutlar
#define YUKSEKLIK 20
#define GENISLIK 50

typedef enum {
    ASAGI, SOL, YUKARI, SAG
} Yon;

class Labirent {
private:
    char harita[YUKSEKLIK][GENISLIK];      // Sabit boyutlu dizi
    bool ziyaretEdildi[YUKSEKLIK][GENISLIK]; // Ziyaret kontrolü
    
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