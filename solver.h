#ifndef SOLVER_H
#define SOLVER_H

#include <stdbool.h>


int Ucgen_Cozum_Yontemleri(double* kenarlar, double* acilar, int kenar_sayisi);
// Uc Kenar (Side-Side-Side)	Kosinus Teoremi
int Ucgen_Cozum_Yontemleri_SSS(double* kenarlar, double* acilar);

// Iki Kenar & Aralarindaki Aci (Side-Angle-Side)	Kosinus Teoremi
int Ucgen_Cozum_Yontemleri_SAS(double* kenarlar, double* acilar);

// Iki Aci & Arasindaki Kenar (Angle-Side-Angle)	Sinus Teoremi
// Iki Aci & Karsisindaki Kenar (Side-Angle-Angle)  Sinus Teoremi
int Ucgen_Cozum_Yontemleri_ASA_SAA(double* kenarlar, double* acilar);

// Iki Kenar & Karsisindaki Aci (Side-Side-Angle)	Sinus Teoremi
int Ucgen_Cozum_Yontemleri_SSA(double* kenarlar, double* acilar);

//-----------------------------------------------------------------------------

double hesapla_ucgen_cevre(double* kenarlar);

double hesapla_ucgen_alan(double* kenarlar);

void hesapla_ucgen_kenar_yukseklikleri(double* kenarlar);

double hesapla_ucgen_ic_teget_cember_yaricap(double* kenarlar);

double hesapla_ucgen_dis_teget_cember_yaricap(double* kenarlar, double* acilar);

void hesapla_ucgen_analiz_trigonometrik_oranlar(double* acilar);

void hesapla_ucgen_turu(double* kenarlar, double* acilar);

#endif // SOLVER_H
