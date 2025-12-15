#include "Labirent.hpp"
#include <iostream>

using namespace std;

int main() {
    // Harita.txt dosyasını okuyarak Labirent nesnesi oluştur
    Labirent oyun("Harita.txt");
    
    // Çözümü başlat
    oyun.coz();

    // Konsol hemen kapanmasın
    cin.get(); 
    return 0;
}