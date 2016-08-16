#ifndef MAPNODE_HPP
#define MAPNODE_HPP
#include "player.hpp"

struct player_list;

typedef player_list* ptr_pl;

struct Estremi{
	int minY, maxY, minX;
};

class MapNode
{
    protected:
        int X, Y;
        int setting, materiali, building, level, owner;
        MapNode *N, *S, *E, *W;
        MapNode *next;
		MapNode *Prec;
        MapNode *LineRight;
        MapNode* FindNode(int x, int y);
        Estremi EstremiMappa();
        void LinkLine(MapNode* pos);
        MapNode* mostLeftLine(int y);
		static MapNode* Inst;
    public:
        MapNode();
		//~MapNode();
		void Init();
        MapNode* gate_nord(MapNode* pos);
        MapNode* gate_sud(MapNode* pos);
        MapNode* gate_est(MapNode* pos);
        MapNode* gate_ovest(MapNode* pos);
        MapNode* create_nord(MapNode* pos);
        MapNode* create_sud(MapNode* pos);
        MapNode* create_est(MapNode* pos);
        MapNode* create_ovest(MapNode* pos);
		MapNode* GiveNext(){ return next; }
		MapNode* FindByCoords(int x, int y);
		void SetNext(MapNode* nex);
		void ClearList();
		static MapNode* Instance();
		int read_x() { return(X); }
		int read_y() { return(Y); }
        void Stampa(ptr_pl Pl, int currpl);
        int returnSetting(MapNode* pos);
        int returnBuilding(MapNode* pos);
        void setLevel(int lev);
        int getLevel();
        void setBuilding(int code);
        int tellOwner();
        void setOwner(int n);
		int GetSetting(MapNode* pos) { return pos->setting; };
		
		
};

typedef MapNode* PMapNode;

#endif // MAPNODE_HPP
