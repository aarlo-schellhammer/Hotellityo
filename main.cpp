// Lopputyö 4-5 pistettä

// Aarlo Schellhammer

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits> // numeric_limits<streamsize>::max() käyttö syötteen tarkistuksessa ja cin.ignore

using namespace std;

// Globaali vakio
const int MIN_HUONEITA = 40;   // Hotellin minimihuoneiden määrä
const int MAX_HUONEITA = 300;  // Hotellin maksimihuoneiden määrä
const int HINTA_1HH = 100;     // Yhden hengen huoneen €/yö
const int HINTA_2HH = 150;     // Kahden hengen huoneen €/yö

// Huoneen rakenne
struct Huone
{
    bool yhdenHengen;   // true = 1hh, false = 2hh
    int huoneNumero;     // Huoneen numero
    int varausNumero;    // Satunnainen varausnumero
    string varausNimi;   // Varaajan nimi
    bool onkoVarattu;    // Onko huone varattu
};

// Funktioiden esittely
vector<Huone> luoHotelli();                       // Luo hotellin ja huoneet
void teeVaraus(vector<Huone> &hotellinHuoneet);   // Varausjärjestelmä
void etsiVaraus(const vector<Huone> &hotellinHuoneet); // Varauksen etsintä
void naytaHuoneet(const vector<Huone> &hotellinHuoneet); // Näytä kaikki huoneet
bool onkoHotellia(const vector<Huone> &hotellinHuoneet); // Tarkistaa onko hotelli luotu
int tarkistusLuku(int min, int max);                  // Luvun tarkistus
string tarkistusTeksti();                               // Tekstin tarkistus
void tallennaTiedosto(const vector<Huone> &hotellinHuoneet); // Tallentaa tiedot tiedostoon
vector<Huone> lataaTiedosto();                   // Lataa tiedot tiedostosta

// Pääohjelma
int main()
{
    srand(time(0)); // Arpomista varten

    // Lataa aiemmin tallennetut varaukset tiedostosta
    vector<Huone> hotellinHuoneet = lataaTiedosto();

    while (true) // Main menu
    {
        cout << "\nMenu\n";
        cout << "1) Luo uusi hotelli\n";
        cout << "2) Tee uusi varaus\n";
        cout << "3) Etsi varaus\n";
        cout << "4) Näytä kaikki huoneet\n";
        cout << "0) Lopeta ja tallenna tehdyt muutokset\n";
        cout << "Valinta: ";

        int valinta = tarkistusLuku(0, 4); // Lukee syötteen ja tarkistus

        switch (valinta)
        {
        case 0: // Lopeta ohjelma ja tallenna
            tallennaTiedosto(hotellinHuoneet);
            cout << "Ohjelma suljetaan. Varaustilanne tallennettu.\n";
            return 0;
        case 1: // Luo uusi hotelli
            hotellinHuoneet = luoHotelli();
            break;
        case 2: // Tee uusi varaus
            teeVaraus(hotellinHuoneet);
            break;
        case 3: // Etsi varaus
            etsiVaraus(hotellinHuoneet);
            break;
        case 4: // Näytä kaikki huoneet
            naytaHuoneet(hotellinHuoneet);
            break;
        default:
            break;
        }
    }
}

// Luo hotellin ja huoneet
vector<Huone> luoHotelli()
{
    // Arpoo parillisen huoneiden määrän väliltä MIN_HUONEITA-MAX_HUONEITA (40-300) 
    // Jakaa kahdelle, sitten kertoo, jotta huoneita on parillinen määrä
    int huoneMaara = ((rand() % ((MAX_HUONEITA - MIN_HUONEITA) / 2 + 1)) + MIN_HUONEITA / 2) * 2;
    vector<Huone> huoneet(huoneMaara); // Vektori huoneista

    for (int i = 0; i < huoneMaara; i++)
    {
        huoneet[i].onkoVarattu = false;         // Aluksi huoneet vapaana
        huoneet[i].yhdenHengen = (i < huoneMaara / 2); // Ensimmäinen puolisko 1hh, loppu 2hh
        huoneet[i].huoneNumero = i + 1;         // Huoneen numero
        huoneet[i].varausNimi = "Ei varausta"; // Nimikenttä jos ei ole varausta
        huoneet[i].varausNumero = 0;            // Ei varausnumeroa
    }

    cout << "Uusi hotelli luotu. Huoneita yhteensä: " << huoneMaara << endl;
    return huoneet;
}

// Varaus ohjelma
void teeVaraus(vector<Huone> &huoneet)
{
    if (!onkoHotellia(huoneet)) // Tarkistus että hotelli on luotu
        return;

    cout << "Valitse huonetyyppi:\n1) Yhden hengen (100 €/yö)\n2) Kahden hengen (150 €/yö)\nValinta: ";
    int valinta = tarkistusLuku(1, 2); // Kysytään huonetyyppi ja tarkistus
    bool haluttuKoko = (valinta == 1);

    // Etsitään ensimmäinen vapaa huone halutussa koossa
    int vapaa = -1;
    for (int i = 0; i < huoneet.size(); i++)
    {
        if (huoneet[i].yhdenHengen == haluttuKoko && !huoneet[i].onkoVarattu)
        {
            vapaa = i;
            break;
        }
    }

    if (vapaa == -1) // Jos ei vapaita huoneita
    {
        cout << "Ei vapaita huoneita halutussa koossa.\n";
        return;
    }

    // Kysytään varauksen nimi
    cout << "Syötä varauksen nimi: ";
    string nimi = tarkistusTeksti();

    // Kysytään öiden määrä
    cout << "Montako yötä haluat varata? ";
    int yonMaara = tarkistusLuku(1, 365);

    // Arvotaan alennus 0%, 10% tai 20%
    int alennus = 0;
    int luku = rand() % 3;
    if (luku == 1)
        alennus = 10;
    else if (luku == 2)
        alennus = 20;

    // Tallennetaan varaus huoneeseen
    huoneet[vapaa].onkoVarattu = true;
    huoneet[vapaa].varausNimi = nimi;
    huoneet[vapaa].varausNumero = rand() % 90000 + 10000; // Satunnainen varausnumero 10000-99999

    int hinta = haluttuKoko ? HINTA_1HH : HINTA_2HH;
    int kokonaishinta = yonMaara * hinta * (100 - alennus) / 100;

    // Vahvistus
    cout << "\nVaraus tehty\n";
    cout << "Huone: " << huoneet[vapaa].huoneNumero << endl;
    cout << "Nimi: " << nimi << endl;
    cout << "Varausnumero: " << huoneet[vapaa].varausNumero << endl;
    cout << "Öitä: " << yonMaara << endl;
    cout << "Alennus: " << alennus << "%\n";
    cout << "Kokonaishinta: " << kokonaishinta << " €\n";
}

// Etsi varaus nimellä tai numerolla
void etsiVaraus(const vector<Huone> &huoneet)
{
    if (!onkoHotellia(huoneet)) // Tarkistetaan hotellin olemassaolo
        return;

    cout << "Etsi varaus:\n1) Nimellä\n2) Varausnumerolla\nValinta: ";
    int valinta = tarkistusLuku(1, 2); // Kysyy hakutavan
    bool loytyi = false;

    if (valinta == 1) // Haku nimellä
    {
        cout << "Syötä nimi: ";
        string nimi = tarkistusTeksti();
        for (const auto &h : huoneet)
        {
            if (h.onkoVarattu && h.varausNimi == nimi)
            {
                cout << "--- Varaus löytyi ---\n";
                cout << "Huone: " << h.huoneNumero << "\nNimi: " << h.varausNimi << "\nVarausnumero: " << h.varausNumero << endl;
                loytyi = true;
                break;
            }
        }
    }
    else // Haku varausnumerolla
    {
        cout << "Syötä varausnumero: ";
        int numero = tarkistusLuku(10000, 99999);
        for (const auto &h : huoneet)
        {
            if (h.onkoVarattu && h.varausNumero == numero)
            {
                cout << "Varaus löytyi\n";
                cout << "Huone: " << h.huoneNumero << "\nNimi: " << h.varausNimi << "\nVarausnumero: " << h.varausNumero << endl;
                loytyi = true;
                break;
            }
        }
    }

    if (!loytyi)
        cout << "Varausta ei löytynyt.\n";
}

// Näytä kaikki huoneet
void naytaHuoneet(const vector<Huone> &huoneet)
{
    if (!onkoHotellia(huoneet))
        return;

    for (const auto &h : huoneet)
    {
        cout << "Huone " << h.huoneNumero << " (" << (h.yhdenHengen ? "1hh" : "2hh") << ") ";
        if (h.onkoVarattu)
            cout << "VARATTU (" << h.varausNimi << ", nro " << h.varausNumero << ")";
        cout << endl;
    }
}

// Kysyy että hotelli on luotu
bool onkoHotellia(const vector<Huone> &huoneet)
{
    if (huoneet.empty())
    {
        cout << "Hotellia ei ole luotu. Luo ensin hotelli.\n";
        return false;
    }
    return true;
}

// Syötteen tarkistus luvuille
int tarkistusLuku(int min, int max)
{
    int luku;
    while (true)
    {
        if (cin >> luku && luku >= min && luku <= max)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Poistaa rivin loppuun mahdolliset ylimääräiset merkit
            return luku;
        }
        else
        {
            cout << "Virheellinen syöte, yritä uudelleen: ";
            cin.clear(); // Nollaa aiemman virheellisen syötteeen
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ohittaa virheelliset merkit
        }
    }
}

// Syötteen tarkistus tekstille
string tarkistusTeksti()
{
    string rivi;
    getline(cin, rivi);
    while (rivi.empty())
    {
        cout << "Syöte tyhjä, yritä uudelleen: ";
        getline(cin, rivi);
    }
    return rivi;
}

// Tiedoston tallennus
void tallennaTiedosto(const vector<Huone> &huoneet)
{
    ofstream tiedosto("hotelli.txt"); // Avaa tiedoston kirjoitusta varten
    if (!tiedosto.is_open())
    {
        cout << "Tiedoston tallennus epäonnistui!\n";
        return;
    }

    for (const auto &h : huoneet)
    {
        // Tallennetaan kaikki tiedot yhteen riviin
        tiedosto << h.huoneNumero << " " << h.yhdenHengen << " " << h.onkoVarattu << " "
                 << h.varausNumero << " " << h.varausNimi << "\n";
    }
    tiedosto.close();
}

// Tiedoston lataus
vector<Huone> lataaTiedosto()
{
    vector<Huone> huoneet; // Vektori johon luodut Huone tallennetaan

    // Avaa tiedoston lukua varten
    fstream tiedosto("hotelli.txt", ios::in);
    if (!tiedosto.is_open())
        return huoneet; // Jos tiedoston avaaminen epäonnistuu palauttaa tyhjän vektorin

    // Apumuuttujat
    int huoneNumero, varausNumero;
    bool yhdenHengen, onkoVarattu;
    string varausNimi;

    // Lukee tiedoston rivi riviltä
    while (tiedosto >> huoneNumero >> yhdenHengen >> onkoVarattu >> varausNumero)
    {
        // Ohittaa rivin lopun ennen varausnimen lukua (välilyönnit)
        tiedosto.ignore(numeric_limits<streamsize>::max(), '\n');

        // Lukee koko jäljellä olevan rivin huoneen varausnimeksi
        getline(tiedosto, varausNimi);

        // Jos varausnimi ei ole tyhjä tallentaa tiedot vektoriin
        if (!varausNimi.empty())
        {
            Huone h; // Luodaan Huone-olio
            h.huoneNumero = huoneNumero;
            h.yhdenHengen = yhdenHengen;
            h.onkoVarattu = onkoVarattu;
            h.varausNumero = varausNumero;
            h.varausNimi = varausNimi;

            huoneet.push_back(h); // Lisää Huoneen vektoriin
        }
    }

    tiedosto.close(); // Sulkee tiedoston
    return huoneet;   // Palauttaa luetun huonevektorin
}