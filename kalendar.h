//mista.h - hlavickovy soubor pro tridu kalendar
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//	    Petr Grenar, xgrena06@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015

class Prechod;

class Udalost{
	public:
			Udalost(double cas_udalosti, Prechod * prechod, double cas_zpozdeni);
			~Udalost();
			double cas_udalosti;
			Prechod *dej_prechod_ukazatel();
			double dej_cas_udalosti();
			
	private:
			Prechod * prechod;
			double cas_zpozdeni;
			bool compareByTime(Udalost *a, Udalost *b);
			
	
/*	bool operator <(const Udalost& another) const
	{
		return (this->cas_udalosti < another.cas_udalosti); // overload Operator
	}
*/

};

class Kalendar
{
private:
//Privatni promenne tridy
        double Time;   //Vaha hrany
		std::string nazev; // nazev hrany
		std::list<Udalost *> seznam_udalosti; //seznam udalosti 

public:
//Deklarace tridnich metod
	Kalendar(); // konstruktor
	~Kalendar(); //destruktor
	void pridej_udalost(Udalost * udalost); 
	Udalost *dej_dalsi_udalost();
	void smaz_udalost(); // smaze prvni udalost
	
	bool prazdny_kalendar(); //vrati true pokud uz neni zadna udalost k vykonani
	void nastav_aktualni_cas(double cas);
	double dej_aktualni_cas();
	void vytiskni_aktualni_cas();
	void vytiskni_kalendar();
};
