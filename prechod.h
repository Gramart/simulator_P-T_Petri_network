//prechody.h - hlavickovy soubor pro tridu Prechod
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#include <vector>

#pragma once
class Hrany;

class Prechod
{
private:
//Privatni promenne tridy
        int Typ;                //Typ prechodu => 0 = pripritni; 1 = casovany; 2 = pravdepodobnostni
        int Priorita;           //Priorita prechodu

        int Pravdepodobnost;    //Pravdepodobnost prechodu
	std::string nazev; // nazev prechodu 

public:
//Deklarace tridnich metod
	void test();
	Prechod(std::string nazev);
	~Prechod(void);
	void nastav_typ(int typ);
	void nastav_prioritu(int priorita);
	void nastav_casovani(int casovani);
	void nastav_pravdepodobnost(int pravdepodobnost);
	void nastav_prechod(int typ, int parametr, int parametr2);
	void pridej_in_hranu(Hrany* hrana);
	void pridej_out_hranu(Hrany* hrana);
	int dej_typ();
	int dej_prioritu();
	double dej_casovani();
	int dej_pravdepodobnost();
	double Casovani;           //Casovani prechodu
	double Casovani2;			//U rovnomerneho rozlozeni - max hodnota
	int pocet_provedeni = 0;
	double min_prichod = 0;
	double max_prichod = 0;
	double prumerny_prichod = 0;
	bool lock = false;
	std::string dej_nazev(); // vrati nazev prechodu
	std::vector<Hrany *> vector_vstupnich_hran; //ulozene vstupni hrany prechodu
	std::vector<Hrany *> vector_vystupnich_hran; // ulozene vystupni hrany prechodu
	
};
