#include "Labirent.hpp"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void imleciGizle() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}

// Renkli yazı için yardımcı fonksiyon
void renkliYaz(char karakter, int renk) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, renk);
    cout << karakter;
    SetConsoleTextAttribute(hConsole, 7); // Varsayılan renge dön
}

Labirent::Labirent(string dosyaAdi) {
    this->yukseklik = 0;
    this->genislik = 0;
    this->ziyaretEdildi = NULL;
    imleciGizle();
    haritaOku(dosyaAdi);
}

Labirent::~Labirent() {
    for (int i = 0; i < yukseklik; ++i) {
        delete[] harita[i];
        delete[] ziyaretEdildi[i];
    }
    delete[] harita;
    delete[] ziyaretEdildi;
}

void Labirent::haritaOku(string dosyaAdi) {
    ifstream dosya(dosyaAdi);
    string satir;
    
    if (dosya.is_open()) {
        while (getline(dosya, satir)) {
            yukseklik++;
            if (satir.length() > genislik) {
                genislik = satir.length();
            }
        }
        dosya.clear();
        dosya.seekg(0);
    } else {
        cout << "Dosya acilamadi!" << endl;
        exit(1);
    }

    harita = new char*[yukseklik];
    ziyaretEdildi = new bool*[yukseklik];
    for (int i = 0; i < yukseklik; ++i) {
        harita[i] = new char[genislik];
        ziyaretEdildi[i] = new bool[genislik];
        for (int j = 0; j < genislik; ++j) {
            ziyaretEdildi[i][j] = false;
        }
    }

    int r = 0;
    bool ilkBoslukBulundu = false;
    while (getline(dosya, satir)) {
        for (int c = 0; c < genislik; ++c) {
            if (c < satir.length()) {
                harita[r][c] = satir[c];
                
                if (harita[r][c] == ' ') {
                    if (r == 0 || c == 0) {
                        if (!ilkBoslukBulundu) {
                            baslangic = Konum(r, c);
                            ilkBoslukBulundu = true;
                        }
                    }
                    if (r == yukseklik - 1 || c == genislik - 1) {
                        cikis = Konum(r, c);
                    }
                }
            } else {
                harita[r][c] = '#';
            }
        }
        r++;
    }
    dosya.close();
    
    cout << "Baslangic: (" << baslangic.x << ", " << baslangic.y << ")" << endl;
    cout << "Cikis: (" << cikis.x << ", " << cikis.y << ")" << endl;
    cout << "\nLabirent cozuluyor...\n" << endl;
    Sleep(1500);
}

void Labirent::haritaCiz() {
    gotoxy(0, 0);
    for (int i = 0; i < yukseklik; ++i) {
        for (int j = 0; j < genislik; ++j) {
            char c = harita[i][j];
            
            // Renkli çıktı
            if (c == '@') {
                renkliYaz(c, 14); // Sarı - Oyuncu
            } else if (c == 'B') {
                renkliYaz(c, 10); // Yeşil - Başlangıç
            } else if (c == 'C') {
                renkliYaz(c, 12); // Kırmızı - Çıkış
            } else if (c == '.') {
                renkliYaz(c, 11); // Açık Mavi - İz
            } else if (c == 'x') {
                renkliYaz(c, 8); // Gri - Çıkmaz yol
            } else {
                cout << c;
            }
        }
        cout << endl;
    }
    
    // Alt bilgi
    gotoxy(0, yukseklik + 1);
    cout << "[@] Oyuncu  [.] Yol Izi  [x] Cikmaz Yol  [B] Baslangic  [C] Cikis" << endl;
}

bool Labirent::gidilebilirMi(int x, int y) {
    if (x < 0 || x >= yukseklik || y < 0 || y >= genislik) {
        return false;
    }
    
    if (harita[x][y] == '#') {
        return false;
    }
    
    if (ziyaretEdildi[x][y]) {
        return false;
    }
    
    return true;
}

void Labirent::coz() {
    char yonOklar[] = {31, 17, 30, 16}; // AŞAĞI, SOL, YUKARI, SAĞ
    
    yolYigiti.ekle(baslangic);
    ziyaretEdildi[baslangic.x][baslangic.y] = true;
    harita[baslangic.x][baslangic.y] = 'B';

    Konum mevcut;
    Konum onceki = baslangic;
    bool hareketEtti = false;

    while (!yolYigiti.bosMu()) {
        mevcut = yolYigiti.getir();
        
        // Önceki pozisyonu iz olarak işaretle (başlangıç değilse)
        if (!(onceki.x == baslangic.x && onceki.y == baslangic.y)) {
            if (harita[onceki.x][onceki.y] == '@') {
                harita[onceki.x][onceki.y] = '.'; // İz bırak
            }
        }
        
        // Şu anki pozisyona oyuncu karakterini koy
        if (!(mevcut.x == cikis.x && mevcut.y == cikis.y)) {
            harita[mevcut.x][mevcut.y] = '@'; // Oyuncu
        }
        
        haritaCiz();
        Sleep(100); // Animasyon hızı
        
        // Çıkışa ulaştık mı?
        if (mevcut.x == cikis.x && mevcut.y == cikis.y) {
            harita[cikis.x][cikis.y] = 'C';
            haritaCiz();
            gotoxy(0, yukseklik + 3);
            cout << "\n========================" << endl;
            cout << ">>> CIKISA ULASILDI! <<<" << endl;
            cout << "========================\n" << endl;
            return;
        }
        
        hareketEtti = false;
        
        // AŞAĞI
        if (gidilebilirMi(mevcut.x + 1, mevcut.y)) {
            Konum yeni(mevcut.x + 1, mevcut.y);
            yolYigiti.ekle(yeni);
            ziyaretEdildi[yeni.x][yeni.y] = true;
            hareketEtti = true;
        }
        // SAĞ
        else if (gidilebilirMi(mevcut.x, mevcut.y + 1)) {
            Konum yeni(mevcut.x, mevcut.y + 1);
            yolYigiti.ekle(yeni);
            ziyaretEdildi[yeni.x][yeni.y] = true;
            hareketEtti = true;
        }
        // YUKARI
        else if (gidilebilirMi(mevcut.x - 1, mevcut.y)) {
            Konum yeni(mevcut.x - 1, mevcut.y);
            yolYigiti.ekle(yeni);
            ziyaretEdildi[yeni.x][yeni.y] = true;
            hareketEtti = true;
        }
        // SOL
        else if (gidilebilirMi(mevcut.x, mevcut.y - 1)) {
            Konum yeni(mevcut.x, mevcut.y - 1);
            yolYigiti.ekle(yeni);
            ziyaretEdildi[yeni.x][yeni.y] = true;
            hareketEtti = true;
        }
        
        // BACKTRACK - Çıkmaz yola gelindi
        if (!hareketEtti) {
            // Geri dönerken çıkmaz yol işareti
            if (!(mevcut.x == baslangic.x && mevcut.y == baslangic.y)) {
                harita[mevcut.x][mevcut.y] = 'x';
            }
            yolYigiti.cikar();
        }
        
        onceki = mevcut;
    }

    gotoxy(0, yukseklik + 3);
    cout << "\n==========================" << endl;
    cout << ">>> CIKIS BULUNAMADI! <<<" << endl;
    cout << "==========================\n" << endl;
}