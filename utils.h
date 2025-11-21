#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


void raporla_ucgen_sonuclari(double* kenarlar, double* acilar, int cozum_sonucu);

int Kenar_Kontrol_Triangle_Pozitif(double a, double b, double c);
int Kenar_Kontrol_Triangle_Uzunluk(double a, double b, double c);

bool KenarVarmi(double kenar);
bool AciVarmi(double aci);

double deg2rad(double x);
double rad2deg(double x);



#endif // UTILS_H