//Hrany.h - hlavickovy soubor pro tridu Mista
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//			Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#define PM	1 // urceni smeru hrany - prechod -> misto
#define MP -1 // urceni smeru hrany - misto -> prechod
#pragma once
class Mista;
class Prechod;

class Hrany
{
private:
//Privatni promenne tridy
        int Vaha;   //Vaha hrany
	std::string nazev; // nazev hrany

public:
//Deklarace tridnich metod
	void test();
	Hrany(Mista *misto, Prechod *hrana, int typ, int vaha);
	~Hrany(void);
	//void nastav_hranu(Mista *misto, Hrany *hrana, int typ, int vaha);
	int dej_vahu();
	std::string dej_nazev();  // vrati nazev hrany
	Mista *Misto_io; //misto vstup,vystup
	Prechod *Prechod_io; //hrana vstup,vystup
	int Typ;
};
