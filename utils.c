#define _USE_MATH_DEFINES
#include "utils.h"
#include "polygon.h"
#include "solver.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



//----------------------------------------------------------------------------------------
void raporla_ucgen_sonuclari(double* kenarlar, double* acilar, int cozum_sonucu) {

    // --- SONUC RAPORLAMA ---
    if (cozum_sonucu == 1) {
        printf("\nCOZUM BASARILI: Tum eksik veriler bulundu.\n");

        printf("\n--- TAMAMLANMIS UCGEN VERILERI ---\n");

        printf("Ucgenin cevresi: %lf\n", hesapla_ucgen_cevre(kenarlar));
        printf("Ucgenin alani: %lf\n", hesapla_ucgen_alan(kenarlar));
        hesapla_ucgen_kenar_yukseklikleri(kenarlar);

        // Dis ve Ic Yaricaplar
        hesapla_ucgen_ic_teget_cember_yaricap(kenarlar);
        hesapla_ucgen_dis_teget_cember_yaricap(kenarlar, acilar);

        hesapla_ucgen_analiz_trigonometrik_oranlar(acilar);
        hesapla_ucgen_turu(kenarlar, acilar);

        printf("\n============================================================|\n");
        printf("Analiz tamamlandi. Devam etmek icin Enter'a basiniz...");

        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        getchar();

    }
    else if (cozum_sonucu == 2) {
        printf("\nCOZUM BELIRSIZ: Girdiginiz verilerle iki farkli ucgen olusturulabilir.\n");
        printf("Tekil bir cozum olmadigi icin eksik veriler tamamlanmamistir.\n");

        printf("\nDevam etmek icin Enter'a basiniz...");
        fflush(stdin);
        getchar();
    }
    else { // cozum_sonucu == 0
        printf("\nCOZUM BASARISIZ: Verilerinizle gecerli bir ucgen olusturulamadi (Hata kodu 0).\n");

        printf("\nDevam etmek icin Enter'a basiniz...");
        fflush(stdin);
        getchar();
    }
}
//----------------------------------------------------------------------------------------
// Kenarlarin pozitif olup olmadigini kontrol eden fonksiyon
int Kenar_Kontrol_Triangle_Pozitif(double a, double b, double c) {
    // Eger kenarlardan biri sifir veya negatifse, gecerli degil
    return (a > 0 && b > 0 && c > 0);
}
// Bir kenarin uzunlugu diger iki kenarin toplamindan buyuk mu?
int Kenar_Kontrol_Triangle_Uzunluk(double a, double b, double c) {
    return (a < b + c && b < a + c && c < a + b);
}


// ---Eksik Veri Saptamasi---
bool KenarVarmi(double kenar) {
    return kenar != 0;
}

bool AciVarmi(double aci) {
    return aci != 0;
}

bool ikiAciVarmi(double A, double B, double C) {
    return ((A != 0 && B != 0) || (A != 0 && C != 0) || (B != 0 && C != 0));
}

double deg2rad(double x) {
    return x * M_PI / 180.0;
}

double rad2deg(double x) {
    return x * 180.0 / M_PI;
}