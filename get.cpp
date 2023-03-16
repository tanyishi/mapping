#include"dstar.h"
#include <iostream>
#include <cmath>
using namespace stdext;
#include <unordered_map>

bool Dstar::occupied(state u) {

  ds_ch::iterator cur = cellHash.find(u);

  if (cur == cellHash.end()) return false;
  return (cur->second.cost < 0);
}

void Dstar::getSucc(state u,list<state> &s) {

  s.clear();
  u.k.first  = -1;
  u.k.second = -1;

  if (occupied(u)) return;

  u.x += 1;
  s.push_front(u);
  u.y += 1;
  s.push_front(u);
  u.x -= 1;
  s.push_front(u);
  u.x -= 1;
  s.push_front(u);
  u.y -= 1;
  s.push_front(u);
  u.y -= 1;
  s.push_front(u);
  u.x += 1;
  s.push_front(u);
  u.x += 1;
  s.push_front(u);

}


double Dstar::getG(state u) {

  if (cellHash.find(u) == cellHash.end())
    return heuristic(u,s_goal);
  return cellHash[u].g;

}

bool Dstar::close(double x, double y) {

  if ((x>=10000) && (y>=10000)) return true;
  return (fabs(x-y) < 0.00001);

}


double Dstar::getRHS(state u) {

  if (u == s_goal) return 0;

  if (cellHash.find(u) == cellHash.end())
    return heuristic(u,s_goal);
  return cellHash[u].rhs;

}

double Dstar::cost(state a, state b) {

  int xd = fabs((double)(a.x-b.x));
  int yd = fabs((double)(a.y-b.y));
  double scale = 1;

  if (xd+yd>1) scale = sqrt((double)(2));

  if (cellHash.count(a) == 0) return scale*C1;
  return scale*cellHash[a].cost;

}

void Dstar::getPred(state u,list<state> &s) {

  s.clear();
  u.k.first  = -1;
  u.k.second = -1;

  u.x += 1;
  if (!occupied(u)) s.push_front(u);
  u.y += 1;
  if (!occupied(u)) s.push_front(u);
  u.x -= 1;
  if (!occupied(u)) s.push_front(u);
  u.x -= 1;
  if (!occupied(u)) s.push_front(u);
  u.y -= 1;
  if (!occupied(u)) s.push_front(u);
  u.y -= 1;
  if (!occupied(u)) s.push_front(u);
  u.x += 1;
  if (!occupied(u)) s.push_front(u);
  u.x += 1;
  if (!occupied(u)) s.push_front(u);

}

double Dstar::trueDist(state a, state b) {

  float x = a.x-b.x;
  float y = a.y-b.y;
  return sqrt(x*x + y*y);

}

float Dstar::keyHashCode(state u) {

  return (float)(u.k.first + 1193*u.k.second);

}


void Dstar::init(int sX, int sY, int gX, int gY) 
{
	cellHash.clear();
	path.clear();
	openHash.clear();
	while (!openList.empty())
		openList.pop();

	k_m = 0;
	s_start.x = sX;    //设定起点和终点坐标
	s_start.y = sY;
	s_goal.x = gX;
	s_goal.y = gY;
	cellInfo tmp;
	tmp.g = tmp.rhs = 0;
	tmp.cost = C1;
	cellHash[s_goal] = tmp;
	tmp.g = tmp.rhs = heuristic(s_start, s_goal);
	tmp.cost = C1;
	cellHash[s_start] = tmp;
	s_start = calculateKey(s_start);
	s_last = s_start;
}

state Dstar::calculateKey(state u)
{
	double val = min(getRHS(u), getG(u));
	u.k.first = val + heuristic(u, s_start) + k_m;
	u.k.second = val;
	return u;
}



void Dstar::updateVertex(state u)
{
    list<state> s;
    list<state>::iterator i;
    double rhs = 0.0;
	if (u != s_goal)
	{
		getSucc(u, s);
		double tmp = INFINITY;
		double tmp2;
		for (i = s.begin(); i != s.end(); i++)
		{
			tmp2 = getG(*i) + cost(u, *i);
			if (tmp2 < tmp) 
               tmp = tmp2;
		}
        rhs = getRHS(u);
	if (!close( rhs , tmp)) 
            setRHS(u, tmp);
	}

    //最优路径点的G和RHS不相等，放到openList中进行检查
 
    auto g = getG(u);
    rhs = getRHS(u);
	if (!close(g,rhs)) 
        insert(u);
}

double Dstar::heuristic(state a, state b) { //用于计算启发值
  return eightCondist(a,b)*C1;
}

double Dstar::eightCondist(state a, state b) {
  double temp;
  double min = fabs((double)(a.x - b.x));
  double max = fabs((double)(a.y - b.y));
  if (min > max) {
    double temp = min;
    min = max;
    max = temp;
  }
  return ((sqrt((double)2)-1.0)*min + max);
}


void Dstar::updateCell(int x, int y, double val) {

   state u;

  u.x = x;
  u.y = y;

  if ((u == s_start) || (u == s_goal)) return;

  makeNewCell(u);
  cellHash[u].cost = val; //val是设定的cost值

  updateVertex(u);
}

void Dstar::makeNewCell(state u) {   //用于计算特定点的Info信息，为之后求出最短路径

  if (cellHash.find(u) != cellHash.end()) return;

  cellInfo tmp;
  tmp.g       = tmp.rhs = heuristic(u,s_goal);
  tmp.cost    = C1;
  cellHash[u] = tmp;     

}

Dstar::Dstar() {

  maxSteps = 8000; 
  C1       = 1;      //我们默认移动一个距离的花费是1

}


list<state> Dstar::getPath() {
  return path;
}
