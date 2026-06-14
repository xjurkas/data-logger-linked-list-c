#include <stdio.h>              //includovanie kniznic
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
typedef struct record {
    char oznacenie;                     //nazvanie premennych, do ktorych budeme ziskavat udaje 
    int cislovanie;
    char druh;
    float latitude;
    float longitude;
    char typ[3];
    float value;
    int time;
    int date;
    struct record *next;            //aby sme vedeli sa posvuat v list
}RECORD;

void v(RECORD **headPointer, int *PocetMerani){
    if((*PocetMerani)==0){                                  //podmienka ak pocet merani je 0. To znamena ze nejsu ziadne zaznamy
        printf("Zoznam neobsahuje zaznamy\n");
        return;
    }
    RECORD *last=*headPointer;          //premennu last davame aby bola napojena na prvy prvok
    int counter=1;                              //pomocny counter, kt bude pisat aky prvok vypisuje
    for(int i=0;i<(*PocetMerani);i++){      //ideme tolko krat, kolko je pocet merani
        printf("%d.\n", counter);
        printf("ID: %c%d%c\t%s\t%g\n",last->oznacenie, last->cislovanie, last->druh, last->typ, last->value);       //vypisovanie oznacenia cislovania druh typ a value
        printf("Poz: ");
        if((last->latitude)>0){
            printf("+%g\t", last->latitude);        //podmienka aby sme vedeli vypisat latitude ako kladne ci zaporne
        }else{
            printf("%g\t", last->latitude);
        }
        if((last->longitude)>0){
            printf("+%g\n", last->longitude);       //podmienka aby sme vedeli vypisat longitude ako kladne ci zaporne
        }else{
            printf("%g\n", last->longitude);
        }
        printf("Dac: %d\t%d\n", last->date, last->time);    //doprintujem datum a cas
        counter++;                                          //zvysujem pomocny counter
        last=last->next;                        //pre premennu last, nastavujem dalsi link
    }

}
void n(RECORD **headPointer,int *PocetMerani){
    if((*PocetMerani)>0){                           //ak boli uz vytvorene zoznamy, tu ich vymazeme 
        while(*headPointer){
            RECORD *tempDelete=*headPointer;        //nastavujeme pomocnej premennej prvy link
            *headPointer=(*headPointer)->next;      //prvy link nastavujeme ako next, to znamena ze k tomu poslednemu uz mame pristup iba pomocou pomocnej premnnej tempdelete
            free(tempDelete);                       //uvolnujem pomocnu premennu
        }
    }
    (*PocetMerani)=0;
    FILE* file1;
    if((file1=fopen("dataloger_V2.txt","r"))==NULL){            //otvorenie suboru a podmienka ak sa nahodou neotvoril
        printf("Nepodarilo sa otvorit subor\n");
        return;
    }
    char line[1024];                //pomocne pole + premenne
    int helper=0;
    float tempNum=0;
    float tempNum2=0;
    RECORD *temp=NULL;
    RECORD *last=NULL;         
    while(fgets(line,sizeof(line),file1)!=NULL){
        if(strchr(line, '\n')){
            *strchr(line, '\n') = 0;
        }
        if(helper==0){
            RECORD *temp = malloc(sizeof(RECORD));
            temp->next=NULL;
            if(last){                           //pokial neni prvy zaznam
                last->next=temp;         //nastavime pointer na dalsi zaznam
            }else{                      //ak uz je vytvoreny, nasej head (*headPointer) dame hodnotu temp
                *headPointer=temp;
            }
            last=temp;             //aktualny list na predosli aby sme mohli dalej ukladat do zoznamu
        }else if(helper==1){                              
            last->oznacenie=line[0];                //ukladadame prvu cast idcka= oznacenie
            last->cislovanie=atoi(&line[1]);        //ukladame druhu cast idcka= cislovania
            last->druh=line[4];                     //ukladame poslednu cast idcka= druh
        }else if(helper==2){
            tempNum=atoi(&line[1]);                 //kedze longitude a latitude mame ako string a potrebujeme snimi pracovat ako s cislami, musime ich upravit a zbavit sa znaku +/-
            tempNum=tempNum*10000;
            tempNum2=atoi(&line[4]);
            tempNum+=tempNum2;
            tempNum=tempNum/10000;
            round(tempNum*10000)/10000;
            if(line[0]=='-'){                       //podmienka ak znak je minusovy, musime cislo vynasobit minus 1, aby sme ho dali do zaporneho
                tempNum=tempNum*(-1);
            }
            last->latitude=tempNum;             //to iste robime longitdue 
            tempNum=atoi(&line[9]);
            tempNum=tempNum*10000;
            tempNum2=atoi(&line[12]);
            tempNum+=tempNum2;
            tempNum=tempNum/10000;
            round(tempNum*10000)/10000;
            if(line[8]=='-'){
                tempNum=tempNum*(-1);
            }
            last->longitude=tempNum;
        }else if(helper==3){
            strcpy(last->typ, line);            //pomoocu strcpy zapisujeme typ  a ukladame do link listu pre typ
            //typ ziskame
        }else if(helper==4){
            last->value=atof(&line[0]);            //pomocou atof ziskavame value a ukladame do link listu pre value
            //hodnotu ziskame
        }else if(helper==5){
            last->time=atoi(&line[0]);              //pomoocu atoi ziskavame cas  a ukladame do link listu pre cas
            //cas ziskame
        }else if(helper==6){                 //pomocou atoi ziskavame datum  a ukladame do link listu pre value
            last->date=atoi(&line[0]);
            (*PocetMerani)++;                   //kedze toto je nas posledny udaj, zvysujeme pocet merani o jedno. kedze sme ho prave zapisali
            helper=-1;
        }
        helper++;
    }
    printf("nko hotove\n");
}
void p(RECORD **headPointer, int *PocetMerani){
    int num;
    printf("Zadaj na ktoru poziciu doplnime zaznam: ");
    scanf("%d", &num);
    if(num<0){
        printf("Zadali ste zapornu poziciu, funkcia skoncila\n");         //ak user da zapornu poziciu, funkcia returne
        return;
    }
    char cInput[17];                    //pomocne pole
    RECORD *addedRecord=malloc(sizeof(RECORD));         //alokujem addedrecord
    float tempNum=0;            //pomocne premenne
    float tempNum2=0;
    printf("Zadaj id: ");               //ziskanie idcka
    scanf("%5s", &cInput);
    addedRecord->oznacenie=cInput[0];       //zapisanie do added record oznacenie idcka
    tempNum=tempNum+(cInput[1] -'0');       //premenime na cislo, kedze teraz je to v stringu
    tempNum=tempNum*100;                    //premiename na cislo
    tempNum+=((cInput[2]-'0')*10);
    tempNum+=(cInput[3]-'0');
    addedRecord->cislovanie=tempNum;            //zapisujeme cislo
    addedRecord->druh=cInput[4];                //zapisujeme druh
    printf("Zadajte poziciu: ");                
    scanf("%16s%*c", &cInput);                  //ziskanie pozicie
    tempNum=atoi(&cInput[1]);                   //uprava aby sme to vedeli zapisat ako cislo a nie ako string 
    tempNum=tempNum*10000;
    tempNum2=atoi(&cInput[4]);
    tempNum+=tempNum2;
    tempNum=tempNum/10000;
    round(tempNum*10000)/10000;
    if(cInput[0]=='-'){                     //podmienka ak znak bol negativny, cislo vynasobime -1
        tempNum=tempNum*(-1);
    }
    addedRecord->latitude=tempNum;              //zapisujeme latitude
    tempNum=atoi(&cInput[9]);                   //rovnaka uprava pre longitude 
    tempNum=tempNum*10000;
    tempNum2=atoi(&cInput[12]);
    tempNum+=tempNum2;
    tempNum=tempNum/10000;
    round(tempNum*10000)/10000;
    if(cInput[8]=='-'){
        tempNum=tempNum*(-1);
    }
    addedRecord->longitude=tempNum;             //zapisujeme longitude
    printf("Zadaj mer. velicinu: ");
    scanf("%2s%*c", &(addedRecord->typ));           //ziskanie a zapisanie typu 
    printf("Zadaj hodnotu: ");
    scanf("%f", &(addedRecord->value));                 //ziskanie a zapisanie hodnoty 
    printf("Zadaj cas: ");
    scanf("%d",&(addedRecord->time));                   //ziskanie a zapisanie casu
    printf("Zadaj datum: ");
    scanf("%d",&(addedRecord->date));                   //ziskanie a zapisanie datumu
    (*PocetMerani)++;                                   //zvysujeme pocet merani kedze sme uspesne zapisali novy record
    if(num==1 || *headPointer==NULL){               //ak je pozicia 1., musime na tento udaj nastavit headpointer a linkneme nanho minuly headpointer
        addedRecord->next = *headPointer;       
        *headPointer = addedRecord;
        return;
    }
    RECORD *current = *headPointer;         //pomocna premennna current, zapisujeme do nej headpointer
    if(num>(*PocetMerani)){             //podmienka ak cislo je vacsie ako pocet merani,musiem ho dat na koniec 
        num=(*PocetMerani)+1;
    }
    for(int i=0;i<num-2;i++){       //ziskanie pozicie na zapis 
        if(current->next==NULL){        //ak current next je NULL, to znamena ze sme na konci a for cyklus sa nam breakne
            break;
        }
        current=current->next;             //posuvanie linkov
    }
    addedRecord->next = current->next;          //linkujeme added record ziskanim linkom
    current->next = addedRecord;        //zapis udajov  do linked list 
    printf("p hotove\n");
    return;
}
void z(RECORD **headPointer, int *PocetMerani){
    char Input[6];      //pomocny string
    printf("Zadaj id na vymazanie: ");
    scanf("%5s", &Input);                   //ziskanie idcka na vymazanie 
    char tempOznacenie=Input[0];        //pomocne premenne
    int tempCislovanie=0;
    int exist=0;  //tf ci existuju zaznamy na vymazanie
    tempCislovanie=(tempCislovanie+(Input[1] -'0'))*100;   //uprava na cislo
    tempCislovanie=tempCislovanie+((Input[2] -'0')*10);
    tempCislovanie+=(Input[3] -'0');
    char tempDruh=Input[4];
    RECORD *current=*headPointer;       //pomocnej premennej current davame hodnotu headpointru 
    RECORD *last=NULL;                    //druha pomocna menom last, aby sme do nej vedeli ulozit link, ktory posiela record ktory vymazavame 
    while(current!=NULL){               //podmineka kym current nieje NULL
        if(current->oznacenie==tempOznacenie && current->cislovanie==tempCislovanie && current->druh==tempDruh){     //ked sa zhoduje idcko, ideme do tejto podmienky   
            RECORD *temp=current;       //pomocna premenna temp, linkujeme do nej current 
            if(last==NULL){                 //ak last je null, v zmysle ak vymazavame prvy record, headpointer nastavujeme na dalsi prvok
                *headPointer=current->next; 
                current=*headPointer;
            }else{
                last->next=current->next;      //ak neni last NULL, v zmysle ak neni na prvom mieste  nahradzujeme last currentom
                current=current->next;
            }
            printf("Zaznam pre ID: %s bol vymazany\n", Input);          //printujeme ked sa id vymazalo
            (*PocetMerani)--;                                   //odcitavame pocet merani
            free(temp);                                         //vymazavame temp, aby sme setrili pamat
            exist=1;                                            //tf premennu davame na 1- true
        }else{
            last=current;                   //ak sa nerovnaju idcka, posuvame sa. last davame na current, a current davame dalej
            current=current->next;
        }
    }
    if(exist==0){                           //ak tf premenna je false, to znamena ze sa nic nevymazalo a vypise nam ze nemalo byt co vymazane
        printf("Pre zadane id, nemalo byt co vymazane\n");
    }
    return;
}
void r(RECORD **headPointer, int *PocetMerani){
    int Input1;
    int Input2;
    printf("Zadaj poz. 1. zaznamu: ");
    scanf("%d", &Input1);                   //ziskanie prvej pozicii
    printf("Zadaj poz. 2. zaznamu: ");
    scanf("%d", &Input2);                   //ziskanie druhej pozicii
    if(Input1==Input2){                     //ak su rovnake pozicie, davame return kedze nemenime poziciu 
        return;
    }
    if(Input1>*PocetMerani || Input1<0){                    //podmienka ak takato pozicia neexistuju 
        printf("Taketo pozicie zoznam neobsahuje\n");
        return;
    }
    if(Input2>*PocetMerani || Input2<0){                       //podmienka ak takato pozicia neexistuju                 
        printf("Taketo pozicie zoznam neobsahuje\n");
        return;
    }
    RECORD *last=NULL;              //pomocne premenne
    RECORD *last2=NULL;
    RECORD *current=*headPointer;
    RECORD *current2=*headPointer;
    for(int i=1;current!=NULL && i<Input1;i++){       //najdeme node na prvej pozicii, nastavujeme jeho predoslu poziciu na last, aby sme to vedeli potom linknut
        last=current;
        current=current->next;
    }
    for(int i=1;current2!=NULL && i<Input2;i++){            //najdeme node na druhej pozicii, nastavujeme jeho predoslu poziciu na last2, aby sme to vedeli potom linknut
        last2=current2;
        current2=current2->next;
    }
    if(last!=NULL){             //ak nieje na prvej pozicii, linkujeme nanho nas current2
        last->next=current2;        
    }else{
        *headPointer=current2;      //ak je na prvej pozicii, linkujeme nanho headpointer
    }
    if(last2!=NULL){            //ak nieje na prvej pozicii, linkujeme nanho nas current
        last2->next=current;
    }else{
        *headPointer=current;           //ak je na prvej pozicii, linkujeme nanho headpointer
    }
    RECORD *temp =current2->next;       //temp premenna do ktorej davame current2
    current2->next=current->next;          //vymiename im linky, pomocou temp premenej
    current->next=temp;                    
    printf("rko hotove\n");
}
void u(RECORD **headPointer, int *PocetMerani){
    int input;                  //pomocna premenna pre input  
    if((*PocetMerani)==0){          
        printf("Chyba usporiadania, zoznam je prazdny\n");          //ak je pocet merani nulovy, to znamena ze nema ziadne zaznamy
    }
    printf("Zadaj ci sa ma usporiadat vzostupne=0 alebo zostupne=1: ");         //otazka ako ich ma usporiadat
    scanf("%d", &input);                                                //ziskanie odpoveded
    if (*headPointer == NULL || (*headPointer)->next == NULL){         //ak je iba jeden zaznam, nema co usporaduvat
        return;
    }
    RECORD* current;                //pomocne premennej
    long long int temp1_a;
    int temp1_b;
    long long int temp2_a;
    int temp2_b;
    char tempOznacenie;
    int tempCislovanie;
    char tempDruh;
    float tempLatitude;
    float tempLongitude;
    char tempTyp[4];
    float tempValue;
    int tempTime;
    int tempDate;
    if(input==0){
        for (int i=0;i<(*PocetMerani)-1;i++){                           //bubble sort
            current=*headPointer;
            for (int j=0;j<(*PocetMerani)-i-1;j++) {                //kedze mame usporaduvat podla datumu a casu, musime ich dat na jedno cislo, aby sme ich vedeli porovnat 
                temp1_a=current->date;
                temp1_a=temp1_a*10000;
                temp1_b=current->time;
                temp1_a=temp1_a+temp1_b;
                temp2_a=(current->next)->date;
                temp2_a=temp2_a*10000;
                temp2_b=(current->next)->time;
                temp2_a=temp2_a+temp2_b;
                if (temp1_a > temp2_a) {                                          //porovnavanie ci je treba ich vymenit
                    tempOznacenie=current->oznacenie;
                    tempCislovanie=current->cislovanie;
                    tempDruh=current->druh;
                    tempLatitude=current->latitude;
                    tempLongitude=current->longitude;
                    tempTyp[0]=(current->typ)[0];
                    tempTyp[1]=(current->typ)[1];
                    tempTyp[2]=(current->typ)[2];
                    tempValue=current->value;
                    tempTime=current->time;
                    tempDate=current->date;

                    current->oznacenie = (current->next)->oznacenie;
                    current->cislovanie = (current->next)->cislovanie;
                    current->druh = (current->next)->druh;
                    current->latitude = (current->next)->latitude;
                    current->longitude = (current->next)->longitude;
                    (current->typ)[0] = ((current->next)->typ)[0];
                    (current->typ)[1] = ((current->next)->typ)[1];
                    (current->typ)[2] = ((current->next)->typ)[2];
                    current->value = (current->next)->value;
                    current->date = (current->next)->date;
                    current->time = (current->next)->time;

                    (current->next)->oznacenie=tempOznacenie;
                    (current->next)->cislovanie=tempCislovanie;
                    (current->next)->druh=tempDruh;
                    (current->next)->latitude=tempLatitude;
                    (current->next)->longitude=tempLongitude;
                    ((current->next)->typ)[0]=tempTyp[0];
                    ((current->next)->typ)[1]=tempTyp[1];
                    ((current->next)->typ)[2]=tempTyp[2];
                    (current->next)->value=tempValue;
                    (current->next)->time=tempTime;
                    (current->next)->date=tempDate;
                }
                current = current->next;        //posuvame sa na dalsie cislo 
            }
        }
    }else if(input==1){                                     //robime to iste, iba opacne. kedze davame od najvacsieho po najmensi
        for(int i=0;i<(*PocetMerani)-1;i++){
            current=*headPointer;
            for (int j=0;j<(*PocetMerani)-i-1;j++) {
                temp1_a=current->date;
                temp1_a=temp1_a*10000;
                temp1_b=current->time;
                temp1_a=temp1_a+temp1_b;
                temp2_a=(current->next)->date;
                temp2_a=temp2_a*10000;
                temp2_b=(current->next)->time;
                temp2_a=temp2_a+temp2_b;
                if (temp1_a < temp2_a) {
                    tempOznacenie=current->oznacenie;
                    tempCislovanie=current->cislovanie;
                    tempDruh=current->druh;
                    tempLatitude=current->latitude;
                    tempLongitude=current->longitude;
                    tempTyp[0]=(current->typ)[0];
                    tempTyp[1]=(current->typ)[1];
                    tempTyp[2]=(current->typ)[2];
                    tempValue=current->value;
                    tempTime=current->time;
                    tempDate=current->date;

                    current->oznacenie = (current->next)->oznacenie;
                    current->cislovanie = (current->next)->cislovanie;
                    current->druh = (current->next)->druh;
                    current->latitude = (current->next)->latitude;
                    current->longitude = (current->next)->longitude;
                    (current->typ)[0] = ((current->next)->typ)[0];
                    (current->typ)[1] = ((current->next)->typ)[1];
                    (current->typ)[2] = ((current->next)->typ)[2];
                    current->value = (current->next)->value;
                    current->date = (current->next)->date;
                    current->time = (current->next)->time;

                    (current->next)->oznacenie=tempOznacenie;
                    (current->next)->cislovanie=tempCislovanie;
                    (current->next)->druh=tempDruh;
                    (current->next)->latitude=tempLatitude;
                    (current->next)->longitude=tempLongitude;
                    ((current->next)->typ)[0]=tempTyp[0];
                    ((current->next)->typ)[1]=tempTyp[1];
                    ((current->next)->typ)[2]=tempTyp[2];
                    (current->next)->value=tempValue;
                    (current->next)->time=tempTime;
                    (current->next)->date=tempDate;
                }
                current = current->next;
            }

    }
    }
    printf("Spajany zoznam bol usporiadany\n");
    return;
}
int main(){
    char input;                                      //input pre zadanie funkcie       
    int datalogertxt_open=0;                         //tf premenna pre zistenie ci bol otvoreny dataloger.txt
    int arrayOpen=0;                                 //tf premenna pre zistenie ci boli otvorene dyn. polia
    int arraySize=0;                                 //premenna do ktorej bude ulozena velkost pola
    int PocetMerani=0;
    RECORD *headPointer=NULL;
    printf("Zadaj pismeno: ");
    scanf("%c",&input);                                         //prve ziskanie znaku
    while(input!='k'){                                          //while cyklus pre opatovne pytanie znakov, ak sa stlaci k = while cyklus sa ukonci
        if(input=='v'){                                         //ak nas input bude "v", tak nam spusti funkciu v
            v(&headPointer, &PocetMerani);
        }
        else if(input=='n')                                         //ak nas input bude "n", tak nam spusti funkciu c
        {
            n(&headPointer, &PocetMerani);
        }
        else if(input=='p')                                         //ak nas input bude "p", tak nam spusti funkciu p
        {
            p(&headPointer, &PocetMerani);
        }
        else if(input=='u')                                         //ak nas input bude "u", tak nam spusti funkciu u
        {
            u(&headPointer, &PocetMerani);
        }
        else if(input=='r')                                         //ak nas input bude "r", tak nam spusti funkciu r
        {
            r(&headPointer, &PocetMerani);
        }
        else if(input=='z')                                         //ak nas input bude "z", tak nam spusti funkciu z
        {
            z(&headPointer,&PocetMerani);
        }
        scanf("%c", &input);                                        //ziskanie inputu
    }
    RECORD *current= headPointer;           //pomocne premenne 
    RECORD *next;
    while(current!=NULL){           //while cyklis kym nejsme na konci
        next=current->next;         
        free(current);              //uvolnujeme pamat
        current=next;               //posuvame sa na dalsi prvok
    }
    printf("Pamat uspesne uvolnena, program skoncil");
}