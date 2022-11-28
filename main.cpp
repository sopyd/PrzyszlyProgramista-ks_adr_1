#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

struct Kontakt {
    int idAdresata, idUzytkownika;
    string imie, nazwisko, nr_telefonu, email, adres;
};

struct Uzytkownik {
    int idUzytkownika;
    string nazwa, haslo;
};

string insertLine() {
    string input = "";
    cin.sync();
    getline(cin, input);
    return input;
}

char insertSign() {
    string input = "";
    char sign  = {0};

    while (true) {
        getline(cin, input);

        if (input.length() == 1) {
            sign = input[0];
            break;
        }
        cout << "To nie jest pojedynczy znak. Wpisz ponownie." << endl;
    }
    return sign;
}

int odczytajUzytkownikowZPlikuTxt (vector<Uzytkownik> &uzytkownicy) {
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::in);

    if (plik.good() == 0) {
        return 0;
    }
    string dane;
    int iloscUzytkownikow = 0;

    while (!plik.eof()) {
        getline(plik, dane, '|');

        if (dane != "") {
            uzytkownicy.push_back(Uzytkownik());
            uzytkownicy[iloscUzytkownikow].idUzytkownika = atoi(dane.c_str());
            getline(plik, dane, '|');
            uzytkownicy[iloscUzytkownikow].nazwa = dane;
            getline(plik, dane);
            uzytkownicy[iloscUzytkownikow].haslo = dane;
            iloscUzytkownikow++;
        }
    }
    plik.close();
    return iloscUzytkownikow;
}

void dopiszUzytkownikaDoPlikuTxt (vector<Uzytkownik> uzytkownicy, int iloscUzytkownikow) {
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out | ios::app);

    if (iloscUzytkownikow == 0) {
        plik << uzytkownicy[iloscUzytkownikow].idUzytkownika << "|";
    }
    else {
        plik << endl << uzytkownicy[iloscUzytkownikow].idUzytkownika << "|";
    }
    plik << uzytkownicy[iloscUzytkownikow].nazwa << "|";
    plik << uzytkownicy[iloscUzytkownikow].haslo;
    plik.close();
}

int rejestracja (vector<Uzytkownik> &uzytkownicy, int iloscUzytkownikow) {
    string nazwa, haslo;
    cout << "Podaj nazwe uzytkownika: ";
    //nazwa = insertLine();
    cin >> nazwa;
    vector<Uzytkownik>::iterator itr = uzytkownicy.begin();

    while (itr != uzytkownicy.end()) {
        if (itr -> nazwa == nazwa) {
            cout << "Uzytkownik o takiej nazwie juz istnieje. Wpisz inna nazwe uzytkownika: ";
            //nazwa = insertLine(); // jak dam insertline() to mozna dodac uzytkownika bez danych
            cin >> nazwa;
            itr = uzytkownicy.begin();
        }
        else
            itr++;
    }
    cout << "Podaj haslo: ";
    //haslo = insertLine();
    cin >> haslo;
    uzytkownicy.push_back(Uzytkownik());
    uzytkownicy[iloscUzytkownikow].nazwa = nazwa;
    uzytkownicy[iloscUzytkownikow].haslo = haslo;
    uzytkownicy[iloscUzytkownikow].idUzytkownika = iloscUzytkownikow + 1;
    cout << "Konto zostalo utworzone" << endl;
    Sleep(1000);
    dopiszUzytkownikaDoPlikuTxt (uzytkownicy, iloscUzytkownikow);
    return ++iloscUzytkownikow;
}

int logowanie (vector<Uzytkownik> &uzytkownicy, int iloscUzytkownikow) {
    string nazwa, haslo;
    cout << "Podaj nazwe uzytkownika: ";
    nazwa = insertLine();;

    vector<Uzytkownik>::iterator itr = uzytkownicy.begin();

    while (itr != uzytkownicy.end()) {

        if (itr -> nazwa == nazwa) {

            for (int j = 0; j < 3; j++) {
                cout << "Podaj haslo (pozostalo " << 3 - j << " prob): ";
                haslo = insertLine();;

                if (itr -> haslo == haslo) {
                    cout << "Zalogowales sie." << endl;
                    Sleep (1000);
                    return itr -> idUzytkownika;
                }
            }
            cout << "Podales bledne haslo. Poczekaj 3 sekundy przed kolejna proba." << endl;
            Sleep (3000);
            return 0;
        }
        itr++;
    }
    cout << "Nie ma uzytkownika o takiej nazwie" << endl;
    Sleep (1500);
    return 0;
}

void nadpiszPlikTxtUzytkownicy (vector<Uzytkownik> uzytkownicy) {
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out);

    for (int i = 0; i < (int) uzytkownicy.size(); i++) {
        if (i == 0) {
            plik << uzytkownicy[i].idUzytkownika << "|";
        }
        else {
            plik << endl << uzytkownicy[i].idUzytkownika << "|";
        }
        plik << uzytkownicy[i].nazwa << "|";
        plik << uzytkownicy[i].haslo;
    }
    plik.close();
}

void zmienHaslo (vector<Uzytkownik> &uzytkownicy, int iloscUzytkownikow, int idZalogowanegoUzytkownika) {
    string haslo;
    cout << "Podaj nowe haslo: ";
    haslo = insertLine();;

    for (int i = 0; i < iloscUzytkownikow; i++) {

        if (uzytkownicy[i].idUzytkownika == idZalogowanegoUzytkownika) {
            uzytkownicy[i].haslo = haslo;
            nadpiszPlikTxtUzytkownicy(uzytkownicy);
            cout << "Haslo zostalo zmienione." << endl;
            Sleep (1500);
        }
    }
}

string zamienPierwszaLitereNaDuzaPozostaleNaMale(string tekst) {
    if (!tekst.empty()) {
        transform(tekst.begin(), tekst.end(), tekst.begin(), ::tolower);
        tekst[0] = toupper(tekst[0]);
    }
    return tekst;
}

void nadpiszPlikTxtAdresaci (vector<Kontakt> kontakty, vector<Kontakt>::iterator itr, string dzialanie) {
    fstream plik, plikTymczasowy;
    plik.open("Adresaci.txt", ios::in);
    plikTymczasowy.open("Adresaci_tymczasowy.txt", ios::out);

    string linia, idKontaktu;

    while(!plik.eof()) {
        getline(plik, idKontaktu, '|');
        getline(plik, linia);

        if (itr -> idAdresata == atoi(idKontaktu.c_str()) && dzialanie == "edytowanie") {
            plikTymczasowy << itr -> idAdresata << "|";
            plikTymczasowy << itr -> idUzytkownika << "|";
            plikTymczasowy << itr -> imie << "|";
            plikTymczasowy << itr -> nazwisko << "|";
            plikTymczasowy << itr -> nr_telefonu << "|";
            plikTymczasowy << itr -> email << "|";
            plikTymczasowy << itr -> adres << endl;
        }
        else if (itr -> idAdresata == atoi(idKontaktu.c_str()) && dzialanie == "usuwanie") {
            continue;
        }
        else if (atoi(idKontaktu.c_str())) {
            plikTymczasowy << idKontaktu + "|" + linia << endl;
        }
    }
    plik.close();
    plikTymczasowy.close();

    remove("Adresaci.txt");
    rename("Adresaci_tymczasowy.txt", "Adresaci.txt");
}

void wyswietlKontakt (vector<Kontakt> &kontakty, vector<Kontakt>::iterator itr) {
    cout << itr -> idAdresata << endl;
    cout << itr -> imie << endl;
    cout << itr -> nazwisko << endl;
    cout << itr -> nr_telefonu << endl;
    cout << itr -> email << endl;
    cout << itr -> adres << endl << endl;
}

void edytujKontakt(vector<Kontakt> &kontakty) {
    int id;
    bool czyIstniejeKontakt = false;
    char wybor;
    string noweDane;
    cout << "Wprowadz ID kontaktu do edycji: ";
    cin >> id;

    for (vector<Kontakt>::iterator itr = kontakty.begin(); itr != kontakty.end(); itr++) {

        if (itr -> idAdresata == id) {
            czyIstniejeKontakt == true;

            while (1) {
                system ("cls");
                cout << "Kontakt do edycji: " << endl;
                wyswietlKontakt(kontakty, itr);
                cout << endl << "Wybierz dane do edycji: " << endl;
                cout << "1. Imie." << endl;
                cout << "2. Nazwisko." << endl;
                cout << "3. Nr telefonu." << endl;
                cout << "4. Email." << endl;
                cout << "5. Adres." << endl;
                cout << "6. Powrot do menu." << endl;
                //wybor = insertSign(); // uzycie tej funkcji powoduje niechciany napis przy edycji kontaktu
                wybor = getch();
                cin.sync();

                switch (wybor) {
                case '1': {
                    cout << "Wprowadz nowe dane: ";
                    noweDane = insertLine();
                    itr -> imie = zamienPierwszaLitereNaDuzaPozostaleNaMale(noweDane);
                    nadpiszPlikTxtAdresaci(kontakty, itr, "edytowanie");
                    cout << "Dane zostaly zaktualizowane." << endl;
                    Sleep (1000);
                    continue;
                }
                case '2': {
                    cout << "Wprowadz nowe dane: ";
                    noweDane = insertLine();
                    itr -> nazwisko = zamienPierwszaLitereNaDuzaPozostaleNaMale(noweDane);
                    nadpiszPlikTxtAdresaci(kontakty, itr, "edytowanie");
                    cout << "Dane zostaly zaktualizowane." << endl;
                    Sleep (1000);
                    continue;
                }
                case '3': {
                    cout << "Wprowadz nowe dane: ";
                    noweDane = insertLine();
                    itr -> nr_telefonu = noweDane;
                    nadpiszPlikTxtAdresaci(kontakty, itr, "edytowanie");
                    cout << "Dane zostaly zaktualizowane." << endl;
                    Sleep (1000);
                    continue;
                }
                case '4': {
                    cout << "Wprowadz nowe dane: ";
                    noweDane = insertLine();
                    itr -> email = noweDane;
                    nadpiszPlikTxtAdresaci(kontakty, itr, "edytowanie");
                    cout << "Dane zostaly zaktualizowane." << endl;
                    Sleep (1000);
                    continue;
                }
                case '5': {
                    cout << "Wprowadz nowe dane: ";
                    noweDane = insertLine();
                    itr -> adres = noweDane;
                    nadpiszPlikTxtAdresaci(kontakty, itr, "edytowanie");
                    cout << "Dane zostaly zaktualizowane." << endl;
                    Sleep (1000);
                    continue;
                }
                case '6': {
                    break;
                }
                default: {
                    cout << "Wybrano niepoprawna opcje." << endl;
                    Sleep (1000);
                    continue;
                }
                }
                break;
            }
        }
    }
    if(czyIstniejeKontakt == false) {
        cout << endl << "Nie ma kontaktu o takim ID" << endl << endl;
        Sleep (2000);
    }
}

void usunKontakt(vector<Kontakt> &kontakty, int &iloscKontaktowUzytkownika) {
    int id;
    bool czyIstniejeKontakt = false;
    cout << "Wprowadz ID kontaktu do usuniecia: ";
    cin >> id;

    for (vector<Kontakt>::iterator itr = kontakty.begin(); itr < kontakty.end(); itr++) {

        if (itr -> idAdresata == id) {
            czyIstniejeKontakt = true;
            cout << "Kontakt do usuniecia: " << endl;
            wyswietlKontakt(kontakty, itr);
            cout << "Czy usunac kontakt? Wpisz: T/N" << endl;

            if (getch() == 't' || getch() == 'T') {
                nadpiszPlikTxtAdresaci (kontakty, itr, "usuwanie");
                kontakty.erase(itr);
                iloscKontaktowUzytkownika--;
                cout << endl << "Kontakt zostal usuniety" << endl;
                Sleep(3000);
            }
            else if ((getch() == 'n' || getch() == 'N'))
                break;
        }
    }
    if (czyIstniejeKontakt == false) {
        cout << endl << "Nie ma kontaktu o takim ID" << endl << endl;
        Sleep (2000);
    }
}

void wyswietlWszystkieKontakty (vector<Kontakt> &kontakty) {

    for (vector<Kontakt>::iterator itr = kontakty.begin(); itr != kontakty.end(); itr++) {
        wyswietlKontakt(kontakty, itr);
    }
    system ("pause");
}

void wyszukajPoNazwisku(vector<Kontakt> &kontakty) {
    string nazwisko;
    cout << "Podaj nazwisko wyszukiwanego kontaktu: ";
    nazwisko = insertLine();

    bool czyIstniejeKontakt = false;

    for (vector<Kontakt>::iterator itr = kontakty.begin(); itr != kontakty.end(); itr++) {

        if (itr -> nazwisko == zamienPierwszaLitereNaDuzaPozostaleNaMale(nazwisko)) {
            czyIstniejeKontakt = true;
            wyswietlKontakt(kontakty, itr);
        }
    }
    if (czyIstniejeKontakt == false)
        cout << endl << "Nie ma takiego kontaktu!" << endl << endl;
    system ("pause");
}

void wyszukajPoImieniu(vector<Kontakt> &kontakty) {
    string imie;
    cout << "Podaj imie wyszukiwanego kontaktu: ";
    imie = insertLine();;
    bool czyIstniejeKontakt = false;

    for (vector<Kontakt>::iterator itr = kontakty.begin(); itr != kontakty.end(); itr++) {

        if (itr -> imie == zamienPierwszaLitereNaDuzaPozostaleNaMale(imie)) {
            czyIstniejeKontakt = true;
            wyswietlKontakt(kontakty, itr);
        }
    }
    if (czyIstniejeKontakt == false)
        cout << endl << "Nie ma takiego kontaktu!" << endl << endl;
    system ("pause");
}

void dopiszKontaktDoPlikuTxt (vector<Kontakt> &kontakty, int iloscKontaktowUzytkownika) {
    fstream plik;
    plik.open("Adresaci.txt", ios::out | ios::app);
    plik << kontakty[iloscKontaktowUzytkownika].idAdresata << "|";
    plik << kontakty[iloscKontaktowUzytkownika].idUzytkownika << "|";
    plik << kontakty[iloscKontaktowUzytkownika].imie << "|";
    plik << kontakty[iloscKontaktowUzytkownika].nazwisko << "|";
    plik << kontakty[iloscKontaktowUzytkownika].nr_telefonu << "|";
    plik << kontakty[iloscKontaktowUzytkownika].email << "|";
    plik << kontakty[iloscKontaktowUzytkownika].adres << endl;
    plik.close();
}

int sprawdzIloscWszystkichKontaktow () {
    fstream plik;
    plik.open("Adresaci.txt", ios::in);

    if (plik.good() == false) {
        return 0;
    }

    string linia;
    int iloscLinii = 0;

    while(!plik.eof()) {
        getline(plik, linia);

        if (linia != "") {
            iloscLinii++;
        }
    }
    plik.close();
    return iloscLinii;
}

int sprawdzIdOstatniegoKontaktu() {
    int iloscWszystkichKontaktow = sprawdzIloscWszystkichKontaktow();

    if (iloscWszystkichKontaktow == 0) {
        return 0;
    }
    else {
        fstream plik;
        string linia;
        plik.open("Adresaci.txt", ios::in);

        for (int i=0; i < iloscWszystkichKontaktow; i++ ) {

            if (i == iloscWszystkichKontaktow-1) {
                getline(plik,linia,'|');
                plik.close();
                return atoi(linia.c_str());
            }
            else {
                getline(plik,linia);
            }
        }
    }
}

void dodajKontakt (vector<Kontakt> &kontakty, int &iloscKontaktowUzytkownika, int idZalogowanegoUzytkownika) {
    string imie, nazwisko, nr_telefonu, email, adres;
    cout << "Podaj imie: ";
    imie = insertLine();;
    cout << "Podaj nazwisko: ";
    nazwisko = insertLine();;
    cout << "Podaj nr telefonu: ";
    nr_telefonu = insertLine();
    cout << "Podaj adres e-mail: ";
    email = insertLine();
    cout << "Podaj adres: ";
    adres = insertLine();

    kontakty.push_back(Kontakt());
    int iloscWszystkichKontaktow = sprawdzIloscWszystkichKontaktow();

    if (iloscWszystkichKontaktow == 0) {
        kontakty[iloscKontaktowUzytkownika].idAdresata = 1;
    }
    else {
        kontakty[iloscKontaktowUzytkownika].idAdresata = sprawdzIdOstatniegoKontaktu() + 1;
    }
    kontakty[iloscKontaktowUzytkownika].idUzytkownika = idZalogowanegoUzytkownika;
    kontakty[iloscKontaktowUzytkownika].imie = zamienPierwszaLitereNaDuzaPozostaleNaMale(imie);
    kontakty[iloscKontaktowUzytkownika].nazwisko = zamienPierwszaLitereNaDuzaPozostaleNaMale(nazwisko);
    kontakty[iloscKontaktowUzytkownika].nr_telefonu = nr_telefonu;
    kontakty[iloscKontaktowUzytkownika].email = email;
    kontakty[iloscKontaktowUzytkownika].adres = adres;

    dopiszKontaktDoPlikuTxt(kontakty, iloscKontaktowUzytkownika);
    iloscKontaktowUzytkownika++;

    cout << endl << "Kontakt zostal dodany do ksiazki adresowej." << endl;
    Sleep(2000);
}

int odczytajKontaktyZPlikuTxt (vector<Kontakt> &kontakty, int idZalogowanegoUzytkownika) {
    fstream plik;
    plik.open("Adresaci.txt", ios::in);

    if (plik.good() == false) {
        return 0;
    }
    string dane,idKontaktu,idUzytkownika;
    int iloscKontaktow = 0;

    while (!plik.eof()) {
        getline(plik, idKontaktu, '|');

        if (idKontaktu != "") {
            getline(plik, idUzytkownika, '|');

            if(atoi(idUzytkownika.c_str()) == idZalogowanegoUzytkownika) {
                kontakty.push_back(Kontakt());
                kontakty[iloscKontaktow].idAdresata = atoi(idKontaktu.c_str());
                kontakty[iloscKontaktow].idUzytkownika = atoi(idUzytkownika.c_str());
                getline(plik, dane, '|');
                kontakty[iloscKontaktow].imie = dane;
                getline(plik, dane, '|');
                kontakty[iloscKontaktow].nazwisko = dane;
                getline(plik, dane, '|');
                kontakty[iloscKontaktow].nr_telefonu = dane;
                getline(plik, dane, '|');
                kontakty[iloscKontaktow].email = dane;
                getline(plik, dane);
                kontakty[iloscKontaktow].adres = dane;
                iloscKontaktow++;
            }
            else {
                getline(plik, dane, '|');
                getline(plik, dane, '|');
                getline(plik, dane, '|');
                getline(plik, dane, '|');
                getline(plik, dane);
            }
        }
    }
    plik.close();
    return iloscKontaktow;
}

void przejdzDoKsiazkiAdresowej(vector <Uzytkownik> &uzytkownicy, int &iloscUzytkownikow, int idZalogowanegoUzytkownika) {
    vector<Kontakt> kontakty;
    char wybor;

    int iloscKontaktowUzytkownika = odczytajKontaktyZPlikuTxt(kontakty, idZalogowanegoUzytkownika);

    while (1) {
        system ("cls");
        cout << "1. Dodaj nowy kontakt." << endl;
        cout << "2. Wyszukaj po imieniu." << endl;
        cout << "3. Wyszukaj po nazwisku." << endl;
        cout << "4. Wyswietl wszystkie kontakty." << endl;
        cout << "5. Usun kontakt." << endl;
        cout << "6. Edytuj kontakt." << endl;
        cout << "7. Zmien haslo." << endl;
        cout << "8. Wyloguj sie." << endl;
        cout << "Twoj wybor: ";
        wybor = insertSign();

        if (!(wybor == '1' || wybor == '8' || wybor == '7') &&  iloscKontaktowUzytkownika == 0) {
            cout << "Ksiazka adresowa jest pusta." << endl << "Dodaj plik 'Adresaci.txt' lub dodaj nowe kontakty." <<endl;
            Sleep (4000);
        }
        else if (wybor == '1') {
            system ("cls");
            dodajKontakt(kontakty, iloscKontaktowUzytkownika, idZalogowanegoUzytkownika);
        }
        else if (wybor == '2') {
            system ("cls");
            wyszukajPoImieniu(kontakty);
        }
        else if (wybor == '3') {
            system ("cls");
            wyszukajPoNazwisku(kontakty);
        }
        else if (wybor == '4') {
            system ("cls");
            wyswietlWszystkieKontakty(kontakty);
        }
        else if (wybor == '5') {
            system ("cls");
            usunKontakt(kontakty, iloscKontaktowUzytkownika);
        }
        else if (wybor == '6') {
            system ("cls");
            edytujKontakt(kontakty);
        }
        else if (wybor == '7') {
            zmienHaslo(uzytkownicy, iloscUzytkownikow, idZalogowanegoUzytkownika);
        }
        else if (wybor == '8') {
            idZalogowanegoUzytkownika = 0;
            break;
        }
        else {
            cout << "Wybrano niepoprawna opcje." << endl;
            Sleep (1000);
            continue;
        }
    }
}

int main() {
    vector <Uzytkownik> uzytkownicy;
    int idZalogowanegoUzytkownika = 0;
    int iloscUzytkownikow = odczytajUzytkownikowZPlikuTxt (uzytkownicy);
    char wybor;

    while (1) {
        system ("cls");
        cout << "1. Rejestracja." << endl;
        cout << "2. Logowanie." << endl;
        cout << "3. Zakoncz program." << endl;
        cout << "Twoj wybor: ";
        wybor = insertSign();

        if (wybor == '1') {
            iloscUzytkownikow = rejestracja(uzytkownicy, iloscUzytkownikow);
        }
        else if (wybor == '2') {
            idZalogowanegoUzytkownika = logowanie(uzytkownicy, iloscUzytkownikow);

            if (idZalogowanegoUzytkownika != 0) {
                przejdzDoKsiazkiAdresowej(uzytkownicy, iloscUzytkownikow, idZalogowanegoUzytkownika);
            }
        }
        else if (wybor == '3')
            exit(0);

        else {
            cout << "Wybrano niepoprawna opcje." << endl;
            Sleep (1000);
            continue;
        }
    }
    return 0;
}
