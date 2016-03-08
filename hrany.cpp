//hrany.cpp - trida reprezentujici Hrany
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#include "hrany.h"
#include "mista.h"

#include <stdio.h>
#include <string>

//Konstruktory

Hrany::Hrany(Mista *misto, Prechod *prechod, int typ, int vaha)
//Hrany::Hrany(std::string nazev)
{
	//Vaha = 1;
	//this->nazev = nazev;
	Misto_io = misto;
	Prechod_io = prechod;
	Typ = typ;
	Vaha = vaha;
}

Hrany::~Hrany(void)
{
}

//Zkouska funkcnosti metody tridy
void Hrany::test(void)
{
	printf("Já jsem metoda třídy Hrany :D\n");
}

//Metoda pro nastaveni vahy hrany
/*
void Hrany::nastav_hranu(Mista *misto, Hrany *hrana, int typ, int vaha)
{
	Misto_io = misto;
	Hrana_io = hrana;
	Typ = typ;
	Vaha = vaha;
}*/

//Metoda vracejici vahu hrany
int Hrany::dej_vahu()
{
	return Vaha;
}
std::string Hrany::dej_nazev()
{
	return this->nazev;
}
