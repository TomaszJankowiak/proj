#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define WYM1 10000

using namespace std;

int *tab1; // tablica liczb do posortowania
int *pom; // tablica pomocnicza potrzebna przy scalaniu


void tablica(){
    tab1 = new int[WYM1]; // przydzielenie pamięci na tablicę liczb
    for(int i=0; i<WYM1; i++){
        tab1[i]=rand(); // na potrzeby testow wizualnych warto dopisac %50, aby liczby byly male
    }
}

void babelkowe(int tab[]){ // nieuzywane, ale zostawilem na pamiatke
    for (int i=0; i<(WYM1-1); i++)
        for (int j=0; j<(WYM1-1); j++)
            if (tab[j]>tab[j+1])
                swap(tab[j], tab[j+1]);
}


void scal(int tab[], int lewy, int srodek, int prawy){ // lewy - początek tablicy, prawy — koniec
    // scalenie posortowanych tablic tab[lewy...srodek] oraz tab[srodek+1...prawy]
    int lewy1 = lewy, prawy1 = srodek; // podtablica 1
    int lewy2 = srodek+1, prawy2 = prawy; // podtablica 2
    int i = lewy1;                                  //wykonuj scalenia za pomocą tablicy pomocniczej
    while((lewy1 <= prawy1) && (lewy2 <= prawy2)){ // aż do wyczerpania tablic
        if(tab[lewy1] < tab[lewy2]){
            pom[i] = tab[lewy1];
            lewy1++;
        }
    else{
        pom[i] = tab[lewy2];
        lewy2++;
    }
        i++;
    }
    while(lewy1 <= prawy1){
        pom[i] = tab[lewy1];
        lewy1++;
        i++;
    }
    while(lewy2 <= prawy2){
        pom[i] = tab[lewy2];
        lewy2++;
        i++;
    }
    for(i = lewy; i <= prawy; i++) // skopiuj z tymczasowej tablicy pomocniczej do oryginalnej
    tab[i] = pom[i];
}

void przez_scalanie(int tab[], int lewy, int prawy) {
    int srodek;
    if (lewy<prawy) { // gdy jest wiecej niz jeden element
        srodek=(lewy+prawy)/2; // środek tablicy
        przez_scalanie(tab, lewy, srodek); // sortowanie podtablic lewej i prawej
        przez_scalanie(tab, srodek+1, prawy);
        scal(tab, lewy, srodek, prawy); // scalenie dwoch posortowanych tablic
    }
}


void szybkie(int tab[], int lewy, int prawy){
    if (lewy < prawy){
        int m=lewy; // piwot
        for (int i=lewy+1; i<=prawy; i++)
            if (tab[i]<tab[lewy])
                swap(tab[++m],tab[i]);
        swap(tab[lewy],tab[m]);
        szybkie(tab,lewy,m-1);
        szybkie(tab,m+1,prawy);
    }
}

void odwrotne(int tab[], int lewy, int prawy){ // szybkie w druga strone
    if (lewy < prawy){
        int m=prawy;
        for (int i=prawy-1; i>=lewy; i--)
            if (tab[i]<tab[prawy])
                swap(tab[--m],tab[i]);
        swap(tab[prawy],tab[m]);
        odwrotne(tab,lewy,m-1);
        odwrotne(tab,m+1,prawy);
    }
}


void maxHeap(int tab[], int n, int parent) {
		long max, lewy, prawy;
		max = parent;
		lewy = 2 * parent + 1;
		prawy = 2 * parent + 2;
		if (lewy<n && tab[lewy]>tab[max]) max = lewy;
		if (prawy<n && tab[prawy]>tab[max]) max = prawy;
		if (max != parent) {
			swap(tab[parent], tab[max]);
			maxHeap(tab, n, max);
		}
	}

	void buildHeap(int tab[], int n) {
		for (int i = n / 2 - 1; i >= 0; i--)
			maxHeap(tab, n, i);
	}

	void kopcowe(int tab[], int n) {
		buildHeap(tab, n);
		for (int i = n - 1; i >= 0; i--) {
			swap(tab[i], tab[0]);
			maxHeap(tab, i, 0);
		}
	}

	int partition(int tab[], int low, int high){
		int pivot = tab[high];
		int i = (low - 1);

		for (int j = low; j <= high - 1; j++){
			if (tab[j] <= pivot){
				i++;
				swap(tab[i], tab[j]);
			}
		}
		swap(tab[i + 1], tab[high]);
		return (i + 1);
	}

	void introspektywne(int tab[], int n, int m) {
		int divide;
		if (m <= 0) {
			kopcowe(tab, n);
			return;
		}
		divide = partition(tab, 0, n);
		if (divide > 9) {
			introspektywne(tab, divide, m - 1);
		}
		if (n - 1 - divide > 9) {
			introspektywne(tab + divide + 1, n - 1 - divide, m - 1);
		}
	}


int main()
{
    srand(time(NULL)); // konfiguracja generatora liczb losowych, ta linijka zapewnia pseudolosowosc liczb calkowitych w programie
    clock_t start, koniec; // do zczytania czasu wykonywania sortowania
    float roznica=0, czas=0, t=0; // j.w.
    int m; // maksymalna dozwolona glebokosc wywolan rekurencyjnych sortowania szybkiego w sortowaniu introspektywnym
    m=log2(WYM1);

    for (int a=0; a<100; a++){ // a-liczba tablic
        tablica();

     /*   cout<<endl<<"Po losowaniu: "<<endl; // wypisanie elementow tablicy przed czesciowym posortowaniem
        for(int i=0;i<WYM1;i++)              // przydatne do testow wizualnych na mniejszych tablicach
            cout<<tab1[i]<<" ";
        cout<<endl;*/

    //!    Przygotowanie tablic do sortowania:
    //    szybkie(tab1, 0, WYM1*0.997-1);
    //    odwrotne(tab1, 0, WYM1-1);

   /*     cout<<"Przed posortowaniem: "<<endl; // wypisanie elementow tablicy po czesciowym posortowaniu
        for(int i=0;i<WYM1;i++)               // na potrzeby testow wizualnych dla mniejszych tablic
            cout<<tab1[i]<<" ";
        cout<<endl;*/

        start = clock();

   //!     Wywolanie odpowiedniego algorytmu sortowania:
//        babelkowe(tab1); // lepiej nie uzywac
  //      pom = new int[WYM1]; // przydzielenie pamięci na tablicę pomocniczą; musi tak byc przed
   //     przez_scalanie(tab1, 0, WYM1-1); // wywolaniem algorytmu sortowania przez scalanie
        szybkie(tab1, 0, WYM1-1);
  //      introspektywne(tab1, WYM1, m);

        koniec = clock();
        roznica = koniec - start; // roznica - czas sortowania 1 tablicy
        t=t+roznica; // t - czas sortowania 100 tablic

   /*     cout<<"Po posortowaniu: "<<endl; // wypisanie elementow tablicy po posortowaniu
        for(int i=0;i<WYM1;i++)           // na potrzeby testow wizualnych dla mniejszych tablic
            cout<<tab1[i]<<" ";
        cout<<endl;*/

        delete[] tab1;
 //       delete[] pom; // odkomentowac przy sortowaniu przez scalanie
    }
                                       // jednostka zmiennej t to ilosc taktow zegara, ale mozna
    czas = ((float)t)/CLOCKS_PER_SEC; // przeliczyc ja na sekundy za pomoca takiej funkcji
    cout << "Czas sortowania wyniosl: " << czas << " sekund. ";


    return 0;
}
