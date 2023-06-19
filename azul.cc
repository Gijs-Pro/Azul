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
  return;
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
        cout << kleurGroen << "     Totaalscore: "  << totaalScore;
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

void Azul::verwerkZet(pair<int,int> coords)
{
  int scoreV = 1;
  int scoreH = 1;
  int rijWalker = coords.first;
  int kolomWalker = coords.second;
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

  //gevalsonderscheiding voor het ophogen van totaalScore
  if(scoreH == 1){
    vScoreOphogingen.push_back(scoreV);
    totaalScore += scoreV;
  }
  else if(scoreV == 1){
    vScoreOphogingen.push_back(scoreH);
    totaalScore += scoreH;
  }
  else{
    vScoreOphogingen.push_back(scoreV + scoreH);
    totaalScore += scoreV + scoreH;
  }
}

bool Azul::doeZet (int rij, int kolom)
{
  if(geldigBord){
    pair<int,int> coords = make_pair(rij,kolom);
    if(magZet(coords)){
      vZetten.push_back(coords);
      bord[rij][kolom] = 1;
      verwerkZet(coords);

      return true;
    }
  }
  return false;
}  // doeZet

bool Azul::unDoeZet ()
{
  if(vZetten.size() != 0 || !geldigBord){
    bord[vZetten.back().first][vZetten.back().second] = 0;
    vZetten.pop_back();
    totaalScore -=vScoreOphogingen.back();
    vScoreOphogingen.pop_back();
    return true;
  }
  return false;
}  // unDoeZet

bool Azul::bepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(geldigBord){
    //We hebben een manier nodig om alle bedekkingen van het bord af te gaan
    //Deze aanroep is de eerste, initialiseren de waardes,roepen de functie 
    //aan en returnen de waardes:
    mini = 0; maxi = 0; volgordesMini = 0; volgordesMaxi = 0;
    bepaalMiniMaxiScoreRec_p(mini, volgordesMini, maxi, volgordesMaxi);
    cout << "De minimale score is ";
    cout << kleurGroen << mini << kleurWit;
    cout << ", en de maximale score is ";
    cout << kleurBlauw << maxi << kleurWit<< endl;
    cout << "Het aantal volgordes voor de minimale score is ";
    cout << kleurGroen << volgordesMini;
    cout << ", en het aantal volgorde voor de maximale score is ";
    cout << kleurBlauw << volgordesMaxi << endl;
    return true;
  }
  cout << "geen geldig bord" << endl;
  return false;

}  // bepaalMiniMaxiScoreRec

bool Azul::bepaalMiniMaxiScoreRec_p (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  return false;
}

bool Azul::bepaalMiniMaxiScoreTD (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi)
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // bepaalMiniMaxiScoreTD

bool Azul::bepaalMiniMaxiScoreBU (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi,
                                  vector< pair<int,int> > &zettenReeksMini,
                                  vector< pair<int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // bepaalMiniMaxiScoreBU

void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfZettenReeksen
