#define _USE_MATH_DEFINES

#include "utils.h"
#include "polygon.h"
#include "solver.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPSILON 0.0001

/*
 * Ucgen_Cozum_Yontemleri: Bilinen kenar ve aci sayisina gore uygun cozum yontemini secer.
 * * Geri Donus Degeri:
 * 1: Tekil Cozum Basarili (Veriler guncellendi).
 * 2: Cift Cozum (SSA Belirsiz Durum - Veri guncellenmedi).
 * 0: Cozum Basarisiz (Yetersiz Veri veya Gecersiz Geometri).
 */
int Ucgen_Cozum_Yontemleri(double* kenarlar, double* acilar, int kenar_sayisi) {
    if (kenar_sayisi != 3) {
        printf("HATA: Bu fonksiyon sadece ucgenler (3 kenar) icin tasarlanmistir.\n");
        return 0;
    }

    double a = kenarlar[0], b = kenarlar[1], c = kenarlar[2];
    double A = acilar[0], B = acilar[1], C = acilar[2];

    int bilinenKenarSayisi = KenarVarmi(a) + KenarVarmi(b) + KenarVarmi(c);
    int bilinenAciSayisi = AciVarmi(A) + AciVarmi(B) + AciVarmi(C);

    printf("\n--- ANALIZ ---\nBilinen Kenar: %d, Bilinen Aci: %d\n", bilinenKenarSayisi, bilinenAciSayisi);

    // En az 3 bilgi (kenar veya aci) olmali
    if (bilinenKenarSayisi + bilinenAciSayisi < 3) {
        printf("HATA: Ucgeni cozmek icin en az 3 bagimsiz bilgi (K-A kombinasyonu) gereklidir.\n");
        return 0;
    }

    // Yetersiz Bilgi Kontrolu: 3 aci biliniyorsa tekil kenar cozulemez
    if (bilinenKenarSayisi == 0 && bilinenAciSayisi == 3) {
        printf("BILGI: Ucgenin sekli belirlendi, ancak kenar uzunlugu belirlenemez (Sadece benzer ucgenler cozulebilir).\n");
        return 0; // Tekil uzunluk cozumu yok.
    }

    // --- 1. SSS (Side-Side-Side) Durumu ---
    if (bilinenKenarSayisi == 3) {
        printf(">>> Durum: SSS (Uc kenar biliniyor)\n");
        return Ucgen_Cozum_Yontemleri_SSS(kenarlar, acilar);
    }

    // --- 2. ASA / SAA (Angle-Side-Angle / Side-Angle-Angle) Durumu ---
    // En az iki aci biliniyorsa, 3. aci da biliniyordur. Sinus Teoremi uygulanir.
    if (bilinenAciSayisi >= 2) {
        printf(">>> Durum: ASA/SAA (Iki aci ve bir/iki kenar biliniyor)\n");
        return Ucgen_Cozum_Yontemleri_ASA_SAA(kenarlar, acilar);
    }

    // --- 3. SAS (Side-Angle-Side) Durumu ---
    // İki kenar ve aralarindaki aci biliniyor. SSA'dan once kontrol edilmelidir.
    if (bilinenKenarSayisi == 2 && bilinenAciSayisi == 1) {
        // Kontrol: Bilinen aci, bilinen iki kenar arasindaki aci mi? (a, b, C / b, c, A / a, c, B)
        if ((KenarVarmi(a) && KenarVarmi(b) && AciVarmi(C)) ||
            (KenarVarmi(b) && KenarVarmi(c) && AciVarmi(A)) ||
            (KenarVarmi(a) && KenarVarmi(c) && AciVarmi(B)))
        {
            printf(">>> Durum: SAS (Iki kenar ve aralarindaki aci biliniyor)\n");
            return Ucgen_Cozum_Yontemleri_SAS(kenarlar, acilar);
        }
    }

    // --- 4. SSA (Side-Side-Angle) Durumu ---
    // Geriye kalan tek 2 Kenar ve 1 Aci kombinasyonu SSA'dir.
    if (bilinenKenarSayisi == 2 && bilinenAciSayisi == 1) {

        // Bu noktada, tum 2K+1A kombinasyonlari SAS'a takilmadiysa, SSA'dir.
        printf(">>> Durum: SSA (Iki kenar ve karsisindaki aci biliniyor - Belirsiz Durum)\n");

        int sonuc = Ucgen_Cozum_Yontemleri_SSA(kenarlar, acilar);

        // SSA'dan gelen 2'yi ana programa ilet.
        if (sonuc == 2) {
            printf("BILGI: SSA cozumu cift sonuclu (Belirsiz). Veri guncellenmedi.\n");
            return 2;
        }
        return sonuc;
    }

    // --- 5. Tanimlanamayan/Yetersiz Veri ---
    printf("HATA: Gecerli bir cozum yontemi tespit edilemedi (Muhtemelen yetersiz bagimsiz veri).\n");
    return 0;
}

// Üç Kenar (Side-Side-Side)	Kosinüs Teoremi
int Ucgen_Cozum_Yontemleri_SSS(double* kenarlar, double* acilar) {

    double a = kenarlar[0];
    double b = kenarlar[1];
    double c = kenarlar[2];

    // 1. Ucgen Olusumu Kontrolu (Triangle Inequality)
    if (a >= b + c || b >= a + c || c >= a + b) {
        printf("HATA: Girdiginiz kenar uzunluklari ile ucgen olusturulamaz (Ucgen Esitsizligi).\n");
        return 0;
    }

    // 2. Kosinus Teoremi ile Acilari Hesaplama (Radyan Cinsinden)

    // A Acisi (a'nin karsisi)
    double cosA = (b * b + c * c - a * a) / (2 * b * c);
    double A_radyan = acos(cosA);

    // B Acisi (b'nin karsisi)
    double cosB = (a * a + c * c - b * b) / (2 * a * c);
    double B_radyan = acos(cosB);

    // C Acisi (c'nin karsisi) - 180 derece (π radyan) kuralı ile
    double C_radyan = M_PI - A_radyan - B_radyan;


    // 3. Sonuclari Dereceye Cevirip Ana Dizilere Kaydetme (A, B, C aci degerleri)

    acilar[0] = rad2deg(A_radyan); // A acisi
    acilar[1] = rad2deg(B_radyan); // B acisi
    acilar[2] = rad2deg(C_radyan); // C acisi

    printf("COZUM: SSS - A = %lf, B = %lf, C = %lf derecedir.\n", acilar[0], acilar[1], acilar[2]);

    return 1;

}

// İki Kenar & Aralarındaki Açı (Side-Angle-Side)	Kosinüs Teoremi
int Ucgen_Cozum_Yontemleri_SAS(double* kenarlar, double* acilar) {

    double a = kenarlar[0], b = kenarlar[1], c = kenarlar[2];
    double A = acilar[0], B = acilar[1], C = acilar[2];

    double bilinmeyen_kenar;

    // 1. Durum: a, c ve B biliniyor (b bilinmiyor)
    if (KenarVarmi(a) && KenarVarmi(c) && acilar[1] > 0 && !KenarVarmi(b)) {
        printf(">>> SAS Durumu: a, c ve B biliniyor.\n");
        // Kosinüs Teoremi ile 'b' kenarini bul: b^2 = a^2 + c^2 - 2ac * cos(B)
        double B_radyan = deg2rad(B);
        bilinmeyen_kenar = sqrt(a * a + c * c - 2 * a * c * cos(B_radyan));
        b = bilinmeyen_kenar;
        kenarlar[1] = b;
    }

    // 2. Durum: b, c ve A biliniyor (a bilinmiyor)
    else if (KenarVarmi(b) && KenarVarmi(c) && acilar[0] > 0 && !KenarVarmi(a)) {
        printf(">>> SAS Durumu: b, c ve A biliniyor.\n");
        // Kosinüs Teoremi ile 'a' kenarini bul: a^2 = b^2 + c^2 - 2bc * cos(A)
        double A_radyan = deg2rad(A);
        bilinmeyen_kenar = sqrt(b * b + c * c - 2 * b * c * cos(A_radyan));
        a = bilinmeyen_kenar;
        kenarlar[0] = a;
    }

    // 3. Durum: a, b ve C biliniyor (c bilinmiyor)
    else if (KenarVarmi(a) && KenarVarmi(b) && acilar[2] > 0 && !KenarVarmi(c)) {
        printf(">>> SAS Durumu: a, b ve C biliniyor.\n");
        // Kosinüs Teoremi ile 'c' kenarini bul: c^2 = a^2 + b^2 - 2ab * cos(C)
        double C_radyan = deg2rad(C);
        bilinmeyen_kenar = sqrt(a * a + b * b - 2 * a * b * cos(C_radyan));
        c = bilinmeyen_kenar;
        kenarlar[2] = c;

    }
    else {
        printf("HATA: Gecersiz SAS kombinasyonu veya eksik veri.\n");
        return 0;
    }

    if (!AciVarmi(A)) {
        double cosA = (b * b + c * c - a * a) / (2 * b * c);
        double A_radyan = acos(cosA);
        A = rad2deg(A_radyan);
    }

    // B Açısı (b'nin karşısı) - Eðer B bilinmiyorsa hesapla, biliniyorsa geç
    if (!AciVarmi(B)) {
        double cosB = (a * a + c * c - b * b) / (2 * a * c);
        double B_radyan = acos(cosB);
        B = rad2deg(B_radyan);
    }

    // B Açısı (b'nin karşısı) - Eðer B bilinmiyorsa hesapla, biliniyorsa geç
    if (!AciVarmi(C)) {
        double cosC = (a * a + b * b - c * c) / (2 * a * b);
        double C_radyan = acos(cosC);
        C = rad2deg(C_radyan);
    }


    acilar[0] = A;
    acilar[1] = B;
    acilar[2] = C;

    printf("COZUM: SAS - a=%lf, b=%lf, c=%lf. A=%lf, B=%lf, C=%lf.\n", kenarlar[0], kenarlar[1], kenarlar[2], acilar[0], acilar[1], acilar[2]);
    return 1; // Başarılı çözüm

}

// İki Açı & Arasındaki Kenar (Angle-Side-Angle)	Sinüs Teoremi
// İki Aci & Karşısındaki Kenar (Side-Angle-Angle)  Sinus Teoremi
int Ucgen_Cozum_Yontemleri_ASA_SAA(double* kenarlar, double* acilar) {

    double a = kenarlar[0], b = kenarlar[1], c = kenarlar[2];
    double A = acilar[0], B = acilar[1], C = acilar[2];


    // 1. Eksik Aciyi Bulma

    // Acilarin toplami kontrolu
    double aci_toplami = A + B + C;

    // Eğer tüm açılar biliniyorsa ve toplamı 180 degilse, kullanıcı hatalı veri girmiştir.
    if (AciVarmi(A) && AciVarmi(B) && AciVarmi(C) && fabs(aci_toplami - 180.0) > 0.001) {
        printf("HATA: Bilinen acilarin toplami 180 derece degildir (%lf).\n", aci_toplami);
        return 0;
    }

    // Eksik olan 1 aciyi bul
    if (!AciVarmi(A)) A = 180.0 - B - C;
    if (!AciVarmi(B)) B = 180.0 - A - C;
    if (!AciVarmi(C)) C = 180.0 - A - B;

    // Güncel açilari diziye kaydet
    acilar[0] = A;
    acilar[1] = B;
    acilar[2] = C;

    // Açıların toplamı 180'den büyükse (Negatif kenar/açı olmasını engellemek için)
    if (A <= 0 || B <= 0 || C <= 0) {
        printf("HATA: Hesaplanan aci sifir veya negatiftir.\n");
        return 0;
    }

    // 2. Sinüs Teoremi ile Eksik Kenarları Bulma

    // Once bilinen bir kenar/aci cifti bulmamiz gerekiyor ki Sinus Teoremi'ndeki orani bulalim.
    double bilinen_kenar, bilinen_aci_radyan;

    if (KenarVarmi(a)) {
        bilinen_kenar = a;
        bilinen_aci_radyan = deg2rad(A);
    }
    else if (KenarVarmi(b)) {
        bilinen_kenar = b;
        bilinen_aci_radyan = deg2rad(B);
    }
    else if (KenarVarmi(c)) {
        bilinen_kenar = c;
        bilinen_aci_radyan = deg2rad(C);
    }
    else {
        // Bu durum olmamali, cunku en az 1 kenar biliniyor olmaliydi (ASA/SAA tanimi).
        printf("HATA: Kenar bilgisi eksik.\n");
        return 0;
    }

    // Sinüs Teoremi Oranı (a / sin(A) = b / sin(B) = c / sin(C))
    double oran = bilinen_kenar / sin(bilinen_aci_radyan);

    // Eksik kenarları bul
    if (!KenarVarmi(a)) {
        a = oran * sin(deg2rad(A));
        kenarlar[0] = a;
    }
    if (!KenarVarmi(b)) {
        b = oran * sin(deg2rad(B));
        kenarlar[1] = b;
    }
    if (!KenarVarmi(c)) {
        c = oran * sin(deg2rad(C));
        kenarlar[2] = c;
    }

    printf("COZUM: ASA/SAA - a=%lf, b=%lf, c=%lf. A=%lf, B=%lf, C=%lf.\n", kenarlar[0], kenarlar[1], kenarlar[2], acilar[0], acilar[1], acilar[2]);
    return 1;

}

/*
 * SSA cozumunu gerceklestiren ana algoritma
 * (k1, k2: bilinen kenarlar; A1_deg: k1'in karsisindaki aci)
 * Kalan acilari ve kenari hesaplar.
 */
int Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(double k1, double k2, double A1_deg, double* sonuc_k3, double* sonuc_A2, double* sonuc_A3) {

    double A1_rad = deg2rad(A1_deg);
    double h = k2 * sin(A1_rad);

    // Yukseklik h: k2 * sin(A1)

    // --- Durum 0: Ucgen Olusmaz ---
    if (k1 < h) {
        printf("HATA: Ucgen olusturulamaz (karsi kenar k1 < h).\n");
        return 0;
    }

    // Genis/Dik Aci Kontrolu
    if (A1_deg >= 90.0 && k1 <= k2) {
        printf("HATA: Genis/Dik acinin karsisindaki kenar en uzun olmali. Ucgen olusmaz.\n");
        return 0;
    }

    // --- Cozum: Sinus Teoremi ile bilinmeyen A2 acisini hesapla ---
    // sin(A2) / k2 = sin(A1) / k1
    double sin_A2 = (k2 * sin(A1_rad)) / k1;

    // sin_A2 degeri 1.0'dan buyuk olmamalı (k1 < h'den sonra bu genelde garantidir)
    if (fabs(sin_A2) > 1.0) sin_A2 = 1.0;

    // A2'nin dar acisini bul (asin her zaman (-90, 90) arasi verir)
    double A2_rad_1 = asin(sin_A2);
    double A2_deg_1 = rad2deg(A2_rad_1);

    // --- Birinci Ucgenin Cozumu (A2 dar aci) ---
    double A3_deg_1 = 180.0 - A1_deg - A2_deg_1;
    double A3_rad_1 = deg2rad(A3_deg_1);

    // Kalan kenar k3'u Sinus Teoremi ile hesapla
    double k3_1 = (k1 * sin(A3_rad_1)) / sin(A1_rad);

    // --- Durum 3: Belirsiz Durum (Iki Ucgen) ---
    // A1 dar aci OLMALI (A1 < 90), k1 (karsi kenar) > h ve k1 < k2 OLMALI.
    if (A1_deg < 90.0 && k1 > h && k1 < k2) {
        printf("BILGI: Iki farkli ucgen olusur (Belirsiz Durum).\n");

        // **NOT:** Programin amaci eksik veriyi tamamlamak, bu yuzden cift cozumde veri *DEGISTIRILMEMELI*.
        // Ancak bu fonksiyon, ilk cozumu donduruyor ve '2' ile uyariyor.
        return 2;
    }

    // --- 4. Adim: TEK COZUM ISE SONUCLARI ATA ve return 1 ---
    // Eger buraya gelindiyse, tek cozum oldugu kesindir (k1=h veya k1 >= k2).

    *sonuc_A2 = A2_deg_1;
    *sonuc_A3 = A3_deg_1;
    *sonuc_k3 = k3_1;

    printf("COZUM: Tekil SSA cozumu bulundu.\n");
    return 1;
}



// İki Kenar & Karsisindaki Aci (Side-Side-Angle) Sinus Teoremi
int Ucgen_Cozum_Yontemleri_SSA(double* kenarlar, double* acilar) {

    // a = kenarlar[0], b = kenarlar[1], c = kenarlar[2];
    // A = acilar[0], B = acilar[1], C = acilar[2];

    // --- Onemli Not: Fonksiyonda yerel a, b, c, A, B, C degerlerini kullanmayi birakiyoruz. ---

    // Durum 1: a, b biliniyor, A acisi biliniyor (k1=a, k2=b, A1=A). Bilinmeyenler: c, B, C
    if (AciVarmi(acilar[0]) && KenarVarmi(kenarlar[0]) && KenarVarmi(kenarlar[1])) {
        // k1=kenarlar[0], k2=kenarlar[1], A1=acilar[0]. Sonuclar: c, C, B
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[0], kenarlar[1], acilar[0],
            &kenarlar[2], &acilar[2], &acilar[1] // &c, &C, &B adresleri
        );
    }
    // Durum 2: a, c biliniyor, A acisi biliniyor (k1=a, k2=c, A1=A). Bilinmeyenler: b, C, B
    else if (AciVarmi(acilar[0]) && KenarVarmi(kenarlar[0]) && KenarVarmi(kenarlar[2])) {
        // k1=kenarlar[0], k2=kenarlar[2], A1=acilar[0]. Sonuclar: b, C, B
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[0], kenarlar[2], acilar[0],
            &kenarlar[1], &acilar[2], &acilar[1] // &b, &C, &B adresleri
        );
    }
    // Durum 3: b, c biliniyor, B acisi biliniyor (k1=b, k2=c, A1=B). Bilinmeyenler: a, A, C
    else if (AciVarmi(acilar[1]) && KenarVarmi(kenarlar[1]) && KenarVarmi(kenarlar[2])) {
        // k1=kenarlar[1], k2=kenarlar[2], A1=acilar[1]. Sonuclar: a, C, A
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[1], kenarlar[2], acilar[1],
            &kenarlar[0], &acilar[2], &acilar[0] // &a, &C, &A adresleri
        );
    }
    // Durum 4: b, a biliniyor, B acisi biliniyor (k1=b, k2=a, A1=B). Bilinmeyenler: c, A, C
    else if (AciVarmi(acilar[1]) && KenarVarmi(kenarlar[1]) && KenarVarmi(kenarlar[0])) {
        // k1=kenarlar[1], k2=kenarlar[0], A1=acilar[1]. Sonuclar: c, A, C
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[1], kenarlar[0], acilar[1],
            &kenarlar[2], &acilar[0], &acilar[2] // &c, &A, &C adresleri
        );
    }
    // Durum 5: c, a biliniyor, C acisi biliniyor (k1=c, k2=a, A1=C). Bilinmeyenler: b, A, B
    else if (AciVarmi(acilar[2]) && KenarVarmi(kenarlar[2]) && KenarVarmi(kenarlar[0])) {
        // k1=kenarlar[2], k2=kenarlar[0], A1=acilar[2]. Sonuclar: b, A, B
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[2], kenarlar[0], acilar[2],
            &kenarlar[1], &acilar[0], &acilar[1] // &b, &A, &B adresleri
        );
    }
    // Durum 6: c, b biliniyor, C acisi biliniyor (k1=c, k2=b, A1=C). Bilinmeyenler: a, A, B
    else if (AciVarmi(acilar[2]) && KenarVarmi(kenarlar[2]) && KenarVarmi(kenarlar[1])) {
        // k1=kenarlar[2], k2=kenarlar[1], A1=acilar[2]. Sonuclar: a, A, B
        return Ucgen_Cozum_Yontemleri_SSA_Ana_Cozucu(
            kenarlar[2], kenarlar[1], acilar[2],
            &kenarlar[0], &acilar[0], &acilar[1] // &a, &A, &B adresleri
        );
    }

    printf("HATA: Gecerli SSA verisi tespit edilemedi.\n");
    return 0;
}





//----------------------------------------------------------------------------------------------------------------





double hesapla_ucgen_cevre(double* kenarlar) {
    return (kenarlar[0] + kenarlar[1] + kenarlar[2]);
}

double hesapla_ucgen_alan(double* kenarlar) {
    double s = (kenarlar[0] + kenarlar[1] + kenarlar[2]) / 2;
    return (sqrt(s * (s - kenarlar[0]) * (s - kenarlar[1]) * (s - kenarlar[2])));
}

void hesapla_ucgen_kenar_yukseklikleri(double* kenarlar) {

    double Alan = hesapla_ucgen_alan(kenarlar);

    if (Alan <= 0.0) {
        printf("BILGI: Yukseklikler hesaplanamadi (Alan sifir veya gecersiz).\n");
        return;
    }

    printf("\n----- YUKSEKLIK ANALIZI -----\n");
    printf("Kenar a'ya ait yukseklik (Ha): %lf\n", ((2.0 * Alan) / kenarlar[0]));
    printf("Kenar b'ye ait yukseklik (Hb): %lf\n", ((2.0 * Alan) / kenarlar[1]));
    printf("Kenar c'ye ait yukseklik (Hc): %lf\n", ((2.0 * Alan) / kenarlar[2]));
}

double hesapla_ucgen_ic_teget_cember_yaricap(double* kenarlar) {

    double alan = hesapla_ucgen_alan(kenarlar);
    double cevre = hesapla_ucgen_cevre(kenarlar);

    double yari_cevre = cevre / 2.0;

    if (alan <= 0.0 || yari_cevre <= 0.0) {
        return 0.0;
    }
    return (alan / yari_cevre);
}

double hesapla_ucgen_dis_teget_cember_yaricap(double* kenarlar, double* acilar) {

    double a = kenarlar[0];
    double A_deg = acilar[0];

    double A_rad = deg2rad(A_deg);
    double sin_A = sin(A_rad);

    if (fabs(sin_A) < 0.0001 || a <= 0.0) {
        return 0.0;
    }

    double R = a / (2.0 * sin_A);
    return R;
}

void hesapla_ucgen_analiz_trigonometrik_oranlar(double* acilar) {

    printf("\n--- TRIGONOMETRIK ORANLAR ANALIZI ---\n");
    for (int i = 0; i < 3; i++) {
        double aci_deg = acilar[i];
        double aci_rad = deg2rad(aci_deg);
        char aci_adi = 'A' + i; // A, B, C adlandırması için

        printf("\n[%c = %.2f Derece]\n", aci_adi, aci_deg);

        // Sinüs ve Kosinüs hesaplamaları
        double sin_val = sin(aci_rad);
        double cos_val = cos(aci_rad);

        printf("  Sinus (sin):   %lf\n", sin_val);
        printf("  Kosinus (cos): %lf\n", cos_val);

        // Tanjant hesaplamasi (cos 90 veya 270'de tanimsiz oldugu icin kontrol)
        // Cosinus degeri 0'a çok yakinsa (90 veya 270 derece) tanjanti tanimsiz kabul et.
        if (fabs(cos_val) < 0.0001) {
            printf("  Tanjant (tan): TANIMSIZ (Aci 90 derece ya da 90 dereceye yakin)\n");
        }
        else {
            printf("  Tanjant (tan): %lf\n", sin_val / cos_val);
        }
    }
}

void hesapla_ucgen_turu(double* kenarlar, double* acilar) {

    double a = kenarlar[0], b = kenarlar[1], c = kenarlar[2];
    double A = acilar[0], B = acilar[1], C = acilar[2];

    printf("\n--- UCGEN TURU ANALIZI ---\n");

    // --- 1. KENARLARINA GÖRE SINIFLANDIRMA ---
    printf("Kenarlarina Gore: ");

    bool eskenar = (fabs(a - b) < EPSILON && fabs(b - c) < EPSILON);
    bool ikizkenar = (fabs(a - b) < EPSILON || fabs(b - c) < EPSILON || fabs(a - c) < EPSILON);

    if (eskenar) {
        printf("Eskenar Ucgen (a = b = c)\n");
    }
    else if (ikizkenar) {
        printf("Ikizkenar Ucgen (En az iki kenar esit)\n");
    }
    else {
        printf("Cesitkenar Ucgen (Tum kenarlar farkli)\n");
    }

    // --- 2. AÇILARINA GÖRE SINIFLANDIRMA ---
    printf("Acilarina Gore: ");

    bool dik_aci = (fabs(A - 90.0) < EPSILON || fabs(B - 90.0) < EPSILON || fabs(C - 90.0) < EPSILON);

    bool genis_aci = (A > 90.0 + EPSILON || B > 90.0 + EPSILON || C > 90.0 + EPSILON);

    if (dik_aci) {
        printf("Dik Acili Ucgen  (Bir aci 90)\n");
    }
    else if (genis_aci) {
        printf("Genis Acili Ucgen (Bir aci > 90)\n");
    }
    else {
        printf("Dar Acili Ucgen (Tum acilar < 90)\n");
    }
}


