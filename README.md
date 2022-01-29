# Si4735_AM_FM_SSB_radio
Si4735 ile DSP radyo

This All Band Radio Sketch has been written by Ricardo Lima Caratti and original software located at https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_02_LCD_20x4_I2C/SI473X_04_ALL_IN_ONE/SI473X_04_ALL_IN_ONE.ino

Bu yazılılım PU2CLR çağrı işaretli Ricardo Lima Caratti tarafından hazırlanmış olup orijinal yazılım https://github.com/pu2clr/SI4735/blob/master/examples/SI47XX_02_LCD_20x4_I2C/SI473X_04_ALL_IN_ONE/SI473X_04_ALL_IN_ONE.ino adresindedir.
  
  PU2CLR tarafından yazılan programda bâzı değişiklikler ve ilâveler yapılmıştır. Bu değişiklik ve ilâveler şunlardır:
  
  DEĞİŞİKLİKLER:
  1) Baskı devre tasarımına göre tuşların Arduino'ya bağlandığı bacaklar değiştirilmiştir.
  2) Ekran tasarımı değiştirilmiştir. 4 satır, 20 karakterlik ekrana daha çok bilgi sığdırabilmek için bâzı ifadeler kısaltılmış (STEREO ST gibi),SNR, S Metre,
  Adım, Ses yerine simgeler kullanılmıştır.
  3) Band sayısı arttırılmış ve bandların başlangıç-bitiş frekansları yeniden belirlenmiş ve isimleri düzenlenmiştir.
  4) AM kipinde frekans adımlarına 9 kHz ve 100 kHz ilâve edilmiştir.
  
  İLAVELER:

 1) Devre, 2 adet Li Ion pille beslenmek üzere tasarlanmış olduğundan, pil geriliminin ölçülerek ekranda gösterilmesi için bir rutin ilave edilmiştir.
  GErilim ölçme devresindeki R1 ve R2 dirençlerinin değerini ölçerek aşağıdaki
  // Voltmetre değişkenleri kısmında OHM cinsinden r1 ve r2'nin değeri olarak yazınız. Benim devremde, R1 için 10k'lık bir direnç, R2 için 3,3k'lık bir
  direnç kullanılmı, direnç değerleri ölçülmüş ve 10k'lık direnç 9970 ohm, 3,3k'lık direnç ise 3260 ohm bulunmuş ve aşağıdaki şekilde
  programa tanıtılmıştır. 
  

float r1 = 9970.0;

float r2 = 3260.0;

Pil geriliminin ölçümü bu iki direnç değerinin hassasiyetine bağımlıdır.
DİKKAT!!!!!!
Arduino Pro Mini'nin girişlerine 3V'tan fazla gerilim uygulanmamalıdır. Bunun için R1 ve R2 ile yapacağınız gerilim bölücünün bölme oranını
hesaplarken buna dikkat ediniz. Bölme oranı şöyle hesaplanır:
Bölme oranı=(R1+R2)/R2
