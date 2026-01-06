using System;
using System.Drawing;
using System.Windows.Forms;
using System.IO.Ports; 
using System.Text;     

namespace bb3 
{
    public partial class MainForm : Form
    {
    	 string gelenVeriHavuzu = "";
        // Gelen veri işleme tamamen kaldırılmıştır.
        
        public MainForm()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;

           
            textBoxSicaklik.ReadOnly = true;
        }
        
        // --- PORT AÇMA/KAPAMA ---
        void ButtonPORTACClick(object sender, EventArgs e)
        {
            if(!serialPort1.IsOpen){
                // textBoxPORTNO adı varsayılmıştır
                serialPort1.PortName = textBoxPORTNO.Text; 
                 serialPort1.BaudRate = 9600; 
                try {
                    serialPort1.Open();
                    MessageBox.Show("Port Açıldı.", "Başarılı");
                } catch (Exception ex) {
                    MessageBox.Show("Port açılamadı: " + ex.Message, "Hata");
                }
            }
        }
        
        void ButtonportkapatClick(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen){
                serialPort1.Close();
                MessageBox.Show("Port Kapatıldı.", "Başarılı");
            }
        }
        
        // ***************************************************************
        // A. GÖNDERME FONKSİYONLARI (PC → TIVA C)
        // ***************************************************************

        // --- 1. SAAT GÖNDERME (textboxmikromejsi) ---
        void ButtometinGondernClick(object sender, EventArgs e) 
        {
             if (serialPort1.IsOpen)
            {
                
                string saat_str = texboxmikromejsi.Text; 
                
                // Format kontrolü: HH.MM.SS* (Toplam 9 karakter)
                if (saat_str.Length == 9 && 
                    saat_str[2] == '.' && saat_str[5] == '.' && saat_str[8] == '*')
                {
                    serialPort1.Write(saat_str); 
                    MessageBox.Show("Saat gönderildi: " + saat_str, "Başarılı");
                } 
                else 
                {
                    MessageBox.Show("Saat formatı HH.MM.SS* olmalıdır (9 karakter, * dahil).", "Hata");
                }
            } else {
                 MessageBox.Show("Lütfen portu açın.", "Hata");
            }
        }
        
        // --- 2. XYZ GÖNDERME (textbox1) ---
        

        void textboxmikromejsiTextChanged(object sender, EventArgs e) { }
        void textbox1TextChanged(object sender, EventArgs e) { }
       void SerialPort1DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
{
    // Arayüz thread'ine güvenli geçiş yap (Invoke)
    this.Invoke(new MethodInvoker(delegate 
    {
        try
        {
            string yeniGelen = serialPort1.ReadExisting();
            gelenVeriHavuzu += yeniGelen;

            // Paket T ile başlayıp ! ile bitiyor mu?
            while (gelenVeriHavuzu.Contains("T") && gelenVeriHavuzu.Contains("!"))
            {
                int bas = gelenVeriHavuzu.IndexOf("T");
                int son = gelenVeriHavuzu.IndexOf("!");

                if (son > bas)
                {
                    // T12:00:00S25B1! paketini ayıkla
                    string paket = gelenVeriHavuzu.Substring(bas, son - bas + 1);

                    // Harf etiketlerine göre verileri parçala
                    int sIndeks = paket.IndexOf("S");
                    int bIndeks = paket.IndexOf("B");

                    if (sIndeks != -1 && bIndeks != -1)
                    {
                        // 1. SAAT (T ile S arası)
                        textBoxFromMikroTime.Text = paket.Substring(1, sIndeks - 1);

                        // 2. SICAKLIK (S ile B arası)
                        textBoxSicaklik.Text = paket.Substring(sIndeks + 1, bIndeks - sIndeks - 1) + " °C";

                        // 3. BUTON (B ile ! arası)
                        string butonPart = paket.Substring(bIndeks + 1, (paket.Length - 1) - bIndeks - 1);
                        
                        if (butonPart == "1")
                        {
                            textBox4.Text = "BASILDI";
                            textBox4.BackColor = Color.Green; // Bastığında yeşil olsun
                        }
                        else
                        {
                            textBox4.Text = "Pasif";
                            textBox4.BackColor = Color.Red; // Pasifken kırmızı kalsın
                        }
                    }
                  
                    gelenVeriHavuzu = gelenVeriHavuzu.Substring(son + 1);
                }
                else
                {
                   
                    gelenVeriHavuzu = gelenVeriHavuzu.Substring(bas);
                    break; // Döngüden çık, yeni veriyi bekle
                }
            }
        }
        catch { }
    }));
}	
        	
        	
        	
		void TextBoxPORTNOTextChanged(object sender, EventArgs e)
		{
	
		}
		void TexboxmikromejsiTextChanged(object sender, EventArgs e)
		{
	
		}
		void TomikroxyzTextChanged(object sender, EventArgs e)
		{
	
		}
		void TomikroxyzbutonClick(object sender, EventArgs e)
		{
			{
            if (serialPort1.IsOpen)
            {
                
                string metin_str = tomikroxyz.Text;
                
                // Format kontrolü: 1-4 karakter olmalı ve sonunda # olmalı.
                if (metin_str.Length >= 2 && metin_str.Length <= 4 && metin_str.EndsWith("#"))
                {
                    string metin_kismi = metin_str.Substring(0, metin_str.Length - 1);
                    string padded_metin = metin_kismi.PadRight(3, ' ');
                    
                    serialPort1.Write(padded_metin.ToUpper() + "#"); 
                    MessageBox.Show("Metin gönderildi: " + padded_metin.ToUpper() + "#", "Başarılı");
                } 
                else 
                {
                    MessageBox.Show("Metin 1-3 karakter olmalı ve sonunda # olmalıdır (Maks. 4 karakter).", "Hata");
                }
            } else {
                 MessageBox.Show("Lütfen portu açın.", "Hata");
            }
	
		}
        
    }
		void Label1Click(object sender, EventArgs e)
		{
	
		}
		void Label2Click(object sender, EventArgs e)
		{
	
		}
		void Label3Click(object sender, EventArgs e)
		{
	
		}
		void TextBoxSicaklikTextChanged(object sender, EventArgs e)
		{
	
		}
		void TextfromthemikroClick(object sender, EventArgs e)
		{
	
		}
		void Label5Click(object sender, EventArgs e)
		{
	
		}
		void TextBox4TextChanged(object sender, EventArgs e)
		{
	
		}
		void TextBoxFromMikroTimeTextChanged(object sender, EventArgs e)
		{
	
		}
		void Label4Click(object sender, EventArgs e)
		{
	
		}
}
}
