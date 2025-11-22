# Shape-Analyzer
# 📐 ShapeAnalyzer v1.0: C Dilinde Gelişmiş Geometrik Analiz Aracı

[![GitHub top language](https://img.shields.io/github/languages/top/Darswamor/ShapeAnalyzer)](https://github.com/Darswamor/Shape-Analyzer)
[![GitHub license](https://img.shields.io/github/license/Darswamor/ShapeAnalyzer)](https://github.com/Darswamor/Shape-Analyzer/blob/main/LICENSE)

Bu proje, Makine Mühendisliği 1. Sınıf öğrencisi Ömer Faruk Arslantürk tarafından C programlama dili kullanılarak geliştirilmiş, **modüler** ve **algoritmik derinliğe** sahip bir geometrik çözüm aracıdır. Kullanıcıdan alınan eksik kenar ve açı verilerini (0 olarak girilen) güvenilir trigonometrik algoritmalarla tamamlar ve detaylı bir rapor sunar.

## 🌟 Projenin Amacı

Projenin temel amacı, trigonometrik ve geometrik algoritmaları (**özellikle SSA belirsizlik yönetimi**) C dilinde başarıyla uygulayarak, **güvenilir ve modüler bir mühendislik hesaplama aracı** geliştirmektir.

---

## 🚀 Ana Özellikler (v1.0 - Üçgen Çözücü)

| Kategori | Özellik | Uygulanan Teorem/Algoritma |
| :--- | :--- | :--- |
| **Temel Çözüm** | **SSS, SAS, ASA, SAA** | Kosinüs ve Sinüs Teoremleri |
| **Kritik Yönetim**| **SSA Belirsizlik Kontrolü** | Belirsiz durumda (`COZUM BELIRSIZ`), iki olası çözümün önüne geçilerek veri bütünlüğü korunur. |
| **İleri Analiz** | Alan & Çevre | Alan için **Heron Formülü** uygulanır. |
| **Geometrik Analiz**| Yükseklikler ($h_a, h_b, h_c$) | Alan ve kenar bilgileri kullanılarak hesaplanır. |
| **Yarıçaplar** | İç ($r$) ve Dış ($R$) Çemberler | İç ve Dış Teğet Çember yarıçapları hesaplanır. |
| **Sınıflandırma** | Üçgen Türü Analizi | Açı ve kenar uzunluklarına göre **Dik/Geniş Açılı** ve **Eşkenar/İkizkenar** ayrımı yapılır. |

---

## 🛠️ Kurulum ve Derleme

Bu proje, standart GNU GCC Derleyicisi kullanılarak derlenir.

1.  **Gereklilik:** GCC/MinGW (C derleyicisi) sisteminizde kurulu olmalıdır.
2.  **Derleme:** Proje klasöründeyken terminalde aşağıdaki komutu çalıştırın. `-lm` bayrağı, matematik kütüphanesini bağlamak için zorunludur.

    ```bash
    gcc main.c solver.c utils.c polygon.c -o shapeanalyzer -lm
    ```

3.  **Çalıştırma:**
    ```bash
    ./shapeanalyzer
    ```

## 🧪 Örnek Test Senaryoları

Programın algoritmik gücünü test etmek için aşağıdaki kritik senaryoları kullanabilirsiniz. (Programın 'Analiz Alt Menüsü'nden 3 girerek başlayın).

| Test No. | Çözüm Tipi | Kenar a, b, c | Açı A, B, C | Beklenen Sonuç |
| :---: | :--- | :--- | :--- | :--- |
| **1** | **SSS (Dik Üçgen)** | 3, 4, 5 | 0, 0, 0 | `COZUM BASARILI`. Açı C: $\approx 90.00^{\circ}$ |
| **2** | **SAS** | 12, 0, 15 | 0, 30, 0 | `COZUM BASARILI`. Eksik kenar b bulunur. |
| **3** | **SSA (Belirsiz)** | 12, 0, 15 | 40, 0, 0 | **`COZUM BELIRSIZ`**. Veri güncellenmez. |

---

## ⚖️ Lisans

Bu proje **MIT Lisansı** altında lisanslanmıştır. Metin şu şekildedir:

> Copyright (c) 2025 Ömer Faruk Arslantürk

Daha fazla bilgi için `LICENSE` dosyasını inceleyebilirsiniz.

## ➡️ Gelecek Planları (v2.0)

* Dörtgenler (4 kenar) için çevre ve alan hesaplamalarının eklenmesi.
* Genel Analiz döngüsünün `case 4` ve `case 5` bloklarının etkinleştirilmesi.
