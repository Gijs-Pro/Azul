// Implementatie van klasse Azul

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "azul.h"
using namespace std;

Azul::Azul ()
{
  totaalScore = 0;
}  // default constructor

Azul::Azul (int nwHoogte, int nwBreedte)
{
  //Check of de waardes geldig zijn
  if(!integerInBereik(nwHoogte,1, MaxDimensie) || 
     !(integerInBereik(nwHoogte,1, MaxDimensie))){
    geldigBord = false;
    return;
  }// if
  //Standaard bordt creëeren met de waardes
  hoogte = nwHoogte; breedte = nwBreedte;
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      bord[i][j] = 0;
    }// for
  }// for
  geldigBord = true;
  totaalScore = 0;
}  // constructor met parameters

Azul::~Azul ()
{
  // TODO: implementeer zo nodig deze destructor

}  // destructor


void Azul::testFunctie()
{
  totaalScore = 0;
}

int Azul::getVakje (int rij, int kolom)
{ 
  if(geldigBord && integerInBereik(rij, 0, hoogte-1) && 
                   integerInBereik(kolom, 0, breedte-1)){
    return bord[rij][kolom];
  }// if
  return -1;
}  // getVakje

bool Azul::leesInBord (const char* invoerNaam)
{
  geldigBord = true;
  int getal; // verse ingelezen waarde
  //Het inlezen van het bord
  ifstream fin; //invoer
  fin.open(invoerNaam);
  //check of het bestand wel te openen is
  if(!fin.is_open()){
    cout << "geen geldig bestand" << endl;
    geldigBord = false;
    return geldigBord;
  }// if
  //Hoogte en breedte registreren
  fin >> getal; hoogte = getal; 
  fin >> getal; breedte = getal;
  if(!integerInBereik(hoogte, 1, MaxDimensie) ||
     !integerInBereik(hoogte, 1, MaxDimensie) ){
    geldigBord = false;
    return geldigBord;
  }// if
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      fin >> getal;
      if(getal != 0 && getal != 1){
        geldigBord = false;
        return geldigBord;
      }// if
      bord[i][j] = getal;
    }// for
  }// for
  fin.close();
  return geldigBord;
}  // leesInBord

void Azul::drukAfBord ()
{
  if(geldigBord){
    cout << ' ' << ' ';
    for(int k = 0; k < breedte; k++ ){
      cout << kleurPaars << k << ' ' << kleurWit;
    }// for
    cout << endl;
    for(int i = 0; i < hoogte; i++){
      cout << kleurPaars << i << kleurWit;
      if(i < 10){
        cout << ' ';
      }// if
      for(int j = 0; j < breedte; j++){
        if(!bord[i][j]){cout << 0;}
        else{cout << kleurRood << 1 << kleurWit;}
        cout << ' ';
      }// for
      if(i == hoogte / 2){
        cout << kleurGroen << "     Totaalscore: "  << totaalScore << kleurWit;
      }// if
      cout << endl;
    }// for 
  }// for
}  // drukAfBord

bool Azul::magZet (pair<int,int> coords)
{
  if( integerInBereik(coords.first, 0, hoogte-1) && 
      integerInBereik(coords.second, 0, breedte-1) &&
      bord[coords.first][coords.second] == 0){
    return true;
  }// if
  return false;
}  // magZet

int Azul::verwerkZet (pair<int,int> coords)
{
  int scoreV = 1;//score voor de rij
  int scoreH = 1;//score voor de kolom
  int rijWalker = coords.first;//gaat lopen door de rij
  int kolomWalker = coords.second;//gaat lopen door de kolom
  int ophoogScore = 0;//op te hogen score
  //scoreV en scoreH bepalen:
  //walken omhoog:
  while(rijWalker != 0 && bord[rijWalker-1][kolomWalker] == 1){
    scoreV++;
    rijWalker--;
  }// while
  rijWalker = coords.first;//resetten voor de volgende while
  //walken omlaag:
  while(rijWalker != hoogte - 1 && bord[rijWalker + 1][kolomWalker] == 1){
    scoreV++;
    rijWalker++;
  }// while
  rijWalker = coords.first;//resetten voor de volgende while
  //walken links;
  while(kolomWalker != 0 && bord[rijWalker][kolomWalker - 1] == 1){
    scoreH++;
    kolomWalker--;
  }// while
  kolomWalker = coords.second;//resetten voor de volgende while
  //walken rechts;
  while(kolomWalker != breedte - 1 && bord[rijWalker][kolomWalker + 1] == 1){
    scoreH++;
    kolomWalker++;
  }// while

  //gevalsonderscheiding voor het ophogen van ophoogScore
  if(scoreH == 1){
    ophoogScore += scoreV;
  }// if
  else if(scoreV == 1){
    ophoogScore += scoreH;
  }// else if
  else{
    ophoogScore += scoreV + scoreH;
  }// if
  return ophoogScore;
}  // verwerkZet

bool Azul::doeZet (int rij, int kolom)
{
  if(geldigBord){
    pair<int,int> coords = make_pair(rij,kolom);//helaas mocht de header file
                                                //niet aangepast worden, dus
                                                //converteren we hier naar pair
    if(magZet(coords)){
      vZetten.push_back(coords);
      bord[rij][kolom] = 1;
      totaalScore += verwerkZet(coords);
      return true;
    }// if
  }// if
  return false;
}  // doeZet

bool Azul::unDoeZet ()
{
  if(vZetten.size() != 0 || !geldigBord){
    bord[vZetten.back().first][vZetten.back().second] = 0;
    totaalScore -= verwerkZet(vZetten.back());
    vZetten.pop_back();
    return true;
  }// if
  return false;
}  // unDoeZet

bool Azul::bordVol ()
{ 
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      if(bord[i][j] == 0){
        return false;
      }// if
    }// for
  }// for
  return true;
}  // bordVol

bool Azul::bepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(geldigBord){
    //We hebben een manier nodig om alle bedekkingen van het bord af te gaan
    //Deze aanroep is de eerste, initialiseren de waardes,roepen de functie 
    //aan en returnen de waardes:
    mini = INT_MAX; maxi = 0; volgordesMini = 0; volgordesMaxi = 0;
    bepaalMiniMaxiScoreRec_p(mini, volgordesMini, maxi, volgordesMaxi);
    return true;
  }// if
  cout << "geen geldig bord" << endl;
  return false;

}  // bepaalMiniMaxiScoreRec

void Azul::bepaalMiniMaxiScoreRec_p (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(bordVol()){
    if(totaalScore == maxi){
      volgordesMaxi++;
    }// if
    if(totaalScore > maxi){
      maxi = totaalScore;
      volgordesMaxi = 1;
    }// if
    if(totaalScore == mini){
      volgordesMini++;
    }// if
    if(totaalScore < mini){
      mini = totaalScore;
      volgordesMini = 1;
    }// if
    return;
  }// if
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      if(magZet(make_pair(i,j))){
        doeZet(i,j);
        bepaalMiniMaxiScoreRec_p(mini, volgordesMini, maxi, volgordesMaxi);
        unDoeZet();
      }// if
    }// for
  }// for
  return;
}  // bepaalMiniMaxiScoreRec_p

void Azul::vulTabelDefault (long long tabel[][5], int getal, int verbWaardes)
{
  for(int i = 0; i < getal+1; i++){//tabel invullen
    for(int j = 0; j < 5; j++){
      if(j == 0){tabel[i][j] = INT_MAX;}
      else{tabel[i][j] = 0;}
    }// for
  }// for
  //we verwerken alvast de beginstap
  tabel[verbWaardes][0] = 0; tabel[verbWaardes][1] = 1;
  tabel[verbWaardes][2] = 0; tabel[verbWaardes][3] = 1; 
  tabel[verbWaardes][4] = 1;
}  // vulTabelDefault

bool Azul::bepaalMiniMaxiScoreTD (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi)
{
  if(geldigBord){
    int teller = 0;//teller voor verboden waardes
    int grootte = hoogte * breedte;//aantal vakjes van het bord
    int getal = (1<<grootte) - 1; //binaire waarde van het bord, grootte van:
    long long tabel[getal+1][5];// mini-volgordesmini-maxi-volgordes maxi-gehad
    int verbWaardes = 0;//gaat het binaire getal van het huidige bord bevatten
    for(int i = 0; i < hoogte; i++){// we vullen eerst het hele bord
      for(int j = 0; j < breedte; j++){
        if(!bord[i][j]){
          doeZet(i,j);
          teller++;
        }// if
        else{
          //verboden waarde gevonden, dit maken we een 1 in de binaire waarde
          verbWaardes = verbWaardes | (1<<(i*breedte + j));
        }// else
      }// for
    }// for
    //we geven de tabel default waardes en de startwaardes
    vulTabelDefault(tabel, getal, verbWaardes);
    //recursieve aanroep vult de tabel in
    bepaalMiniMaxiScoreTD_p (tabel,getal,verbWaardes);
    //zetten netjes ongedaan maken
    for(int i = 0; i < teller; i++){
      unDoeZet();
    }// for
    //waardes van de laatste rij van de tabel verwerken
    mini = tabel[getal][0] + totaalScore;
    volgordesMini = tabel[getal][1];
    maxi = tabel[getal][2] + totaalScore;
    volgordesMaxi = tabel[getal][3];
    return true;
  }// if
  cout << "geen geldig bord" << endl;
  return false;
}  // bepaalMiniMaxiScoreTD

void Azul::zetInTabel (int getal, int tempGetal, int verschil, 
                       long long tabel[][5])
{
  if(tabel[tempGetal][2] + verschil > tabel[getal][2]){
    tabel[getal][2] = tabel[tempGetal][2] + verschil;//tabeldata updaten
    tabel[getal][3] = tabel[tempGetal][3]; //aantal manieren updaten
  }// if
  //als deze zogenaamde zet + zijn waarde gelijk is aan dat in de tabel:
  else if(tabel[tempGetal][2] + verschil == tabel[getal][2]){
    tabel[getal][3] += tabel[tempGetal][3];//aantal manieren ophogen
  }// else if
  //analoog voor minimum, waardes in tabel zijn INT_MAX als default waarde
  if(tabel[tempGetal][0] + verschil < tabel[getal][0]){
    tabel[getal][0] = tabel[tempGetal][0] + verschil;
    tabel[getal][1] = tabel[tempGetal][1]; 
  }// if
  else if(tabel[tempGetal][0] + verschil == tabel[getal][0]){
    tabel[getal][1] += tabel[tempGetal][1];
  }// else if
}  // zetInTabel

void Azul::bepaalMiniMaxiScoreTD_p (long long tabel[][5], int getal, int verbWaardes)
{
  int tempGetal;//bord waarmee vergeleken wordt
  int pos;// positie in binaire getal
  int verschil;//waarde van een zet tussen de borden getal en tempGetal
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      pos = i*breedte + j;//hoeveelste bit veranderd in het binaire getal
      if(bord[i][j] && ((verbWaardes>>pos) % 2) != 1){
        verschil = verwerkZet(make_pair(i,j));//waarde van de huidige "zet"
        tempGetal = getal & ~(1<<pos);//opslaan binaire getal van dit bord
        if(tabel[tempGetal][4] != 1){// als deze stand nog niet verwerkt is
          bord[i][j] = false; //"undo zet" voor het huidige vakje
          totaalScore -= verschil;//totaalscore ook updaten
          bepaalMiniMaxiScoreTD_p(tabel, tempGetal, verbWaardes);
          doeZet(i,j);//zet weer terug doen
          vZetten.pop_back();//maar niet opslaan in de vector
        }// if
        //als deze zogenaamde zet + zijn waarde groter is dan dat in de tabel:
        zetInTabel(getal, tempGetal, verschil, tabel);
      }// if
    }// for
  }// for
  tabel[getal][4] = 1;// de waardes hier in de tabel zullen altijd goed zijn.
}  // bepaalMiniMaxiScoreTD_p

void Azul::drukaftabel(long long tabel[][5]){//testfunctie voor debuggen
  for(int i = 0; i < (1<<breedte*hoogte); i++){
    cout << i << ' ';
    for(int j = 0; j < 5; j++){
      cout << tabel[i][j] << " ";
    }
    cout << endl;
  }
}

bool Azul::bepaalMiniMaxiScoreBU (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi,
                                  vector< pair<int,int> > &zettenReeksMini,
                                  vector< pair<int,int> > &zettenReeksMaxi)
{
  if(geldigBord){
    int grootte = hoogte*breedte;//aantal vakjes
    int getal = (1<<grootte) - 1;//aantal rijen van de tabel
    long long tabel[getal+1][5];// mini-volgordesmini-maxi-volgordes maxi-gehad
    int verschil;//verschil tussen fase getalMet en getalZonder
    int zoveelsteBit;//waarde van bit k
    int getalMet,getalZonder;//bord met/zonder huidige zet
    int rij, kolom;//coordinaten voor de zet
    int teller = 0;//teller die meerdere malen gebruikt wrodt
    int verbWaardes = 0;//hierin komt de waarde van het startbord
    int aantalVerbWaardes = 0;//aantal 1en in verbodenwaardes
    vector <int> aantalTegels[grootte+1];//de index representeert het aantal
                                         //gelegde tegels voor een bepaald bord
    bool toegestaan;//gaat checken of een te evalueren zet wel nodig is
    for(int i = 0; i < hoogte; i++){//(aantal) verbodenwaardes initialiseren
      for(int j = 0; j < breedte; j++){
        if(bord[i][j]){
          verbWaardes = verbWaardes | (1<<(i*breedte + j));
          aantalVerbWaardes++;
        }// if
      }// for
    }// for
    vulTabelDefault(tabel, getal, verbWaardes);//tabel default gevuld
    //alle mogelijke borden met bepaald aantal gelegde tegels in array stoppen
    for(int i = 0; i <= getal; i++){//i wordt aantal mogelijke borden
      teller = 0;
      for(int j = 0; j < grootte; j++){//j itereert door de bits van i
        if((i>>j) % 2 == 1){
          teller++;
        }// if
      }// for
      if(teller > aantalVerbWaardes){//alleen aantal tegels > begin bord pakken
        aantalTegels[teller].push_back(i);
      }// if
    }// for
    //nu de tabel bottom up vullen door de array omhoog af te lopen, met deze
    //volgorde is elke waarde die in de tabel wordt ingevuld bij evaluatie van
    //een bepaald bord (oftewel alle tegels individueel weghalen en vergelijken)
    //vast en kan deze gebruikt worden voor een stap verder in aantalTegels
    for(int i = aantalVerbWaardes+1; i <= grootte; i++){//itereren over aantalTegels[i]
      for(int j = 0; j < aantalTegels[i].size(); j++){// een binair getal met i tegels
        getalMet = aantalTegels[i].at(j);
        teller = 0;
        toegestaan = true;
        for(int k = 0; k < grootte; k++){//checken of de zet ingevuld mag worden
          if((verbWaardes>>k) % 2 == 1 && (getalMet>>k) % 2 == 0){
            toegestaan = false;
          }// if
        }// for
        if(toegestaan){//zet heeft geen 0 bij een vakje die in het
                       //bord een 1 was, dus de zet mag geëvalueerd worden
          for(int k = 0; k < grootte; k++){//alle bits die 1 zijn als zet doen
            zoveelsteBit = (getalMet>>k) % 2;
            if(zoveelsteBit == 1 && ((verbWaardes>>k) % 2) != 1){
              rij = k / breedte; kolom = k % breedte;//coords uit bit halen
              doeZet(rij, kolom);
              teller++;//voor aantal zetten terug verderop in de code
            }// if
          }// for
          //bordstand is goed om verschil aantallen met verwerkzet te krijgen.
          //nu elk ingevuld vakje "weghalen" en opslaan als getalZonder, daarna
          //tabelplek van getalMet invullen met behulp van getalZonder
          for(int k = 0; k < grootte; k++){//positie van het bord
            zoveelsteBit = (aantalTegels[i].at(j)>>k) % 2;
            if(zoveelsteBit == 1 && ((verbWaardes>>k) % 2) != 1){
              rij = k / breedte; kolom = k % breedte;//coords uit bit halen
              verschil = verwerkZet(make_pair(rij, kolom));//verwerkzet op bit k
              getalZonder = getalMet & ~(1<<k);//zet weghalen voor nieuw bord
              zetInTabel(getalMet, getalZonder, verschil, tabel);//verwerken
            }// if
          }// for
          //klaar met tabel invullen voor bord j, nu undoen tot beginbord.
          for(int k = 0; k < teller; k++){
            unDoeZet();//netjes terug naar het begin bord
          }// for
        }// if
      }// for
    }// for
    //nu de laatste rij van de tabel aflezen voor de informatie
    mini = tabel[getal][0] + totaalScore;
    volgordesMini = tabel[getal][1];
    maxi = tabel[getal][2] + totaalScore;
    volgordesMaxi = tabel[getal][3];
    //nu tabel af is de reeksen vinden
    vindReeksen(tabel, getal, verbWaardes, aantalTegels, 
                zettenReeksMini, zettenReeksMaxi);
    return true;
  }
  else{//bord was niet geldig
    return false;
  }
}  // bepaalMiniMaxiScoreBU

void Azul::vindReeksen (long long tabel[][5], int getal, int verbWaardes,
                        vector <int> aantalTegels[],
                        vector< pair<int,int> > &zettenReeksMini,
                        vector< pair<int,int> > &zettenReeksMaxi)
{
  int evaluatieBord = getal;// wordt het bord waar we vanuit werken, eerst vol
  int k;//k gaat door de bits van evaluatiebord lopen
  int waardeCheck;//huidige te evalueren bord (met -1 tegel tov evaluatiebord)
  int huidigeBesteVerschil;//beste voor mini/maxi, dus slechtste voor mini
  int potentieelVerschil;
  int huidigeBesteZet; //deze zet wordt opgeslagen als een binair getal
  pair<int,int> huidigeBesteCoords;
  //eerst zettenReeksMaxi:
  while(evaluatieBord != verbWaardes){//itereren over de borden
    cout <<"evaluatieBord is ";
    cout << evaluatieBord << endl;
    k = 0;//weer vooraan beginnen
    huidigeBesteVerschil = 0;//we zoeken een verse nieuwe beste waarde
    while(k < hoogte*breedte){//itereren over de bits van evaluatiebord
      if((evaluatieBord>>k) % 2 == 1 && (verbWaardes>>k) % 2 != 1){
        //waarde bekijken bij getal als bit k van evaluatieBord 0 was:
        waardeCheck = evaluatieBord & ~(1<<k);
        cout << "waardeCheck is ";
        cout << waardeCheck << endl;
        cout << "huidigeBesteVerschil is ";
        cout << huidigeBesteVerschil << endl;
        potentieelVerschil = tabel[evaluatieBord][2] - tabel[waardeCheck][2];
        cout << "potentieelVerschil is ";
        cout << potentieelVerschil << endl;
        if(potentieelVerschil > huidigeBesteVerschil){//kijken of de zet beter is
          huidigeBesteVerschil = potentieelVerschil;//nieuwe drempel
          huidigeBesteZet = waardeCheck;
          huidigeBesteCoords.first = k / breedte;//nieuwe coordinaat opslaan
          huidigeBesteCoords.second = k % breedte;        }
      }
      k++;
    }
    zettenReeksMaxi.insert(zettenReeksMaxi.begin(), huidigeBesteCoords);
    evaluatieBord = huidigeBesteZet;
  }
  cout << "klaar met maxi, nu mini" << endl;
  drukaftabel(tabel);
  evaluatieBord = getal;
  //nu zettenreeksMini
  while(evaluatieBord != verbWaardes){//itereren over de borden
    k = 0;//weer vooraan beginnen
    huidigeBesteVerschil = INT_MAX;//we zoeken een verse nieuwe slechste waarde
    while(k < hoogte*breedte){//itereren over de bits van evaluatiebord
      if((evaluatieBord>>k) % 2 == 1 && (verbWaardes>>k) % 2 != 1){
        //waarde bekijken bij getal als bit k van evaluatieBord 0 was:
        waardeCheck = evaluatieBord & ~(1<<k);
        if(tabel[evaluatieBord][0] - tabel[waardeCheck][0]
           < huidigeBesteVerschil){//kijken of de zet beter is
          huidigeBesteVerschil = tabel[waardeCheck][2];//nieuwe drempel
          cout << "nieuw beste bord gevonden, huidigeBesteZet wordt " << waardeCheck << endl;
          huidigeBesteZet = waardeCheck;
          huidigeBesteCoords.first = k / breedte;//nieuwe coordinaat opslaan
          huidigeBesteCoords.second = k % breedte;        }
      }
      k++;
    }
    zettenReeksMini.insert(zettenReeksMini.begin(), huidigeBesteCoords);
    evaluatieBord = huidigeBesteZet;
  }
}
// mini-volgordesmini-maxi-volgordes maxi-gehad

void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  cout << "voor een " << kleurRood << "minimale" << kleurWit;
  cout << " score doet u de zetten:";
  for(int i = 0; i < zettenReeksMini.size(); i++){
    if(i != 0 && i != zettenReeksMini.size()){cout << '-';}
    cout << " (" << zettenReeksMini[i].first << ',' <<zettenReeksMini[i].second;
    cout << ") ";
  }
  cout << endl;
  cout << "voor een " << kleurGroen << "maximale" << kleurWit;
  cout << " score doet u de zetten:";
  for(int i = 0; i < zettenReeksMaxi.size(); i++){
    if(i != 0 && i != zettenReeksMaxi.size()){cout << '-';}
    cout << " (" << zettenReeksMaxi[i].first << ',' <<zettenReeksMaxi[i].second;
    cout << ") ";
  }
  cout << endl;
  zettenReeksMini.clear(); zettenReeksMaxi.clear();//vectoren legen
}  // drukAfZettenReeksen

