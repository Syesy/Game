#include <iostream>
#include <math.h>
#include "MapNode.hpp"
#include "player.hpp"

using namespace std;



player::player(){
	curr_pos=NULL;
	debt=false;
	DebtAmount=0;
    x=0;
    y=0;
    money=1000;
    title[0]='\0';
    name[0]='\0';
    pl_number=0;
	Rate=0;
}

player::player(PMapNode start_position, char nome[], int pl_nu, char titolo[]){
	curr_pos=start_position;
	debt=false;
	DebtAmount=0;
    x=0;
    y=0;
    money=1000;
    strcpy(titolo, title);
    strcpy(nome, name);
    pl_number=pl_nu;
	Rate=0;
}

bool player::SetDebt(int Amo)
{
	const int MaxDebt=10000;
	if (Amo<=MaxDebt)
	{
		DebtAmount=Amo;
		Rate=(Amo*2)/100;
		if (Rate==0)
			Rate=1;
		debt=true;
		return true;
	}
	return false;
}

bool player::CheckExpire()
{
	if (money>=DebtAmount)
		return true;
	return false;
}

bool player::ExpireDebt()
{
	if (money>=DebtAmount)
	{
		money-=DebtAmount;
		debt=false;
		Rate=0;
		return true;
	}
	return false;
}

void player::ScaleRate()
{
	
	double rata=double(Rate);
	double RateScale=(ceil((5*rata)/100));
	
	if (money>=Rate)
	{
		if (DebtAmount-Rate>0)
		{
			money-=Rate;
			DebtAmount-=Rate;
			Rate=Rate+RateScale;
		}
		else
		{
			money-=DebtAmount;
			Rate=0;
			DebtAmount=0;
			debt=false;
		}
			
	}else
	{
		if (DebtAmount-money>0)
		{
			money=0;
			DebtAmount-=money;
			Rate=Rate+RateScale;
		}
		else
		{
			DebtAmount=0;
			money-=DebtAmount;
			debt=false;
			Rate=0;
		}
	}
}

player::~player()
{

}
	
void strcpy(char st1[], char st2[])
{
	int i=0;
	while (st1[i]!='\0')
	{
		st2[i]=st1[i];
		i++;
	}
	st2[i]='\0';
}


void player::say_title(){
    if (title[0]!='\0')
        cout << title;
}

void player::say_money(){
    cout << money;
}

void player::say_name(){
    cout << name;
}
PMapNode player::current_pos(){
    return curr_pos;
}

void player::say_pn(){
    cout << pl_number;
}
void player::ChangeCapital(int sum)
{
	money=money+sum;
}

void player::change_title(char titolo[]){
    strcpy(titolo, title);
}

void player::move_sud(PMapNode First){
    if (First->gate_sud(curr_pos)!=NULL){
        curr_pos=First->gate_sud(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }else{
        curr_pos=First->create_sud(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }
}

void player::move_nord(PMapNode First){
    if (First->gate_nord(curr_pos)!=NULL){
        curr_pos=First->gate_nord(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }else{
        curr_pos=First->create_nord(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }
}

void player::move_est(PMapNode First){
    if (First->gate_est(curr_pos)!=NULL){
        curr_pos=First->gate_est(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }else{
        curr_pos=First->create_est(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();

    }
}

void player::move_ovest(PMapNode First){
    if (First->gate_ovest(curr_pos)!=NULL){
        curr_pos=First->gate_ovest(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }else{
        curr_pos=First->create_ovest(curr_pos);
        x=curr_pos->read_x();
        y=curr_pos->read_y();
    }
}

void player::SetName(char nome[])
{
	strcpy(nome,name);
}

void player::SetTitle(char titolo[])
{
	strcpy(titolo,title);
}

void player::SetPn(int pn)
{
	pl_number=pn;
}

void player::SetStart(PMapNode node)
{
	curr_pos=node;
}



void init_player(ptr_pl *head){
    (*head)=NULL;
}

void push_player(ptr_pl *head, player *cha){
    ptr_pl tmp;
    tmp=(*head);
    (*head)=new player_list;
    (*head)->ch=cha;
    (*head)->next=tmp;
}

// precondition: la lista head non può essere vuota

player* pop_player(ptr_pl *head){
    ptr_pl tmp=(*head);
	if (tmp==NULL)
	{
	
	}
    else if ((tmp->next)==NULL){
        (*head)=NULL;
        return tmp->ch;
    }else{
        ptr_pl tmp_next=tmp->next;
        while (tmp_next->next!=NULL){
            tmp=tmp->next;
            tmp_next=tmp_next->next;
        }
        player* temp;
        temp=tmp_next->ch;
        tmp->next=NULL;
        return temp;
    }
}

int find(ptr_pl head, int x, int y, int curpn)
{
	int pn=0;
	ptr_pl tmp=head;
	while (tmp!=NULL)
	{
		player* ch=tmp->ch;
		if (ch->GetX()==x && ch->GetY()==y)
		{
			if (ch->tell_pn()!=curpn)
				pn=ch->tell_pn();
			else
				return ch->tell_pn();
				
		}
		tmp=tmp->next;
	}
	return pn;
}