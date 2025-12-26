# 📺 Ödev 1: 16x2 LCD Sürücü Yazımı
**(16x2 LCD Driver Development)**

Bu proje, Tiva C Serisi TM4C123G LaunchPad mikrodenetleyicisi için bir 16x2 Karakter LCD sürücüsünün (driver) temelden geliştirilmesini içermektedir. Hazır kütüphaneler yerine, LCD'nin donanımsal çalışma protokolü kodlanmıştır.

---

## 🇹🇷 TÜRKÇE AÇIKLAMA

### 📋 Proje Özeti
* **Amaç:** LCD ekranın Tiva C ile haberleşmesini sağlayan düşük seviyeli (low-level) sürücü fonksiyonlarını yazmak.
* **Çalışma Modu:** LCD, pin tasarrufu sağlamak amacıyla 4-bit modunda sürülmektedir.

### 🛠 Fonksiyonlar
* **Lcd_init():** LCD'yi başlatır, imleci ve ekranı ayarlar.
* **Lcd_Cmd():** LCD'ye kontrol komutları gönderir (Ekranı temizle, satır değiştir vb.).
* **Lcd_Putch():** Ekrana tek bir karakter yazdırır.
* **Lcd_Print():** Ekrana tam bir metin (string) yazdırır.

### ⚙️ Gereksinimler
* **Donanım:** Tiva C TM4C123G LaunchPad, 16x2 LCD, 10kΩ Potansiyometre.
* **Yazılım:** Code Composer Studio (CCS).

### 📄 Bağlantı Tablosu
| LCD Pin | TM4C123G Pin | Görev |
| :--- | :--- | :--- |
| RS | PB0 | Komut / Veri Seçimi |
| E | PB1 | Enable (Yazma Tetikleyici) |
| D4-D7 | PB4-PB7 | Veri Hatları |
| V0 | Potansiyometre | Kontrast Ayarı |

---

## 🇺🇸 ENGLISH DESCRIPTION

### 📋 Project Overview
* **Objective:** Developing a custom low-level C driver for a 16x2 Character LCD using the Tiva C Series microcontroller.
* **Interface:** The driver operates in 4-bit mode to minimize GPIO usage.

### 📂 File Structure
* `main.c`: Simple test application that displays a welcome message.
* `lcd.c / lcd.h`: The core driver files containing initialization and communication logic.
* `devre_semasi.png`: Fritzing circuit diagram for hardware verification.

---

### 🚀 How to Run
1. Wire the LCD according to the provided table.
2. Compile and flash the code using Code Composer Studio.
3. Adjust the 10k potentiometer to see the text clearly on the screen.











































































































