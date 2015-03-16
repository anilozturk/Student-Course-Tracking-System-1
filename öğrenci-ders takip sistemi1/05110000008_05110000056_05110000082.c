#include <stdio.h>
#include <stdlib.h>

#define KAYIT_SAY 900

struct ogrenci
{
    int ogr_no;
    char isim[31];
    int sinif;
    int donemlik_kredi;
    float donemlik_ort;
} bir_ogrenci;

struct ders
{
    int ders_kod;
    char ders_ad[26];
    int kredi;
    char gorevli[31];
    char derslik[21];
} bir_ders;

int menu_goruntule();
int ogrenci_no_al(int *);
int ders_kodu_al(int *);
int donem_al(int *);
void bir_ogrenci_goster(FILE *,int);
void ogrenci_bilgi_ders_liste(FILE *,FILE *,FILE *,int);
void sinif_liste(FILE *);
void basari_istatistik(FILE *);
void bir_ders_goster(FILE *,int);
void ders_bilgi_ogrenci_liste(FILE *,FILE *,FILE *,int);
void donem_listele(FILE *,int);
void ogrenci_ekle(FILE *,int);
void ders_ekle(FILE *,int);
void gorevli_derslik_degistir(FILE *,int);
void ogrenci_not_giris(FILE *,FILE *,FILE *,int);
void ogrenci_sil(FILE *,FILE *,int);

int main()
{
    int secim,ogrenci_no,ders_kodu,donem_no;
    FILE *fptr;//ogrenciler.dat
    FILE *fptr2;//notlar.txt
    FILE *fptr3;//dersler.dat

    if ((fptr=fopen("ogrenciler.dat","rb+"))==NULL)
        printf("Ogrenciler dosyasi acilamadi");
    else{
        if ((fptr2=fopen("notlar.txt","r+"))==NULL)
            printf("Notlar dosyasi acilamadi");
        else{
            if ((fptr3=fopen("dersler.dat","rb+"))==NULL)
                printf("Dersler dosyasi acilamadi");
            else{
                do{
                    secim=menu_goruntule();
                    switch (secim){
                        case 1:
                            ogrenci_no_al(&ogrenci_no);//once numarayi alip sonra fonksiyona gonderiyoruz
                            bir_ogrenci_goster(fptr,ogrenci_no);
                            break;
                        case 2:
                            ogrenci_no_al(&ogrenci_no);
                            ogrenci_bilgi_ders_liste(fptr,fptr2,fptr3,ogrenci_no);
                            break;
                        case 3:
                            sinif_liste(fptr);
                            break;
                        case 4:
                            basari_istatistik(fptr);
                            break;
                        case 5:
                            ders_kodu_al(&ders_kodu);
                            bir_ders_goster(fptr3,ders_kodu);
                            break;
                        case 6:
                            ders_kodu_al(&ders_kodu);
                            ders_bilgi_ogrenci_liste(fptr,fptr2,fptr3,ders_kodu);
                            break;
                        case 7:
                            donem_al(&donem_no);
                            donem_listele(fptr3,donem_no);
                            break;
                        case 8:
                            ogrenci_no_al(&ogrenci_no);
                            ogrenci_ekle(fptr,ogrenci_no);
                            break;
                        case 9:
                            donem_al(&donem_no);
                            ders_ekle(fptr3,donem_no);
                            break;
                        case 10:
                            ders_kodu_al(&ders_kodu);
                            gorevli_derslik_degistir(fptr3,ders_kodu);
                            break;
                        case 11:
                            ders_kodu_al(&ders_kodu);
                            ogrenci_not_giris(fptr,fptr2,fptr3,ders_kodu);
                            break;
                        case 12:
                            ogrenci_no_al(&ogrenci_no);
                            ogrenci_sil(fptr,fptr2,ogrenci_no);
                            break;
                    }
                }while (secim!=13);
            }
        }
    }
    return 0;
}

int menu_goruntule()
{
    int secim;

    printf("\n1.Bir ogrencinin bilgilerinin listelenmesi:\n");
    printf("2.Bir ogrencinin bilgilerinin ve bu donem aldigi derslerin listelenmesi:\n");
    printf("3.Bir sinifta okuyan ogrencilerin listelenmesi:\n");
    printf("4.Bolumun bu doneme ait basari istatistiklerinin listelenmesi:\n");
    printf("5.Bir dersin bilgilerinin listelenmesi:\n");
    printf("6.Bir dersin bilgilerinin ve bu donem alan ogrencilerin listelenmesi:\n");
    printf("7.Bir donemde verilen derslerin bilgilerinin listelenmesi:\n");
    printf("8.Yeni bir ogrencinin eklenmesi:\n");
    printf("9.Yeni bir dersin eklenmesi:\n");
    printf("10.Bir dersin ogretim uyesinin ve dersliginin degistirilmesi:\n");
    printf("11.Bir dersi alan ogrencilerin notlarinin girilmesi:\n");
    printf("12.Bir ogrencinin kaydinin silinmesi:\n");
    printf("13.Cikis:\n");

    printf("Seciminizi girin:\n");
    scanf("%d",&secim);

    return secim;
}

void bir_ogrenci_goster(FILE *fptr,int no)
{
    fseek(fptr,(no-100)*sizeof(struct ogrenci),SEEK_SET);//dosya isaretcisi okunmasi gereken yere gidiyor.
    fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

    if (bir_ogrenci.ogr_no==no){// kullanicidan alinan numara dosyadakiyle kontrol ediliyor.
        printf("Ogrenci No   Ogrenci Adi   Sinifi   Toplam Kredisi    Not Ort\n");
        printf("---------------------------------------------------------------\n");
        printf("%-13d %-14s %-13d %-12d %.2f\n",bir_ogrenci.ogr_no,bir_ogrenci.isim,bir_ogrenci.sinif,bir_ogrenci.donemlik_kredi,bir_ogrenci.donemlik_ort);
    }
    else if (bir_ogrenci.ogr_no==0)
        printf("Girdiginiz numaraya ait ogrenci kaydi bulunmamaktadir.\n");
}

void ogrenci_bilgi_ders_liste(FILE *fptr,FILE *fptr2,FILE *fptr3,int no)
{
    int ders_kodu,ogrenci_no,ogrenci_not;

    bir_ogrenci_goster(fptr,no);//ustteki fonksiyon tekrar cagriliyor.

    rewind(fptr2);
    fscanf(fptr2,"%d %d %d",&ders_kodu,&ogrenci_no,&ogrenci_not);
    printf("\nBu donem aldigi dersler:\n");
    printf("\nDers Kodu   Ders Adi   Ders Kredisi   Alinan Not\n");
    printf("-------------------------------------------------\n");
    while (!feof(fptr2)){

        if (no==ogrenci_no){//girilen numara txt dosyasindaki kayitlarla karsilastiriliyor.
            fseek(fptr3,(ders_kodu-1)*sizeof(struct ders),SEEK_SET);
            fread(&bir_ders,sizeof(struct ders),1,fptr3);

            if (ders_kodu==bir_ders.ders_kod){//txt'deki ders kodu dosyadaki kodla karsilastiriliyor.
                printf("%-12d %-13s %-13d %d\n",ders_kodu,bir_ders.ders_ad,bir_ders.kredi,ogrenci_not);
                rewind(fptr3);
            }
        }

        fscanf(fptr2,"%d %d %d",&ders_kodu,&ogrenci_no,&ogrenci_not);
    }
}

int ogrenci_no_al(int *no)
{
    do{
        printf("Ogrenci numarasini giriniz:\n");
        scanf("%d",no);
    }
    while (*no<100 || *no>999);

    return *no;
}

void sinif_liste(FILE *fptr)
{
    struct ogrenci sirala[KAYIT_SAY]= {{0,"",0,0,0.00}};
    struct ogrenci gecici;
    int sinif,i,top_ogrenci=0,basarisiz_ogr=0;
    int gecis_say=0;
    int yer_degisti;
    float genel_not=0;

    printf("Ogrencilerini goruntulemek istediginiz sinifi girin:\n");
    scanf("%d",&sinif);

    rewind(fptr);
    for (i=0; i<KAYIT_SAY; i++){
        fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);
        if (bir_ogrenci.ogr_no!=0){
            if (bir_ogrenci.sinif==sinif){
                sirala[i]=bir_ogrenci;//istenen ozellikteki ogrenciler seciliyor.
                genel_not+=bir_ogrenci.donemlik_ort;
                top_ogrenci++;
                if (bir_ogrenci.donemlik_ort<60)
                    basarisiz_ogr++;
            }
        }
    }

    do{//secilen ogrenciler siraya sokuluyor.
        gecis_say++;
        yer_degisti=0;
        for (i=0; i<KAYIT_SAY-gecis_say; i++){
            if (sirala[i].donemlik_ort<sirala[i+1].donemlik_ort){
                gecici=sirala[i];
                sirala[i]=sirala[i+1];
                sirala[i+1]=gecici;
                yer_degisti=1;
            }
            else if ((sirala[i].donemlik_ort==sirala[i+1].donemlik_ort) && (sirala[i].donemlik_kredi<sirala[i+1].donemlik_kredi)){
                gecici=sirala[i];
                sirala[i]=sirala[i+1];
                sirala[i+1]=gecici;
                yer_degisti=1;
            }
        }
    }while(yer_degisti==1);

    printf("Sira No  Ogr No    Ad Soyad      Sinif     Kredi Say   Not Ort\n");
    printf("--------------------------------------------------------------\n");
    for (i=0; i<KAYIT_SAY; i++){
        if (sirala[i].ogr_no!=0)
            printf("%-9d %-8d %-15s %-8d %-10d %.2f\n",i+1,sirala[i].ogr_no,sirala[i].isim,sirala[i].sinif,sirala[i].donemlik_kredi,sirala[i].donemlik_ort);
    }

    printf("\nSinifin genel not ortalamasi:%.2f\n",genel_not/top_ogrenci);
    printf("Donemlik not ortalamasi 60 in altinda olan ogrencilerin sayisi:%d yuzdesi:%.2f\n",basarisiz_ogr,(float)basarisiz_ogr*100/top_ogrenci);
}

void basari_istatistik(FILE *fptr)
{
    int not_liste[10]={0},index,i,top_ogr=0;

    rewind(fptr);//dosya belirtecinin "ogrenciler.dat" dosyasinin basina gelmesi saglaniyor.
    fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);
    while (!feof(fptr)){
        if (bir_ogrenci.ogr_no!=0){
            top_ogr++;
            index=bir_ogrenci.donemlik_ort/10;//ornegin notu 95 olan en yuksek index'i-9 olan index- aliyor.
            if (index==10)//eger ogrencinin aldigi puan 100 ise en yuksek olan index'e atiliyor.
                not_liste[index-1]++;
            else
                not_liste[index]++;
        }

        fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);
    }

    printf("Not Araligi   Ogrenci Sayisi   Ogrenci Yuzdesi\n");
    for (i=10; i>0; i--)
        printf("%2d-%-14d %-14d %-14.2f\n",(i-1)*10,i*10-1,not_liste[i-1],(float)not_liste[i-1]*100/top_ogr);
}

void bir_ders_goster(FILE *fptr3,int kod)
{
    fseek(fptr3,(kod-1)*sizeof(struct ders),SEEK_SET);
    fread(&bir_ders,sizeof(struct ders),1,fptr3);

    if (bir_ders.ders_kod==kod){//kod al fonksiyonuyla alinan kod kayittaki kodla ayniysa yazdirma islemi yapiliyor.
        printf("Ders Kodu   Ders Adi    Kredi    Ogretim Uyesi     Derslik\n");
        printf("---------------------------------------------------------------\n");
        printf("%-11d %-13s %-7d %-12s %9s\n",bir_ders.ders_kod,bir_ders.ders_ad,bir_ders.kredi,bir_ders.gorevli,bir_ders.derslik);
    }
    else if (bir_ders.ders_kod==0)
        printf("Girdiginiz numaraya ait ders kaydi bulunmamaktadir.\n");
}

int ders_kodu_al(int *kod)//1'den fazla programda kod istendigi icin fonksiyon yaptik.
{
    do{
        printf("Dersin kodunu giriniz:\n");
        scanf("%d",kod);//*kod zaten deger oldugu icin & karakterine ihtiyac duymadik.
    }
    while (*kod<10 || *kod>89);

    return *kod;
}

void ders_bilgi_ogrenci_liste(FILE *fptr,FILE *fptr2,FILE * fptr3,int kod)
{
    int ders_kodu,ogrenci_no,ogrenci_not,ogr_say=0,basarisiz_ogr=0;
    float genel_not=0;

    bir_ders_goster(fptr3,kod);//bu fonksiyon tekrar cagriliyor.
    rewind(fptr2);//notlar.txt dosyasinin basina gonderiliyor.
    fscanf(fptr2,"%d %d %d",&ders_kodu,&ogrenci_no,&ogrenci_not);
    printf("\nBu donem alan ogrenciler:\n");
    printf("\nOgr No   Ad Soyad     Sinifi   Alinan Not\n");
    printf("-------------------------------------------------\n");
    while (!feof(fptr2)){
        if (kod==ders_kodu){
            fseek(fptr,(ogrenci_no-100)*sizeof(struct ogrenci),SEEK_SET);
            fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);
            genel_not+=ogrenci_not;

            if (ogrenci_no==bir_ogrenci.ogr_no){
                printf("%-8d %-15s %-8d %d\n",ogrenci_no,bir_ogrenci.isim,bir_ogrenci.sinif,ogrenci_not);
                ogr_say++;
                if (ogrenci_not<60)
                    basarisiz_ogr++;
                rewind(fptr);
            }
        }

        fscanf(fptr2,"%d %d %d",&ders_kodu,&ogrenci_no,&ogrenci_not);
    }

    printf("\nDersi alan ogrenci sayisi:%d\n",ogr_say);
    printf("Dersin genel not ortalamasi:%.2f\n",genel_not/ogr_say);
    printf("Notu 60 in altinda olan ogrencilerin sayisi:%d ve yuzdesi:%.2f\n",basarisiz_ogr,(float)basarisiz_ogr*100/ogr_say);
}

void donem_listele(FILE *fptr3,int donem)
{
    int kredi_top=0,sayac=0;

    fseek(fptr3,(donem*10-1)*sizeof(struct ders),SEEK_SET);//girilen donem numarasiyla baslayan ilk ders kaydina gidiliyor.

    printf("Ders Kodu   Ders Adi    Kredi    Ogretim Uyesi     Derslik\n");
    printf("---------------------------------------------------------------\n");
    do{
        fread(&bir_ders,sizeof(struct ders),1,fptr3);
        sayac++;
        if (bir_ders.ders_kod!=0){
            kredi_top+=bir_ders.kredi;
            printf("%-11d %-13s %-7d %-12s %9s\n",bir_ders.ders_kod,bir_ders.ders_ad,bir_ders.kredi,bir_ders.gorevli,bir_ders.derslik);
        }
    }while (sayac!=10);//1 donemde en fazla 10 ders olabilcegi icin sayac 10 a kadar donduruluyor.

    printf("\nDerslerin kredi toplami:%d\n",kredi_top);
}

int donem_al(int *donem)//1'den fazla programda donem numarasi istendigi icin fonksiyon yaptik.

{
    do{
        printf("Donem numarasini giriniz:\n");
        scanf("%d",donem);
    }while (*donem<1 || *donem>8);

    return *donem;
}

void ogrenci_ekle(FILE *fptr,int no)
{
    struct ogrenci yeni= {0,"",0,0,0.00};

    fseek(fptr,(no-100)*sizeof(struct ogrenci),SEEK_SET);
    fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

    if (bir_ogrenci.ogr_no==no)
        printf("Girdiginiz numaraya ait ogrenci kaydi zaten var!\n");
    else{
        yeni.ogr_no=no;//klavyeden girdigim numarayi yeni bir degiskene atadik.
        printf("Ogrencinin adini giriniz:\n");
        fflush(stdin);//tamponu sifirlar.
        gets(yeni.isim);
        printf("Ogrencinin sinifini giriniz:\n");
        scanf("%d",&yeni.sinif);
        fseek(fptr,-1*sizeof(struct ogrenci),SEEK_CUR);
        fwrite(&yeni,sizeof(struct ogrenci),1,fptr);
    }
}

void ders_ekle(FILE *fptr3,int donem)
{
    struct ders yeni= {0,"",0,"",""};
    int bulundu=0,sayac;

    fseek(fptr3,(donem*10-1)*sizeof(struct ders),SEEK_SET);
    fread(&bir_ders,sizeof(struct ders),1,fptr3);
    sayac=0;
    while (sayac!=10){
        if (bir_ders.ders_kod==0){
            yeni.ders_kod=donem*10+sayac;//hangi kayit bossa yeni ders oraya kaydediliyor.
            printf("Dersin kodu:%d\n",yeni.ders_kod);
            bulundu=1;
            break;
        }

        fread(&bir_ders,sizeof(struct ders),1,fptr3);
        sayac++;
    }

    if (bulundu==0)
        printf("Bu doneme baska ders acilamaz.Butun kayitlar dolu!\n");
    else{
        printf("Dersin adini giriniz:\n");
        fflush(stdin);
        gets(yeni.ders_ad);
        printf("Dersin kredisini giriniz:\n");
        scanf("%d",&yeni.kredi);
        printf("Dersi veren ogretim uyesini giriniz:\n");
        fflush(stdin);
        gets(yeni.gorevli);
        printf("Dersin verilecegi derslik adini giriniz:\n");
        fflush(stdin);
        gets(yeni.derslik);

        fseek(fptr3,-1*sizeof(struct ders),SEEK_CUR);
        fwrite(&yeni,sizeof(struct ders),1,fptr3);
    }
}

void gorevli_derslik_degistir(FILE *fptr3,int kod)
{
    fseek(fptr3,(kod-1)*sizeof(struct ders),SEEK_SET);
    fread(&bir_ders,sizeof(struct ders),1,fptr3);

    if (bir_ders.ders_kod==kod){
        printf("Bu dersi veren yeni ogretim uyesini giriniz:\n");
        fflush(stdin);
        gets(bir_ders.gorevli);
        printf("Bu dersin verilecegi yeni dersligi giriniz:\n");
        fflush(stdin);
        gets(bir_ders.derslik);

        fseek(fptr3,-1*sizeof(struct ders),SEEK_CUR);
        fwrite(&bir_ders,sizeof(struct ders),1,fptr3);
    }
    else
        printf("Girdiginiz numaraya ait ders kaydi bulunmamaktadir.\n");
}

void ogrenci_not_giris(FILE *fptr,FILE *fptr2,FILE *fptr3,int kod)
{
    int no,puan;
    char cevap;

    fseek(fptr3,(kod-1)*sizeof(struct ders),SEEK_SET);
    fread(&bir_ders,sizeof(struct ders),1,fptr3);

    if (bir_ders.ders_kod==kod){
        do{
            ogrenci_no_al(&no);

            fseek(fptr,(no-100)*sizeof(struct ogrenci),SEEK_SET);
            fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

            if (bir_ogrenci.ogr_no==no){
                printf("Ogrencinin bu dersten aldigi notu giriniz:\n");
                scanf("%d",&puan);

                fseek(fptr2,0,SEEK_END);
                fprintf(fptr2,"%d %d %d\n",kod,no,puan);
                bir_ogrenci.donemlik_ort=(bir_ogrenci.donemlik_kredi*bir_ogrenci.donemlik_ort+bir_ders.kredi*puan)/(bir_ogrenci.donemlik_kredi+bir_ders.kredi);
                bir_ogrenci.donemlik_kredi+=bir_ders.kredi;

                fseek(fptr,-1*sizeof(struct ogrenci),SEEK_CUR);
                fwrite(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

                printf("Baska ogrenci var mi?\n");
                fflush(stdin);
                scanf("%c",&cevap);
            }
            else
                printf("Girdiginiz numaraya ait ogrenci kaydi bulunmamaktadir.\n");
        }while (cevap=='E' || cevap=='e');//baska ogrenci oldugu surece donucek
    }
    else
        printf("Girdiginiz numaraya ait ders kaydi bulunmamaktadir.\n");
}

void ogrenci_sil(FILE *fptr,FILE *fptr2,int no)
{
    int kod,ogr_no,puan;
    FILE *cptr;

    fclose(fptr2);
    if ((fptr2=fopen("notlar.txt","r+"))==NULL)
        printf("Notlar dosyasi acilamadi");
    else{
        fseek(fptr,(no-100)*sizeof(struct ogrenci),SEEK_SET);
        fread(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

        if (bir_ogrenci.ogr_no==no){
            bir_ogrenci.ogr_no=0;//ogrencinin kaydini silmek amaciyla numarasini 0'a atiyoruz.
            fseek(fptr,-1*sizeof(struct ogrenci),SEEK_CUR);
            fwrite(&bir_ogrenci,sizeof(struct ogrenci),1,fptr);

            cptr=fopen("gecici.txt","w");//yazmak amaciyla yeni dosya aciyoruz.
            fscanf(fptr2,"%d %d %d",&kod,&ogr_no,&puan);

            while (!feof(fptr2)){
                if (ogr_no!=no)//txt'deki numara klavyeden girilen numaraya esit degilse gecici actigimiz txt dosyasina degerleri giriyoruz.
                    fprintf(cptr,"%d %d %d\n",kod,ogr_no,puan);

                fscanf(fptr2,"%d %d %d",&kod,&ogr_no,&puan);
            }

            fclose(cptr);
            fclose(fptr2);
            remove("notlar.txt");//var olan txt dosyasi silinir.
            rename("gecici.txt","notlar.txt");//gecici olarak actigimiz dosyanin adi notlar.txt olarak eski adýna degistirilir.
            printf("Ogrenci basariyla silindi!\n");
        }
        else
            printf("Girdiginiz numaraya ait ogrenci kaydi bulunmamaktadir.\n");

        fclose(fptr2);//notlar.txt dosyasi en son kapatilir.
    }
}
