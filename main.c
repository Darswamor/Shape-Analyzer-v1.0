#define _CRT_SECURE_NO_WARNINGS // (Daha önceki scanf hatasý için)
#define _USE_MATH_DEFINES     // <<< BU SATIRI EKLEYÝN (M_PI için)

#include "utils.h"    // Yardimci fonksiyonlar için
#include "polygon.h"  // Polygon ile ilgili fonksiyonlar için
#include "solver.h"   // Hesaplamalar için
#include "menu.h" 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void baslat_analiz_dongusu();
void kullanicidan_veri_al(int kenar_sayisi, double* kenarlar, double* acilar);

int main() {
    int ana_secim;

    do {
        main_menu_goster();


        if (scanf("%d", &ana_secim) != 1) {

            int temp_c;
            while ((temp_c = getchar()) != '\n' && temp_c != EOF);
            printf("\nHATA: Gecersiz secim. Lutfen 0-3 arasi bir sayi girin.\n");
            continue; // Dongu basina don
        }

        int temp_c;
        while ((temp_c = getchar()) != '\n' && temp_c != EOF);
        // ************************************************************

        switch (ana_secim) {
        case 1:
            // Geometrik Analiz Alt Menusunu ve Cozum Dongusunu Baslat
            baslat_analiz_dongusu();
            break;
        case 2:
            // Surum Notlari
            surum_notlari_goster();
            break;
        case 3:
            // Kullanim Kilavuzu
            kullanim_kilavuzu_goster();
            break;
        case 0:
            printf("\nProgramdan cikiliyor. Iyi gunler!\n");
            return 0; // Programi güvenli bir sekilde sonlandir
        default:
            printf("\nHATA: Gecersiz secim. Lutfen 0-3 arasi bir sayi girin.\n");
            break;
        }

    } while (1); // 0 ile sonlandirildigi icin surekli dongude kalabilir.
}

// ====================================================================
// --- FONKSIYON UYGULAMALARI ---
// ====================================================================

// --- 1. Kullanicidan Kenar ve Aci Verilerini Alan Fonksiyon ---
void kullanicidan_veri_al(int kenar_sayisi, double* kenarlar, double* acilar) {

    // Kenar uzunluklarini al
    for (int i = 0; i < kenar_sayisi; i++) {
        printf("%d. kenarin uzunlugunu girin (bilmiyorsaniz 0): ", i + 1);
        if (scanf("%lf", &kenarlar[i]) != 1) {
            while (getchar() != '\n');
            kenarlar[i] = 0.0;
        }
    }

    // Acilari al
    for (int i = 0; i < kenar_sayisi; i++) {
        printf("%d. aciyi girin (bilmiyorsaniz 0): ", i + 1);
        if (scanf("%lf", &acilar[i]) != 1) {
            while (getchar() != '\n');
            acilar[i] = 0.0;
        }
        // Aci 180'den buyukse veya negatifse uyari verip 0'a cek
        if (acilar[i] > 180.0 || acilar[i] < 0.0) {
            printf("Uyari: Gecersiz aci degeri. Sifir olarak islenecektir.\n");
            acilar[i] = 0.0;
        }
    }
}

// ====================================================================
// --- 2. Geometrik Analiz Dongusunu Baslatan Fonksiyon ---
// Ana Menuden (secenek 1) cagrilir.
// ====================================================================

void baslat_analiz_dongusu() {
    int kenar_sayisi;

    int c; // Menuye geri donmek icin

    do {
        // Analiz alt menusunu gosterir ve giris ister.
        menu_1_goster();

        // Girisi alir ve kontrol eder.
        if (scanf("%d", &kenar_sayisi) != 1) {
            int temp_c;
            while ((temp_c = getchar()) != '\n' && temp_c != EOF);
            printf("HATA: Lutfen gecerli bir tam sayi girin.\n");
            continue;
        }

        // Ana Menuye Donus (-1)
        if (kenar_sayisi == -1) {
            printf("\nAna Menuye donuluyor...\n");
            return;
        }

        // Programdan Cikis (0)
        if (kenar_sayisi == 0) {
            printf("\nProgramdan cikiliyor. Iyi gunler!\n");
            exit(0);
        }

        if (kenar_sayisi < 3) {
            printf("HATA: Analiz icin en az 3 kenar giriniz.\n");
            continue;
        }

        // --- KRITIK ERKEN CIKIS KONTROLU ---
        switch (kenar_sayisi) {
        case 3:
            // Ucgen destekleniyor.
            break;
        case 4:
            printf("\nBILGI: %d kenarli sekiller henuz desteklenmiyor.\n", kenar_sayisi);
            printf("BILGI: (Sonraki guncellemede eklenmesi bekleniyor)\n");
            printf("Devam etmek icin Enter'a basin...");

            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            continue; // (menu_1_goster) don
        case 5:
            printf("\nBILGI: %d kenarli sekiller henuz desteklenmiyor.\n", kenar_sayisi);
            printf("BILGI: (Sonraki guncellemede eklenmesi bekleniyor)\n");
            printf("Devam etmek icin Enter'a basin...");

            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            continue; // (menu_1_goster) don

        default:
            printf("\nHATA: %d kenarli sekiller tanimsiz veya desteklenmiyor.\n", kenar_sayisi);
            printf("Devam etmek icin Enter'a basin...");

            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
            continue; // (menu_1_goster) don
        }

        // --- HAFIZA TAHSISI ---
        // Ileride dortgen ve besgen eklenecegi icin dinamik tahsis gereklidir.
        double* kenarlar = (double*)malloc(kenar_sayisi * sizeof(double));
        double* acilar = (double*)malloc(kenar_sayisi * sizeof(double));

        if (kenarlar == NULL || acilar == NULL) {
            fprintf(stderr, "HATA: Hafiza tahsisi basarisiz!\n");
            if (kenarlar != NULL) free(kenarlar);
            if (acilar != NULL) free(acilar);
            return; // Ana menuye guvenli donus
        }

        printf("\n--- VERI GIRISI: %d Kenarli Sekil ---\n", kenar_sayisi);
        kullanicidan_veri_al(kenar_sayisi, kenarlar, acilar);

        // --- GEOMETRIK COZUM VE RAPORLAMA ---
        int cozum_sonucu = 0; // Varsayilan sonuc

        switch (kenar_sayisi) {
        case 3:
            printf("\n--- UCGEN ANALIZI BASLADI ---\n");
            cozum_sonucu = Ucgen_Cozum_Yontemleri(kenarlar, acilar, kenar_sayisi);

            // Raporlama fonksiyonunu cagir
            raporla_ucgen_sonuclari(kenarlar, acilar, cozum_sonucu);
            break;

        case 4:
            //printf("\n--- DORTGEN ANALIZI BASLADI (v2.0) ---\n");
            printf("\nBILGI: Dortgenler icin cozum algoritmalari henuz gelistirilmedi.\n");
            // cozum_sonucu = Dortgen_Cozum_Yontemleri(kenarlar, acilar, 4);
            // raporla_dortgen_sonuclari(kenarlar, acilar, cozum_sonucu);
            break;

        case 5:
            //printf("\n--- BESGEN ANALIZI BASLADI (v3.0) ---\n");
            printf("\nBILGI: Besgenler icin cozum algoritmalari henuz gelistirilmedi.\n");
            break;

        default:
            printf("\nBILGI: %d kenarli sekiller henuz desteklenmiyor.\n", kenar_sayisi);
            break;
        }

        // --- HAFIZA TEMIZLIGI ---
        free(kenarlar);
        free(acilar);


    } while (1);
}
//---------------------------------------------------------------------------------------------------------------------
