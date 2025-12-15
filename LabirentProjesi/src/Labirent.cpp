#include "Labirent.hpp"
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

// Konsol imlecini belirli konuma götürür
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Konsol imlecini gizler
void imleciGizle() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

Labirent::Labirent(string dosyaAdi) {
    imleciGizle();
    
    // Tüm haritayı boşlukla doldur
    for (int i = 0; i < YUKSEKLIK; i++) {
        for (int j = 0; j < GENISLIK; j++) {
            harita[i][j] = ' ';
            ziyaretEdildi[i][j] = false;
        }
    }
    
    haritaOku(dosyaAdi);
}

Labirent::~Labirent() {
    // Sabit diziler olduğu için hafıza temizliğine gerek yok
}

void Labirent::haritaOku(string dosyaAdi) {
    // Dosyayı aç
    FILE* fp = fopen(dosyaAdi.c_str(), "r");
    
    if (fp == NULL) {
        cout << "Dosya acilamadi!" << endl;
        exit(1);
    }
    
    // Dosyadan satır satır oku
    char satir[100];
    int satirNo = 0;
    bool ilkBoslukBulundu = false;
    
    while (fgets(satir, 100, fp) != NULL && satirNo < YUKSEKLIK) {
        // Satırdaki her karakteri haritaya aktar
        for (int j = 0; j < GENISLIK && satir[j] != '\0' && satir[j] != '\n'; j++) {
            harita[satirNo][j] = satir[j];
            
            // Başlangıç ve çıkış noktalarını bul
            if (satir[j] == ' ') {
                // İlk satır veya sol kenarda boşluk = başlangıç
                if (satirNo == 0 || j == 0) {
                    if (!ilkBoslukBulundu) {
                        baslangic.x = satirNo;
                        baslangic.y = j;
                        ilkBoslukBulundu = true;
                    }
                }
                // Son satır veya sağ kenarda boşluk = çıkış
                if (satirNo == YUKSEKLIK - 1 || j == GENISLIK - 1) {
                    cikis.x = satirNo;
                    cikis.y = j;
                }
            }
        }
        satirNo++;
    }
    
    fclose(fp);
    
    cout << "Baslangic: (" << baslangic.x << ", " << baslangic.y << ")" << endl;
    cout << "Cikis: (" << cikis.x << ", " << cikis.y << ")" << endl;
    cout << "\nLabirent cozuluyor...\n" << endl;
    Sleep(1500);
}

void Labirent::haritaCiz() {
    // İmleci başa al
    gotoxy(0, 0);
    
    // Haritayı yazdır
    for (int i = 0; i < YUKSEKLIK; i++) {
        for (int j = 0; j < GENISLIK; j++) {
            cout << harita[i][j];
        }
        cout << endl;
    }
    
    // Alt bilgi
    cout << "\n[@] Oyuncu  [.] Yol Izi  [x] Cikmaz Yol  [B] Baslangic  [C] Cikis" << endl;
}

bool Labirent::gidilebilirMi(int x, int y) {
    // Sınır kontrolü
    if (x < 0 || x >= YUKSEKLIK || y < 0 || y >= GENISLIK) {
        return false;
    }
    
    // Duvar mı?
    if (harita[x][y] == '#') {
        return false;
    }
    
    // Daha önce ziyaret edildi mi?
    if (ziyaretEdildi[x][y] == true) {
        return false;
    }
    
    return true;
}

void Labirent::coz() {
    // Başlangıç noktasını yığıta ekle
    yolYigiti.ekle(baslangic);
    ziyaretEdildi[baslangic.x][baslangic.y] = true;
    harita[baslangic.x][baslangic.y] = 'B';
    
    Konum suankiKonum;
    Konum oncekiKonum = baslangic;
    
    // Yığıt boş olana kadar devam et
    while (!yolYigiti.bosMu()) {
        // Yığıtın tepesindeki konumu al
        suankiKonum = yolYigiti.getir();
        
        // Önceki konuma iz bırak
        if (oncekiKonum.x != baslangic.x || oncekiKonum.y != baslangic.y) {
            if (harita[oncekiKonum.x][oncekiKonum.y] == '@') {
                harita[oncekiKonum.x][oncekiKonum.y] = '.';
            }
        }
        
        // Şu anki konuma oyuncu karakteri koy
        if (suankiKonum.x != cikis.x || suankiKonum.y != cikis.y) {
            harita[suankiKonum.x][suankiKonum.y] = '@';
        }
        
        // Haritayı çiz
        haritaCiz();
        Sleep(100);
        
        // Çıkışa ulaştık mı?
        if (suankiKonum.x == cikis.x && suankiKonum.y == cikis.y) {
            harita[cikis.x][cikis.y] = 'C';
            haritaCiz();
            cout << "\n>>> CIKISA ULASILDI! <<<\n" << endl;
            return;
        }
        
        bool hareketEttikMi = false;
        
        // 1. AŞAĞI kontrol et
        if (gidilebilirMi(suankiKonum.x + 1, suankiKonum.y)) {
            Konum yeniKonum;
            yeniKonum.x = suankiKonum.x + 1;
            yeniKonum.y = suankiKonum.y;
            
            yolYigiti.ekle(yeniKonum);
            ziyaretEdildi[yeniKonum.x][yeniKonum.y] = true;
            hareketEttikMi = true;
        }
        // 2. SAĞ kontrol et
        else if (gidilebilirMi(suankiKonum.x, suankiKonum.y + 1)) {
            Konum yeniKonum;
            yeniKonum.x = suankiKonum.x;
            yeniKonum.y = suankiKonum.y + 1;
            
            yolYigiti.ekle(yeniKonum);
            ziyaretEdildi[yeniKonum.x][yeniKonum.y] = true;
            hareketEttikMi = true;
        }
        // 3. YUKARI kontrol et
        else if (gidilebilirMi(suankiKonum.x - 1, suankiKonum.y)) {
            Konum yeniKonum;
            yeniKonum.x = suankiKonum.x - 1;
            yeniKonum.y = suankiKonum.y;
            
            yolYigiti.ekle(yeniKonum);
            ziyaretEdildi[yeniKonum.x][yeniKonum.y] = true;
            hareketEttikMi = true;
        }
        // 4. SOL kontrol et
        else if (gidilebilirMi(suankiKonum.x, suankiKonum.y - 1)) {
            Konum yeniKonum;
            yeniKonum.x = suankiKonum.x;
            yeniKonum.y = suankiKonum.y - 1;
            
            yolYigiti.ekle(yeniKonum);
            ziyaretEdildi[yeniKonum.x][yeniKonum.y] = true;
            hareketEttikMi = true;
        }
        
        // Hiçbir yere gidilemediyse (Çıkmaz sokak - BACKTRACKING)
        if (hareketEttikMi == false) {
            // Başlangıç noktası değilse çıkmaz yol işareti koy
            if (suankiKonum.x != baslangic.x || suankiKonum.y != baslangic.y) {
                harita[suankiKonum.x][suankiKonum.y] = 'x';
            }
            // Yığıttan çıkar (geri dön)
            yolYigiti.cikar();
        }
        
        oncekiKonum = suankiKonum;
    }
    
    cout << "\n>>> CIKIS BULUNAMADI! <<<\n" << endl;
}