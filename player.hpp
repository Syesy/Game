#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "MapNode.hpp"

class MapNode;
typedef MapNode* PMapNode;
void strcpy(char st1[], char st2[]);
void strcpy(char st1[], char c);



class player{
protected:
    int x;
    int y;
    PMapNode curr_pos;
	bool debt;
	int DebtAmount;
	int Rate;

    int money;
    char name[20];
    char title[20];

    int pl_number;
public:
    player();
	
    player(PMapNode start_position, char nome[], int pl_nu, char titolo[]);
	
	~player();
	
	void SetName(char nome[]);
	
	void SetTitle(char title[]);
	
	void SetPn(int pn);
	
	void SetX(int X) { x=X; }
	
	void SetY(int Y) { y=Y; }
	
	void SetStart(PMapNode node);
	
    void move_sud(PMapNode First);

    void move_nord(PMapNode First);

    void move_est(PMapNode First);

    void move_ovest(PMapNode First);

	void ChangeCapital(int sum);

    void change_title(char titolo[]);

    void say_pn();

    void say_name();

    void say_money();

    void say_title();

    PMapNode current_pos();
	
	bool GetDebt() { return debt; }
	
	bool SetDebt(int Amo);
	
	bool ExpireDebt();
	
	bool CheckExpire();
	
	int tell_pn() { return pl_number; }
	
	void ScaleRate();
	
	int GetX() { return x; }
	
	int GetY() { return y; }

};

struct player_list{
	player* ch;
    player_list *next;
};


typedef player_list* ptr_pl;

void push_player(ptr_pl *head, player *cha);
void init_player(ptr_pl *head);
player* pop_player(ptr_pl *head);
int find(ptr_pl head, int x, int y, int curpn);

#endif