#ifndef KUTUPHANE_H
#define KUTUPHANE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Karakter uzunluk sabitleri
#define AD_BOYUTU      35
#define YAZAR_BOYUTU   25
#define OGR_AD_BOYUT   22
#define SATIR_BOYUTU   256
#define MAX_AD         50
#define MAX_YAZAR      50

// Kitap yapısı
struct Kitap {
    int id;
    char ad[MAX_AD];
    char yazar[MAX_YAZAR];
    int stok;
    struct Kitap *next;
};

// Öğrenci yapısı
struct Ogrenci {
    int id;
    char ad[MAX_AD];
    int kitap_durumu;
    struct Ogrenci *next;
};

// Kuyruk yapısı
struct Kuyruk {
    int ogr_id;
    int kitap_id;
    char ogr_ad[MAX_AD];
    char kitap_ad[MAX_AD];
    struct Kuyruk *next;
};

// Yığın yapısı
struct Yigin {
    int ogr_id;
    int kitap_id;
    char ogr_ad[MAX_AD];
    char kitap_ad[MAX_AD];
    struct Yigin *next;
};

// Global değişkenler
extern struct Kitap *kitap_bas;
extern struct Ogrenci *ogr_bas;
extern struct Kuyruk *kuyruk_ilk;
extern struct Kuyruk *kuyruk_son;
extern struct Yigin *yigin_ust;

// Yardımcı string işlemleri
void satir_trim(char *s);
void ltrim(char *s);
int sayi_mi(const char *p);

// Kitap işlemleri
void kitap_ekle(int id, const char *ad, const char *yazar, int stok);
void* kitap_ara(int id);
void kitap_listele(void);

// Öğrenci işlemleri
void ogrenci_ekle(int id, const char *ad, int durum);
void* ogrenci_ara(int id);
void ogrenci_durum_guncelle(int id, int durum);
void ogrenci_listele(void);

// Kuyruk işlemleri
void kuyruk_ekle(int ogr_id, int kitap_id, const char *ogr_ad, const char *kitap_ad);
void kuyruk_islem(void);
void kuyruk_listele(void);

// Yığın işlemleri
void yigin_ekle(int ogr_id, int kitap_id, const char *ogr_ad, const char *kitap_ad);
void yigin_islem(void);
void yigin_listele(void);

// Dosya işlemleri
void kitap_dosyadan(void);
void ogrenci_dosyadan(void);

// Ana işlem fonksiyonları
void kitap_odunc_al(int ogr_id, int kitap_id);
void kitap_iade_et(int ogr_id, int kitap_id);

// Menü
void menu(void);

#endif
