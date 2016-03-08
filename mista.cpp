//mista.cpp - trida reprezentujici Mista
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#include "mista.h"
#include "znacka.h"
#include <stdio.h>
#include <string>
#include <climits>

//Konstruktory
Mista::Mista(std::string nazev, int pocet, int kapacita)
{
	Kapacita = kapacita;
	P_stav = pocet;
	A_stav = pocet;
	this->nazev = nazev;
	pocetZnacekC = pocet;
}

Mista::~Mista(void)
{
}

//Zkouska funkcnosti metody tridy
void Mista::test(void)
{
	printf("Já jsem metoda třídy mista :D\n");
}

//Metoda pro nastaveni kapacity mista
void Mista::nastav_kapacitu(int kapacita)
{
	Kapacita = kapacita;	
}

//Metoda pro nastaveni pocatecniho stavu
void Mista::nastav_p_stav(int p_stav)
{
	P_stav = p_stav;
}

//Metoda pro nastaveni aktuaniho stavu
void Mista::nastav_a_stav(int a_stav)
{
	A_stav = a_stav;
}

//Metody vracejici kapacitu, pocatecni a aktualni stav mista
int Mista::dej_kapacitu()
{
	return Kapacita;
}

int Mista::dej_p_stav()
{
	return P_stav;
}

int Mista::dej_a_stav()
{
	return A_stav;
}

std::string Mista::dej_nazev()
{
	return this->nazev;
}

void Mista::pridej_znacku(Znacka* znacka)
{
	Znacky_list.push_back(znacka);
}

int Mista::vrat_velikost_listu()
{
	return Znacky_list.size();
}

void Mista::vypis_znacky()
{
	std::list<Znacka *>::const_iterator iterator;
	for (iterator = Znacky_list.begin(); iterator != Znacky_list.end(); ++iterator) {
		std::cout << (*iterator)->dej_nazev();
	}
}

void Mista::znackaplus()
{
	pocetZnacekC++;
}

void Mista::znackaminus()
{
	pocetZnacekC--;
}

int Mista::pocetZnacek()
{
	return pocetZnacekC;
}
