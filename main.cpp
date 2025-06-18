#include "kutuphane.h"

// Global değişkenlerin tanımlanması
struct Kitap *kitap_bas = NULL;
struct Ogrenci *ogr_bas = NULL;
struct Kuyruk *kuyruk_ilk = NULL;
struct Kuyruk *kuyruk_son = NULL;
struct Yigin *yigin_ust = NULL;

void satir_trim(char *s)//satır sonundaki \r ve \n karakterlerini silmek için oluşturdum
{
    size_t l = strlen(s);
    while (l && (s[l - 1] == '\n' || s[l - 1] == '\r'))
        s[--l] = '\0';
}

void ltrim(char *s)//satır başındaki boşluk karakterlerini silmek için oluşturdum
{
    while (*s && isspace((unsigned char)*s))
        memmove(s, s + 1, strlen(s));
}
//verilen karakter dizisinin tam sayı olup olmadığını kontrol etmek için oluşturdum
int sayi_mi(const char *p)
{
    if (!p || !*p) return 0;
    while (*p) if (!isdigit((unsigned char)*p++)) return 0;
    return 1;
}



//yeni kitap oluşturmak ve listeye eklemek için oluşturdum
void kitap_ekle(int id, const char *ad, const char *yazar, int stok)
{
    struct Kitap *yeni = (struct Kitap*)malloc(sizeof(struct Kitap));
    if (!yeni) return;
    
    yeni->id = id;
    strncpy(yeni->ad, ad, AD_BOYUTU - 1);
    yeni->ad[AD_BOYUTU - 1] = '\0';
    strncpy(yeni->yazar, yazar, YAZAR_BOYUTU - 1);
    yeni->yazar[YAZAR_BOYUTU - 1] = '\0';
    yeni->stok = stok;
    yeni->next = kitap_bas;
    kitap_bas = yeni;
}
//tüm kitapları ekrana listelr
void kitap_listele(void)
{
    struct Kitap *t;
    printf("\nKitap Listesi:\n");
    printf("ID\tAd\t\tYazar\t\tStok\n");
    printf("----------------------------------------\n");
    for (t = kitap_bas; t != NULL; t = t->next) {
        printf("%d\t%-15s\t%-15s\t%d\n", t->id, t->ad, t->yazar, t->stok);
    }
}
//Belirtilen id ye sahip kitabı arar 
void* kitap_ara(int id)
{
    struct Kitap *t;
    for (t = kitap_bas; t != NULL; t = t->next) {
        if (t->id == id) return (void*)t;
    }
    return NULL;
}


//yeni öğrenci oluişturmak ve listeye eklemek için oluşturdum
void ogrenci_ekle(int id, const char *ad, int durum)
{
    struct Ogrenci *yeni = (struct Ogrenci*)malloc(sizeof(struct Ogrenci));
    if (!yeni) return;
    
    yeni->id = id;
    strncpy(yeni->ad, ad, OGR_AD_BOYUT - 1);
    yeni->ad[OGR_AD_BOYUT - 1] = '\0';
    yeni->kitap_durumu = durum;
    yeni->next = ogr_bas;
    ogr_bas = yeni;
}

//Belirtilen id ye sahip öğrenciyi arar
void* ogrenci_ara(int id)
{
    struct Ogrenci *t;
    for (t = ogr_bas; t != NULL; t = t->next) {
        if (t->id == id) return (void*)t;
    }
    return NULL;
}

//Belirli bir öğrencinin kitap durumunu günceller
void ogrenci_durum_guncelle(int id, int durum)
{
    struct Ogrenci *o = (struct Ogrenci*)ogrenci_ara(id);
    if (o) o->kitap_durumu = durum;
}

//kayıtlı tüm öğrencileri listelerr
void ogrenci_listele(void)
{
    struct Ogrenci *t;
    printf("\nogrenci Listesi:\n");
    printf("ID\tAd\t\tKitap Durumu\n");
    printf("----------------------------------------\n");
    for (t = ogr_bas; t != NULL; t = t->next) {
        printf("%d\t%-15s\t%d\n", t->id, t->ad, t->kitap_durumu);
    }
}

//ödünç alma kuyruguna öğrenci ve kitap bilgilerini ekler
void kuyruk_ekle(int ogr_id, int kitap_id, const char *ogr_ad, const char *kitap_ad)
{
    struct Kuyruk *yeni = (struct Kuyruk*)malloc(sizeof(struct Kuyruk));
    if (!yeni) return;
    
    yeni->ogr_id = ogr_id;
    yeni->kitap_id = kitap_id;
    strncpy(yeni->ogr_ad, ogr_ad, OGR_AD_BOYUT - 1);
    yeni->ogr_ad[OGR_AD_BOYUT - 1] = '\0';
    strncpy(yeni->kitap_ad, kitap_ad, AD_BOYUTU - 1);
    yeni->kitap_ad[AD_BOYUTU - 1] = '\0';
    yeni->next = NULL;
    
    if (!kuyruk_ilk) kuyruk_ilk = yeni;
    else kuyruk_son->next = yeni;
    kuyruk_son = yeni;
}

//kuyruktaki ilk öğrencinin işlemini yapar
void kuyruk_islem(void)
{
    if (!kuyruk_ilk) { puts("Kuyruk bos."); return; }
    
    struct Kuyruk *is = kuyruk_ilk;
    struct Kitap *k = (struct Kitap*)kitap_ara(is->kitap_id);
    
    if (k && k->stok > 0) {
        k->stok--;
        ogrenci_durum_guncelle(is->ogr_id, 1);
        printf("%s '%s' kitabini odunc aldi.\n", is->ogr_ad, is->kitap_ad);
    }
    else {
        puts("Kitap stokta yok!");
    }
    
    kuyruk_ilk = kuyruk_ilk->next;
    if (!kuyruk_ilk) kuyruk_son = NULL;
    free(is);
}

//ödünç alma kuyrugunu ekrana listele 
void kuyruk_listele(void)
{
    struct Kuyruk *t;
    printf("\nKuyruk Listesi:\n");
    printf("ogrenci\t\tKitap\n");
    printf("----------------------------------------\n");
    for (t = kuyruk_ilk; t != NULL; t = t->next) {
        printf("%s\t\t%s\n", t->ogr_ad, t->kitap_ad);
    }
}

//iade edilen kitapları yığına eklemek için oluşturdum
void yigin_ekle(int ogr_id, int kitap_id, const char *ogr_ad, const char *kitap_ad)
{
    struct Yigin *yeni = (struct Yigin*)malloc(sizeof(struct Yigin));
    if (!yeni) return;
    
    yeni->ogr_id = ogr_id;
    yeni->kitap_id = kitap_id;
    strncpy(yeni->ogr_ad, ogr_ad, OGR_AD_BOYUT - 1);
    yeni->ogr_ad[OGR_AD_BOYUT - 1] = '\0';
    strncpy(yeni->kitap_ad, kitap_ad, AD_BOYUTU - 1);
    yeni->kitap_ad[AD_BOYUTU - 1] = '\0';
    yeni->next = yigin_ust;
    yigin_ust = yeni;
}
//iade edilen kitapları yığından silmek için kurdum
void yigin_islem(void)
{
    if (!yigin_ust) { 
        puts("Yigin bos."); 
        return; 
    }
    
    struct Yigin *r = yigin_ust;
    struct Kitap *k = (struct Kitap*)kitap_ara(r->kitap_id);
    struct Ogrenci *o = (struct Ogrenci*)ogrenci_ara(r->ogr_id);
    
    if (k && o) {
        k->stok++;
        o->kitap_durumu = 0;
        printf("%s '%s' kitabini iade etti.\n", r->ogr_ad, r->kitap_ad);
    }
    
    yigin_ust = yigin_ust->next;
    free(r);
    printf("Iade yigini guncellendi.\n");
    yigin_listele();
}

//iade edilen kitapları yığından listelemek için oluşturdum
void yigin_listele(void)
{
    struct Yigin *t;
    printf("\nIade Yigini:\n");
    printf("ogrenci\t\tKitap\n");
    printf("----------------------------------------\n");
    for (t = yigin_ust; t != NULL; t = t->next) {
        printf("%s\t\t%s\n", t->ogr_ad, t->kitap_ad);
    }
}

//öğrencinin kitabını ödünç almak için oluşturdum
void kitap_odunc_al(int ogr_id, int kitap_id)
{
    struct Ogrenci *o = (struct Ogrenci*)ogrenci_ara(ogr_id);
    if (!o) {
        puts("ogrenci bulunamadi!");
        return;
    }
    
    struct Kitap *k = (struct Kitap*)kitap_ara(kitap_id);
    if (!k) {
        puts("Kitap bulunamadi!");
        return;
    }
    
    if (k->stok > 0) {
        k->stok--;
        o->kitap_durumu = 1;
        printf("%s '%s' kitabini odunc aldi.\n", o->ad, k->ad);
        kuyruk_ekle(ogr_id, kitap_id, o->ad, k->ad);
        printf("odunc kuyrugu guncellendi.\n");
        kuyruk_listele();
    }
    else {
        puts("Kitap stokta yok! Kuyruga eklendi.");
        kuyruk_ekle(ogr_id, kitap_id, o->ad, k->ad);
        printf("odunc kuyrugu guncellendi.\n");
        kuyruk_listele();
    }
}
//öğrencinin kitabını iade etmek için oluşturdum
void kitap_iade_et(int ogr_id, int kitap_id)
{
    struct Ogrenci *o = (struct Ogrenci*)ogrenci_ara(ogr_id);
    if (!o) {
        puts("ogrenci bulunamadi!");
        return;
    }
    
    if (!o->kitap_durumu) {
        puts("ogrencinin kitabi yok!");
        return;
    }
    
    struct Kitap *k = (struct Kitap*)kitap_ara(kitap_id);
    if (!k) {
        puts("Kitap bulunamadi!");
        return;
    }
    
    yigin_ekle(ogr_id, kitap_id, o->ad, k->ad);
    printf("Kitap iade yiginina eklendi.\n");
    yigin_listele();
    
    struct Kuyruk *onceki = NULL;
    struct Kuyruk *simdi = kuyruk_ilk;
    
    while (simdi != NULL) {
        if (simdi->ogr_id == ogr_id && simdi->kitap_id == kitap_id) {
            if (onceki == NULL) {
                kuyruk_ilk = simdi->next;
            } else {
                onceki->next = simdi->next;
            }
            if (simdi == kuyruk_son) {
                kuyruk_son = onceki;
            }
            free(simdi);
            printf("odunc kuyrugu guncellendi.\n");
            kuyruk_listele();
            return;
        }
        onceki = simdi;
        simdi = simdi->next;
    }
    

    if (kuyruk_ilk) {
        kuyruk_islem();
    }
}
//dosyasından kitap bilgilerini okur
void kitap_dosyadan(void)
{
    FILE *fp = fopen("kitaplar.txt", "r");
    if (!fp) { perror("kitaplar.txt"); exit(1); }
    char s[SATIR_BOYUTU];
    // İlk satırı atla (başlık satırı)
    fgets(s, sizeof s, fp);
    while (fgets(s, sizeof s, fp)) {
        satir_trim(s);
        if (!*s) continue;
        char *tok = strtok(s, "\t");
        if (!tok || !sayi_mi(tok)) continue;
        int id = atoi(tok);
        char *ad = strtok(NULL, "\t");
        char *yazar = strtok(NULL, "\t");
        char *stokS = strtok(NULL, "\t");
        if (!ad || !yazar || !stokS) continue;
        ltrim(ad); ltrim(yazar); ltrim(stokS);
        kitap_ekle(id, ad, yazar, atoi(stokS));
    }
    fclose(fp);
}

void ogrenci_dosyadan(void)
{
    FILE *fp = fopen("ogrenciler.txt", "r");
    if (!fp) { perror("ogrenciler.txt"); exit(1); }
    char s[SATIR_BOYUTU];
    // İlk satırı atla (başlık satırı)
    fgets(s, sizeof s, fp);
    while (fgets(s, sizeof s, fp)) {
        satir_trim(s);
        if (!*s) continue;
        char *tok = strtok(s, "\t");
        if (!tok || !sayi_mi(tok)) continue;
        int ogr_id = atoi(tok);
        char *ogr_ad = strtok(NULL, "\t");
        char *durumS = strtok(NULL, "\t");
        if (!ogr_ad || !durumS) continue;
        ltrim(ogr_ad); ltrim(durumS);
        ogrenci_ekle(ogr_id, ogr_ad, atoi(durumS));
    }
    fclose(fp);
}

//menü
void menu(void)
{
    int c;
    do {
        puts("\n--- MENU ---");
        puts("1) Tum kitap listesini goster");
        puts("2) odunc alma kuyrugunu goster");
        puts("3) Iade edilen kitaplari goster");
        puts("4) Kitap odunc ver");
        puts("5) Kitap iade al");
        puts("6) cikis");

        printf("Secim: ");
        if (scanf("%d", &c) != 1) { c = 0; }
        while (getchar() != '\n');

        switch (c) {
            case 1: kitap_listele(); break;
            case 2: kuyruk_listele(); break;
            case 3: yigin_listele(); break;
            case 4: {
                int ogr_id, kitap_id;
                printf("ogrenci ID: ");
                scanf("%d", &ogr_id);
                printf("Kitap ID: ");
                scanf("%d", &kitap_id);
                kitap_odunc_al(ogr_id, kitap_id);
                break;
            }
            case 5: {
                int ogr_id, kitap_id;
                printf("ogrenci ID: ");
                scanf("%d", &ogr_id);
                printf("Kitap ID: ");
                scanf("%d", &kitap_id);
                kitap_iade_et(ogr_id, kitap_id);
                break;
            }
            case 6: puts("cikis yapiliyor..."); break;
            default: puts("Gecersiz secim!");
        }
    } while (c != 6);
}

int main(void)
{
    puts("=== KUTUPHANE OTOMASYONU ===");
    puts("Dosyalar yukleniyor...");
    kitap_dosyadan();
    ogrenci_dosyadan();
    puts("Dosyalar yuklendi. Menuye yonlendiriliyorsunuz...");
    menu();
    return 0;
}
