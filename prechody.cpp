//prechody.cpp - trida reprezentujici Prechody
#include "prechody.h"
#include <stdio.h>

//Konstruktory
Prechody::Prechody(void)
{
}

Prechody::~Prechody(void)
{
}

//Zkuska funkcnosti metody tridy
void Prechody::test(void)
{
	printf("Já jsem metoda třídy prechody :D\n");
}

//Metoda pro nastaveni typu prechodu
void Prechody::nastav_typ(int typ)
{
	Typ = typ;
}

//Metoda pro nastaveni priority prechodu
void Prechody::nastav_prioritu(int priorita)
{
	Priorita = priorita;
}

//Metoda pro nastaveni casovani prechodu
void Prechody::nastav_casovani(int casovani)
{
	Casovani = casovani;
}

//Metoda pro nastaveni pravdepodobnosti prechodu
void Prechody::nastav_pravdepodobnost(int pravdepodobnost)
{
	Pravdepodobnost = pravdepodobnost;
}

//Metoda zajistujici nastaveni prechodu podle zadaneho typu
void Prechody::nastav_prechod(int typ, int parametr)
{
	nastav_typ(typ);	
	if(typ == 0)
	{
		nastav_prioritu(parametr);
	}
	else if(typ == 1)
	{
		nastav_casovani(parametr);
	}
	else if(typ == 2)
	{
		nastav_pravdepodobnost(parametr);
	}
}

//Metody vracejici ty, prioritu, casovani a pravdepodobnost prechodu
int Prechody::dej_typ()
{
	return Typ;
}

int Prechody::dej_prioritu()
{
	return Priorita;
}

int Prechody::dej_casovani()
{
	return Casovani;
}

int Prechody::dej_pravdepodobnost()
{
	return Pravdepodobnost;
}
