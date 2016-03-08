//prechody.h - hlavickovy soubor pro tridu Znacka
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015
#pragma once
class Znacka
{
private:
//Privatni promenne tridy
	std:: string nazev; 	// nazev prechodu
	Mista* misto;		//Kde se znacka nachazi

public:
//Deklarace tridnich metod
	void test();
	Znacka(std::string nazev, Mista* misto);	//Inicializace
	~Znacka(void);
	std::string dej_nazev(); 			// vrati nazev znacky
	std::string dej_misto(); 			// vrati nazev mista
	void zmen_misto(Mista* misto);			//Zmena mista
};
