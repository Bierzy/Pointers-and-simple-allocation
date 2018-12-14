/*
 ********************************************
 * 		 Made by KB.		    *
 * 	   Na uzytek wlasny dla MB	    *
 ********************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int
main()
{
	int **ptr;	// wskaznik do wskaznika do inta
	int *ptr2;	// wskaznik do inta
	int x = 10;

/*	pamiec


adres:	0	1	2	3	4	5	6	7	8	9	10	...
			ptr			ptr2			x
wartosc:		5			8			10

typ:			wskaznik		wskaznik		int
			do			do inta
			wskaznika
		czyli wskazuje na wskaznik do inta
*/

	ptr2 = &x;	// good		przypisz adres do zmiennej int x
	ptr = &ptr2;	// good		przypisz adres do wskaznika do inta ptr2

	printf("%d\n", x);	// good
	printf("%d\n", *ptr2);	// good	odwolanie do wartosci z adresu przechowywanego w ptr2 czyli do komorki 8 a w tej komorce jest wartosc
				// 10. Tak naprawde to moze byc kilka komorek ale dla uproszczenia 1 przyjmujemy tu w tym przykladzie ze
				// jedna zmienna na jedna komorke.

//printf("linijka 31:  %%d\\n\t  %d\n", *ptr); // not good 	//do odkomentowania i zobaczenia co robi i ze zwraca blad
								// odwolujemy sie do wartosci tego adresu ktory przechowuje ptr
								// ona ma zapisany adres 5 czyli chcemy wyswietlic to co jest pod adresem 
								// 5 a tam jest adres zeby dobrze wyswietlic adres trzeba tak

	printf("%p\n", *ptr);	//good		// to wyswietli to co na co wskazuje ptr czyli zawartosc komorki o adresie "5" a w niej
						// znajduje sie adres do komorki "8".
	printf("%p\n", &x);	//good		// wyswietli sie adres do x czyli komorki "8" w przykladzie. Taki sam jak wyzej



			/* Wrocmy do czegos ciekawego */
/*	pamiec


adres:	0	1	2	3	4	5	...	1115	1116	1117	1118	...	1125	1126	...
			ptr			ptr2		ptr[0]	ptr[1]	ptr[2]	ptr[4]		ptr[9]
wartosc:		1115			8		  0	  0	  0	  0		  0

typ:			wskaznik		wskaznik	wsk	wsk	wsk	wsk		wsk
			do			do inta		int	int	int	int		int
			wskaznika
		czyli wskazuje na wskaznik do inta
*/

	ptr = calloc(10, sizeof(int*));	// Alokacja pamieci dla 10 elementowej tablicy w ktorej elementami sa? Jak myslisz? Wskazniki do inta.
					// Teraz ptr wskazuje na poczatek tablicy wskaznikow do intow.
					// Czemu wskaznik do inta by tutaj nie dzialal jako wskaznik do poczatku tablicy?
					// Bo wskaznik do inta nie moze wskazywac na wskaznik do inta.
					// Akurat tu by chyba troche dzialal zwykly wskaznik do long long inta ale jest to bledem.
					// [przynajmniej u mnie bo sizeof(long long int) == sizeof(int*)]
	/* sprawdzmy to */

	long long int *ptr3 = NULL;

	ptr3 = calloc(10 , sizeof(int*));	// Tak nie robic! Dobrze skonfigurowany kompilator bedzie wywalal Warningi przy probach
						// manipulowania elementami.
						// [Jednak u mnie dziala bo alokuje tyle samo bajtow dla 10 long long intow
						// co dla 10 wskaznikow do intow]

	printf("Rozmiar long long inta i wskaznika do inta: %ld %ld\n", sizeof(long long int), sizeof(int*));// moze zadziala

	for (int ii = 0; ii < 10; ii++) {
		*(ptr3 + ii) = &x;	// Warning ze robi inta z adresu do inta tylko ze pod adresem (ptr + ii) jest miejsce na
					// wskaznik do inta. nie mam pomyslu jak mozna to naprawic

		printf("\t%p\n",  *(ptr3 + ii));// A z tym? Mamy wskaznik do long long inta.
						// long long int ma rozmiar [u mnie] 8 bajtow czyli tyle co wskaznik do inta.
						// Uzywajac wskaznika do long long inta obsluguje tablice wskaznikow do inta XD.
						// No i przesuwajac po tej tablicy nie ma spiny bo idac jeden element do przodu
						// przeskakuje o sizeof(long long int) bajtow czyli u mnie 8,
						// a o tyle samo przeskakiwaloby gdyby ptr3 byl wskaznikiem do wskaznika do inta.

		printf("\t%lld\n", *(ptr3 + ii));// Kompilator sie pluje zeby dac w formacie printfa lld wiec dalem dla porownania.
	}


	// Teraz moze byc problem
	free(ptr3);
	// XD A jednak nie ma problemu (czasami gdy funkcja free dostaje wskaznik do pierwszego elementu ale
	// o innym typie niz elementy tablicy dynamicznej to wywala jakis blad) np. podczas zwalniania pamieci dla
	// pewnych struktur danych.

	// powrot do tablicy (1)	/* 	ptr = calloc(10, sizeof(int*));		*/


	*(ptr[2] + 1);		// To jest rownowazne *(*(ptr + 2) + 1)
				// To cos juz ma sens poniewaz *(ptr + 2) jest adresem do inta
				// wiec adres + 1 to jakis adres kilka bajtow dalej
				// i odwolanie do adresu.
	free(ptr);

	int *ptr_5 = NULL;

	ptr_5 = calloc(10, sizeof(int));


//	*(ptr_5[2] + 1);	// Nie skompiluje \\ do odkomentowania i testu
				// Oznacza to dokladnie *(*(ptr_5 + 2) + 1)
				// Takie polecenie zacznie sie wykonywac od najbardziej wewnetrznego nawiasu
				// ptr_5 + 2 to adres komorki przesunietej o 2 w prawo od elementu zerowego tablicy
				// na ktora wskazuje ptr_5
				// inaczej adres elementu o indeksie 2 // dla niedoswiadczonych &ptr_5[2]
				// *(ptr_5 + 2) jest wartoscia pod tym adresem czyli zwyklym intem
				// zwykly int + 1 nie oznacza zadnego adresu a co za tym idzie kompilator wywali blad
				// ze powinien to byc wskaznik a nie int

	*ptr[2] + 1;		// To krzywdy nie zrobi. Pobralo wartosc z komorki o dwa dalej niz wskazuje ptr
				// w sumie nie wiadomo gdzie. Nie wykrzacza bo nic z ta wartoscia tam 2 komorki dalej nie robi

//	printf("Let's get ready to SIGSEV\n");	// do odkomentowania i analizy
//	*ptr[2] += 100;		// To juz cos robi bo bo chce zmienic wartosc w tej komorce a na to OS pozwolic nie moze.

	//*ptr_5[2] + 1;		// rowniez nie skompiluje polecam analize
					// rownowazne *(*(ptr_5 + 2)) + 1
					// ponownie *(ptr_5 + 2) to zwykly int wiec nie jest poprawnym adresem ktorego oczekuje zewnetrzna *


	free(ptr_5);

	/* Uwaga 1
		malloc nie sprawdza overflow
	   Uwaga 2
		calloc sprawdza overflow
	   Uwaga 3
		Prawidlowe podejscie do dynamicznej alokacji pamieci powinno uwzglednic przypadek bledu w alokacji jak np.
		brak odpowiedniej ilosci wolnej pamieci. Jezeli nie uwzglednimy mozliwosci bledu mozemy w rezultacie 
		odwolywac sie do adresu zerowego co wywola segmentation fault. W takiej sytuacji funkcje zwracaja wkaznik
		do adresu zerowego. Przyklad przeciwdzialania errorom z manuala:
			p = malloc(size);
			if (p == NULL)
				err(1, NULL);
		[potrzebne: #include <err.h>]
	   Uwaga 4
		Zwalnianie pamieci jest bardzo wazne. Do dobrych nawykow nalezy uzywanie funkcji free przed zakonczeniem programu.
		W wiekszych programach wykorzystujacych znaczne zasoby jest to konieczne. W ogolnosci brak free na koncu nic nie powinien
		sprawiac wielkich problemow, w szczegolnosci ze zakonczenie funkcji main rozpoczyna zwalnianie pamieci przez program.
		Polecam jednak uzywac.
	 */

	return (0);
}
