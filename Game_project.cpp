//============================================================================
// Name        : class.cpp
// Author      : patrick
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "player.hpp"
#include "MapNode.hpp"
#include "command.hpp"
#include "constant.hpp"
using namespace std;




void InsArray(char a[],int len)
{
	char in;
	int i=0;
	while (in!='.' && i<len)
	{
		cin >> in;
		a[i]=in;
		i++;
	}
	i--;
	a[i]='\0';
}

void start(ptr_pl *PlList, PMapNode first, ptr_pl *List, int& PlCount)
{
	char ins='0';
	int gamer=1;
	char Name[21];
	char Title[21];
	while (ins!='e')
	{
		cout << "Inserisci Nome giocatore " << gamer << " (MAX 20 caratteri)" << endl;
		InsArray(Name,21);
		cout << "Inserisci Titolo giocatore " << gamer << " (MAX 20 caratteri)" << endl;
		InsArray(Title,21);
		player* Pl= new player(first, Name, gamer, Title);
		push_player(PlList, Pl);
		push_player(List, Pl);
		gamer++;
		cout << "Digitare e per terminare l'inserimento dei giocatori, altrimenti qualsiasi altro carattere." << endl;
		cin >> ins;
	}
	PlCount=gamer-1;
}

void RemoveSpace(char buf[])
{
	int i=0;
	int j=0;
	char buf2[256];
	while (buf[i]!='\0')
	{
		if (buf[i]!=' ')
		{
			buf2[j]=buf[i];
			j++;
		}
		i++;
	}
	buf2[j]='\0';
	j=0;
	while (buf2[j]!='\0')
	{
		buf[j]=buf2[j];
		j++;
	}
	buf[j]='\0';
}

bool Strcmp(char s1[], char c)
{
	if (s1[0]==c && (s1[1]=='\0'||s1[1]=='\n'||s1[1]=='\r'))
		return true;
	return false;
}
void GetConfVariables(bool & testmode)
{
	FILE *conf=fopen("conf.txt", "r");
	char buf[256];
	while (fgets(buf, 256, conf))
	{
		RemoveSpace(buf);
		char VariableName[100];
		int i=0;
		while (buf[i]!=':')
		{
			VariableName[i]=buf[i];
			i++;
		}
		VariableName[i]='\0';
		if (Strcmp(VariableName, "TEST_MODE"))
		{
			i++;
			if (buf[i]=='1')
				testmode=true;
		}
	}
	fclose(conf);
}

void Destruct(PMapNode first, ptr_pl plhead, ptr_pl pllist)
{
	first->ClearList();
	delete first;
	ptr_pl Ptmp=plhead;
	ptr_pl Ptmpn=Ptmp->next;
	while (Ptmpn!=NULL)
	{
		Ptmp->next=Ptmpn->next;
		delete Ptmpn;
		Ptmpn=Ptmp->next;
	}
	delete plhead;
	Ptmp=pllist;
	Ptmpn=Ptmp->next;
	while (Ptmpn!=NULL)
	{
		Ptmp->next=Ptmpn->next;
		delete Ptmpn;
		Ptmpn=Ptmp->next;	
	}
	delete pllist;
}


int main(int argc, char* argv[])
{
	bool TestMode=false;
	GetConfVariables(TestMode);
	PMapNode PFirst= new MapNode();
	ptr_pl plhead;
	ptr_pl pllist;
	init_player(&pllist);
	init_player(&plhead);
	int PlCycle=1;
	int PlCount;
	cout << "\n";
	if (TestMode)
	{
		cout << "---TESTMODE ENABLED---\n";
		cout << "Durante il gioco digita / per inserire comandi\n";
		cout << "<SYNTAX> /CommandName.arg1.arg2\n";
		cout << "Digita /help per una lista di comandi\n";
		cout << "\n\n";
	}
	cout << "Benvenuti in CASH!" << endl;
	cout << "Durante il gioco Premete R per riavviarlo e Q per uscire\n";
	cout << "BUON DIVERTIMENTO!\n\n";
	start(&plhead, PFirst,&pllist,PlCount);
	char move;
	bool quit=false;
	bool restart=false;
	while (true && !quit && !restart)
	{
		cout << "Player count " << PlCount << endl;
		if (PlCycle>PlCount)
		{
			ptr_pl tmp=pllist;
			while (tmp!=NULL)
			{
				player* ch=tmp->ch;
				ch->ScaleRate();
				tmp=tmp->next;
			}
			PlCycle=1;
		}
		else
		{
			bool IsStart;
			int Nmove=1;
			player *CurrPl=pop_player(&plhead);
			while (Nmove<=MaxMove && !quit && !restart)
			{
				PMapNode CurrPos=CurrPl->current_pos();
				if (CurrPos->read_x()==0 && CurrPos->read_y()==0)
					IsStart=true;
				else
					IsStart=false;
				cout << "Turno giocatore ";
				CurrPl->say_pn();
				cout << endl;
				cout << "Nome: ";
				CurrPl->say_name();
				cout << endl;
				cout << "Titolo: ";
				CurrPl->say_title();
				cout <<endl;
				cout << "Monete: ";
				CurrPl->say_money();
				cout << endl;
				cout << "Scegli cosa Vuoi Fare: " << endl;
				cout << "Muovi Sud (S)\n";
				cout << "Muovi Nord (N)\n";
				cout << "Muovi Est (E)\n";
				cout << "Muovi Ovest (O)\n";
				if (IsStart)
					if (!CurrPl->GetDebt())
						cout << "Sottoscrivi Debito (P)\n";
				if (CurrPl->GetDebt() && CurrPl->CheckExpire())
					cout << "Salda Debito (D)\n";
		
				PFirst->Stampa(pllist, CurrPl->tell_pn());
				bool done=false;
				bool IsCommand=false;
				while (!done && !quit && !restart)
				{
					cin >> move;
					if (move=='r' || move=='R')
					{
						cout << "Sei sicuro di voler riavviare il gioco? [Y/N]\n";
						char conf;
						cin >> conf;
						if (conf=='Y' || conf=='y')
							restart=true;
						done=true;
					}
					else if (move=='q' || move=='Q')
					{
						cout << "Sei sicuro di voler Uscire dal gioco? [Y/N]\n";
						char conf;
						cin >> conf;
						if (conf=='Y' || conf=='y')
							quit=true;
						done=true;
							
					}
					else if (move=='s' || move=='S') 
					{
						CurrPl->move_sud(PFirst);
						done=true;
						Nmove++;
					}
					else if (move=='n' || move=='N')
					{
						CurrPl->move_nord(PFirst);
						done=true;
						Nmove++;
					}
					else if (move=='e' || move=='E')
					{
						CurrPl->move_est(PFirst);
						done=true;
						Nmove++;
					}
					else if (move=='o' || move=='O')
					{
						CurrPl->move_ovest(PFirst);
						done=true;
						Nmove++;
					}
					else if (IsStart && !CurrPl->GetDebt())
					{
						if (move=='p' || move=='P')
						{
							cout << "Inserisci Ammontare Debito\n";
							int Debt;
							cin >> Debt;
							CurrPl->SetDebt(Debt);
							done=true;
							Nmove++;
						}
					}
					else if (CurrPl->GetDebt() && CurrPl->CheckExpire())
					{
						if (move=='d' || move=='D')
						{
							CurrPl->ExpireDebt();
							done=true;
							Nmove++;
						}
					}
					if (TestMode)
					{
						if (move=='/')
						{
							char Command[100];
							bool exit=false;
							while (!exit && !IsCommand)
							{
								cout << "Inserisci Comando oppure digita 'E' per uscire\n";
								scanf("%s",Command);
								scanf("%*[^\n]%1*[\n]");
								if (Strcmp(Command, 'e')==1 || Strcmp(Command, 'E')==1)
									exit=true;
								if (!exit)
								{
									bool check=InterpretCommand(CurrPl, Command, PFirst, pllist);
									if (check)
										IsCommand=true;
									else
										cout << "Comando non valido\n";
								}
								
							}
							done=true;
							//fflush(stdin);
						}
					}
					if (!done && !IsCommand)
						cout << "Mossa non valida\n";
				}
			}
			push_player(&plhead, CurrPl);
			PlCycle++;
		}
	}
	Destruct(PFirst, plhead, pllist);
	if (restart)
		main(argc, argv);
	return 0;
}
