#ifndef YIGIT_HPP
#define YIGIT_HPP

#include <iostream>

// Her bir veriyi tutacak düğüm yapısı
template <typename T>
class Dugum {
public:
    T veri;
    Dugum* sonraki;

    Dugum(T veri) {
        this->veri = veri;
        this->sonraki = NULL;
    }
};

// Yığıt Sınıfı
template <typename T>
class Yigit {
private:
    Dugum<T>* tepe; // Yığıtın en üstünü gösteren işaretçi

public:
    Yigit() {
        tepe = NULL;
    }

    ~Yigit() {
        while (!bosMu()) {
            cikar();
        }
    }

    // Yığıta veri ekleme (Push)
    void ekle(T veri) {
        Dugum<T>* yeniDugum = new Dugum<T>(veri);
        yeniDugum->sonraki = tepe;
        tepe = yeniDugum;
    }

    // Yığıttan veri çıkarma (Pop)
    void cikar() {
        if (!bosMu()) {
            Dugum<T>* silinecek = tepe;
            tepe = tepe->sonraki;
            delete silinecek;
        }
    }

    // En üstteki veriyi getirme (Top/Peek)
    // Hata kontrolü için basit bir mekanizma, boşsa hata verebilir.
    // Kullanmadan önce bosMu kontrolü yapılmalı.
    T getir() {
        if (!bosMu()) {
            return tepe->veri;
        }
        // Boşsa varsayılan değer veya hata fırlatma eklenebilir
        // Ancak bizim kodumuzda bosMu kontrolü ile kullanacağımız için
        // şu anlık sadece tepe verisini dönüyoruz.
        return tepe->veri; 
    }

    // Yığıt boş mu kontrolü
    bool bosMu() {
        return tepe == NULL;
    }
};

#endif