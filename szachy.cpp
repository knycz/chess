#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <windows.h>
#include <cstring>



using namespace std;
#define ROZMIAR_PLANSZY 8

class Bierka;
class Gracz;

class Miejsce{
    public:
        int x;
        int y;
};

class Ruch: public Miejsce{
    public:
        Bierka* bierka;
        bool bicie;
};

class Bierka{
        protected:
		int x; //wiersz planszy
		int y;//kolumna planszy
		string nazwa;

		public:
        Gracz* gracz;

		bool WyprobujSzach(vector<Miejsce>* szachowanie, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY], int x, int y);
		bool WyprobujRuch(vector<Ruch>* ruchy, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY], int x, int y);
		virtual vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) = 0;	//Mozliwe ruchy pionka
		virtual vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) = 0;
		virtual char Wypisz() = 0; //symbol bierki na planszy
        virtual bool CzyKrol() = 0;
        void Zbij();	//Usuwa pionek
        virtual void NrRuchuPlus() = 0;

        int dajx();
        int dajy();
        void ustawx(const int&);
        void ustawy(const int&);
        string PodajNazwe();
        Bierka(int, int, Gracz*);

};

class Gracz{
    protected:
		bool mat;
		bool szach;

	public:
	    int liczruchbzbic;
	    string typ;

	    vector<class Bierka*> Bierki;	//Bierki gracza pozostajace na planszy

		string nazwa_gracza;	//Nazwa gracza.
		vector<class Miejsce> MiejscaSzachowane(Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]); //przez przeciwnika
        vector<class Ruch> RuchyPodSzachem(Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz(char bierka);	//dodaje nawiasy gracza do bierki
		int WykonajRuch(class Ruch rusz, Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		bool WykonajRuchProbny(class Ruch rusz, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		int virtual Ruch(Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) = 0;	                //Rusza graczem
		bool Szach(Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		void Usun(Bierka*);		//Usuwa bierke z listy bierek.
		void UstawMat();
		void ZwolnijMat();
		void UstawSzach();
		void ZwolnijSzach();
		bool CzySzach();
		bool CzyMat();
		void WyswietlMozliweRuchy(Bierka* [ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);

		Gracz(string nazwa_gracza);
};

class Gra{
	private:
		int numer_ruchu = 1;
		Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY];
		Gracz* gracz1;
		Gracz* gracz2;

	public:
		Gra(Gracz*, Gracz*);
		int Ruch();
		void Wypisz(); //Wypisuje stan planszy
};

// Metody GRA
    Gra::Gra(Gracz* gracz1, Gracz* gracz2){
        this->gracz1 = gracz1;
        this->gracz2 = gracz2;

        for(int i=0; i<ROZMIAR_PLANSZY; i++)
        {
            for(int j=0; j<ROZMIAR_PLANSZY; j++)
            {
                plansza[i][j] = nullptr;
            }
        }

        // umieszczanie pionkow na planszy.

        for(int i=0; i < gracz1->Bierki.size(); i++)
        {
            plansza[gracz1->Bierki.at(i)->dajx()][gracz1->Bierki.at(i)->dajy()] = gracz1->Bierki.at(i);
        }

        for(int i=0; i < gracz2->Bierki.size(); i++)
        {
            plansza[gracz2->Bierki.at(i)->dajx()][gracz2->Bierki.at(i)->dajy()]=gracz2->Bierki.at(i);
        }
        //pionki umieszczone.

    }

	int Gra::Ruch(){
                cout << "Tura " << (numer_ruchu+1)/2 << endl;
				cout << "Ruch " << numer_ruchu << endl;

                Gracz* gracz;

                if(numer_ruchu%2 == 1) {
                    gracz = gracz1;
                }
                else{
                    gracz = gracz2;
                }
                //gracz->WyswietlMozliweRuchy(plansza);

				if(!(gracz1->liczruchbzbic>=50 && gracz2->liczruchbzbic>=50) && gracz->Ruch(plansza)) //drugi warunek na mozliwe ruchy
				{
				    gracz->Szach(plansza);
				    Wypisz();
                    cout << "\n Szach gracz1: " << gracz1->CzySzach() << "  Szach gracz2: " << gracz2->CzySzach() << endl << endl;
					numer_ruchu++;

					if(gracz1->CzyMat())
					{
					    cout << "MAT" << endl;
						//wygral gracz 2.
						cout << "Wygral gracz '" << gracz2->nazwa_gracza << endl;
						return 0;
					}
                    else if(gracz2->CzyMat()){
                        cout << "MAT" << endl;
						//wygral gracz 1.
						cout << "Wygral gracz '" << gracz1->nazwa_gracza << endl;
						return 0;
					}
					else
					{
						//Gramy dalej
						return 1;
					}
				}
				else
				{
					cout << "REMIS!!!" << endl;
					return 0;
				}
			}

    void Gra::Wypisz(){
            int i,j,k;
				cout << endl;

                cout << " ";
				for(k=0; k < ROZMIAR_PLANSZY; k++)
				{
					cout << " " << (char)('A'+k) ;
				}

				//kolejne wiersze planszy
				for (i=0; i<ROZMIAR_PLANSZY; i++)
				{
					cout << "\n" << 8-i;

					for(j=0; j<ROZMIAR_PLANSZY; j++)
					{
						cout<<' ';

						if(plansza[i][j] != nullptr)
						{
                           if(plansza[i][j]->gracz == gracz1)  cout << plansza[i][j]->Wypisz();
                            else cout << (char)tolower(plansza[i][j]->Wypisz());
						}
						else
						{
							cout << ".";
						}
					}
					cout << " " << 8-i;
				}
                cout << "\n";
                cout << " ";
				for(k=0; k < ROZMIAR_PLANSZY; k++)
				{
					cout << " " << (char)('A'+k);
				}


    }

//Metody GRACZ
	int Gracz::WykonajRuch(class Ruch rusz, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
                string bicie="";
                string zbijam_bierke;
                if(rusz.bierka->PodajNazwe() == "Pionek"){
                    rusz.bierka->NrRuchuPlus();
                }

                if(plansza[rusz.x][rusz.y] != nullptr && plansza[rusz.x][rusz.y]->gracz != this)
                {
                    //Cos tu jest. Zakladam ze bicie.
                    liczruchbzbic = 0;
                    zbijam_bierke = plansza[rusz.x][rusz.y]->PodajNazwe();
                    plansza[rusz.x][rusz.y]->Zbij(); //usuwa z listy pionkow u gracza
                    bicie=" (bicie)";
                    plansza[rusz.x][rusz.y] = nullptr; //usuwa z planszy
                } else {
                liczruchbzbic++;
                }

                cout << endl << "Gracz '" << nazwa_gracza <<"': " << rusz.bierka->PodajNazwe() <<"  z [" << (char)('A'+ rusz.bierka->dajy()) << 8-rusz.bierka->dajx() <<"] na [" << (char)('A'+rusz.y) << 8-rusz.x <<"]"<< bicie << " " << zbijam_bierke << endl;
                //update planszy
                plansza[rusz.x][rusz.y] = plansza[rusz.bierka->dajx()][rusz.bierka->dajy()]; //przestawiam na puste pole lub po zbiciu
                plansza[rusz.bierka->dajx()][rusz.bierka->dajy()] = nullptr; // oddaje dotychczasowe pole
                //update wsp dla bierki
                rusz.bierka->ustawx(rusz.x); //wstawiam na nowe pole
                rusz.bierka->ustawy(rusz.y);

            return 1;
    }

	void Gracz::Usun(Bierka* bierka){	//Usuwa bierke z listy.
        int i=0;

        while((i<Bierki.size()) && this->Bierki.at(i) != bierka){
            i++;
        }
        //znaleziono
        if(i<Bierki.size()){
        //usuwamy.
            this->Bierki.erase(this->Bierki.begin()+i);//,this->Bierki.begin()+i+1); Czy i+1 potrzebne
        }
    }

    bool Gracz::WykonajRuchProbny(class Ruch rusz, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
            string bicie="";

            /*if(plansza[rusz.x][rusz.y] != nullptr && plansza[rusz.x][rusz.y]->gracz != this)
            {
                plansza[rusz.x][rusz.y] = nullptr; //usuwa z planszy
                bicie = "bicie";
            }*/
            //update planszy
        plansza[rusz.x][rusz.y] = plansza[rusz.bierka->dajx()][rusz.bierka->dajy()]; //przestawiam na puste pole lub po zbiciu
        plansza[rusz.bierka->dajx()][rusz.bierka->dajy()] = nullptr; // oddaje dotychczasowe pole
        //cout << "SPR: Gracz '" << nazwa_gracza <<"': " << rusz.bierka->PodajNazwe() <<"  z [" << (char)('A'+ rusz.bierka->dajy()) << 8-rusz.bierka->dajx() <<"] na [" << (char)('A'+rusz.y) << 8-rusz.x <<"]"<< bicie << endl;
        //cout << endl << "SPR: Gracz '" << nazwa_gracza <<"': " << rusz.bierka->PodajNazwe() <<"  z [" << (char)('A'+ rusz.bierka->dajy()) << 8-rusz.bierka->dajx() <<"] na [" << (char)('A'+rusz.y) << 8-rusz.x <<"]"<< endl;
        rusz.bierka->ustawx(rusz.x); //wstawiam na nowe pole
        rusz.bierka->ustawy(rusz.y);

        return true;
    }

    vector<class Miejsce> Gracz::MiejscaSzachowane(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
        vector<class Miejsce> tmp;
        vector<class Miejsce> matowane_msc;

        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(plansza[i][j] != nullptr)
                    if(plansza[i][j]->gracz != this){
                        tmp = plansza[i][j]->MozliwySzach(plansza);
                        matowane_msc.insert(matowane_msc.end(),tmp.begin(),tmp.end());
                        tmp.clear();
                    }
            }
        }

        int i = 0 ;
        //usuwam zduplikowane miejsca szachowane z wektora
        while(i < matowane_msc.size()-1){
            int j = i + 1;
            while( j < matowane_msc.size())
                if(matowane_msc.at(i).x == matowane_msc.at(j).x && matowane_msc.at(i).y == matowane_msc.at(j).y){
                    matowane_msc.erase(matowane_msc.begin() + j);
                } else j++;
            i++;
        }
        return matowane_msc;
    }

    vector<class Ruch> Gracz::RuchyPodSzachem(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
        vector<class Ruch> wszystkie_ruchy;
        vector<class Ruch> mozliwe_ruchy;
        vector<class Ruch> tmp;

        for(int i = 0; i < this->Bierki.size(); i++){
            tmp = this->Bierki.at(i)->MozliweRuchy(plansza);
           	wszystkie_ruchy.insert(wszystkie_ruchy.end(),tmp.begin(),tmp.end());
           	tmp.clear();
        }

        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch_probny;
            //zapamietuje parametry bierki
            ruch_probny.bierka = wszystkie_ruchy.at(z).bierka;
            ruch_probny.x = wszystkie_ruchy.at(z).bierka->dajx();
            ruch_probny.y = wszystkie_ruchy.at(z).bierka->dajy();

            //zapamietuje parametry miejsca na ktore sie ruszam
            Miejsce msc;
            msc.x = (wszystkie_ruchy.at(z)).x;
            msc.y = (wszystkie_ruchy.at(z)).y;
            Bierka* oddam = plansza[msc.x][msc.y];

            //zmieniam plansze
            WykonajRuchProbny(wszystkie_ruchy.at(z), plansza);

            int m = 0;
            //szukam krola
            while(!this->Bierki.at(m)->CzyKrol()){
                m++;
            }
            Bierka* Krol = this->Bierki.at(m);

            //jezeli krol NIE jest matowany na zadnym z miejsc po wykonanym ruchu
            vector<class Miejsce> miejsca_szachowane = MiejscaSzachowane(plansza);

            bool ruch_dozwolony = true;

            for(int t =0; t < miejsca_szachowane.size(); t++){
                Miejsce msc_szach = miejsca_szachowane.at(t);
                if(Krol->dajx()== msc_szach.x && Krol->dajy()== msc_szach.y){
                    ruch_dozwolony = false;
                }
            }

            if(ruch_dozwolony) mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
            //Cofnij ruch probny
            WykonajRuchProbny(ruch_probny, plansza);
            plansza[msc.x][msc.y] = oddam;
        }
        return mozliwe_ruchy;
    }

    bool Gracz::Szach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<class Miejsce> szach_msc;
		vector<class Miejsce> tmp;

		for(int i=0; i< this->Bierki.size(); i++){
			tmp = this->Bierki.at(i)->MozliwySzach(plansza);
			szach_msc.insert(szach_msc.end(),tmp.begin(),tmp.end());
			tmp.clear();
		}

		if(szach_msc.size()>0){
			for(int i = 0; i < szach_msc.size(); i++){
                class Miejsce msc;
                msc.x = (szach_msc.at(i)).x;
                msc.y = (szach_msc.at(i)).y;

                if(plansza[msc.x][msc.y] != nullptr){
                //cout << "PRZED USTAWIENIEM SZACHA" << endl;
                    if(plansza[msc.x][msc.y]->CzyKrol() && this != plansza[msc.x][msc.y]->gracz){ //Krol nalezy do przeciwnika
                        plansza[msc.x][msc.y]->gracz->UstawSzach();
                        //cout << "USTAWIAM SZACH";
                        if(plansza[msc.x][msc.y]->gracz->RuchyPodSzachem(plansza).size()==0) {
                            plansza[msc.x][msc.y]->gracz->UstawMat();
                        }
                        return true;
                    }
                }
			}
		}

        return false;

    }

	char Gracz::Wypisz(char bierka){
        return bierka;
    }

    void Gracz::UstawMat(){
        this->mat = true;
    }

    void Gracz::ZwolnijMat(){
        this->mat = false;
    }

    void Gracz::UstawSzach(){
        this->szach = true;
    }

    void Gracz::ZwolnijSzach(){
        this->szach = false;
    }

    bool Gracz::CzyMat(){
        return mat;
    }

    bool Gracz::CzySzach(){
        return szach;
    }

    void Gracz::WyswietlMozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
        vector<class Ruch> tmp;
        vector<class Ruch> mozliwe_ruchy;

        for(int i = 0; i < this->Bierki.size(); i++){
            tmp = this->Bierki.at(i)->MozliweRuchy(plansza);
           	mozliwe_ruchy.insert(mozliwe_ruchy.end(),tmp.begin(),tmp.end());
           	tmp.clear();
        }

        for(int i=0; i < mozliwe_ruchy.size(); i++){
            class Ruch rusz = mozliwe_ruchy.at(i);
                string bicie="";
                string zbijam_bierke;

                if(plansza[rusz.x][rusz.y] != nullptr && plansza[rusz.x][rusz.y]->gracz != this)
                {
                    //Cos tu jest. Zakladam ze bicie.
                    zbijam_bierke = plansza[rusz.x][rusz.y]->PodajNazwe();
                    bicie=" (bicie): ";
                }
           cout << "Ruch probny'" <<  nazwa_gracza  <<"': " << rusz.bierka->PodajNazwe() <<"  z [" << (char)('A'+ rusz.bierka->dajy()) << 8-rusz.bierka->dajx() <<"] na [" << (char)('A'+rusz.y) << 8-rusz.x <<"]"<< bicie << " " << zbijam_bierke << endl;
        }
    }

    Gracz::Gracz(string nazwa_gracza){
        this->nazwa_gracza = nazwa_gracza;
        this->mat = false;
        this->szach = false;
        this->liczruchbzbic = 0;
    }

//Koniec metod GRACZ

//METODY BIERKA

    Bierka::Bierka(int i, int j, Gracz* gracz){
        this->x=i;
        this->y=j;
        this->gracz=gracz;
        gracz->Bierki.push_back(this);
    }

	bool Bierka::WyprobujRuch(vector<Ruch>* ruchy, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY], int i, int j){
		Ruch gdzie;
		//bierka nie wyjdzie poza planszę i nie ma bicia
 		if((i<ROZMIAR_PLANSZY) && (j<ROZMIAR_PLANSZY) && i >= 0 && j >= 0 ){
			if(plansza[i][j] == nullptr){
                gdzie.bierka = this;
                gdzie.x = i;
                gdzie.y = j;
                gdzie.bicie = 0;
                ruchy->push_back(gdzie);
                return true;
            }
            //jest bicie
            else if(plansza[i][j]->gracz != this->gracz && !plansza[i][j]->CzyKrol()) //nie mozna zbic krola
            {
                //Obcy gracz
                gdzie.bierka = this;
                gdzie.x=i;
                gdzie.y=j;
                gdzie.bicie=1;
                ruchy->push_back(gdzie);

                return false; //do MozliwychRuchow->nie moze przeskoczyc bierki
            } else //if(plansza[j][i]->gracz==this->gracz)
            {
                //ten sam gracz
                return false;
            }
        }
        else
        //bierka poza plansza
		{
			return false;
		}

	}

	bool Bierka::WyprobujSzach(vector<Miejsce>* szachowanie, Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY], int i, int j){
		Miejsce msc;
        msc.x = i;
        msc.y = j;

 		if( (i < ROZMIAR_PLANSZY) && (j < ROZMIAR_PLANSZY) && i >= 0 && j >= 0 ){ 	//szach nie wyjdzie poza planszę
            if(plansza[i][j] != nullptr && (plansza[i][j]->CzyKrol() && (this->gracz != plansza[i][j]->gracz))){
                //szachowanie przechodzi przez krola przeciwnika tzn. nie moze wykonac ruchu w tym kierunku (np. oddalenie)
                szachowanie->push_back(msc);
                return true;
            } else
                if(plansza[i][j] == nullptr){
                    szachowanie->push_back(msc);
                    return true;
                }
                else { //napotykamy bierke rozna od krola
                    szachowanie->push_back(msc);
                    return false; //dalej nie pojdzie w WyprobujSzach
                }
        } else {
			return false;
		}
	}

    void Bierka::Zbij(){
        //zbija pionek.
            this->gracz->Usun(this);
        }

    int Bierka::dajx(){
        return x;
    }

    int Bierka::dajy(){
        return y;
    }

    void Bierka::ustawx(const int& xx){
        this->x = xx;
    }

    void Bierka::ustawy(const int& yy){
        this->y = yy;
    }

    string Bierka::PodajNazwe(){
        return nazwa;
    }
//KONIEC METOD BIERKA

//Klasy pochodne GRACZA
    //LOSOWY
	class Losowy: public Gracz{
		public:
		int Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) override;
		Losowy(string nazwa_gracza):Gracz(nazwa_gracza)
		{
			;
		}
	};

	int Losowy::Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<class Ruch> tmp;
		vector<class Ruch> mozliwe_ruchy;
		if(szach){
           mozliwe_ruchy = RuchyPodSzachem(plansza);
           if(mozliwe_ruchy.size() > 0){
                ZwolnijSzach();
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }
		} else {
            for(int i=0;i<Bierki.size(); i++)
            {
                tmp=this->Bierki.at(i)->MozliweRuchy(plansza);
                mozliwe_ruchy.insert(mozliwe_ruchy.end(),tmp.begin(),tmp.end());
                tmp.clear();
            }

            if(mozliwe_ruchy.size()>0)
            {
                //jesli sa ruchy do wykonania.
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza); //[losowa]
            }
            else
            {
                return 0;
            }
		}
	}

	class Rozsadny: public Gracz{
		public:
        bool SprawdzSzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]); //sprawdza ruch do przodu czy szachujacy
		bool SprawdzMat(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]); //sprawdza ruch do przodu czy matujacy
        int Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) override;
		Rozsadny(string nazwa_gracza):Gracz(nazwa_gracza)
		{
			;
		}
	};

	//ROZSADNY
	bool Rozsadny::SprawdzSzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<class Miejsce> matowane_msc;
		vector<class Miejsce> tmp;

		for(int i=0; i<this->Bierki.size(); i++)
		{
			tmp = this->Bierki.at(i)->MozliwySzach(plansza);
			matowane_msc.insert(matowane_msc.end(),tmp.begin(),tmp.end());
			tmp.clear();
		}

		if(matowane_msc.size()>0)
		{
			for(int i = 0; i < matowane_msc.size(); i++){
                Miejsce msc;
                msc.x = (matowane_msc.at(i)).x;
                msc.y = (matowane_msc.at(i)).y;

                if(plansza[msc.x][msc.y] != nullptr){ //jesli bierka to istnieja metody
                    if(plansza[msc.x][msc.y]->CzyKrol() && this != plansza[msc.x][msc.y]->gracz){ //Krol wystepuje na miejscach szachowanych
                        //cout << "Ruch jest szachujacy" << endl;
                        return true;
                    }
                }
			}
		}
		else
		{
			return false;
		}
    return false;
    }

    bool Rozsadny::SprawdzMat(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<class Miejsce> matowane_msc;
		vector<class Miejsce> tmp;

		for(int i=0; i < this->Bierki.size(); i++)
		{
			tmp = this->Bierki.at(i)->MozliwySzach(plansza);
			matowane_msc.insert(matowane_msc.end(),tmp.begin(),tmp.end());
			tmp.clear();
		}
        //cout << "SprawdzMat" << endl; //DO USUN
		if(matowane_msc.size()>0)
		{
			for(int i = 0; i < matowane_msc.size(); i++){
                Miejsce msc;
                msc.x = (matowane_msc.at(i)).x;
                msc.y = (matowane_msc.at(i)).y;
                //cout << endl << "Miejsc szachowanych: " << matowane_msc.size() << " "<<  nazwa_gracza << ": " << msc.x << "," << msc.y <<" " << endl;
                if(plansza[msc.x][msc.y] != nullptr){ //jesli bierka to istnieja metody

                    if(plansza[msc.x][msc.y]->CzyKrol() && this != plansza[msc.x][msc.y]->gracz){ //Krol wystepuje na miejscach szachowanych //MozliweRuchy elimnuja pionki wlasciciela, nie trzeba sprawdzac koloru
                        if((plansza[msc.x][msc.y]->gracz->RuchyPodSzachem(plansza).size()==0) ){  //krol przeciwnika nie ma mozliwosci ruchu
                            //cout << "RUCH MATUJACY!!!" << endl; //DO USUN
                            return true;
                        }
                    }
                }
			}
		}
		else
		{
			return false;
		}

		return false;
    }

	int Rozsadny::Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<class Ruch> tmp;
		vector<class Ruch> wszystkie_ruchy;
        vector<class Ruch> mozliwe_ruchy;

        if(!szach){
            for(int i=0; i<this->Bierki.size(); i++)
            {
                tmp = this->Bierki.at(i)->MozliweRuchy(plansza);
                wszystkie_ruchy.insert(wszystkie_ruchy.end(), tmp.begin(), tmp.end());
                tmp.clear();
            }
        } else {
           wszystkie_ruchy = RuchyPodSzachem(plansza);
           if(wszystkie_ruchy.size() > 0) ZwolnijSzach();
        }

        //RUCHY MATUJACE
        for(int z = 0; z < wszystkie_ruchy.size(); z++){

            class Ruch ruch_probny;
            ruch_probny.bierka = wszystkie_ruchy.at(z).bierka;
            ruch_probny.x = wszystkie_ruchy.at(z).bierka->dajx();
            ruch_probny.y = wszystkie_ruchy.at(z).bierka->dajy();

            Miejsce msc;
            msc.x = (wszystkie_ruchy.at(z)).x;
            msc.y = (wszystkie_ruchy.at(z)).y;
            Bierka* oddam = plansza[msc.x][msc.y];

            WykonajRuchProbny(wszystkie_ruchy.at(z), plansza);

            if(SprawdzMat(plansza)){
                mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
            }
            WykonajRuchProbny(ruch_probny, plansza);
            plansza[msc.x][msc.y] = oddam;
        }

        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

		//RUCHY BIJACE
		//szukam Hetmana
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch = wszystkie_ruchy.at(z);
            if(ruch.bicie){
                if(plansza[ruch.x][ruch.y]->PodajNazwe() == "Hetman") {
                        cout << "BIJE HETMANA" << endl; // DO USUN
                        return WykonajRuch(ruch, plansza);}
            }
        }
        //szukam wież
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch = wszystkie_ruchy.at(z);
            if(ruch.bicie){
                if(plansza[ruch.x][ruch.y]->PodajNazwe() == "Wieza") {
                        mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
                }
            }
        }
        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

        //szukam goncow
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch = wszystkie_ruchy.at(z);
            if(ruch.bicie){
                if(plansza[ruch.x][ruch.y]->PodajNazwe() == "Goniec") {
                        mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
                }
            }
        }
        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

        //szukam skoczkow
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch = wszystkie_ruchy.at(z);
            if(ruch.bicie){
                if(plansza[ruch.x][ruch.y]->PodajNazwe() == "Skoczek") {
                        mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
                }
            }
        }
        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

        //szukam pionow
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch = wszystkie_ruchy.at(z);
            if(ruch.bicie){
                if(plansza[ruch.x][ruch.y]->PodajNazwe() == "Pionek") {
                        mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
                }
            }
        }
        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

        //RUCHY SZACHUJACE
        for(int z = 0; z < wszystkie_ruchy.size(); z++){
            class Ruch ruch_probny;
            ruch_probny.bierka = wszystkie_ruchy.at(z).bierka;
            ruch_probny.x = wszystkie_ruchy.at(z).bierka->dajx();
            ruch_probny.y = wszystkie_ruchy.at(z).bierka->dajy();


            Miejsce msc;
            msc.x = (wszystkie_ruchy.at(z)).x;
            msc.y = (wszystkie_ruchy.at(z)).y;
            Bierka* oddam = plansza[msc.x][msc.y];

            WykonajRuchProbny(wszystkie_ruchy.at(z), plansza);

            if(SprawdzSzach(plansza)){
                //cout << "SA RUCHY SZACHUJACE:" << wszystkie_ruchy.at(z).x << ";" << wszystkie_ruchy.at(z).y << endl;
                mozliwe_ruchy.push_back(wszystkie_ruchy.at(z));
            }
            WykonajRuchProbny(ruch_probny, plansza); //Cofam ruch
            plansza[msc.x][msc.y] = oddam;

        }

        if(mozliwe_ruchy.size()>0) {
            int losowa = rand()%(int)mozliwe_ruchy.size();
            //cout << "WYKONUJE RUCH SZACHUJACY";
			return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
		}

		//POZOSTALE RUCHY
		if(wszystkie_ruchy.size()>0)
		{
		    //cout << "RUCH LOSOWY" << endl;
			int losowa = rand()%(int)wszystkie_ruchy.size();
			return WykonajRuch(wszystkie_ruchy.at(losowa), plansza);
		}
		else
		{
			return 0;
		}
	}

	//KONSERWATYWNY
	class Konserwatywny: public Gracz{
		public:
        int strona_planszy; //0 - gora, 1 dol
		int Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]) override;
		vector<class Ruch> RuchMaxNaPrawo(vector<class Ruch> ruchy);
		vector<class Ruch> RuchMaxDoPrzodu(vector<class Ruch> ruchy);
		Bierka* BierkaPrzyKrawedzi(vector<class Bierka*> te_same_berki);
		Konserwatywny(string nazwa_gracza, int strona_planszy):Gracz(nazwa_gracza)
		{
			this->strona_planszy = strona_planszy;
		}
	};

	vector<class Ruch> Konserwatywny::RuchMaxNaPrawo(vector<class Ruch> ruchy){
        vector<class Ruch> na_prawo;
        int poz_max = -1;//ruch_1.y;
        for(int i = 0; i < ruchy.size(); i++){
            class Ruch ruch = ruchy.at(i);
            if(ruch.y > poz_max){
                poz_max = ruch.y;
                na_prawo.clear();
                na_prawo.push_back(ruch);
            } else if(ruch.y == poz_max)
                na_prawo.push_back(ruch);
        }
        return na_prawo;
	}

	vector<class Ruch> Konserwatywny::RuchMaxDoPrzodu(vector<class Ruch> ruchy){
	    vector<class Ruch> do_przodu;

	    if(!strona_planszy){ //jestesmy na gorze planszy

            int poz_max = -1;//ruch_1.x;//bierka->dajx();
            for(int i = 0; i < ruchy.size(); i++){
                class Ruch ruch = ruchy.at(i);
                if(ruch.x > poz_max){
                    poz_max = ruch.x;
                    do_przodu.clear();
                    do_przodu.push_back(ruch);
                } else if(ruch.x == poz_max)
                    do_przodu.push_back(ruch);
            }
        } else {

            int poz_max =  8;//ruch_1.x;//ruch_1.bierka->dajx();
            for(int i = 0; i < ruchy.size(); i++){
                class Ruch ruch = ruchy.at(i);
                if(ruch.x < poz_max){
                    poz_max = ruch.x;
                    do_przodu.clear();
                    do_przodu.push_back(ruch);
                } else if(ruch.x == poz_max)
                    do_przodu.push_back(ruch);
            }
        }

    return do_przodu;
	}

	class Bierka* Konserwatywny::BierkaPrzyKrawedzi(vector<class Bierka*> te_same_bierki){
       class Bierka* bierka_skrajna;
       bierka_skrajna = te_same_bierki.at(0);
        if(!strona_planszy){ //jestesmy na gorze
            for(int z = 1; z < te_same_bierki.size(); z++){
                if(te_same_bierki.at(z)->dajx() < bierka_skrajna->dajx()){
                    bierka_skrajna = te_same_bierki.at(z);
                }
            }
        } else { //jestesmy na dole
            for(int z = 1; z < te_same_bierki.size(); z++){
                if(te_same_bierki.at(z)->dajx() > bierka_skrajna->dajx()){
                    bierka_skrajna = te_same_bierki.at(z);
                }
            }
        }
    return bierka_skrajna;
	}

	int Konserwatywny::Ruch(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
        vector<class Ruch> wszystkie_ruchy;
        vector<class Ruch> mozliwe_ruchy;
        vector<class Ruch> tmp;
        vector<class Bierka*> naj_z_prawej;


        if(szach){

            mozliwe_ruchy = RuchyPodSzachem(plansza); //rozpatruje ruchy po ktorych zniknie szach
            mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
            mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);

            if(mozliwe_ruchy.size()>0) {
                ZwolnijSzach();
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }

        } else {

            for(int i=0; i<Bierki.size(); i++)
            {
                tmp = this->Bierki.at(i)->MozliweRuchy(plansza);
                wszystkie_ruchy.insert(wszystkie_ruchy.end(), tmp.begin(), tmp.end());
                tmp.clear();
            }
            int poz_max = -1;
            for(int i = 0; i< wszystkie_ruchy.size(); i++){ //wybor bierki najbardziej na prawo
                int poz_y = wszystkie_ruchy.at(i).bierka->dajy();
                if(poz_y > poz_max){
                    naj_z_prawej.clear();
                    poz_max = poz_y;
                    naj_z_prawej.push_back(wszystkie_ruchy.at(i).bierka);
                } else if(poz_y == poz_max && wszystkie_ruchy.at(i).bierka != wszystkie_ruchy.at(i-1).bierka) {
                     naj_z_prawej.push_back(wszystkie_ruchy.at(i).bierka);
                }
            }

            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Krol"){
                    mozliwe_ruchy = naj_z_prawej.at(i)->MozliweRuchy(plansza);
                    mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                    mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
                }
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }


            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Hetman"){
                    mozliwe_ruchy = naj_z_prawej.at(i)->MozliweRuchy(plansza);
                    mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                    mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
                }
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }


            vector<class Bierka*> Wieze;
            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Wieza"){
                    Wieze.push_back(naj_z_prawej.at(i));
                }
            }

            if(Wieze.size()>0){
                class Bierka* przy_krawedzi = BierkaPrzyKrawedzi(Wieze);
                mozliwe_ruchy = przy_krawedzi->MozliweRuchy(plansza);
                mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }


            vector<class Bierka*> Gonce;
            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Goniec"){
                    Gonce.push_back(naj_z_prawej.at(i));
                }
            }
            if(Gonce.size()>0){
                class Bierka* przy_krawedzi = BierkaPrzyKrawedzi(Gonce);
                mozliwe_ruchy = przy_krawedzi->MozliweRuchy(plansza);
                mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }


            vector<class Bierka*> Skoczki;
            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Skoczek"){
                    Skoczki.push_back(naj_z_prawej.at(i));
                }
            }
            if(Skoczki.size()>0){
                class Bierka* przy_krawedzi = BierkaPrzyKrawedzi(Skoczki);
                mozliwe_ruchy = przy_krawedzi->MozliweRuchy(plansza);
                mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }


            vector<class Bierka*> Pionki;
            for(int i = 0; i < naj_z_prawej.size(); i++){
                if(naj_z_prawej.at(i)->PodajNazwe() == "Pionek"){
                    Pionki.push_back(naj_z_prawej.at(i));
                }
            }
            if(Pionki.size()>0){
                class Bierka* przy_krawedzi = BierkaPrzyKrawedzi(Pionki);
                mozliwe_ruchy = przy_krawedzi->MozliweRuchy(plansza);
                mozliwe_ruchy = RuchMaxNaPrawo(mozliwe_ruchy);
                mozliwe_ruchy = RuchMaxDoPrzodu(mozliwe_ruchy);
            }
            if(mozliwe_ruchy.size()>0) {
                int losowa = rand()%(int)mozliwe_ruchy.size();
                return WykonajRuch(mozliwe_ruchy.at(losowa), plansza);
            }
        }

	}

//Klasy pochodne BIERKI
	class Wieza:public Bierka{
		public:
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
		bool CzyKrol();
		void NrRuchuPlus() override;
		Wieza (int i, int j, Gracz* gracz): Bierka(i,j,gracz)
		{
			this->nazwa="Wieza";
		}
	};

	vector<Ruch> Wieza::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;
		int i;

		//Skaczemy wzdłuż wiersza
		i=y+1;
		while(WyprobujRuch(&ruchy,plansza,x,i)==1)
		{
			i++;
		}

		i=y-1;
		while(WyprobujRuch(&ruchy,plansza,x,i)==1)
		{
			i=i-1;
		}
		//Skaczemy wzdłuż kolumny

		i=x+1;
		while(WyprobujRuch(&ruchy,plansza,i,y)==1)
		{
			i++;
		}

		i=x-1;
		while(WyprobujRuch(&ruchy,plansza,i,y)==1)
		{
			i=i-1;
		}

		return ruchy;
	}

	vector<Miejsce> Wieza::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;
		int i;

		//Skaczemy wzdłuż wiersza
		i=y+1;
		while(WyprobujSzach(&szachowanie,plansza,x,i)==1)
		{
			i++;
		}

		i=y-1;
		while(WyprobujSzach(&szachowanie,plansza,x,i)==1)
		{
			i=i-1;
		}
		//Skaczemy wzdłuż kolumny

		i=x+1;
		while(WyprobujSzach(&szachowanie,plansza,i,y)==1)
		{
			i++;
		}

		i=x-1;
		while(WyprobujSzach(&szachowanie,plansza,i,y)==1)
		{
			i=i-1;
		}

		return szachowanie;
	}

	char Wieza::Wypisz(){
		return 'W';
	}

	bool Wieza::CzyKrol(){
        return false;
    }

    void Wieza::NrRuchuPlus(){;}

	class Skoczek:public Bierka{
		public:
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
        bool CzyKrol();
        void NrRuchuPlus() override;
		Skoczek (int i, int j, Gracz* gracz): Bierka(i,j, gracz)
		{
			this->nazwa="Skoczek";
		}
	};

	vector<Ruch> Skoczek::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;

		WyprobujRuch(&ruchy,plansza,x+2,y-1);
		WyprobujRuch(&ruchy,plansza,x+2,y+1);
		WyprobujRuch(&ruchy,plansza,x-2,y-1);
		WyprobujRuch(&ruchy,plansza,x-2,y+1);
		WyprobujRuch(&ruchy,plansza,x+1,y+2);
		WyprobujRuch(&ruchy,plansza,x+1,y-2);
		WyprobujRuch(&ruchy,plansza,x-1,y+2);
		WyprobujRuch(&ruchy,plansza,x-1,y-2);

		return ruchy;
	}

	vector<Miejsce> Skoczek::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;

		WyprobujSzach(&szachowanie,plansza,x+2,y-1);
		WyprobujSzach(&szachowanie,plansza,x+2,y+1);
		WyprobujSzach(&szachowanie,plansza,x-2,y-1);
		WyprobujSzach(&szachowanie,plansza,x-2,y+1);
		WyprobujSzach(&szachowanie,plansza,x+1,y+2);
		WyprobujSzach(&szachowanie,plansza,x+1,y-2);
		WyprobujSzach(&szachowanie,plansza,x-1,y+2);
		WyprobujSzach(&szachowanie,plansza,x-1,y-2);

		return szachowanie;
	}

	char Skoczek::Wypisz(){
		return 'S';
	}

    bool Skoczek::CzyKrol(){
        return false;
    }

    void Skoczek::NrRuchuPlus(){;}

	class Krol:public Bierka{
		public:
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
        bool CzyKrol();
        void NrRuchuPlus() override;
		Krol(int i,int j, Gracz* gracz):Bierka(i,j, gracz)
		{
			this->nazwa="Krol";
		}
	};

	vector<Ruch> Krol::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;
		vector<Ruch> ruchy_krola;
		vector<Ruch> tmp;
		vector<Miejsce> tmp2;
		Ruch r_temp;
		Miejsce m_temp;
        //WATP
		WyprobujRuch(&ruchy,plansza,x-1,y-1);
		WyprobujRuch(&ruchy,plansza,x,y-1);
		WyprobujRuch(&ruchy,plansza,x+1,y-1);
		WyprobujRuch(&ruchy,plansza,x-1,y);
		WyprobujRuch(&ruchy,plansza,x+1,y);
		WyprobujRuch(&ruchy,plansza,x-1,y+1);
		WyprobujRuch(&ruchy,plansza,x,y+1);
		WyprobujRuch(&ruchy,plansza,x+1,y+1);

        tmp = ruchy;
        tmp2 = gracz->MiejscaSzachowane(plansza); //funkcja przeglada miejsca szachowane przez przeciwnika

        for(int w = 0; w < tmp.size(); w++){
            r_temp = tmp.at(w); //przegladam ruchy krola
            bool czy_prawidlowy = true;

            for(int j = 0; j < tmp2.size(); j++){
                m_temp = tmp2.at(j); //po polach matowanych
                if(r_temp.x == m_temp.x && r_temp.y == m_temp.y){ //jezeli ruch krola na polu matowanym to nie bedzie wstawiony na wektor mozliwych
                    czy_prawidlowy = false;
                }
            }
            if(czy_prawidlowy)
            {
                ruchy_krola.push_back(r_temp);
            }
        }
		return ruchy_krola;
	}

	vector<Miejsce> Krol::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;

		WyprobujSzach(&szachowanie,plansza,x-1,y-1);
		WyprobujSzach(&szachowanie,plansza,x,y-1);
		WyprobujSzach(&szachowanie,plansza,x+1,y-1);
		WyprobujSzach(&szachowanie,plansza,x-1,y);
		WyprobujSzach(&szachowanie,plansza,x+1,y);
		WyprobujSzach(&szachowanie,plansza,x-1,y+1);
		WyprobujSzach(&szachowanie,plansza,x,y+1);
		WyprobujSzach(&szachowanie,plansza,x+1,y+1);

		return szachowanie;
	}

	char Krol::Wypisz(){
		return 'K';
	}

    bool Krol::CzyKrol(){
        return true;
    }

    void Krol::NrRuchuPlus(){;}

	class Goniec:public Bierka{
		public:
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
		bool CzyKrol();
		void NrRuchuPlus() override;
		Goniec (int i, int j , Gracz* gracz): Bierka(i,j, gracz)
		{
			this->nazwa="Goniec";
		}
	};

	vector<Ruch> Goniec::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;

		int i;

		//W prawy dolny rog
		i=1;
		while(WyprobujRuch(&ruchy,plansza,x+i,y+i)==1)
		{
			i++;
		}
		i=1;
		// W lewy gorny
		while(WyprobujRuch(&ruchy,plansza,x-i,y-i)==1)
		{
			i++;
		}
		//W lewy dolny
		i=1;
		while(WyprobujRuch(&ruchy,plansza,x+i,y-i)==1)
		{
			i++;
		}
		i=1;
		// W prawy gorny
		while(WyprobujRuch(&ruchy,plansza,x-i,y+i)==1)
		{
			i++;
		}

		return ruchy;
	}

	vector<Miejsce> Goniec::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;

		int i;

		//W prawy dolny rog
		i=1;
		while(WyprobujSzach(&szachowanie,plansza,x+i,y+i)==1)
		{
			i++;
		}
		i=1;
		// W lewy gorny
		while(WyprobujSzach(&szachowanie,plansza,x-i,y-i)==1)
		{
			i++;
		}
		//W lewy dolny
		i=1;
		while(WyprobujSzach(&szachowanie,plansza,x+i,y-i)==1)
		{
			i++;
		}
		i=1;
		// W prawy gorny
		while(WyprobujSzach(&szachowanie,plansza,x-i,y+i)==1)
		{
			i++;
		}

		return szachowanie;
	}

	char Goniec::Wypisz(){
		return 'G';
	}

    bool Goniec::CzyKrol(){
        return false;
    }

    void Goniec::NrRuchuPlus(){;}

	class Hetman:public Bierka{
		public:
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
		bool CzyKrol();
		void NrRuchuPlus() override;
		Hetman (int i,int j, Gracz* gracz): Bierka(i,j, gracz)
		{
			this->nazwa="Hetman";
		}
	};

	vector<Ruch> Hetman::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;

		int i;

		//W prawy dolny rog
		i=1;
		while(WyprobujRuch(&ruchy,plansza,x+i,y+i)==1)
		{
			i++;
		}
		i=1;
		// W lewy gorny
		while(WyprobujRuch(&ruchy,plansza,x-i,y-i)==1)
		{
			i++;
		}
		//W lewy dolny
		i=1;
		while(WyprobujRuch(&ruchy,plansza,x+i,y-i)==1)
		{
			i++;
		}
		i=1;
		// W prawy gorny
		while(WyprobujRuch(&ruchy,plansza,x-i,y+i)==1)
		{
			i++;
		}

		//Skaczemy wzdłuż wierszy
		i=y+1;
		while(WyprobujRuch(&ruchy,plansza,x,i)==1)
		{
			i++;
		}
		i=y-1;
		while(WyprobujRuch(&ruchy,plansza,x,i)==1)
		{
			i=i-1;
		}
		//Skaczemy wzdłuż kolumny
		i=x+1;
		while(WyprobujRuch(&ruchy,plansza,i,y)==1)
		{
			i++;
		}
		i=x-1;
		while(WyprobujRuch(&ruchy,plansza,i,y)==1)
		{
			i=i-1;
		}

		return ruchy;
	}

	vector<Miejsce> Hetman::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;

		int i;

		//W prawy dolny rog
		i=1;
		while(WyprobujSzach(&szachowanie,plansza,x+i,y+i)==1)
		{
			i++;
		}
		i=1;
		// W lewy gorny
		while(WyprobujSzach(&szachowanie,plansza,x-i,y-i)==1)
		{
			i++;
		}
		//W lewy dolny
		i=1;
		while(WyprobujSzach(&szachowanie,plansza,x+i,y-i)==1)
		{
			i++;
		}
		i=1;
		// W prawy gorny
		while(WyprobujSzach(&szachowanie,plansza,x-i,y+i)==1)
		{
			i++;
		}

		//Skaczemy wzdłuż wierszy
		i=y+1;
		while(WyprobujSzach(&szachowanie,plansza,x,i)==1)
		{
			i++;
		}
		i=y-1;
		while(WyprobujSzach(&szachowanie,plansza,x,i)==1)
		{
			i=i-1;
		}
		//Skaczemy wzdłuż kolumny
		i=x+1;
		while(WyprobujSzach(&szachowanie,plansza,i,y)==1)
		{
			i++;
		}
		i=x-1;
		while(WyprobujSzach(&szachowanie,plansza,i,y)==1)
		{
			i=i-1;
		}

		return szachowanie;
	}

	char Hetman::Wypisz(){
		return 'H';
	}

    bool Hetman::CzyKrol(){
        return false;
    }

    void Hetman::NrRuchuPlus(){;}

	class Pionek:public Bierka{
		public:
        int nr_ruchu;
        int wiersz_pocz;
		vector<Ruch> MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
        vector<Miejsce> MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]);
		char Wypisz();
		bool CzyKrol();
		void NrRuchuPlus() override;
		Pionek (int i, int j, Gracz* gracz): Bierka(i,j, gracz)
		{
			this->nazwa="Pionek";
			this->wiersz_pocz = i; //WATP
			this->nr_ruchu = 0;
		}
        Pionek (int i, int j, Gracz* gracz, int wiersz_pocz): Bierka(i,j, gracz)
		{
			this->nazwa="Pionek";
			this->wiersz_pocz = wiersz_pocz; //WATP
			this->nr_ruchu = 0;
		}
	};

	vector<Ruch> Pionek::MozliweRuchy(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Ruch> ruchy;

        if(wiersz_pocz==1){
            if(plansza[x+1][y]==nullptr && x<7) //wykluczam bicie na wprost
                if(WyprobujRuch(&ruchy,plansza,x+1,y)==1 && nr_ruchu == 0)
                    {
                        if(plansza[x+2][y]==nullptr && x<6){ //wykluczam bicie na wprost o 2 pola
                            WyprobujRuch(&ruchy,plansza,x+2,y);
                        }
                    }
            if(x<7 && y<7){ //(x+1)<8 && (y+1) <8
                if(plansza[x+1][y+1] != nullptr)
                    WyprobujRuch(&ruchy, plansza, x+1, y+1); //
            }
            if(x<7 && y>0){
                if(plansza[x+1][y-1] != nullptr)
                    WyprobujRuch(&ruchy, plansza, x+1, y-1);
            }

        } else if(wiersz_pocz==6){ //WATP zaimplementowac bicia dla drugiego gracza
            if(plansza[x-1][y]==nullptr && x>0) //wykluczam bicie na wprost
                if(WyprobujRuch(&ruchy,plansza,x-1,y)==1 && nr_ruchu == 0)
                    {
                        if(plansza[x-2][y]==nullptr && x>1){
                            WyprobujRuch(&ruchy,plansza,x-2,y);
                        }
                    }
            if(x>0 && y<7){ //(x-1)>=0 && (y+1) <8
                if(plansza[x-1][y+1] != nullptr)
                    WyprobujRuch(&ruchy, plansza, x-1, y+1); //
            }
            if(x>0 && y>0){
                if(plansza[x-1][y-1] != nullptr)
                    WyprobujRuch(&ruchy, plansza, x-1, y-1);
            }
        }

    return ruchy;
	}

	vector<Miejsce> Pionek::MozliwySzach(Bierka* plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY]){
		vector<Miejsce> szachowanie;

        if(wiersz_pocz==1){

            WyprobujSzach(&szachowanie, plansza,x+1,y);
            WyprobujSzach(&szachowanie, plansza, x+1, y+1);
            WyprobujSzach(&szachowanie, plansza, x+1, y-1);

        } else {
            WyprobujSzach(&szachowanie, plansza,x-1, y);
            WyprobujSzach(&szachowanie, plansza, x-1, y+1);
            WyprobujSzach(&szachowanie, plansza, x-1, y-1);
        }

		return szachowanie;
	}

	char Pionek::Wypisz(){
		return 'P';
	}

    bool Pionek::CzyKrol(){
        return false;
    }

    void Pionek::NrRuchuPlus(){
       nr_ruchu++;
    }

/********************** --- MAIN --- ***************************/

int main(int argc, char **argv) {

	Gracz* gracz1;
	Gracz* gracz2;
	Gra* gierka;

	srand(time(nullptr));

	//Tworzymy graczy

	gracz1 = new Rozsadny("Czarny");
	gracz2 = new Losowy("Bialy");
	//gracz2 = new Konserwatywny("Bialy", 1); //0 gora planszy //1 dol

	//pionki gracza 1
	for(int i=0; i<8; i++)
        new Pionek(1,i,gracz1);
	new Wieza(0,0,gracz1);
	new Skoczek(0,1,gracz1);
	new Goniec(0,2,gracz1);
	new Hetman(0,3,gracz1);
	new Krol(0,4,gracz1);
    new Goniec(0,5,gracz1);
	new Skoczek(0,6,gracz1);
	new Wieza(0,7,gracz1);

	//pionki gracza 2
	for(int i=0; i<8; i++)
        new Pionek(6,i,gracz2);
	new Wieza(7,0,gracz2);
	new Skoczek(7,1,gracz2);
	new Goniec(7,2,gracz2);
	new Hetman(7,4,gracz2);
	new Krol(7,3,gracz2);
	new Goniec(7,5,gracz2);
	new Skoczek(7,6,gracz2);
	new Wieza(7,7,gracz2);

	gierka=new Gra(gracz1, gracz2);

    cout << "Szachownica " << endl;
    gierka->Wypisz();
    cout << " Wielkie " << gracz1->typ << " " << "male" << gracz2->typ << endl;
	while(gierka->Ruch())
    {
       //Sleep(1 * CLOCKS_PER_SEC);
	}

return 0;
}
