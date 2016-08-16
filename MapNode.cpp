#include <iostream>
#include "MapNode.hpp"
#include "player.hpp"
#include <ctime>
#include <cstdlib>

using namespace std;

// POSTCONDITION: INIZIALIZZA IL NODO CON COORDINATE 0.0
// Settin: -1 Banca, 0 Prateria, 1 Deserto, 2 Foresta, 3 Bufali, 4 Oro, 5 Argento, 6 Rame.
// Building: -1 Niente, 0 Fattoria, 1 

MapNode* MapNode::Inst=NULL;
MapNode::MapNode()
{
    X = 0;
    Y = 0;
    setting = -1;
    materiali = -1;
    building = -1;
    level = -1;
    owner = -1;
    N = NULL;
    S = NULL;
    E = NULL;
    W = NULL;
    next = NULL;
	Prec=NULL;
    LineRight = NULL;
	if (Inst==NULL)
		Inst=this;
}

/*MapNode::~MapNode()
{
	cout << "Destructing MapNode\n";
	delete this;
}*/

void MapNode::SetNext(MapNode* nex)
{
	next=nex;
}

MapNode* MapNode::FindByCoords(int x, int y)
{
	MapNode* Target=NULL;
	MapNode* curr=this;
	while (curr!=NULL && !Target)
	{
		if (curr->X==x && curr->Y==y)
			Target=curr;
		curr=curr->next;
	}
	return Target;
}

// PRECONDITION: x e y sono le coordinate del nodo cercato
// POSTCONDITION: ritorna un puntatore al nodo se il nodo esiste, NULL altrimenti
MapNode* MapNode::FindNode(int x, int y)
{
    MapNode* t = this;
    bool found = false;
    while((t!=NULL)&&(!found))
    {
        if ((x == t->X)&&(y == t->Y)) found = true;
        else t = t->next;
    }
    return(t);
}

// POSTCONDITION: ritorna l'ordinata minima presente nelle cordinate della mappa;
Estremi MapNode::EstremiMappa()
{
    Estremi e;
    e.maxY = 0;
    e.minX = 0;
    e.minY = 0;
    MapNode* t = this->next;
    while(t!=NULL)
    {
        if (t->X < e.minX) e.minX = t->X;
        if (t->Y > e.maxY) e.maxY = t->Y;
        else if (t->Y < e.minY) e.minY = t->Y;
        t = t->next;
    }
    return(e);
}

// PRECONDITION: pos è la posizione che deve essere collegata ai nodi dello stesso livello
void MapNode::LinkLine(MapNode* pos)
{
    MapNode *t = this, *succ = NULL, *prec = NULL;
    while(t != NULL)
    {
        if(t->Y == pos->Y)          /* Consideriamo solo i nodi sullo stesso livello */
        {
            if (t->X < pos->X)
            {
                if (prec == NULL) prec = t;
                else if (t->X > prec->X) prec = t;
            }
            else if (t->X > pos->X)
            {
                if (succ == NULL) succ = t;
                else if (t->X < succ->X) succ = t;
            }
        }
        t = t->next;
    }
    if (prec != NULL) prec->LineRight = pos;
    pos->LineRight = succ;
}

// PRECONDITION: y è l'ordinata dell'elemento cercato
// POSTCONDITION: ritorna un puntatore all'elemento più a sinistra di quella linea
MapNode* MapNode::mostLeftLine(int y)
{
    MapNode *t = this, *mostleft = NULL;
    while(t != NULL)
    {
        if (y == t->Y)
        {
            if (mostleft == NULL) mostleft = t;
            else if (t->X < mostleft->X) mostleft = t;
        }
        t = t->next;
    }
    return(mostleft);
}

// PRECONDITION: pos != NULL
// POSTCONDITION: ritorna un pos->N
MapNode* MapNode::gate_nord(MapNode* pos)
{
    return(pos->N);
}

MapNode* MapNode::gate_sud(MapNode* pos)
{
    return(pos->S);
}

void MapNode::ClearList()
{
	if (N==NULL && S==NULL && E==NULL && W==NULL)
		return;
	PMapNode Head=this;
	PMapNode nex=Head->next;
	while (nex!=NULL)
	{
		Head->next=nex->next;
		delete nex;
		nex=Head->next;
	}
	Head->S=NULL;
	Head->N=NULL;
	Head->E=NULL;
	Head->W=NULL;
	LineRight=NULL;
}

MapNode* MapNode::gate_est(MapNode* pos)
{
    return(pos->E);
}

MapNode* MapNode::gate_ovest(MapNode* pos)
{
    return(pos->W);
}

// PRECONDITION: pos->N == NULL
// POSTCONDITION: crea un nuovo nodo a NORD di pos e aggiorna i puntatori della mappa e il puntatore next.
//                Ritorna un puntatore al nodo
MapNode* MapNode::create_nord(MapNode* pos)
{
    pos->N = new MapNode();
    MapNode* NewNode = pos->N;
    NewNode->X = pos->X;
    NewNode->Y = pos->Y + 1;
    NewNode->S = pos;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    LinkLine(NewNode);
    /* Aggiorniamo la pseudolista: l'inserimento in coda ha costo lineare, mentre l'inserimento al secondo posto ha costo
    costante. Inseriamo al secondo posto e non al primo perchè la funzione FindNode viene chiamata sul nodo (0,0).*/
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
	NewNode->Prec=this;
	if (next!=NULL)
		next->Prec=NewNode;
    next = NewNode;
    srand(time(0));
    int r = rand() % 2;
    if (r == 0)
    {
        r = rand() % 10;
        if (r < 5) r = 0;
        else if (r < 9) r = 2;
        else r = 3;
    }
    else if (r == 1)
    {
        r = rand() % 10;
        if (r < 5) r = 1;
        else if (r < 7) r = 5;
        else if (r < 9) r = 6;
        else r = 4;
    }
    if ((r == 2)||(r == 4)||(r == 5)||(r == 6))
        NewNode->materiali = ((rand() % 10) + 1) * 10 + 100;
    NewNode->setting = r;
    return(NewNode);
}

MapNode* MapNode::create_sud(MapNode* pos)
{
    pos->S = new MapNode();
    MapNode* NewNode = pos->S;
    NewNode->X = pos->X;
    NewNode->Y = pos->Y - 1;
    NewNode->N = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
	NewNode->Prec=this;
	if (next!=NULL)
		next->Prec=NewNode;
    next = NewNode;
        srand(time(0));
    int r = rand() % 2;
    if (r == 0)
    {
        r = rand() % 10;
        if (r < 5) r = 0;
        else if (r < 9) r = 2;
        else r = 3;
    }
    else if (r == 1)
    {
        r = rand() % 10;
        if (r < 5) r = 1;
        else if (r < 7) r = 5;
        else if (r < 9) r = 6;
        else r = 4;
    }
    if ((r == 2)||(r == 4)||(r == 5)||(r == 6))
        NewNode->materiali = ((rand() % 10) + 1) * 10 + 100;
    NewNode->setting = r;
    return(NewNode);
}

PMapNode MapNode::Instance()
{
	if (Inst==NULL)
	{
		PMapNode tmp=new MapNode;
		tmp->Init();
		return tmp;
	}else
	{
		return Inst;
	}
}

void MapNode::Init()
{
	X=0;
	Y=0;
	setting = -1;
    materiali = -1;
    building = -1;
    level = -1;
    owner = -1;
    N = NULL;
    S = NULL;
    E = NULL;
    W = NULL;
    next = NULL;
	Prec=NULL;
    LineRight = NULL;
	Inst=this;
	
}

MapNode* MapNode::create_est(MapNode* pos)
{
    pos->E = new MapNode();
    MapNode* NewNode = pos->E;
    NewNode->X = pos->X + 1;
    NewNode->Y = pos->Y;
    NewNode->W = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
	NewNode->Prec=this;
	if (next!=NULL)
		next->Prec=NewNode;
    next = NewNode;
        srand(time(0));
    int r = rand() % 2;
    if (r == 0)
    {
        r = rand() % 10;
        if (r < 5) r = 0;
        else if (r < 9) r = 2;
        else r = 3;
    }
    else if (r == 1)
    {
        r = rand() % 10;
        if (r < 5) r = 1;
        else if (r < 7) r = 5;
        else if (r < 9) r = 6;
        else r = 4;
    }
    if ((r == 2)||(r == 4)||(r == 5)||(r == 6))
        NewNode->materiali = ((rand() % 10) + 1) * 10 + 100;
    NewNode->setting = r;
    return(NewNode);
}

MapNode* MapNode::create_ovest(MapNode* pos)
{
    pos->W = new MapNode();
    MapNode* NewNode = pos->W;
    NewNode->X = pos->X - 1;
    NewNode->Y = pos->Y;
    NewNode->E = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
	NewNode->Prec=this;
	if (next!=NULL)
		next->Prec=NewNode;
    next = NewNode;
        srand(time(0));
    int r = rand() % 2;
    if (r == 0)
    {
        r = rand() % 10;
        if (r < 5) r = 0;
        else if (r < 9) r = 2;
        else r = 3;
    }
    else if (r == 1)
    {
        r = rand() % 10;
        if (r < 5) r = 1;
        else if (r < 7) r = 5;
        else if (r < 9) r = 6;
        else r = 4;
    }
    if ((r == 2)||(r == 4)||(r == 5)||(r == 6))
        NewNode->materiali = ((rand() % 10) + 1) * 10 + 100;
    NewNode->setting = r;
    return(NewNode);
}

void MapNode::Stampa(ptr_pl Pl, int currpl)
{
    Estremi l = EstremiMappa();
    cout << "\n\n\n";
    for(int current_y = l.maxY; current_y >= l.minY; current_y = current_y - 1)
    {
        MapNode *mostLeft = mostLeftLine(current_y);
        MapNode *curr = mostLeft->LineRight, *prec = mostLeft;
        // PRIMA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
        if (mostLeft->setting == -1) cout << "|     |";
        else if (mostLeft->setting == 0) cout << "|/////|";
        else if (mostLeft->setting == 1) cout << "|_ _ _|";
        else if (mostLeft->setting == 2) cout << "|^ ^ ^|";
        else if (mostLeft->setting == 3) cout << "|!  V |";
        else if (mostLeft->setting == 4)
		{
			int gamer;	
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
        else if (mostLeft->setting == 5) 		
		{
			int gamer;	
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
        else if (mostLeft->setting == 6) 		
		{
			int gamer;	
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
        while (curr != NULL)
        {
            cout << "  ";
            if (curr->W == NULL) for(int nodiFantasma = curr->X - prec->X -1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
            if (curr->setting == -1) cout << "|     |";
        else if (curr->setting == 0) cout << "|/////|";
        else if (curr->setting == 1) cout << "|_ _ _|";
        else if (curr->setting == 2) cout << "|^ ^ ^|";
        else if (curr->setting == 3) cout << "|!  V |";
        else if (curr->setting == 4) 
		{
			int gamer;	
			if ((gamer=find(Pl,curr->X, curr->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
        else if (curr->setting == 5)
		{
			int gamer;	
			if ((gamer=find(Pl,curr->X, curr->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
        else if (curr->setting == 6)
		{
			int gamer;	
			if ((gamer=find(Pl,curr->X, curr->Y, currpl))==0) 
				cout << "|-----|";
			else
				cout << "|-G-" << gamer << "-|";
		}
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // SECONDA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
        if (mostLeft->setting == -1)
		{
			int gamer;	
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y, currpl))==0) 
				cout << "| $$$ |";
			else
				cout << "| G$" << gamer << " |";
		}
        else if	(mostLeft->setting == 0) 
		{
			int gamer;
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y,currpl))==0) 
				cout << "|/////|";
			else
				cout << "|/G/" << gamer << "/|";
		}
        else if (mostLeft->setting == 1)
		{
			int gamer;
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y,currpl))==0) 
				cout << "| _ _ |";
			else
				cout << "| G_" << gamer << " |";
		}
        else if (mostLeft->setting == 2) 
		{
			int gamer;
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y,currpl))==0) 
				cout << "| ^ ^ |";
			else
				cout << "| G^" << gamer << " |";
		}		
        else if (mostLeft->setting == 3) 
		{
			int gamer;
			if ((gamer=find(Pl,mostLeft->X, mostLeft->Y,currpl))==0) 
				cout << "|  V  |";
			else
				cout << "| G " << gamer << " |";
		}	
        else if (mostLeft->setting == 4) cout << "|--o--|";
        else if (mostLeft->setting == 5) cout << "|--a--|";
        else if (mostLeft->setting == 6) cout << "|--r--|";
        while (curr != NULL)
        {
            if (curr->W == NULL) {
                cout << "  ";
                for(int nodiFantasma = curr->X - prec->X - 1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
            }
            else cout << "--";
			if (curr->setting == -1)
			{
				int gamer;
				if ((gamer=find(Pl,curr->X, curr->Y,currpl))==0) 
					cout << "| $$$ |";
				else
					cout << "| G$" << gamer << " |";
			}	
			else if (curr->setting == 0) 
			{
				int gamer;
				if ((gamer=find(Pl,curr->X, curr->Y,currpl))==0) 
					cout << "|/////|";
				else
					cout << "|/G/" << gamer << "/|";
			}	
			else if (curr->setting == 1) 
			{
				int gamer;
				if ((gamer=find(Pl,curr->X, curr->Y,currpl))==0) 
					cout << "| _ _ |";
				else
					cout << "| G_" << gamer << " |";
			}	
			else if (curr->setting == 2) 
			{
				int gamer;
				if ((gamer=find(Pl,curr->X, curr->Y,currpl))==0) 
					cout << "| ^ ^ |";
				else
					cout << "| G^" << gamer << " |";
			}	
			else if (curr->setting == 3)
			{
				int gamer;
				if ((gamer=find(Pl,curr->X, curr->Y,currpl))==0) 
					cout << "|  V  |";
				else
					cout << "| G " << gamer << " |";
			}	
        else if (curr->setting == 4) cout << "|--o--|";
        else if (curr->setting == 5) cout << "|--a--|";
        else if (curr->setting == 6) cout << "|--r--|";
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // TERZA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
        if (mostLeft->setting == -1) cout << "|     |";
        else if (mostLeft->setting == 0) cout << "|/////|";
        else if (mostLeft->setting == 1) cout << "|_ _ _|";
        else if (mostLeft->setting == 2) cout << "|^ ^ ^|";
        else if (mostLeft->setting == 3) cout << "| V  !|";
        else if (mostLeft->setting == 4) cout << "|-----|";
        else if (mostLeft->setting == 5) cout << "|-----|";
        else if (mostLeft->setting == 6) cout << "|-----|";
        while (curr != NULL)
        {
            cout << "  ";
            if (curr->W == NULL) for(int nodiFantasma = curr->X - prec->X -1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
            if (curr->setting == -1) cout << "|     |";
        else if (curr->setting == 0) cout << "|/////|";
        else if (curr->setting == 1) cout << "|_ _ _|";
        else if (curr->setting == 2) cout << "|^ ^ ^|";
        else if (curr->setting == 3) cout << "| V  !|";
        else if (curr->setting == 4) cout << "|-----|";
        else if (curr->setting == 5) cout << "|-----|";
        else if (curr->setting == 6) cout << "|-----|";
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // QUARTA PARTE: EVENTUALI COLLEGAMENTI
        for(int i = 2; i > 0; i = i - 1)
        {
            for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
            if (mostLeft->S != NULL) cout << "   |     ";
            else cout << "         ";
            while (curr != NULL)
            {
                if (curr->W == NULL) for(int nodiFantasma = curr->X - prec->X - 1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
                if (curr->S != NULL) cout << "   |     ";
                else cout << "         ";
                curr = curr->LineRight;
                prec = prec->LineRight;
            }
            cout << "\n";
            prec = mostLeft;
            curr = mostLeft->LineRight;
        }
    }
}


int MapNode::returnSetting(MapNode* pos)
{
    return(setting);
}

int MapNode::returnBuilding(MapNode* pos)
{
    return(building);
}

void MapNode::setLevel(int lev)
{
    level = lev;
}

int MapNode::getLevel()
{
    return(level);
}

void MapNode::setBuilding(int code)
{
    building = code;
}

int MapNode::tellOwner()
{
    return(owner);
}

void MapNode::setOwner(int n)
{
    owner = n;
}
