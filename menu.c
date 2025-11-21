#include "utils.h"
#include "polygon.h"
#include "solver.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>








// printf("\n|============================================================================|\n");
// 1 + 76 + 1 = 78 karakter
//--------------------------------------------------------------------------
// Menu Fonksiyonlari
//--------------------------------------------------------------------------
void main_menu_goster() {
    printf("\n/============================================================================\\\n");
    printf("|==========           ShapeAnalyzer 1.0 (Cokgen Analizi)           ==========|\n");
    printf("|============================================================================|\n");
    printf("|==========         Lutfen yapmak istediginiz islemi secin         ==========|\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("|===== 1) Geometrik Analize Basla                                  ==========|\n");
    printf("|===== 2) Surum Notlari / Yenilikler                               ==========|\n");
    printf("|===== 3) Kullanim Kilavuzu / Hakkinda                             ==========|\n");
    printf("|===== 0) Programdan Cikis                                         ==========|\n");
    printf("|============================================================================|\n");
    printf("|===== Seciminiz: ");
}

void menu_1_goster() {
    printf("\n/============================================================================\\\n");
    printf("|==========                  GEOMETRIK ANALIZ (1)                  ==========|\n");
    printf("|============================================================================|\n");
    printf("|===== Analiz etmek istediginiz seklin kenar sayisini girin:                 |\n");
    printf("|===== NOT!: Su an icin sadece 3 kenarli sekiller desteklenmektedir.         |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("|***** >> Ucgen   >> 3 <<                                               *****|\n");
    printf("|***** >> Dortgen >> 4 <<                                               *****|\n");
    printf("|***** >> Besgen  >> 5 <<                                               *****|\n");
    printf("|***** >> Ana Menuye Donmek icin: -1                                    *****|\n");
    printf("|***** >> Programdan Cikmak icin: 0                                     *****|\n");
    printf("|============================================================================|\n");
    printf("| Kenar Sayisi: ");
}

void surum_notlari_goster() {
    printf("\n/============================================================================\\\n");
    printf("|==========            SHAPE ANALYZER SURUM NOTLARI (2)            ==========|\n");
    printf("|============================================================================|\n");
    printf("|==========           ** PROJE CEKIRDEK SURUMU: v1.0 **            ==========|\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| [YENI]: SSS, SAS, ASA/SAA ve SSA algoritmalari eklendi.                    |\n");
    printf("| [YENI]: Alan (Heron), Cevre, Yukseklik (Ha, Hb, Hc) eklendi.               |\n");
    printf("| [YENI]: Ic/dis Teget Cember Yaricaplar  ve Trigonometrik Oranlar eklendi.  |\n");
    printf("| [YENI]: Aci ve Kenar uzunluklarina gore Ucgen Turu Analizi.                |\n");
    printf("| [GELISTIRME]: Moduler (utils/solver) C yapisi uygulandi.                   |\n");
    printf("| [GELISTIRME]: Kullanici dostu Ana Menu akisi eklendi.                      |\n");
    printf("| [DUZELTME]: SSA belirsizlik durumunda veri guncellenmesi engellendi.       |\n");
    printf("|============================================================================|\n");

    
    printf("Devam etmek icin Enter'a basin...");

    getchar();
}

void kullanim_kilavuzu_goster() {
    printf("\n/============================================================================\\\n");
    printf("|==========          SHAPE ANALYZER KULLANIM KILAVUZU (3)          ==========|\n");
    printf("|============================================================================|\n");
    printf("| A) KULLANIM KURALLARI:                                                     |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| 1. BILINMEYEN DEGER: |Bilmediginiz her kenar ve aci icin '0'(sifir)        |\n");
    printf("|                      |girmeniz, programin calismasi icin gerekmektedir.    |\n");
    printf("|----------------------|-----------------------------------------------------|\n");
    printf("| 2. SSA KONTROLU:     |Program, belirsiz (iki olasi) cozumde, veri          |\n");
    printf("|                      |degisikligi yapmaz ve sizi uyarir.                   |\n");
    printf("|----------------------|-----------------------------------------------------|\n");
    printf("| 3. CIKIS/MENU:       |Alt menude '-1' ile Ana Menuye donebilirsiniz.       |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| B) PROGRAM ve GELISTIRICI HAKKINDA:                                        |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| GELISTIRICI: Omer Faruk Arslanturk (Darswamor)                             |\n");
    printf("| UNIVERSITE: Istanbul Universitesi-Cerrahpasa / Makine Muhendisligi 1. Sinif|\n");
    printf("| PROJE AMACI: Trigonometrik ve geometrik algoritmalari                      |\n");
    printf("| (ozellikle SSA belirsizlik yonetimi) C dilinde basariyla uygulayarak,      |\n");
    printf("| guvenilir ve moduler bir muhendislik hesaplama araci gelistirmek.          |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| C) KAPSAMLI TEST SENARYOLARI                                               |\n");
    printf("|----------------------------------------------------------------------------|\n");
    printf("| 1. SSS | Kenarlar: 3,4,5 | Acilar: 0,0,0 |                                 |\n");
    printf("| Amac: Tum Eksik Acilarin Bulunmasi.                                        |\n");
    printf("| Beklenen Sonuc: BASARILI                                                   |\n");
    printf("|                                                                            |\n");
    printf("| 2. SAS | Kenarlar: 12,0,15 | Acilar: 0,30,0 |                              |\n");
    printf("| Amac: Tum Eksik Kenarlar ve Acilarin Bulunmasi.                            |\n");
    printf("| Beklenen Sonuc: BASARILI                                                   |\n");
    printf("|                                                                            |\n");
    printf("| 3. ASA | Kenarlar: 0,20,0 | Acilar: 45,0,75 |                              |\n");
    printf("| Amac: Tum Eksik Kenarlar ve Acilarin Bulunmasi.                            |\n");
    printf("| Beklenen Sonuc: BASARILI                                                   |\n");
    printf("|                                                                            |\n");
    printf("| 4. SAA | Kenarlar: 5,0,0 | Acilar: 60,0,40 |                               |\n");
    printf("| Amac: Tum Eksik Kenarlar ve Acilarin Bulunmasi.                            |\n");
    printf("| Beklenen Sonuc: BASARILI                                                   |\n");
    printf("|                                                                            |\n");
    printf("| 5. SSA | Kenarlar: 15,0,10 | Acilar: 100,0,0 |                             |\n");
    printf("| Amac: Tum Eksik Kenarlar ve Acilarin Bulunmasi.                            |\n");
    printf("| Beklenen Sonuc: BASARILI                                                   |\n");
    printf("|                                                                            |\n");
    printf("| 6. SSA | Kenarlar: 12,0,15 | Acilar: 40,0,0 |                              |\n");
    printf("| Amac: Belirsiz durumun tetiklenmesi ve programin dogru uyari vermesi.      |\n");
    printf("| Beklenen Sonuc: COZUM BELIRSIZ uyarisi. Veriler 0 olarak kalir.            |\n");
    printf("|                                                                            |\n");
    printf("| 7. Gecersiz Giris | Kenarlar 1,2,10 | Acilar: 0,0,0 |                      |\n");
    printf("| Amac: Ucgen Esitsizligi (1+2 != 10) kontrolunun calismasi.                 |\n");
    printf("| Beklenen Sonuc: COZUM BASARISIZ (Hata kodu 0).                             |\n");
    printf("|============================================================================|\n");

    
    printf("Devam etmek icin Enter'a basin...");
    
    getchar();
}