#ifndef AGENT_H
#define AGENT_H
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

struct TableEntry
{
  int nextHop;
  int hopCount;

  TableEntry(int nextHop, int hopCount)
  : nextHop(nextHop), hopCount(hopCount)
  {
  };
};

class World;
class Agent
{
  int d_type; // 0 - normal selfish/selfless, 1 - TFT, 2 - battery based
  int d_id;
  int d_x; //position on the x-axis
  int d_y; //position on the y-axis

  float d_return = 1;

  vector<TableEntry> d_routeTable;
  vector<int> d_memoryTable;
  bool d_tableUpdated = true;

  int d_battery;
  float d_selfish;
  bool d_wantsToRoute;

  int d_messagesSent = 0;
  int d_messagesTotal = 0;
  int d_messagesRouted = 0;
  int d_messagesRecieved = 0;

public:
  Agent(int id, int x, int y, int populationSize, float selfishness, int type);
  Agent(const Agent&);
  Agent(Agent&&);
  ~Agent(){};
  Agent& operator=(const Agent&) = default;

  void move(bool mode, World &world);
  void die() { d_x = -1; };

  int x() { return d_x; };
  int y() { return d_y; };
  int id() { return d_id; };
  int type() { return d_type; };
  bool isTableUpdated(){ return d_tableUpdated; };
  void setSelfish(float set) { d_selfish = set; };
  void setType(int set) { d_type = set; };

  TableEntry &routeTable(int idx){ return d_routeTable[idx]; };
  vector<TableEntry> &routeTable(){ return d_routeTable; };
  int messagesTotal(){ return d_messagesTotal; };
  int messagesSent(){ return d_messagesSent; };
  int messagesRouted(){ return d_messagesRouted; };
  int messagesRecieved(){ return d_messagesRecieved; };
  int battery(){ return d_battery; };

  float fitness(){ return (float)d_messagesSent / d_messagesTotal; };

  bool isNeighbour(Agent &agent, World &world);
  float isSelfish() { return d_selfish; };
  //returns a vector of all agents within range of this agent which are willing to route
  //the vector stores them in descending order of proximity (furthest first)
  bool operator() (Agent&, Agent&); //sorting on fitness
  float  distanceTo(Agent&);


  void updateRouteTable(vector<Agent> &population, World &world);
  void setTableTo(bool set){ d_tableUpdated = set; };
  void flushRouteTable();
  bool recieveTable(vector<TableEntry> &table, int senderID, vector<Agent> &population);
  bool advertiseTable(vector<Agent> &population, World &world);

  bool sendMessage(Agent&, vector<Agent> &population);
  void routeMessage(Agent&, vector<Agent> &population);

  bool wantsToRoute(Agent &agent); //false if not willing route the message
  void setWillingnessToRoute();
  bool wantsToSend(); //false if not willing to send the message


  bool isWithinRange(Agent&, World &world);
};

#endif
