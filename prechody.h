//prechody.h - hlavickovy soubor pro tridu Prechody
#pragma once
class Prechody
{
private:
//Privatni promenne tridy
        int Typ;                //Typ prechodu => 0 = pripritni; 1 = casovany; 2 = pravdepodobnostni
        int Priorita;           //Priorita prechodu
        int Casovani;           //Casovani prechodu
        int Pravdepodobnost;    //Pravdepodobnost prechodu

public:
//Deklarace tridnich metod
	void test();
	Prechody(void);
	~Prechody(void);
	void nastav_typ(int typ);
	void nastav_prioritu(int priorita);
	void nastav_casovani(int casovani);
	void nastav_pravdepodobnost(int pravdepodobnost);
	void nastav_prechod(int typ, int parametr);
	int dej_typ();
	int dej_prioritu();
	int dej_casovani();
	int dej_pravdepodobnost();
};
