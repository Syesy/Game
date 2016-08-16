#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "command.hpp"
#include "player.hpp"
#include "MapNode.hpp"

class MapNode;

using namespace std;

int CharToInt(char a[])
{
	bool neg=false;
	int i;
	if (a[0]=='-')
	{
		neg=true;
		i=1;
	}else
		i=0;
	int fine=0;
	while (a[i]!='\0')
		i++;
	int cifre;
	if (neg)
		cifre=i-1;
	else
		cifre=i;
	cifre--;
	if (neg)
		i=1;
	else
		i=0;
	while(cifre>=0)
	{
		fine=fine+((a[i]-'0')*pow(10,cifre));
		cifre--;
		i++;
	}
	if (neg)
		fine=-fine;
	return fine;
}

bool Strcmp(char s1[], const char s2[])
{
	int i=0;
	bool same=true;
	while ((s1[i]!='\0' || s2[i]!='\0') && same==1)
	{
		if (s1[i]!=s2[i])
			same=false;
		else
			i++;
	}
	return same;
}

bool Strcmp(char s1[], char s2[])
{
	int i=0;
	bool same=true;
	while ((s1[i]!='\0' || s2[i]!='\0') && same==1)
	{
		if (s1[i]!=s2[i])
			same=false;
		else
			i++;
	}
	return same;
}

struct CommandInfo CmdInfo[]={
	{"money", give_money, 1},
	{"generate",generate_node,  1 },
	{"help", help, 0},
	{"clear",clear_nodes,0},
	{"goto", go_to, 2},
	{"NONE", NULL, 0},
};

void go_to(char arg[], player* ch, PMapNode Node, ptr_pl head)
{
	char arg1[50];
	char arg2[50];
	int i=0;
	while (arg[i]!='.')
	{
		arg1[i]=arg[i];
		i++;
	}
	arg1[i]='\0';
	i++;
	int j=0;
	while (arg[i]!='\0')
	{
		arg2[j]=arg[i];
		i++;
		j++;
	}
	arg2[j]='\0';
	int x=CharToInt(arg1);
	int y=CharToInt(arg2);
	PMapNode found=Node->FindByCoords(x,y);
	if (found)
	{
		ch->SetStart(found);
		ch->SetX(x);
		ch->SetY(y);
	}else{
		cout << "E' posibile teleportarsi solo in nodi esistenti. Usa /generate.numeronodi per generare nodi.\n";
	}
}

void clear_nodes(char arg[],player* ch,PMapNode Node, ptr_pl head)
{
	ptr_pl tmp=head;
	while (tmp!=NULL)
	{
		(tmp->ch)->SetStart(Node);
		(tmp->ch)->SetX(0);
		(tmp->ch)->SetY(0);
		tmp=tmp->next;
	}
	Node->ClearList();
}

bool InterpretCommand(player* ch,char com[],PMapNode Node, ptr_pl head)
{
	cout << "command: " << com << endl;
	char Arguments[50];
	char Command1[100];
	int i=0;
	while (com[i]!='.' && com[i]!='\r' && com[i]!='\n' && i<50)
	{
		Command1[i]=com[i];
		i++;
	}

	Command1[i]='\0';
	int j=0;
	while (Strcmp("NONE",CmdInfo[j].command)!=1)
	{

		if (Strcmp(Command1,CmdInfo[j].command)==1)
		{
			if (CmdInfo[j].arguments==0)
				((CmdInfo[j].command_pointer)(Arguments, ch, Node,head));
			else if (CmdInfo[j].arguments==1)
			{
				OneArguments(com, Arguments);
				((*CmdInfo[j].command_pointer)(Arguments,ch, Node,head));
			}
			else if (CmdInfo[j].arguments==2)
			{
				TwoArguments(com, Arguments);
				((*CmdInfo[j].command_pointer)(Arguments,ch, Node,head));
			}
			return true;
		}
		j++;
	}
	return false;
}

void OneArguments(char command[], char arg[])
{
	int i=0;
	while (command[i]!='.')
		i++;
	i++;
	int j=0;
	while (command[i]!='\0')
	{
		arg[j]=command[i];
		i++;
		j++;
	}
	arg[j]='\0';
}

void TwoArguments(char command[], char arg[])
{
	cout << "Porca\n";
	int i=0;
	while (command[i]!='.')
		i++;
	i++;
	int j=0;
	while (command[i]!='.')
	{
		arg[j]=command[i];
		i++;
		j++;
	}
	arg[j]='.';
	j++;
	i++;
	while (command[i]==' ')
		i++;
	while (command[i]!='\0')
	{
		arg[j]=command[i];
		i++;
		j++;
	}
	arg[j]='\0';
	cout << arg << endl;
}

void give_money(char arg[], player* ch,PMapNode Node, ptr_pl head)
{
	int money=CharToInt(arg);
	ch->ChangeCapital(money);
	
}

void help(char arg[], player* ch,PMapNode Node, ptr_pl head)
{
	int i=0;
	cout << endl;
	cout << "COMMAND LIST:\n";
	while (Strcmp("NONE",CmdInfo[i].command)!=1)
	{
		if (Strcmp("help",CmdInfo[i].command)!=1)
			cout << CmdInfo[i].command << endl;
		i++;
	}
	cout << endl;
}

void generate_node(char arg[],player* ch,PMapNode Node, ptr_pl head)
{
	PMapNode Curr=Node;
	PMapNode First=Node;
	int Nodes=CharToInt(arg);
	if (Nodes>15)
		Nodes=15;
	while(Nodes>0)
	{
		bool done=false;
		if ((First->gate_nord(Curr))==NULL && Nodes>0)
		{
			First->create_nord(Curr);
			Nodes--;
		}
		if ((First->gate_est(Curr))==NULL && Nodes>0)
		{
			First->create_est(Curr);
			Nodes--;
		}
		if ((First->gate_sud(Curr))==NULL && Nodes>0)
		{
			First->create_sud(Curr);
			Nodes--;
		}
		if ((First->gate_ovest(Curr))==NULL && Nodes>0)
		{
			First->create_ovest(Curr);
			Nodes--;
		}
		if (Curr->GiveNext()==NULL)
		{
			Curr=First;
			while (!done)
			{
				if (First->gate_nord(Curr)==NULL)
				{
					First->create_nord(Curr);
					Nodes--;
					done=true;
				}
				else if (First->gate_est(Curr)==NULL)
				{
					First->create_est(Curr);
					Nodes--;
					done=true;
				}
				else if (First->gate_sud(Curr)==NULL)
				{
					First->create_sud(Curr);
					Nodes--;
					done=true;
				}
				else if (First->gate_ovest(Curr)==NULL)
				{
					First->create_ovest(Curr);
					Nodes--;
					done=true;
				}
				else
					Curr=Curr->GiveNext();
			}
			cout << done << endl;
		}
		Curr=Curr->GiveNext();
	}
}