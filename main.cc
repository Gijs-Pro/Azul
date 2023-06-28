// Hoofdprogramma voor oplossing voor programmeeropdracht 3 van Algoritmiek,
// voorjaar 2023: Azul.
//
// Biedt de gebruiker een menustructuur om
// * een nieuw spel te starten
// * een spel in te lezen
// * experimenten uit te voeren
// Bij een gestart of ingelezen spel wordt vervolges een menu geboden om
// - een zet te doen
// - de laatst gedane zet ongedaan te maken
// - rechtstreeks recursief de minimale en maximale score te bepalen
// - met top-down DP de minimale en maximale score te bepalen
// - met bott-up DP de minimale en maximale score te bepalen
//
// Gijs Turkenburg (s3573532)
// Winston Vonk (s3713598)

#include <iostream>
#include <vector>
#include <ctime>  // voor clock() en clock_t
#include "standaard.h"
#include "azul.h"
using namespace std;
const int MaxBestandsNaamLengte = 30; // maximale lengte van een bestandsnaam

// Schrijf het menu voor het spel op het scherm, en vraag een keuze
// van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu ()
{ int keuze;

  cout << endl;
  cout << "1. Een zet doen" << endl;
  cout << "2. Laatst gedane zet ongedaan maken" << endl;
  cout << "3. Minimale / maximale score bepalen (rechtstreeks recursief)"
       << endl;
  cout << "4. Minimale / maximale score bepalen (top-down DP)"
       << endl;
  cout << "5. Minimale / maximale score met bijbehorende zettenreeksen bepalen"
       << endl;
  cout << "  (bottom-up DP)" << endl;
  cout << "6. Stoppen met dit spel" << endl;
  cout << endl;
  cout << "Maak een keuze: ";
  cin >> keuze;

  return keuze;

}  // keuzeUitMenu

// Schrijf het menu voor de experimenten op het scherm, en vraag een keuze
// van de gebruiker.
// Retourneer: de keuze van de gebruiker
int keuzeUitMenu2 ()
{ int keuze;

  cout << endl;
  cout << "Waarmee wilt u experimenten doen?" << endl;
  cout << endl;
  cout << "1. Minimale / maximale score bepalen (rechtstreeks recursief)"
       << endl;
  cout << "2. Minimale / maximale score bepalen (top-down DP)"
       << endl;
  cout << "3. Minimale / maximale score met bijbehorende zettenreeksen bepalen"
       << endl;
  cout << "  (bottom-up DP)" << endl;
  cout << "4. Stoppen met de experimenten" << endl;
  cout << endl;
  cout << "Maak een keuze: ";
  cin >> keuze;

  return keuze;

}  // keuzeUitMenu2

// Bied de gebruiker een menu om voor bord a1
// * een zet te doen
// * de laatst gedane zet ongedaan te maken
// * rechtstreeks recursief de minimale en maximale score te bepalen
// * met top-down DP de minimale en maximale score te bepalen
// * met bott-up DP de minimale en maximale score te bepalen
// Roep vervolgens a1 aan voor de gekozen functie.
// Herhaal dit totdat de gebruiker aangeeft te willen stoppen.
void menuVoorBord (Azul *a1)
{ int keuze,
      hoogte, breedte,
      rij, kolom,  // zet die we doen
      mini, maxi;  // minimale en maximale score
  bool gelukt;
  long long volgordesMini, volgordesMaxi;
  clock_t t1, t2;
  vector <pair <int,int> > zettenReeksMini, zettenReeksMaxi;
  hoogte = a1->getHoogte();
  breedte = a1->getBreedte();

  keuze = 0;
  gelukt = false;  // om compiler warning te voorkomen
  while (keuze!=6)
  {
    a1 -> drukAfBord ();

    keuze = keuzeUitMenu ();

    t1 = clock ();
    cout << endl;
    switch (keuze)
    { case 1: cout << "Geef het nummer van de rij (0.." << hoogte-1 << "): ";
              cin >> rij;
              cout << "Geef het nummer van de kolom (0.." << breedte-1 << "): ";
              cin >> kolom;
              if (!(a1 -> doeZet (rij,kolom)))
              { cout << endl;
                cout << "Er is geen zet gedaan." << endl;
              }
              break;
      case 2: if (!(a1 -> unDoeZet ()))
              { cout << endl;
                cout << "Er is geen zet ongedaan gemaakt." << endl;
              }
              break;
      case 3: gelukt = a1 -> bepaalMiniMaxiScoreRec (mini, volgordesMini,
                               maxi, volgordesMaxi);
              break;
      case 4: gelukt = a1 -> bepaalMiniMaxiScoreTD (mini, volgordesMini,
                               maxi, volgordesMaxi);
              break;
      case 5: gelukt = a1 -> bepaalMiniMaxiScoreBU (mini, volgordesMini,
                     maxi, volgordesMaxi, zettenReeksMini, zettenReeksMaxi);
              break;
      case 6: break;
      default: cout << "Voer een goede keuze in!" << endl;
    }  // switch
    t2 = clock ();

    if (integerInBereik (keuze, 3, 5))
    { cout << endl;
      if (gelukt)
      { cout << "Minimale score = " << mini << endl;
        cout << "Aantal volgordes met die score = " << volgordesMini << endl;
        cout << "Maximale score = " << maxi << endl;
        cout << "Aantal volgordes met die score = " << volgordesMaxi << endl;
        if (keuze==5)
          a1 -> drukAfZettenReeksen (zettenReeksMini, zettenReeksMaxi);
      }
      else
        cout << "Het is niet gelukt om minimale en maximale scores te bepalen."
             << endl;

      cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
        << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;
    }
  }  // while

}  // menuVoorBord

// Voer de experimenten uit, zoals beschreven in de opdracht.
void doeExperimenten ()
{ int keuze;
 
  keuze = 0;
  while (keuze!=4)
  {
    keuze = keuzeUitMenu2 ();
    clock_t t1, t2;
    int grens;
    int i = 2;//start dimensies van borden
    int j = 2;
    Azul *a1;
    int mini, maxi;
    long long volgordesMini, volgordesMaxi;
    vector <pair <int,int> > zettenReeksMini, zettenReeksMaxi;
    //we kunnen maar tot 4x4 borden, want een 5x4 gebruikt te veel geheugen.
    //dit in verband met het bijhouden van de long long waardes van
    //volgordesMini en volgordesMaxi (er is geprobeerd de tabel op te splitsen
    //in eentje voor de volgordes (long long dus) en eentje voor de scores
    //(int dus) maar ook hier kwam een segmentation fault). De oplossing volgens
    //ons is om de code compleet te herschrijven met een vector in een vector
    //als tabel, maar dit konden we niet voor elkaar krijgen.
    //voor recursief werkt 4x4 natuurlijk wel.
    if(keuze == 1){
      grens = 5;
    }// if
    else{
      grens = 4;
    }// else
    while( i != grens+1){
      a1 = new Azul (i, j);
      t1 = clock();
      if(keuze == 1){
        a1->bepaalMiniMaxiScoreRec(mini, volgordesMini, maxi, volgordesMaxi);
      } // if
      if(keuze == 2){
        a1->bepaalMiniMaxiScoreTD(mini, volgordesMini, maxi, volgordesMaxi);
      } // if
      if(keuze == 3){
        a1->bepaalMiniMaxiScoreBU(mini, volgordesMini, maxi, volgordesMaxi,
                                  zettenReeksMini, zettenReeksMaxi);
      } // if
      t2 = clock();
      if(mini == -1){//return die de functies geven als ze te lang duren
        cout << "dit experiment voor een " << i << "x" << j << " bord";
        cout << " duurde langer dan 5 minuten." << endl;
      } // if
      else{
        cout << "Voor een " << i << "x" << j << " bord kregen we: " << endl;
        cout << "mini = " << mini << endl;
        cout << "volgordesMini = " << volgordesMini << endl;
        cout << "maxi = " << maxi << endl;
        cout << "volgordesMaxi = " << volgordesMaxi << endl;
        cout << "Dit kostte " << (t2-t1) << " clock ticks, ofwel "
             << (((double)(t2-t1))/CLOCKS_PER_SEC) << " seconden." << endl;
      } // else
      if(i == j){i++;}
      else{j++;}
    } // while
  } // while
} // doeExperimenten

void hoofdmenu ()
{ Azul *a1;  // pointer, om makkeijk nieuwe objecten te kunnen maken
             // en weer weg te gooien
  int keuze,
      hoogte, breedte;
  char invoerNaam[MaxBestandsNaamLengte+1];

  do
  {
    cout << endl;
    cout << "1. Een nieuw spel starten" << endl;
    cout << "2. Een spel inlezen" << endl;
    cout << "3. Experimenten uitvoeren" << endl;
    cout << "4. Stoppen" << endl;
    cout << endl;
    cout << "Maak een keuze: ";
    cin >> keuze;
    switch (keuze)
    { case 1: cout << "Geef de hoogte van het bord (1.." << MaxDimensie
                   << "): ";
              cin >> hoogte;
              cout << "Geef de breedte van het bord (1.." << MaxDimensie
                   << "): ";
              cin >> breedte;
              a1 = new Azul (hoogte, breedte);
              menuVoorBord (a1);
              delete a1;  // netjes opruimen
              break;
      case 2: a1 = new Azul ();
              cout << "Geef de naam van het tekstbestand met de invoer: ";
              cin >> invoerNaam;
              if (a1 -> leesInBord (invoerNaam)){ 
                menuVoorBord (a1);
              }
              delete a1;  // netjes opruimen
              break;
      case 3: doeExperimenten ();
              break;
      case 4: break;
      default: cout << endl;
               cout << "Voer een goede keuze in!" << endl;
    }

  } while (keuze!=4);

}  // hoofdmenu
  
int main ()
{
  cout << "pils" << endl;
  hoofdmenu ();

  return 0;

}

//TODO: doeExperimenten
