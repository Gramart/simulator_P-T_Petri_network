//mista.h - hlavickovy soubor pro tridu Mista
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
using namespace std;

#pragma once

class Znacka;				//Dopredna deklarace -> aby se vedelo ze neco takoveho existuje

class Mista
{
//Privatni promenne tridy
private:
        int Kapacita;   		//Kapacita mista
        int P_stav;     		//Pocatecni stav mista
        int A_stav;			//Aktualni stav mista
	std::string nazev;		//nazev prechodu
	int pocetZnacekC;		//Pocet znacek v danem miste

public:
//Deklarace tridnich metod
	Znacka* znacka;
	std::list<Znacka *> Znacky_list;	//Seznam znacek v danem miste
	void test();
	Mista(std::string nazev, int pocet, int kapacita);
	~Mista(void);
	void nastav_kapacitu(int kapacita);
	void nastav_p_stav(int p_stav);
	void nastav_a_stav(int a_stav);
	int dej_kapacitu();
	int dej_p_stav();
	int dej_a_stav();
	std::string dej_nazev();
	void pridej_znacku(Znacka* znacka);
	void vypis_znacky();
	int vrat_velikost_listu();
	void znackaplus();
	void znackaminus();
	int pocetZnacek();
	bool lock = false; // zamknuti mista pri naplanovani udalosti
};
