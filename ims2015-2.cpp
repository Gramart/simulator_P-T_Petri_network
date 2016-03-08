#include <list>
#include <iostream>
#include "stdlib.h"
#include "unistd.h"
#include "mista.cpp"
#include "prechod.cpp"
#include "hrany.cpp"
#include "kalendar.cpp"
#include "znacka.cpp"
#include <climits>
#include <cmath>
#include <iomanip>  //setw
#define H_PM	1 // urceni smeru hrany - prechod -> misto
#define H_MP -1 // urceni smeru hrany - misto -> prechod

#define P_CASRR 10	//rovnomerne rozlozeni
#define P_EXPON 11	//exponencialni
#define P_PRIOR 12	//prioritni
#define P_PRAVD 13	//pravdepodobnostni
#define P_KONST 14 // konstantni 

//file: ims2015-2.cpp - main file 
//authors:  Martin Graca, xgraca00@stud.fit.vutbr.cz
//projekt: IMS - simulator P/T petriho siti
//datum: 8. 12. 2015

static int d_v = 0;		//Pro zapnuti nebo vypnuti pomocnych vypisu
Kalendar *kalendarik = new Kalendar();
using namespace std;
std::vector<Mista *> vektor_mist; //pro vypis stavu vsech mist
int model = 0;

#if(LONG_MAX<(1ULL<<32))
typedef long myint32;
#else
typedef int myint32;
#endif

const myint32 pom1 = 1537L;
const myint32 pom2 = 1220703125L;
const myint32 pom3 = 0x7FFFFFFFUL;

static myint32 SRS = pom1;

void RS(long s)
{
	SRS = s;
}

double RB()
{
	SRS *= 	pom2;
	SRS &= pom3;
	double r = static_cast<double>(SRS)/pom3;
	return r;
}

static double (*SRBP)() = RB;

double Random()
{
	return SRBP();
}

double Normalni(double low, double high)
{
	//generovani hodnoty pro prechod s rovnomernym rozlozenim
	return (low+(high-low)*Random());
}

double Exponencialni(double stredni_hodnota)
{
	//exponencialni rozlozeni
	double exp = -stredni_hodnota * std::log(Random());
	return exp;
}

void zamkni_odemkni_misto(Prechod *aktualni_prechod, bool zamkni)
{
	//funkce pro zamknuti nebo odemknuti mista pri casovanem prechodu
	for (unsigned int i=0; i<aktualni_prechod->vector_vstupnich_hran.size();i++){ //pruchod vstupnich hran
		if (zamkni == true)
			aktualni_prechod->vector_vstupnich_hran[i]->Misto_io->lock = true;
		else //(zamkni == false)
			aktualni_prechod->vector_vstupnich_hran[i]->Misto_io->lock = false;
	}

}

void vytiskni_stav_mist(std::vector<Mista *> v_m)
{
	//vytiskne aktualni stav vsech mist
	cout << "==============================================================================\n";
	for (unsigned int i=0; i<v_m.size();i++){
		printf("%-10s%-21s%-6s%-10d\n","# MISTO: ", v_m[i]->dej_nazev().c_str()," STAV: ",v_m[i]->dej_a_stav());
	}
	cout << "==============================================================================\n";
}
void vytiskni_statistiky(std::vector<Prechod *> v_p)
{
	cout << "==================STATISTIKY PRECHODU=========================================\n";
	for (unsigned int i=0; i<v_p.size();i++)
	{
		if (v_p[i]->dej_typ() != P_EXPON)
		{
			printf("%-10s%-25s%-15s%-10d\n","# PRECHOD: ", v_p[i]->dej_nazev().c_str()," POCET PROVEDENI: ",v_p[i]->pocet_provedeni);
		}
		else //u exponencialniho prechodu mame vice statistiky
		{ 
			cout << "------------------------------------------------------------------------------\n";
			if (v_p[i]->dej_nazev() == "prechod_pracuju")
			{
				cout << "# PRECHOD SIMULUJICI PRACI NA PC: " << v_p[i]->dej_nazev() << endl;
			}
			else
			{
				if(model == 0)
				{
					cout << "# PRECHOD SIMULUJICI PRICHOD ZAKAZNIKU: " << v_p[i]->dej_nazev() << endl;
				}
				else if(model == 1)
				{
					cout << "# PRECHOD SIMULUJICI PRICHOD STUDENTU: " << v_p[i]->dej_nazev() << endl;
				}
			}
			cout << "POCET PROVEDENI: " << v_p[i]->pocet_provedeni << endl; 
			cout << "MINIMALNI DOBA: " << v_p[i]->min_prichod << endl;
			cout << "MAXIMALNI DOBA: " << v_p[i]->max_prichod  << endl;
			cout << "PRUMERNA DOBA: " << v_p[i]->prumerny_prichod << endl;
			cout << "------------------------------------------------------------------------------\n";
		}
	}
	cout << "==============================================================================\n";
}
bool presunMHP(Mista* zdroj, Hrany* h, Prechod* p)		//misto->hrana->prechod
{
	bool zdrojOut = false;
	bool hOk = false;
	if(zdroj->dej_a_stav() >= 1)
	{ 
		if(d_v)
		{
			printf("Ve zdrojovem miste je dost tecek.\n");
		}
		zdrojOut = true;
	}
	else
	{	
		if(d_v)
		{
			printf("Ve zdrojovem miste neni dost tecek.\n");
		}
	}
	if((zdroj->dej_a_stav() - h->dej_vahu()) >= 0)
	{
		if(d_v)
		{
			printf("hrana zvladne prenos.\n");
		}
		hOk = true;
	}
	else
	{
		if(d_v)
		{
			printf("hrana nezvladne prenos.\n");
		}
	}
	if(zdrojOut == true && hOk == true)
	{
		if(p->dej_typ() == 1)
		{
			if(d_v)
			{
				printf("Presun z mista do prechodu je mozny\n");
				printf("Cekam\n");
			}
		
			sleep(p->dej_casovani());
		}	
		return true;
	}
	else
	{
		return false;
	}
}

bool presunPHM(Hrany* h, Mista* zdroj, Mista* cil)		//prechod->hrana->misto
{
	if(cil->dej_kapacitu() >= (cil->dej_a_stav() + h->dej_vahu()))
	{
		if(d_v)
		{
			printf("V cilovem miste je dost mista\n");
		}
		return true;
	}
	else
	{
		if(d_v)
		{
			printf("V cilovem miste neni dost mista\n");
		}
		return false;
	}
}

void presun(Mista* zdroj, Mista* cil, Hrany* h1, Hrany* h2, Prechod* prechod, Znacka* z)		//zde se provede konecny prechod
{
	if(d_v)
	{
		printf("Provadim presun\n");
	}
	
	if((presunMHP(zdroj, h1, prechod) == true) && (presunPHM(h2, zdroj, cil) == true))
	{
		zdroj->nastav_a_stav(zdroj->dej_a_stav() - h1->dej_vahu());
		z->zmen_misto(cil);
		cil->nastav_a_stav(cil->dej_a_stav() + h2->dej_vahu());
		zdroj->znackaminus();
		cil->znackaplus();
		if(d_v)
		{
			printf("presun dokoncen\n");
		}
	}
	else
	{
		if(d_v)
		{
			printf("presun neumoznen\n");
		}
	}	
}

void proved_presun(Prechod* prechod)
{
	for (unsigned int i=0; i<prechod->vector_vstupnich_hran.size();i++) //smazeme znacky ze vstupnich mist
	{
		Mista* aktualni_misto = prechod->vector_vstupnich_hran[i]->Misto_io;
		Hrany* aktualni_hrana = prechod->vector_vstupnich_hran[i];
		if(d_v)
		{
			std::cout << "stav " << aktualni_misto->dej_nazev() << " pred presunem :" << aktualni_misto->dej_a_stav() << std::endl;
		}
		aktualni_misto->nastav_a_stav(aktualni_misto->dej_a_stav() - aktualni_hrana->dej_vahu());
		if(d_v)
		{
			std::cout << "stav " << aktualni_misto->dej_nazev() << " po presunu :" << aktualni_misto->dej_a_stav() << std::endl;	
		}
	}
	for (unsigned int i=0; i<prechod->vector_vystupnich_hran.size();i++) //pridame znacky do vystupnich mist
	{
		Hrany * aktualni_hrana = prechod->vector_vystupnich_hran[i];
		Mista* aktualni_misto = prechod->vector_vystupnich_hran[i]->Misto_io;
		if(d_v)
		{
			std::cout << "stav " << aktualni_misto->dej_nazev() << " pred presunem :" << aktualni_misto->dej_a_stav() << std::endl;
		}
		aktualni_misto->nastav_a_stav(aktualni_misto->dej_a_stav() + aktualni_hrana->dej_vahu());
		if(d_v)
		{
			std::cout << "stav " << aktualni_misto->dej_nazev() << " po presunu :" << aktualni_misto->dej_a_stav() << std::endl;
		}
	}
	prechod->pocet_provedeni = prechod->pocet_provedeni + 1; //zvysime pocet provedeni prechodu pro statistiky
	zamkni_odemkni_misto(prechod, false); //odemkni mista
	if(prechod->dej_typ() == P_EXPON) //odemknuti expon prechodu
		prechod->lock = false;
		vytiskni_stav_mist(vektor_mist);
}

bool je_proveditelny(Prechod *aktualni_prechod)
{
	if(aktualni_prechod != NULL)
	{
		if(aktualni_prechod->dej_typ() == P_EXPON) // u exponencialnich zkontrolovat jen zamknuti prechodu
		{
			if(aktualni_prechod->lock == true)
			{ //pokud je prechod zamknuti, je neproveditelny
				return false;
			}
		}
		else
		{
			for(unsigned int i=0; i<aktualni_prechod->vector_vstupnich_hran.size();i++)
			{ //pruchod vstupnich hran	
				if((presunMHP(aktualni_prechod->vector_vstupnich_hran[i]->Misto_io,aktualni_prechod->vector_vstupnich_hran[i],aktualni_prechod) == false) or (aktualni_prechod->vector_vstupnich_hran[i]->Misto_io->lock == true))
				{
					return false;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool compareByPriority(Prechod *a, Prechod *b)
{
	return a->dej_prioritu() > b->dej_prioritu();
}

Prechod* prioritni_presun(vector<Prechod *> vec_pre)
{
	for (unsigned int i=0; i<vec_pre.size();i++)
	{ //pruchod vstupnich hran
		if ( je_proveditelny(vec_pre[i]) )
		{
			return vec_pre[i];
		}
	}
	return NULL;	
}

Prechod* pravdep_presun(Prechod* p1, Prechod* p2)
{
	int v1 = rand() % 100 + 1;
	if(d_v)
	{
		std::cout << "Nahodne cislo " << v1 << "\n";
	}
	if((p1->dej_typ() == P_PRAVD) && (p2->dej_typ() == P_PRAVD))
	{
		if(d_v)
		{
			printf("Oba prechody jsou pravdepodobnostni\n");
		}
		if(v1 <= (p1->dej_pravdepodobnost()))
		{
			if(d_v)
			{
				printf("Pujdeme do prvniho prechodu\n");
			}
			return p1;
		}
		else
		{
			if(d_v)
			{
				printf("Pujdeme do druheho prechodu\n");
			}
			return p2;
		}
	}
	return 0;
}

void projdi_prechody(vector<Prechod *> prechody, vector< Prechod *> prioritni_prechody, vector<Prechod *> pravdep_prechody,vector<Prechod *> pravdep_prechody2)
{
	//projde vsechny prechody a provede ty ktere se daji provest
	bool prechod_proveditelny;
	std::random_shuffle(prechody.begin(),prechody.end());
	 
	for (unsigned int i=0; i<prechody.size();i++)	//pruchod vstupnich hran
	{
		Prechod * aktualni_prechod = prechody[i];
		
		if (aktualni_prechod->dej_typ() == P_PRIOR){ //priorita
			aktualni_prechod = prioritni_presun(prioritni_prechody);
			if (aktualni_prechod != NULL)
			{
				prechod_proveditelny = true;
			}
			else
			{
				prechod_proveditelny = false;
			}
			if (je_proveditelny(aktualni_prechod))
			{
				if(d_v)
				{
					std::cout << "PRESUN PRIORITNIHO: " << aktualni_prechod->dej_nazev() << std::endl;
				}
				proved_presun(aktualni_prechod);
			}
			else
			{
				if(d_v)
				{
					std::cout << "zadny prioritni prechod neni proveditelny\n";
				}
			}
		}
		else if (aktualni_prechod->dej_typ() == P_KONST)
		{
			// prechod muzeme provest hned pokud jde
			prechod_proveditelny = je_proveditelny(aktualni_prechod);
			if (prechod_proveditelny)
			{
				if(d_v)
				{
					cout << "---zpracovavany konstatni prechod: " << aktualni_prechod->dej_nazev() << "---"<<std::endl;
				}
				proved_presun(aktualni_prechod);
			}		
		}
		else if ( (aktualni_prechod->dej_typ() == P_CASRR) or (aktualni_prechod->dej_typ() == P_EXPON) ) //cas
		{			
			// zkontrolujeme zda je prechod proveditelny			
			prechod_proveditelny = je_proveditelny(aktualni_prechod);
			
			if (prechod_proveditelny) //prechod se da provest, naplanujeme ho
			{
				if(d_v)
				{
					std::cout << aktualni_prechod->dej_nazev() << " je proveditelny, planuju udalost \n";
				}
				double zpozdeni = 0;
				if (aktualni_prechod->dej_typ() == P_EXPON)
				{
					aktualni_prechod->lock = true;
					zpozdeni = Exponencialni(aktualni_prechod->Casovani);
					if (aktualni_prechod->min_prichod > zpozdeni)
					{
						aktualni_prechod->min_prichod = zpozdeni;
					}
					if (aktualni_prechod->max_prichod < zpozdeni)
					{
						aktualni_prechod->max_prichod = zpozdeni;
					}
					aktualni_prechod->prumerny_prichod = ((aktualni_prechod->prumerny_prichod + zpozdeni)/(aktualni_prechod->pocet_provedeni+1));
					if(d_v)
					{	
						std::cout << "Exponencial zpozdeni: " << zpozdeni << std::endl;
					}
				}
				else
				{ // P_CASRR
					zpozdeni = Normalni(aktualni_prechod->Casovani,aktualni_prechod->Casovani2);
					if(d_v)
					{
						std::cout << "Normalni zpozdeni: " << zpozdeni << std::endl;
					}
				}
				Udalost * nova_udalost = new Udalost(kalendarik->dej_aktualni_cas() + zpozdeni,aktualni_prechod,zpozdeni); // naplanovani nove udalost
				if(d_v)
				{
					std::cout << "udalost naplanovana na : " << (kalendarik->dej_aktualni_cas() + zpozdeni) << std::endl;
				}
				kalendarik->pridej_udalost(nova_udalost); // pridani udalosti do kalendare
				zamkni_odemkni_misto(aktualni_prechod, true); //zamkni mista, aby nedoslo k dalsimu naplanovani dokud se prechod neprovede
			}
			else{
				if(d_v)
				{
					std::cout << aktualni_prechod->dej_nazev() << " neni proveditelny \n";
				}
			}
		}
		else if (aktualni_prechod->dej_typ() == P_PRAVD) //pravdepodobnost
		{
			if(model == 0)
			{
				aktualni_prechod = pravdep_presun(pravdep_prechody[0],pravdep_prechody[1]);
			}
			else if(model == 1)
			{
				if((aktualni_prechod->dej_nazev() == "prechod_chci_pc") or (aktualni_prechod->dej_nazev() == "prechod_nechci_pc"))
				{
					aktualni_prechod = pravdep_presun(pravdep_prechody[0],pravdep_prechody[1]);
				}
				else{
					aktualni_prechod = pravdep_presun(pravdep_prechody2[0],pravdep_prechody2[1]);
				}
			}
			if (je_proveditelny(aktualni_prechod))
			{
				if(d_v)
				{
					std::cout << "##provadim pravd prechod: " << aktualni_prechod->dej_nazev() << endl;
				}
				proved_presun(aktualni_prechod);
			}
			else
			{
				if(d_v)
				{
					std::cout << " ##pravdepodobnostni prechod " << aktualni_prechod->dej_nazev() << " neni proveditelny \n";
				}
			}	
		}
		else
		{
			if(d_v)
			{
				std::cout << "neznamy prechod\n";
			}
		}
		prechod_proveditelny = true; //nastavime do puvodniho stavu pro dalsi iteraci	
	}
}

int main(int argc, char* argv[] )
{	
	std::vector<Prechod *>  Prechody_list; // seznam vsech prechodu
	std::vector<Prechod *> prioritni_prechody;
	std::vector<Prechod *> pravdep_prechody;
	std::vector<Prechod *> pravdep_prechody2;
	if (argc != 3){
		cout << "spatne parametry: \n";
		cout << "example: make run 0 500\n";
		exit(1);
	}	
	if (atoi(argv[1]) == 0)
	{
		//////////////////////PEKARNA///////////////////////////////////////////	
		model = 0;
	
		Mista* misto_pekar = new Mista("misto_pekar", 1, 1);		//Misto1
		Mista* misto_dopeceno = new Mista("misto_dopeceno", 0,INT_MAX);	//Misto2
		Mista* misto_pult = new Mista("misto_pult", 0,INT_MAX);	//Misto3
		
		Mista* misto_zakaznik = new Mista("misto_zakaznik",0,INT_MAX);
		Mista* misto_je_chleba = new Mista("misto_je_chleba",0,INT_MAX);
		Mista* misto_prodavac = new Mista("misto_prodavac",1,1);
		Mista* misto_mam_prodavace = new Mista("misto_mam_prodavace",0,INT_MAX);
		Mista* misto_mam_chleba = new Mista("misto_mam_chleba",0,INT_MAX);
		Mista* misto_obslouzen = new Mista("misto_obslouzen",0,INT_MAX);
		Mista* misto_odchod = new Mista("misto_odchod",0,INT_MAX);
		
		vektor_mist.push_back(misto_pekar);vektor_mist.push_back(misto_dopeceno);vektor_mist.push_back(misto_pult);
		vektor_mist.push_back(misto_zakaznik);vektor_mist.push_back(misto_je_chleba);vektor_mist.push_back(misto_prodavac);
		vektor_mist.push_back(misto_mam_prodavace);vektor_mist.push_back(misto_mam_chleba);vektor_mist.push_back(misto_obslouzen);
		vektor_mist.push_back(misto_odchod);
		Prechod* prechod_priprava =  new Prechod("prechod_priprava");	//Prechod1
		Prechody_list.push_back(prechod_priprava);
		prechod_priprava->nastav_prechod(P_CASRR,200,250);
		
		Prechod* prechod_na_pult = new Prechod("prechod_na_pult");	//Prechod2
		Prechody_list.push_back(prechod_na_pult);
		prechod_na_pult->nastav_prechod(P_CASRR,5,10);
		
		Prechod* prechod_prichod = new Prechod("prechod_prichod");	//Prechod3
		Prechody_list.push_back(prechod_prichod);
		prechod_prichod->nastav_prechod(P_EXPON,5,0);

		Prechod* prechod_vem_prodavace = new Prechod("prechod_vem_prodavace");	//Prechod4
		Prechody_list.push_back(prechod_vem_prodavace);
		prechod_vem_prodavace->nastav_prechod(P_KONST,0,0);
		
		Prechod *prechod_kontrola_chleba = new Prechod("prechod_kontrola_chleba");	//Prechod4
		Prechody_list.push_back(prechod_kontrola_chleba);
		prechod_kontrola_chleba->nastav_prechod(P_PRIOR,1,0);
		
		Prechod *prechod_vem_chleba = new Prechod("prechod_vem_chleba");	//Prechod4
		Prechody_list.push_back(prechod_vem_chleba);
		prechod_vem_chleba->nastav_prechod(P_PRIOR,2,0);
		
		Prechod *prechod_odchazi_hned = new Prechod("prechod_odchazi_hned");	//Prechod4
		Prechody_list.push_back(prechod_odchazi_hned);
		prechod_odchazi_hned->nastav_prechod(P_PRIOR,0,0);
		
		Prechod* prechod_obsluha = new Prechod("prechod_obsluha");	//Prechod4
		Prechody_list.push_back(prechod_obsluha);
		prechod_obsluha->nastav_prechod(P_CASRR,1,2);
		
		Prechod* prechod_odchod = new Prechod("prechod_odchod");	//Prechod4
		Prechody_list.push_back(prechod_odchod);
		prechod_odchod->nastav_prechod(P_PRAVD,80,0);
		
		Prechod* prechod_odchazi_obslouzen = new Prechod ("prechod_odchazi_obslouzen");
		Prechody_list.push_back(prechod_odchazi_obslouzen);
		prechod_odchazi_obslouzen->nastav_prechod(P_KONST,0,0);
		
		Prechod* prechod_vratit_se = new Prechod("prechod_vratit_se");	//Prechod4
		Prechody_list.push_back(prechod_vratit_se);
		prechod_vratit_se->nastav_prechod(P_PRAVD,20,0);
		
		Hrany* hrana_pekar_priprava =  new Hrany(misto_pekar,prechod_priprava,H_MP,1);		//Hrana1
		Hrany* hrana_priprava_dopeceno =  new Hrany(misto_dopeceno,prechod_priprava,H_PM,1);		//Hrana1
		Hrany* hrana_dopeceno_na_pult =  new Hrany(misto_dopeceno,prechod_na_pult,H_MP,1);		//Hrana1
		Hrany* hrana_na_pult_pekar =  new Hrany(misto_pekar,prechod_na_pult,H_PM,1);		//Hrana1
		Hrany* hrana_na_pult_pult = new Hrany(misto_pult,prechod_na_pult,H_PM,20);
		Hrany* hrana_prichod_zakaznik = new Hrany(misto_zakaznik,prechod_prichod,H_PM,1);
		
		Hrany* hrana_zakaznik_odchazi = new Hrany(misto_zakaznik,prechod_odchazi_hned,H_MP,1);
		Hrany* hrana_zakaznik_kontrola = new Hrany(misto_zakaznik,prechod_kontrola_chleba,H_MP,1);
		Hrany* hrana_pult_kontrola = new Hrany(misto_pult,prechod_kontrola_chleba,H_MP,1);
		Hrany* hrana_kontrola_pult = new Hrany(misto_pult,prechod_kontrola_chleba,H_PM,1);
		Hrany* hrana_kontrola_je_chleba = new Hrany(misto_je_chleba,prechod_kontrola_chleba,H_PM,1);
		
		Hrany* hrana_je_chleba_vem_prodavace = new Hrany(misto_je_chleba,prechod_vem_prodavace,H_MP,1);
		Hrany* hrana_prodavac_vem_prodavace = new Hrany(misto_prodavac,prechod_vem_prodavace,H_MP,1);
		Hrany* hrana_vem_prodavace_mam_prodavace = new Hrany(misto_mam_prodavace,prechod_vem_prodavace,H_PM,1);
		
		Hrany* hrana_pult_vem_chleba = new Hrany(misto_pult,prechod_vem_chleba,H_MP,1);
		Hrany* hrana_mam_prodavace_vem_chleba = new Hrany(misto_mam_prodavace,prechod_vem_chleba,H_MP,1);
		Hrany* hrana_vem_chleba_mam_chleba = new Hrany(misto_mam_chleba,prechod_vem_chleba,H_PM,1);
		
		Hrany* hrana_mam_chleba_obsluha = new Hrany(misto_mam_chleba,prechod_obsluha,H_MP,1);
		Hrany* hrana_obsluha_oslouzen = new Hrany(misto_obslouzen,prechod_obsluha,H_PM,1);
		Hrany* hrana_obsluha_prodavac = new Hrany(misto_prodavac,prechod_obsluha,H_PM,1);
		
		
		Hrany* hrana_obslouzen_odchod = new Hrany(misto_obslouzen,prechod_odchod,H_MP,1);
		Hrany* hrana_obslouzen_vratit = new Hrany(misto_obslouzen,prechod_vratit_se,H_MP,1);
		
		Hrany* hrana_odchod_odchod = new Hrany(misto_odchod,prechod_odchod,H_MP,1);
		Hrany* hrana_vratit_zakaznik = new Hrany(misto_zakaznik,prechod_vratit_se,H_PM,1);
		
		Hrany* hrana_odchod_odchazi = new Hrany(misto_odchod,prechod_odchazi_obslouzen,H_MP,1);
		
		prechod_priprava->pridej_in_hranu(hrana_pekar_priprava);
		prechod_priprava->pridej_out_hranu(hrana_priprava_dopeceno);
		prechod_na_pult->pridej_in_hranu(hrana_dopeceno_na_pult);
		prechod_na_pult->pridej_out_hranu(hrana_na_pult_pult);
		prechod_na_pult->pridej_out_hranu(hrana_na_pult_pekar);	
		prechod_prichod->pridej_out_hranu(hrana_prichod_zakaznik);	
		prechod_odchazi_hned->pridej_in_hranu(hrana_zakaznik_odchazi);
		prechod_kontrola_chleba->pridej_in_hranu(hrana_zakaznik_kontrola);
		prechod_kontrola_chleba->pridej_in_hranu(hrana_pult_kontrola);
		prechod_kontrola_chleba->pridej_out_hranu(hrana_kontrola_pult);
		prechod_kontrola_chleba->pridej_out_hranu(hrana_kontrola_je_chleba);
		prechod_vem_prodavace->pridej_in_hranu(hrana_je_chleba_vem_prodavace);
		prechod_vem_prodavace->pridej_in_hranu(hrana_prodavac_vem_prodavace);
		prechod_vem_prodavace->pridej_out_hranu(hrana_vem_prodavace_mam_prodavace);
		
		prechod_vem_chleba->pridej_in_hranu(hrana_pult_vem_chleba);
		prechod_vem_chleba->pridej_in_hranu(hrana_mam_prodavace_vem_chleba);
		prechod_vem_chleba->pridej_out_hranu(hrana_vem_chleba_mam_chleba);
		
		prechod_obsluha->pridej_in_hranu(hrana_mam_chleba_obsluha);
		prechod_obsluha->pridej_out_hranu(hrana_obsluha_oslouzen);
		prechod_obsluha->pridej_out_hranu(hrana_obsluha_prodavac);
		
		prechod_odchod->pridej_in_hranu(hrana_obslouzen_odchod);
		prechod_odchod->pridej_out_hranu(hrana_odchod_odchod);
		
		prechod_vratit_se->pridej_in_hranu(hrana_obslouzen_vratit);
		prechod_vratit_se->pridej_out_hranu(hrana_vratit_zakaznik);
		
		prechod_odchazi_obslouzen->pridej_in_hranu(hrana_odchod_odchazi);
		
		Znacka* znacka1 = new Znacka("pekar", misto_pekar);	//znacka1
		Znacka* znacka2 = new Znacka("prodavac", misto_prodavac);	//znacka1
		
		prioritni_prechody.push_back(prechod_vem_chleba);
		prioritni_prechody.push_back(prechod_kontrola_chleba);
		prioritni_prechody.push_back(prechod_odchazi_hned);
		
		pravdep_prechody.push_back(prechod_odchod);
		pravdep_prechody.push_back(prechod_vratit_se);
		////////////////KONEC DEFINOVANI PN PEKARNY/////////////////////////////////	
	}
	
	if(atoi(argv[1]) == 1)
	{
		model = 1;

		////////////////UCEBNA//////////////////////////////////////////////////////
		Mista* misto_student = new Mista("misto_student", 0, INT_MAX);
		Mista* misto_pocitace = new Mista("misto_pocitace", 10, 10);
		Mista* misto_mam_pc = new Mista("misto_mam_pc", 0, INT_MAX);
		Mista* misto_odchod = new Mista("misto_odchod1", 0, INT_MAX);
		Mista* misto_rozhodnuti1 = new Mista("misto_rozhodnuti1", 0, INT_MAX);
		Mista* misto_dostanu_pc = new Mista("misto_dostanu_pc", 0, INT_MAX);
		Mista* misto_rozhodnuti2 = new Mista("mista_rozhodnuti2", 0, INT_MAX);
		Mista* misto_navrat = new Mista("mista_navrat", 0, INT_MAX);

		vektor_mist.push_back(misto_student);vektor_mist.push_back(misto_pocitace);vektor_mist.push_back(misto_mam_pc);
		vektor_mist.push_back(misto_odchod);vektor_mist.push_back(misto_rozhodnuti1);vektor_mist.push_back(misto_dostanu_pc);
		vektor_mist.push_back(misto_rozhodnuti2);vektor_mist.push_back(misto_navrat);

		Prechod* prechod_vytvor_studenta = new Prechod("prechod_vytvor_studenta");
		Prechody_list.push_back(prechod_vytvor_studenta);
		prechod_vytvor_studenta->nastav_prechod(P_EXPON, 10, 0);

		Prechod* prechod_nedostanu_pc = new Prechod("prechod_nedostanu_pc");
		Prechody_list.push_back(prechod_nedostanu_pc);
		prechod_nedostanu_pc->nastav_prechod(P_PRIOR, 0, 0);

		Prechod* prechod_dostanu_pc = new Prechod("prechod_dostanu_pc");
		Prechody_list.push_back(prechod_dostanu_pc);
		prechod_dostanu_pc->nastav_prechod(P_PRIOR, 1, 0);

		Prechod* prechod_dostanu_pc_prioritne = new Prechod("prechod_dostanu_pc_prioritne");
		Prechody_list.push_back(prechod_dostanu_pc_prioritne);
		prechod_dostanu_pc_prioritne->nastav_prechod(P_PRIOR, 2, 0);

		Prechod* prechod_pracuju = new Prechod("prechod_pracuju");
		Prechody_list.push_back(prechod_pracuju);
		prechod_pracuju->nastav_prechod(P_EXPON, 50, 0);

		Prechod* prechod_konec = new Prechod("prechod_konec");
		Prechody_list.push_back(prechod_konec);
		prechod_konec->nastav_prechod(P_KONST, 0, 0);

		Prechod* prechod_chci_pc = new Prechod("prechod_chci_pc");
		Prechody_list.push_back(prechod_chci_pc);
		prechod_chci_pc->nastav_prechod(P_PRAVD, 60, 0);

		Prechod* prechod_nechci_pc = new Prechod("prechod_nechci_pc");
		Prechody_list.push_back(prechod_nechci_pc);
		prechod_nechci_pc->nastav_prechod(P_PRAVD, 40, 0);

		Prechod* prechod_odchod = new Prechod("prechod_odchod");
		Prechody_list.push_back(prechod_odchod);
		prechod_odchod->nastav_prechod(P_PRAVD, 80, 0);

		Prechod* prechod_navrat = new Prechod("prechod_navrat");
		Prechody_list.push_back(prechod_navrat);
		prechod_navrat->nastav_prechod(P_PRAVD, 20, 0);

		Prechod* prechod_cesta_zpet = new Prechod("prechod_cesta_zpet");
		Prechody_list.push_back(prechod_cesta_zpet);
		prechod_cesta_zpet->nastav_prechod(P_CASRR, 30, 60);

		Hrany* hrana_h1 = new Hrany(misto_student, prechod_vytvor_studenta, H_PM, 1);
		Hrany* hrana_h2 = new Hrany(misto_student, prechod_dostanu_pc, H_MP, 1);
		Hrany* hrana_h3 = new Hrany(misto_student, prechod_nedostanu_pc, H_MP, 1);
		Hrany* hrana_h4 = new Hrany(misto_student, prechod_cesta_zpet, H_PM, 1);

		Hrany* hrana_h5 = new Hrany(misto_pocitace, prechod_dostanu_pc, H_MP, 1);
		Hrany* hrana_h6 = new Hrany(misto_pocitace, prechod_dostanu_pc_prioritne, H_MP, 1);
		Hrany* hrana_h7 = new Hrany(misto_pocitace, prechod_pracuju, H_PM, 1);

		Hrany* hrana_h8 = new Hrany(misto_mam_pc, prechod_dostanu_pc, H_PM, 1);
		Hrany* hrana_h9 = new Hrany(misto_mam_pc, prechod_pracuju, H_MP, 1);
		Hrany* hrana_h10 = new Hrany(misto_mam_pc, prechod_dostanu_pc_prioritne, H_PM, 1);

		Hrany* hrana_h11 = new Hrany(misto_odchod, prechod_pracuju, H_PM, 1);
		Hrany* hrana_h12 = new Hrany(misto_odchod, prechod_konec, H_MP, 1);
		
		Hrany* hrana_h13 = new Hrany(misto_rozhodnuti1, prechod_nedostanu_pc, H_PM, 1);
		Hrany* hrana_h14 = new Hrany(misto_rozhodnuti1, prechod_chci_pc, H_MP, 1);
		Hrany* hrana_h15 = new Hrany(misto_rozhodnuti1, prechod_nechci_pc, H_MP, 1);

		Hrany* hrana_h16 = new Hrany(misto_dostanu_pc, prechod_chci_pc, H_PM, 1);
		Hrany* hrana_h17 = new Hrany(misto_dostanu_pc, prechod_dostanu_pc_prioritne, H_MP, 1);

		Hrany* hrana_h18 = new Hrany(misto_rozhodnuti2, prechod_nechci_pc, H_PM, 1);
		Hrany* hrana_h19 = new Hrany(misto_rozhodnuti2, prechod_odchod, H_MP, 1);
		Hrany* hrana_h20 = new Hrany(misto_rozhodnuti2, prechod_navrat, H_MP, 1);

		Hrany* hrana_h21 = new Hrany(misto_navrat, prechod_navrat, H_PM, 1);
		Hrany* hrana_h22 = new Hrany(misto_navrat, prechod_cesta_zpet, H_MP, 1);

		prechod_vytvor_studenta->pridej_out_hranu(hrana_h1);

		prechod_nedostanu_pc->pridej_in_hranu(hrana_h3);
		prechod_nedostanu_pc->pridej_out_hranu(hrana_h13);

		prechod_dostanu_pc->pridej_in_hranu(hrana_h2);
		prechod_dostanu_pc->pridej_in_hranu(hrana_h5);
		prechod_dostanu_pc->pridej_out_hranu(hrana_h8);

		prechod_dostanu_pc_prioritne->pridej_in_hranu(hrana_h17);
		prechod_dostanu_pc_prioritne->pridej_in_hranu(hrana_h6);
		prechod_dostanu_pc_prioritne->pridej_out_hranu(hrana_h10);

		prechod_pracuju->pridej_in_hranu(hrana_h9);
		prechod_pracuju->pridej_out_hranu(hrana_h7);
		prechod_pracuju->pridej_out_hranu(hrana_h11);

		prechod_konec->pridej_in_hranu(hrana_h12);

		prechod_chci_pc->pridej_in_hranu(hrana_h14);
		prechod_chci_pc->pridej_out_hranu(hrana_h16);

		prechod_nechci_pc->pridej_in_hranu(hrana_h15);
		prechod_nechci_pc->pridej_out_hranu(hrana_h18);

		prechod_odchod->pridej_in_hranu(hrana_h19);

		prechod_navrat->pridej_in_hranu(hrana_h20);
		prechod_navrat->pridej_out_hranu(hrana_h21);

		prechod_cesta_zpet->pridej_in_hranu(hrana_h22);
		prechod_cesta_zpet->pridej_out_hranu(hrana_h4);

		prioritni_prechody.push_back(prechod_dostanu_pc_prioritne);
		prioritni_prechody.push_back(prechod_dostanu_pc);
		prioritni_prechody.push_back(prechod_nedostanu_pc);
		
		pravdep_prechody.push_back(prechod_chci_pc);
		pravdep_prechody.push_back(prechod_nechci_pc);
		pravdep_prechody2.push_back(prechod_odchod);
		pravdep_prechody2.push_back(prechod_navrat);
		////////////////KONEC DEFINOVANI PN UCEBNA//////////////////////////////////
	}
	if(model == 0)
	{
		cout << "----------------ZPRACOVANI MODELU PEKARNY-------\n";
	}
	else if(model == 1)
	{
		cout << "----------------ZPRACOVANI MODELU UCEBNY--------\n";	
	}
	cout << "----------------START KALENDARE-----------------\n";
	double doba_simulace = atof(argv[2]);
	kalendarik->vytiskni_aktualni_cas(); // pocatecni stav 0
	///
	///zde provedeni okamzitych prechodu + naplanovani udalosti
	vytiskni_stav_mist(vektor_mist);
	projdi_prechody(Prechody_list, prioritni_prechody, pravdep_prechody, pravdep_prechody2);
	
	while(!kalendarik->prazdny_kalendar() && (kalendarik->dej_aktualni_cas() < doba_simulace)) //dokud mame udalosti v kalendari nebo nevyprsel simulacni cas
	{
		if(d_v)
		{
			cout << "----zpracovavani Udalosti z kalendare: \n";
		}
		//kalendarik->vytiskni_kalendar();
		Udalost * aktualni_udalost = kalendarik->dej_dalsi_udalost(); // ziskani udalosti ktera se ma provest
		Prechod * aktualni_prechod = aktualni_udalost->dej_prechod_ukazatel();
		kalendarik->nastav_aktualni_cas(aktualni_udalost->dej_cas_udalosti());
		kalendarik->vytiskni_aktualni_cas();
		////
		///zde se provede casovany aktualni_prechod
		if(d_v)
		{
			cout << "#zpracovavany prechod: " << aktualni_prechod->dej_nazev() << std::endl;
		}
		proved_presun(aktualni_prechod);
		////
		///
		//tady se provedou okamzite prechodu + se pridaji nove prechody do kalendare
		kalendarik->smaz_udalost(); // smaze zpracovanou udalost
		projdi_prechody(Prechody_list, prioritni_prechody, pravdep_prechody, pravdep_prechody2);
		if(d_v)
		{
			cout << "Udalost zpracovana\n";
		}	
	}
	cout << "==============================================================================\n";
	cout << "--------------Kalendar uz je prazdny nebo skoncila doba simulace--------------\n";
	cout << "--------------KONECNY STAV MIST-----------------------------------------------\n";
	//kalendarik->vytiskni_kalendar();
	vytiskni_stav_mist(vektor_mist);
	vytiskni_statistiky(Prechody_list);
	cout << "#DOBA SIMULACE: ";
	kalendarik->vytiskni_aktualni_cas();
	return 0;
} 
