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
  }
  //Standaard bordt creëeren met de waardes
  hoogte = nwHoogte; breedte = nwBreedte;
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      bord[i][j] = 0;
    }
  }
  geldigBord = true;
  totaalScore = 0;
}  // constructor met parameters

Azul::~Azul ()
{
  // TODO: implementeer zo nodig deze destructor

}  // destructor


void Azul::testFunctie()
{
  // int nieuweScore;
  // cout << "vul de nieuwe totaalScore in:" << endl;
  // cin >> nieuweScore;
  // totaalScore = nieuweScore; 
  int bit = 9;
  cout << (bit>>0) % 2 << endl;
  cout << (bit>>1) % 2 << endl;
  cout << (bit>>2) % 2 << endl;
  cout << (bit>>3) % 2 << endl;

}

int Azul::getVakje (int rij, int kolom)
{ 
  if(geldigBord && integerInBereik(rij, 0, hoogte-1) && 
                   integerInBereik(kolom, 0, breedte-1)){
    return bord[rij][kolom];
  }
  return -1;
}  // getVakje

bool Azul::leesInBord (const char* invoerNaam)
{
  geldigBord = true;
  int getal;
  //Het inlezen van het bord
  ifstream fin;
  fin.open(invoerNaam);
  //check of het bestand wel te openen is
  if(!fin.is_open()){
    cout << "geen geldig bestand" << endl;
    geldigBord = false;
    return geldigBord;
  }
  //Hoogte en breedte registreren
  fin >> getal; hoogte = getal; 
  fin >> getal; breedte = getal;
  if(!integerInBereik(hoogte, 1, MaxDimensie) ||
     !integerInBereik(hoogte, 1, MaxDimensie) ){
    geldigBord = false;
    return geldigBord;
  }
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      fin >> getal;
      if(getal != 0 && getal != 1){
        geldigBord = false;
        return geldigBord;
      }
      bord[i][j] = getal;
    }
  }
  fin.close();
  return geldigBord;
}  // leesInBord

void Azul::drukAfBord ()
{
  if(geldigBord){
    cout << ' ' << ' ';
    for(int k = 0; k < breedte; k++ ){
      cout << kleurPaars << k << ' ' << kleurWit;
    }
    cout << endl;
    for(int i = 0; i < hoogte; i++){
      cout << kleurPaars << i << kleurWit;
      if(i < 10){
        cout << ' ';
      }
      for(int j = 0; j < breedte; j++){
        if(!bord[i][j]){cout << 0;}
        else{cout << kleurRood << 1 << kleurWit;}
        cout << ' ';
      }
      if(i == hoogte / 2){
        cout << kleurGroen << "     Totaalscore: "  << totaalScore << kleurWit;
      }
      cout << endl;
    } 
  }
}  // drukAfBord

bool Azul::magZet(pair<int,int> coords)
{
  if( integerInBereik(coords.first, 0, hoogte-1) && 
      integerInBereik(coords.second, 0, breedte-1) &&
      bord[coords.first][coords.second] == 0){
    return true;
  }
  return false;
}

int Azul::verwerkZet(pair<int,int> coords)
{
  int scoreV = 1;
  int scoreH = 1;
  int rijWalker = coords.first;
  int kolomWalker = coords.second;
  int ophoogScore = 0;
  //scoreV en scoreH bepalen:
  //walken omhoog:
  while(rijWalker != 0 && bord[rijWalker-1][kolomWalker] == 1){
    scoreV++;
    rijWalker--;
  }
  rijWalker = coords.first;//resetten voor de volgende while
  //walken omlaag:
  while(rijWalker != hoogte - 1 && bord[rijWalker + 1][kolomWalker] == 1){
    scoreV++;
    rijWalker++;
  }
  rijWalker = coords.first;//resetten voor de volgende while
  //walken links;
  while(kolomWalker != 0 && bord[rijWalker][kolomWalker - 1] == 1){
    scoreH++;
    kolomWalker--;
  }
  kolomWalker = coords.second;//resetten voor de volgende while
  //walken rechts;
  while(kolomWalker != breedte - 1 && bord[rijWalker][kolomWalker + 1] == 1){
    scoreH++;
    kolomWalker++;
  }

  //gevalsonderscheiding voor het ophogen van ophoogScore
  if(scoreH == 1){
    ophoogScore += scoreV;
  }
  else if(scoreV == 1){
    ophoogScore += scoreH;
  }
  else{
    ophoogScore += scoreV + scoreH;
  }
  return ophoogScore;
}

bool Azul::doeZet (int rij, int kolom)
{
  if(geldigBord){
    pair<int,int> coords = make_pair(rij,kolom);
    if(magZet(coords)){
      vZetten.push_back(coords);
      bord[rij][kolom] = 1;
      totaalScore += verwerkZet(coords);
      return true;
    }
  }
  return false;
}  // doeZet

bool Azul::unDoeZet ()
{
  if(vZetten.size() != 0 || !geldigBord){
    bord[vZetten.back().first][vZetten.back().second] = 0;
    totaalScore -= verwerkZet(vZetten.back());
    vZetten.pop_back();
    return true;
  }
  return false;
}  // unDoeZet

bool Azul::bordLeeg ()
{ 
    for(int i = 0; i < hoogte; i++){
        for(int j = 0; j < breedte; j++){
            if(bord[i][j] == 1){
                return false;
            }
        }
    }
    return true;
}

bool Azul::bordVol ()
{ 
    for(int i = 0; i < hoogte; i++){
        for(int j = 0; j < breedte; j++){
            if(bord[i][j] == 0){
                return false;
            }
        }
    }
    return true;
}

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
  }
  cout << "geen geldig bord" << endl;
  return false;

}  // bepaalMiniMaxiScoreRec

void Azul::bepaalMiniMaxiScoreRec_p (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(bordVol()){
    if(totaalScore == maxi){
      volgordesMaxi++;
    }
    if(totaalScore > maxi){
      maxi = totaalScore;
      volgordesMaxi = 1;
    }
    if(totaalScore == mini){
      volgordesMini++;
    }
    if(totaalScore < mini){
      mini = totaalScore;
      volgordesMini = 1;
    }
    return;
  }
  for(int i = 0; i < hoogte; i++){
    for(int j = 0; j < breedte; j++){
      if(magZet(make_pair(i,j))){
        doeZet(i,j);
        bepaalMiniMaxiScoreRec_p(mini, volgordesMini, maxi, volgordesMaxi);
        unDoeZet();
      }
    }
  }
  return;
}

void Azul::vulTabelDefault(long long tabel[][5], int getal)
{
  for(int i = 0; i < getal+1; i++){//tabel invullen
    for(int j = 0; j < 5; j++){
      if(j == 0){tabel[i][j] = INT_MAX;}
      else{tabel[i][j] = 0;}
    }// for
  }// for
}

bool Azul::bepaalMiniMaxiScoreTD (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi)
{
  if(geldigBord){
    int teller = 0;
    int grootte = hoogte * breedte;
    int getal = (1<<grootte) - 1; //1 1 1 1 1 1 1 1 1
    long long tabel[getal+1][5];// mini-volgordesmini-maxi-volgordes maxi-gehad
    int verbWaardes = 0;
    //we geven de tabel default waardes
    vulTabelDefault(tabel, getal);
    for(int i = 0; i < hoogte; i++){// we vullen eerst het hele bord
      for(int j = 0; j < breedte; j++){
        if(!bord[i][j]){
          doeZet(i,j);
          teller++;
        }
        else{
          //verboden waarde gevonden, dit maken we een 1 in de binaire waarde
          verbWaardes = verbWaardes | (1<<(i*breedte + j));
        }
      }// for
    }// for
    //we verwerken alvast de beginstap
    tabel[verbWaardes][0] = 0; tabel[verbWaardes][1] = 1;
    tabel[verbWaardes][2] = 0; tabel[verbWaardes][3] = 1; 
    tabel[verbWaardes][4] = 1;
    //recursieve aanroep vult de tabel in
    bepaalMiniMaxiScoreTD_p (tabel,getal,verbWaardes);
    //zetten netjes ongedaan maken
    for(int i = 0; i < teller; i++){
      unDoeZet();
    }
    //waardes van de laatste rij van de tabel verwerken
    mini = tabel[getal][0] + totaalScore;
    volgordesMini = tabel[getal][1];
    maxi = tabel[getal][2] + totaalScore;
    volgordesMaxi = tabel[getal][3];
    return true;
  }
  cout << "geen geldig bord" << endl;
  return false;
}  // bepaalMiniMaxiScoreTD

void Azul::zetInTabel(int getal, int tempGetal, int verschil, long long tabel[][5]){
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
}

void Azul::bepaalMiniMaxiScoreTD_p (long long tabel[][5], int getal, int verbWaardes)
{
  int tempGetal;
  int pos;
  int verschil;
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
        
      }
    }
  }
  tabel[getal][4] = 1;// de waardes hier in de tabel zullen altijd goed zijn.
}

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
    long long tabel[getal+1][5];
    int verschil;//verschil tussen fase getalMet en getalZonder
    int zoveelsteBit;//waarde van bit k
    int getalZonder;//bord zonder huidige zet
    int getalMet;//bord met huidige zet
    int rij, kolom;//coordinaten voor de zet
    int teller = 0;//teller die meerdere malen gebruikt wrodt
    int verbWaardes = 0;//hierin komt de waarde van het startbord
    int aantalVerbWaardes = 0;//aantal 1en in verbodenwaardes
    vector <int> aantalTegels[grootte+1];
    bool toegestaan;
    for(int i = 0; i < hoogte; i++){//verbodenwaardes initialiseren
      for(int j = 0; j < breedte; j++){
        if(bord[i][j]){
          verbWaardes = verbWaardes | (1<<(i*breedte + j));
          aantalVerbWaardes++;
        }
      }
    }
    vulTabelDefault(tabel, getal);//tabel default gevuld, alleen nog beginstap
    tabel[verbWaardes][0] = 0; tabel[verbWaardes][1] = 1;
    tabel[verbWaardes][2] = 0; tabel[verbWaardes][3] = 1; 
    tabel[verbWaardes][4] = 1;
    // cout << kleurRood << aantalVerbWaardes << kleurWit << endl;
    for(int i = 0; i <= getal; i++){//i wordt aantal mogelijke borden
      // cout << i << endl;
      teller = 0;
      for(int j = 0; j < grootte; j++){//j itereert door de bits van i
        if((i>>j) % 2 == 1){
          teller++;
          // cout << "teller " << teller << " opgehoogd, bij waarde " << j << "in het binaire getal " << i << endl;
        }
      }
      if(teller > aantalVerbWaardes){//alleen aantal tegels > begin bord pakken
        aantalTegels[teller].push_back(i);
      } 
    }

    // cout << "getal is " << getal << endl;
    // cout << "aantalTegels[grootte-1].size() is " << aantalTegels[grootte-1].size() << endl;
    // cout << "grootte is " << grootte << endl;
    // cout << "verbodenwaardes is " << verbWaardes << endl; 
    for(int i = aantalVerbWaardes+1; i <= grootte; i++){//itereren over aantalTegels[i]
      // cout << kleurGroen << "nieuwe iteratie met i = " << i << kleurWit << endl;
      // for(int s = 0; s < aantalTegels[i].size(); s++){cout << aantalTegels[i].at(s) << " - ";}
      // cout << endl;
      for(int j = 0; j < aantalTegels[i].size(); j++){// een binair getal met i tegels
        getalMet = aantalTegels[i].at(j);
        teller = 0;
        toegestaan = true;
        for(int k = 0; k < grootte; k++){//checken of de zet ingevuld mag worden
          if((verbWaardes>>k) % 2 == 1 && (getalMet>>k) % 2 == 0){
            toegestaan = false;
          }
        }
        if(toegestaan){
          for(int k = 0; k < grootte; k++){
            zoveelsteBit = (getalMet>>k) % 2;
            // cout << "k is " << k << " met zoveelsteBit is " << zoveelsteBit << " en verbWaardes>>k %2 dinges is " << ((verbWaardes>>k) % 2) << endl;
            if(zoveelsteBit == 1 && ((verbWaardes>>k) % 2) != 1){
              rij = k / breedte;
              kolom = k % breedte;
              doeZet(rij, kolom);
              teller++;
            }
          }
          //bordstand is goed om verschil aantallen met verwerkzet te krijgen.
          //nu klaarmaken om tabel in te vullen
          for(int k = 0; k < grootte; k++){//positie van het bord
            zoveelsteBit = (aantalTegels[i].at(j)>>k) % 2;
            // cout << kleurPaars << "zoveelste bit is " << zoveelsteBit << ", en de huidige vectorwaarde is " << aantalTegels[i].at(j) << kleurWit << endl;
            if(zoveelsteBit == 1 && ((verbWaardes>>k) % 2) != 1){
              rij = k / breedte;
              kolom = k % breedte;
              verschil = verwerkZet(make_pair(rij, kolom));
              getalZonder = getalMet & ~(1<<k);//bit 1 0 0 1 wordt 1 0 0 0 met k = 3 (als niet werkt: draai om!)
              zetInTabel(getalMet, getalZonder, verschil, tabel);
              // cout << kleurRood << " net tabel geupdate met zet " << kleurWit << "(" << rij << ',' << kolom << ")" << endl;
            }
          }
          //klaar met tabel invullen voor bord j, nu undoen tot beginbord.
          // cout << "bord voor undo" << endl;
          // drukAfBord();
          for(int k = 0; k < teller; k++){
            unDoeZet();
          }
          // cout << "bord na undo" << endl;
          // drukAfBord();
        }
      }
    }
    mini = tabel[getal][0] + totaalScore;
    volgordesMini = tabel[getal][1];
    maxi = tabel[getal][2] + totaalScore;
    volgordesMaxi = tabel[getal][3];
    return true;
  }
  else{
    return false;
  }
}  // bepaalMiniMaxiScoreBU


void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfZettenReeksen

// if(tabel[getal][2] + verschil > tabel[tempGetal][2]){
//               tabel[tempGetal][2] = tabel[getal][2] + verschil;//tabeldata updaten
//               tabel[tempGetal][3] = tabel[getal][3]; //aantal manieren updaten
//               }// if
//               //als deze zogenaamde zet + zijn waarde gelijk is aan dat in de tabel:
//               else if(tabel[getal][2] + verschil == tabel[tempGetal][2]){
//                 tabel[tempGetal][3] += tabel[getal][3];//aantal manieren ophogen
//               }// else if
//               //analoog voor minimum, waardes in tabel zijn INT_MAX als default waarde
//               if(tabel[getal][0] + verschil < tabel[tempGetal][0]){
//                 tabel[tempGetal][0] = tabel[getal][0] + verschil;
//                 tabel[tempGetal][1] = tabel[getal][1]; 
//               }// if
//               else if(tabel[getal][0] + verschil == tabel[tempGetal][0]){
//                 tabel[tempGetal][1] += tabel[getal][1];
//               }// else if

// int grootte = hoogte * breedte;
//     int getal = (1<<grootte) - 1;
//     long long tabel[getal+1][5];
//     int teller = 0;
//     int loper;
//     int aantalMogVakjes = grootte;
//     int verschil;
//     int tempGetal;
//     int pos;
//     int verbWaardes = 0;
//     //verboden waardes maken
//     for(int i = 0; i < hoogte; i++){// we vullen eerst het hele bord
//       for(int j = 0; j < breedte; j++){
//         if(bord[i][j]){
//           //verboden waarde gevonden, dit maken we een 1 in de binaire waarde
//           verbWaardes = verbWaardes | (1<<(i*breedte + j));
//           aantalMogVakjes--;
//         }// if
//       }// for
//     }// for

//     //we geven de tabel default waardes
//     for(int i = 0; i < getal+1; i++){//tabel invullen
//       for(int j = 0; j < 5; j++){
//         if(j == 0){tabel[i][j] = INT_MAX;}
//         else{tabel[i][j] = 0;}
//       }// for
//     }// for

//     //we verwerken alvast de beginstap
//     tabel[verbWaardes][0] = 0; tabel[verbWaardes][1] = 1;
//     tabel[verbWaardes][2] = 0; tabel[verbWaardes][3] = 1; 
//     tabel[verbWaardes][4] = 1;
//     cout << verbWaardes;
//     drukaftabel(tabel);
//     while(teller < aantalMogVakjes){
//       loper = 0;
//        while(loper <= teller){
//         for(int i = 0; i < hoogte; i++){
//           for(int j = 0; j < breedte; j++){
//             pos = i * breedte + j;
//             if(bord[i][j] == 0 && ((verbWaardes>>pos) % 2) != 1){
//               tempGetal = getal & ~(1<<pos);
//               verschil = verwerkZet(make_pair(i,j));
//               doeZet(i,j);
//               unDoeZet();
//             }

//           }
//         }
//         loper++;
//       }

//       teller++;
//     }
//     mini = tabel[getal][0] + totaalScore;
//     volgordesMini = tabel[getal][1];
//     maxi = tabel[getal][2] + totaalScore;
//     volgordesMaxi = tabel[getal][3];
//     drukaftabel(tabel);
//     return true;