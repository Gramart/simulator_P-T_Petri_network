//prechody.cpp - trida reprezentujici Znacka
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#include "znacka.h"
#include <stdio.h>
#include <string>

//Konstruktory
Znacka::Znacka(std::string nazev, Mista* misto)
{
	this->nazev = nazev;
	this->misto = misto;
}

Znacka::~Znacka(void)
{
}

//Zkuska funkcnosti metody tridy
void Znacka::test(void)
{
	printf("Já jsem metoda třídy Znacka :D\n");
}

//Vrati nazev znacky
std::string Znacka::dej_nazev()
{
	return this->nazev;
}

//Vrati nazev mista ve kterem se nachazi
std::string Znacka::dej_misto()
{
	return this->misto->dej_nazev();
}


//Zmeni misto
void Znacka::zmen_misto(Mista* misto)
{
	this->misto = misto;
}
