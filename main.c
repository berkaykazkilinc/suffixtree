#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256
#define KATAR "katar.txt"


// YASAR BERKAY KAZKILINC

struct SuffixTreeDugum
{
    struct SuffixTreeDugum *cocuk[MAX_CHAR];

    struct SuffixTreeDugum *suffixBaglanti;

    int baslangic;
    int *son;

    int suffixIndex;
};

typedef struct SuffixTreeDugum Dugum;

char text[100];
char deneme[100];
Dugum *kok = NULL;

Dugum *lastNewdugum = NULL;
Dugum *activedugum = NULL;

int activeKose = -1;
int activeUzunluk = 0;

int kalanSuffixSayisi = 0;
int yaprakoglu = -1;
int *kokoglu = NULL;
int *bolunmusogul = NULL;
int size = -1;

Dugum *newdugum(int baslangic, int *son)
{
    Dugum *dugum =(Dugum*) malloc(sizeof(Dugum));
    int i;
    for (i = 0; i < MAX_CHAR; i++)
        dugum->cocuk[i] = NULL;

    dugum->suffixBaglanti = kok;
    dugum->baslangic = baslangic;
    dugum->son = son;
    dugum->suffixIndex = -1;
    return dugum;
}

int kenarUzunlugu(Dugum *n)
{
    if(n == kok)
        return 0;
    return *(n->son) - (n->baslangic) + 1;
}

int walkDown(Dugum *currdugum)
{
    if (activeUzunluk >= kenarUzunlugu(currdugum))
    {
        activeKose += kenarUzunlugu(currdugum);
        activeUzunluk -= kenarUzunlugu(currdugum);
        activedugum = currdugum;
        return 1;
    }
    return 0;
}


void controlAgac()
{
    int lenOfText = strlen(text);
    char suffix[100];
    char prefix[100];
    int canBeSuffix = 0;

    for(int i = 0; i < lenOfText; i++)
    {
        for(int j = 0; j < lenOfText; j++)
        {
            if(j > i)
            {
                suffix[j-i-1] = text[j];
            }
        }
        for(int k = 0; k < lenOfText- i - 1; k++)
        {
            for(int p = 0; p < lenOfText; p++)
            {
                if(p > i)
                {
                    prefix[p-i-1] = text[p-i-1];
                }
            }
        }
        if(strcmp(suffix,prefix) == 0 && i != lenOfText-1)
        {
            canBeSuffix = 1;
        }
        for(int m = 0; m < lenOfText; m++)
        {
            suffix[m] = NULL;
            prefix[m] = NULL;
        }
    }
    if(canBeSuffix == 1)
    {
        printf("Suffix Tree Olusturulamaz");
    }
    else
    {
        printf("Suffix Tree Olusturulabilir");
    }
}
void SuffixTreeUzat(int konum)
{
    yaprakoglu = konum;

    kalanSuffixSayisi++;

    lastNewdugum = NULL;

    while(kalanSuffixSayisi > 0)
    {

        if (activeUzunluk == 0)
        {
            activeKose = (int)text[konum]-(int)' ';
        }

        if (activedugum->cocuk[activeKose] == NULL)
        {

            activedugum->cocuk[activeKose] = newdugum(konum, &yaprakoglu);

            if (lastNewdugum != NULL)
            {
                lastNewdugum->suffixBaglanti = activedugum;
                lastNewdugum = NULL;
            }
        }

        else
        {
            Dugum *next = activedugum->cocuk[activeKose];
            if (walkDown(next))
            {
                continue;
            }
            if (text[next->baslangic + activeUzunluk] == text[konum])
            {

                if(lastNewdugum != NULL && activedugum != kok)
                {
                    lastNewdugum->suffixBaglanti = activedugum;
                    lastNewdugum = NULL;
                }

                activeUzunluk++;
                break;
            }

            bolunmusogul = (int*) malloc(sizeof(int));
            *bolunmusogul = next->baslangic + activeUzunluk - 1;

            Dugum *split = newdugum(next->baslangic, bolunmusogul);
            activedugum->cocuk[activeKose] = split;

            split->cocuk[(int)text[konum]-(int)' '] =
                newdugum(konum, &yaprakoglu);
            next->baslangic += activeUzunluk;
            split->cocuk[activeKose] = next;

            if (lastNewdugum != NULL)
            {

                lastNewdugum->suffixBaglanti = split;
            }


            lastNewdugum = split;
        }

        kalanSuffixSayisi--;
        if (activedugum == kok && activeUzunluk > 0)
        {
            activeUzunluk--;
            activeKose = (int)text[konum -kalanSuffixSayisi + 1]-(int)' ';
        }

        else if (activedugum != kok)
        {
            activedugum = activedugum->suffixBaglanti;
        }
    }
}

void setSuffixIndexByDFS(Dugum *n, int etiketYuksekligi)
{
    if (n == NULL) return;

    if (n->baslangic != -1)
    {

    }
    int yaprak = 1;
    int i;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if (n->cocuk[i] != NULL)
        {
            yaprak = 0;
            setSuffixIndexByDFS(n->cocuk[i], etiketYuksekligi +kenarUzunlugu(n->cocuk[i]));
        }
    }
    if (yaprak == 1)
    {
        n->suffixIndex = size - etiketYuksekligi;
    }
}

void freeSuffixTreeBykonumtOrder(Dugum *n)
{
    if (n == NULL)
        return;
    int i;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if (n->cocuk[i] != NULL)
        {
            freeSuffixTreeBykonumtOrder(n->cocuk[i]);
        }
    }
    if (n->suffixIndex == -1)
        free(n->son);
    free(n);
}

void arama()
{
    char aranacak[100];
    printf("Aranacak kelimeyi yaziniz: ");
    scanf("%s",&aranacak);
    int n = 0;
    int m = 0;
    int times = 0;
    int len = strlen(aranacak);
    int lenOfText = strlen(text);
    int bulduMu = 1;
    int firstIndex = 0;

    while(text[n] != '\0')
    {
        for(int i = 0; i < lenOfText; i++)
        {
            bulduMu = 1;
            if(text[i] == aranacak[0])
            {
                for(int j = 1; j < len; j++)
                {
                    if(text[i+j] == aranacak[j])
                    {
                    }
                    else
                    {
                        bulduMu = 0;
                        continue;
                    }
                }
                if(bulduMu)
                {
                    if(times == 0)
                    {
                        firstIndex = i;
                    }
                    times++;
                }
            }
        }
        break;

    }

    if(times > 0)
    {
        printf("'%s' katari %d kez tekrar etmektedir ve %d. siradaki harften baslar.\n", aranacak, times,(firstIndex+1));
    }
    else
    {
        printf("'%s' katari bulunamadi.\n", aranacak);
    }

}
void buildSuffixTree()
{
    size = strlen(text);
    int i;
    kokoglu = (int*) malloc(sizeof(int));
    *kokoglu = - 1;

    kok = newdugum(-1, kokoglu);

    activedugum = kok;
    for (i=0; i<size; i++)
        SuffixTreeUzat(i);
    int etiketYuksekligi = 0;
    setSuffixIndexByDFS(kok, etiketYuksekligi);
}

void GecisYap(Dugum *n, int etiketYuksekligi, int* maxHeight,int* AltDizebaslangicIndex)
{
    if(n == NULL)
    {
        return;
    }
    int i=0;
    if(n->suffixIndex == -1)
    {
        for (i = 0; i < MAX_CHAR; i++)
        {
            if(n->cocuk[i] != NULL)
            {

                GecisYap(n->cocuk[i], etiketYuksekligi +kenarUzunlugu(n->cocuk[i]), maxHeight,AltDizebaslangicIndex);

            }


        }
    }
    else if(n->suffixIndex > -1 &&(*maxHeight < etiketYuksekligi - kenarUzunlugu(n)))
    {

        *maxHeight = etiketYuksekligi - kenarUzunlugu(n);
        *AltDizebaslangicIndex = n->suffixIndex;

    }
}
void arama2(char aranacak[])
{
    int n = 0;
    int m = 0;
    int times = 0;
    int len = strlen(aranacak);
    int lenOfText = strlen(text);
    int bulduMu = 1;
    int firstIndex = 0;

    while(text[n] != '\0')
    {
        for(int i = 0; i < lenOfText; i++)
        {
            bulduMu = 1;
            if(text[i] == aranacak[0])
            {
                for(int j = 1; j < len; j++)
                {
                    if(text[i+j] == aranacak[j])
                    {
                    }
                    else
                    {
                        bulduMu = 0;
                        continue;
                    }
                }
                if(bulduMu)
                {
                    if(times == 0)
                    {
                        firstIndex = i;
                    }
                    times++;
                }
            }
        }
        break;

    }
        printf(" tir ve %d kez tekrar etmektedir\n",times);


}
void EnUzunTekrarEdenAltDizi()
{
    char counter[100];
    int maxHeight = 0;
    int AltDizebaslangicIndex = 0;
    int counterr=0;
    GecisYap(kok, 0, &maxHeight, &AltDizebaslangicIndex);
    printf("%s de tekrarlanan en uzun alt katar: ", text);
    int k;
    for (k=0; k<maxHeight; k++)
    {
        printf("%c", text[k + AltDizebaslangicIndex]);
        counter[k]=text[k+AltDizebaslangicIndex];
    }

    if(k == 0)
    {
        printf("Tekrarlanan Alt Katar Yok");
    }
    arama2(counter);

    printf("\n");
}
void EnCokTekrarEdenAltKatar()
{
    int enbuyuk=0;
    int sayi=0;
    int harf=0;
    int boyut=0;
    int i,j;
    boyut=strlen(text);

    for(i=0; i<boyut; i++)
    {
        for(j=0; j<boyut; j++)
        {
            if(text[i]==text[j])
            {
                sayi++;
            }
        }
        if(sayi>enbuyuk)
        {
            enbuyuk=sayi;
            harf=i;
        }
        sayi=0;
    }
    printf("En cok tekrar eden alt katar: %c ",text[harf]);
    printf(" %d kere tekrar etmistir.\n",enbuyuk);

}

void menuListele()
{

    printf("\n| 1:S katari icin sonek agaci olusturulabilir mi? \n");
    printf("| 2:Sonek agaci olusturulan bir s katari icinde p katari geciyor mu, geciyorsa ilk bulundugu yerin");
    printf("baslangic pozisyonu nedir, kac kez tekrar etmektedir? \n");
    printf("| 3:Sonek agaci olusturulan bir s katari icinde tekrar eden en uzun altkatar nedir, kac kez tekrar etmektedir? \n");
    printf("| 4:Sonek agaci olusturulan bir s katari icinde en cok tekrar eden altkatar nedir, kac kez tekrar etmektedir? \n");
    printf("| 0: Cikis\n");
    printf("-------------------------\n");
    printf("| Secim?: ");
}
void secimUygula(int secim)
{
    switch (secim)
    {
    case 0:
        exit(1);
        break;
    case 1:
        controlAgac();
        break;
    case 2:
    {
        arama();
        break;

    }
    case 3:
    {
        EnUzunTekrarEdenAltDizi();
        freeSuffixTreeBykonumtOrder(kok);
        break;
    }
    case 4:
    {
        EnCokTekrarEdenAltKatar();
        break;

    }
    default:
        printf("Yanlis Secim\n");
    }
}

int main()
{
    FILE *dosya;
    int x=0,k=0;
    dosya=fopen(KATAR,"r+");
    if((dosya=fopen(KATAR,"r+"))== NULL)
    {
        printf("Dosya Acilamadi");
        exit(1);
    }
    while(!feof(dosya))
    {
        fscanf(dosya,"%c",&text[x]);
        x++;
    }
    buildSuffixTree();
    int secim = -1;
    while(secim != 0)
    {
        menuListele();
        scanf("%d", &secim);
        secimUygula(secim);
    }


    return 0;
}
