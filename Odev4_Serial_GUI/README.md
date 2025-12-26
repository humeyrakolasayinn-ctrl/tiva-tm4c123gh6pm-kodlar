💻 Ödev 4: Seri Port ve GUI Haberleşmesi
(Serial Port and GUI Communication)
Bu proje, Tiva C Serisi TM4C123G LaunchPad ile SharpDevelop (C#) üzerinden hazırlanan bir masaüstü arayüzü arasında çift yönlü seri haberleşmeyi (UART) kapsamaktadır.

🇹🇷 TÜRKÇE AÇIKLAMA
📋 Proje Özeti
Haberleşme Protokolü: UART0 birimi kullanılarak 9600 Baud hızında iletişim sağlanmıştır.

Masaüstü Arayüzü (GUI): SharpDevelop ortamında C# dili ile geliştirilmiştir.

Fonksiyonlar:

Port Ayarı: Kullanıcı arayüz üzerinden doğru COM portunu seçerek mikrodenetleyici ile bağlantı kurar.

Veri Gönderimi (To Mikro): PC üzerinden mikrodenetleyiciye saat senkronizasyonu (*) ve özel metin mesajları (#) gönderilir.

Veri Alımı (From Mikro): Mikrodenetleyiciden gelen anlık saat verisi, sıcaklık bilgisi ve buton durumu arayüzde gerçek zamanlı olarak görüntülenir.

⚙️ Arayüz Tasarımı (GUI Design)
Aşağıdaki görselde, Tiva C'den gelen verilerin ve PC'den gönderilen komutların yönetildiği arayüz tasarımı görülmektedir:

📄 Haberleşme Mantığı
PC -> Tiva: "Gönder" butonuna basıldığında saat bilgisi paket sonuna * eklenerek, metin bilgisi ise # eklenerek gönderilir.

Tiva -> PC: Mikrodenetleyici saniyede bir kez tüm verileri (T[Saat]S[Sıcaklık]B[Buton]!) formatında paketleyerek PC'ye iletir.

🇺🇸 ENGLISH DESCRIPTION
📋 Project Overview
Communication: Bi-directional serial communication via UART0 at 9600 baud rate.

User Interface: A custom desktop application developed using C# in SharpDevelop.

Key Features:

Connectivity: Allows opening and closing specific serial ports.

Command Transmission: Sends system clock sync (*) and custom strings (#) to the Tiva C board.

Data Monitoring: Displays real-time data received from the Tiva C, including the microcontroller's internal clock, temperature sensor, and on-board button status.

📂 File Structure
main.c: The C code running on the Tiva C microcontroller.

MainForm.cs: The C# source code for the GUI application.

gui_design.png: Screenshot of the interface design.

🚀 How to Use
Connect the Tiva C LaunchPad to your PC.

Select the correct COM port and click "port aç".

Use the "Gönder" buttons to sync the clock or display a message on the LCD.

Monitor the incoming data under the "from the mikro" sections.
