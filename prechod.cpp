//prechody.cpp - trida reprezentujici Prechod
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#include "prechod.h"
#include <stdio.h>
#include <string>
#include <ctime>
#include <cmath>


//typy prechodu 
#define P_CASRR 10	//rovnomerne rozlozeni
#define P_EXPON 11	//exponencialni
#define P_PRIOR 12	//prioritni
#define P_PRAVD 13	//pravdepodobnostni
#define P_KONST 14 // konstantni 


//Konstruktory
Prechod::Prechod(std::string nazev)
{
	this->nazev = nazev;
}

Prechod::~Prechod(void)
{
}

//Zkuska funkcnosti metody tridy
void Prechod::test(void)
{
	printf("Já jsem metoda třídy prechody :D\n");
}

//Metoda pro nastaveni typu prechodu
void Prechod::nastav_typ(int typ)
{
	Typ = typ;
}

//Metoda pro nastaveni priority prechodu
void Prechod::nastav_prioritu(int priorita)
{
	Priorita = priorita;
}

//Metoda pro nastaveni casovani prechodu
void Prechod::nastav_casovani(int casovani)
{
	Casovani = casovani;
}

//Metoda pro nastaveni pravdepodobnosti prechodu
void Prechod::nastav_pravdepodobnost(int pravdepodobnost)
{
	Pravdepodobnost = pravdepodobnost;
}

//Metoda zajistujici nastaveni prechodu podle zadaneho typu
void Prechod::nastav_prechod(int typ, int parametr, int parametr2)
{
	nastav_typ(typ);	
	if(typ == P_PRIOR)
	{
		nastav_prioritu(parametr);
	}
	else if(typ == P_CASRR)
	{
		if (parametr != 0 or parametr2 != 0){
			Casovani = parametr;
			Casovani2 = parametr2;
			//Casovani = double(((rand()%((parametr2-parametr))) + parametr));
		}
		else
			Casovani = 0;
	}
	else if (typ == P_EXPON)
	{
		//generovani double cisla z intervalu <0;max)
		Casovani = parametr;
		//Casovani = (-parametr * std::log(rand()/RAND_MAX+1.0));
	}
	else if(typ == P_PRAVD)
	{
		nastav_pravdepodobnost(parametr);
	}
}

//Metody vracejici typ, prioritu, casovani a pravdepodobnost prechodu
int Prechod::dej_typ()
{
	return Typ;
}

int Prechod::dej_prioritu()
{
	return Priorita;
}

double Prechod::dej_casovani()
{
	return Casovani;
}

int Prechod::dej_pravdepodobnost()
{
	return Pravdepodobnost;
}

std::string Prechod::dej_nazev()
{
	return this->nazev;
}
void Prechod::pridej_in_hranu(Hrany* hrana){

	vector_vstupnich_hran.push_back(hrana);
}
void Prechod::pridej_out_hranu(Hrany* hrana){

	vector_vystupnich_hran.push_back(hrana);
}
