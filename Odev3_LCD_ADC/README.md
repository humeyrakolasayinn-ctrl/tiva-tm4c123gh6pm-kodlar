# 🕒🌡️ Ödev 3: LCD Üzerinde ADC Okuma ve Saat Uygulaması
**(ADC Reading and Digital Clock on 16x2 LCD)**

Bu proje, **Tiva C Serisi TM4C123G LaunchPad** kullanarak analog bir veriyi (potansiyometre/sıcaklık) okumayı ve aynı anda donanımsal kesmelerle (interrupt) çalışan bir saat sayacını **16x2 LCD** ekran üzerinde görüntülemeyi kapsar.

---

## 🇹🇷 TÜRKÇE AÇIKLAMA

### 📋 Proje Özeti
* **ADC Birimi:** Mikrodenetleyicinin **ADC0** modülü kullanılarak **PE3** pininden gelen analog sinyal dijitalleştirilmiştir.
* **Zamanlama (Timer):** Donanımsal **Timer0** kesmesi kullanılarak hassas bir saat (HH:MM:SS) oluşturulmuştur.
* **Ekran Düzeni:**
  * **1. Satır:** "SAAT: HH:MM:SS" formatında zaman bilgisi.
  * **2. Satır:** Okunan ADC değeri veya hesaplanan sıcaklık verisi.
* **Hassasiyet:** 12-bit ADC çözünürlüğü ve 80MHz işlemci hızı ile yüksek doğrulukta veri işleme sağlanmıştır.

### 🛠 Gereksinimler
* **Donanım:** Tiva C TM4C123G, 16x2 LCD, 10kΩ Potansiyometre (ADC girişi için), 10kΩ Potansiyometre (Kontrast için).
* **Yazılım:** Code Composer Studio (CCS), TivaWare Kütüphanesi.

### ⚙️ Bağlantı Şeması
| Bileşen | Tiva C Pin | Görev |
| :--- | :--- | :--- |
| **Potansiyometre** | **PE3 (AIN0)** | Analog Giriş |
| **LCD RS** | PB0 | Komut Seçimi |
| **LCD E** | PB1 | Enable (Tetikleme) |
| **LCD D4-D7** | PB4-PB7 | 4-bit Veri Yolu |



---

## 🇺🇸 ENGLISH DESCRIPTION

### 📋 Project Overview
* **ADC Implementation:** Analog-to-digital conversion is performed via **ADC0** on the **PE3** pin with a 12-bit resolution.
* **Real-Time Clock:** A secondary function tracks time using **Timer Interrupts**, providing an accurate `HH:MM:SS` display.
* **Multi-tasking:** The system simultaneously handles sensor data acquisition and timing updates without performance loss.

### 📄 Project Logic
The main loop checks for two flags: a 1-second timer flag for clock updates and an ADC completion flag. The LCD is updated periodically to display the current time on the first line and the measured analog value on the second line.

### 📂 File Structure
* `main.c`: ADC and Timer interrupt configuration and display logic.
* `lcd.c / lcd.h`: Driver files for character LCD communication.
* `devre_semasi.png`: Visual diagram showing both the LCD and ADC input connections.
