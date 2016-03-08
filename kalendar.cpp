//mista.cpp - trida reprezentujici kalendar
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015

#include "kalendar.h"
#include "prechod.h"
#include <vector>
#include <algorithm>

#include <iostream>


using namespace std;

Udalost::Udalost(double cas_udalosti, Prechod * prechod, double cas_zpozdeni){ //konstruktor udalosti
	this->cas_udalosti = cas_udalosti;
	this->cas_zpozdeni = cas_zpozdeni;
	this->prechod = prechod;

}
Udalost::~Udalost(){ //destruktor udalosti

}
Prechod * Udalost::dej_prechod_ukazatel(){ // ziskani ukazatele na prechod
	return prechod;
}
double Udalost::dej_cas_udalosti(){
	return this->cas_udalosti;
}

Kalendar::Kalendar()// konstruktor
{
	this->Time = 0; //startovaci cas 
} 
Kalendar::~Kalendar() //destruktor
{
}
void Kalendar::nastav_aktualni_cas(double cas){
	this->Time = cas;
}
double Kalendar::dej_aktualni_cas(){
	return this->Time;
}
void Kalendar::pridej_udalost(Udalost * udalost)
{
	seznam_udalosti.push_back(udalost);
} 
/*
bool operator<(Udalost const & lhs, Udalost const & rhs) {
	return lhs.cas_udalosti < rhs.cas_udalosti; //overload operator < 
}
*/
bool compareByTime(Udalost *a, Udalost *b){
	return a->cas_udalosti < b->cas_udalosti;
}
Udalost * Kalendar::dej_dalsi_udalost()
{	
	//pred vracenim udalosti s nejmensim casem je seznam treba seradit
	//std::cout << "kalendar pred serazenim: \n";
	//this->vytiskni_kalendar();
	seznam_udalosti.sort(compareByTime);
	//seznam_udalosti.sort();
	//std::cout << "\nkalendar po serazeni: \n";
	//this->vytiskni_kalendar();
	//sort(seznam_udalosti.begin(),seznam_udalosti.end(),compareByTime); // serad podle casu udalosti
	return *(this->seznam_udalosti.begin());
}
void Kalendar::smaz_udalost()
{ // smaze prvni udalost
	seznam_udalosti.pop_front();
}
bool Kalendar::prazdny_kalendar() //vrati true pokud uz neni zadna udalost k vykonani
{
	return seznam_udalosti.empty();
}
void Kalendar::vytiskni_aktualni_cas()
{
	cout << "Aktualni cas: " << this->Time << std::endl;
}
void Kalendar::vytiskni_kalendar(){
	std::list<Udalost *>::const_iterator iterator;
	for (iterator = seznam_udalosti.begin(); iterator != seznam_udalosti.end(); ++iterator) {
		std::cout << "|---------------------------------------------------------------------------|\n";
		std::cout << (*iterator)->dej_prechod_ukazatel()->dej_nazev() << " " << (*iterator)->cas_udalosti << std::endl;
		//std::cout << "|---------------------------------------------------------------------------|\n";
		}
}