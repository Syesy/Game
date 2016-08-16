#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "player.hpp"
#include "MapNode.hpp"


struct CommandInfo{
	const char command[100];
	void (*command_pointer)(char arguments[], player* ch, PMapNode Node, ptr_pl head);
	int arguments;
};

bool Strcmp(char s1[], char s2[]);
void OneArguments(char command[], char arg[]);
void TwoArguments(char arg1[], char arg[]);
bool InterpretCommand(player* ch,char com[], PMapNode Node, ptr_pl head);
void give_money(char arg[], player* ch,PMapNode Node, ptr_pl head);
void generate_node(char arg[],player* ch,PMapNode Node, ptr_pl head);
void help(char arg[], player* ch,PMapNode Node, ptr_pl head);
void clear_nodes(char arg[],player* ch,PMapNode Node, ptr_pl head);
void go_to(char arg[], player* ch, PMapNode Node, ptr_pl head);


#endif