#include"dstar.h"
#include <iostream>
#include <math.h>
#include <float.h>
using namespace stdext;
#include <unordered_map>


double Dstar::setRHS(state u, double rhs) {

  makeNewCell(u);
  cellHash[u].rhs = rhs;
  return rhs;
}


void Dstar::setG(state u, double g) {

  makeNewCell(u);
  cellHash[u].g = g;
}


void Dstar::insert(state u) {

  ds_oh::iterator cur;
  float csum;

  u    = calculateKey(u);
  cur  = openHash.find(u);
  csum = keyHashCode(u);
  openHash[u] = csum;
  openList.push(u);
}



bool Dstar::replan() 
{
    //path保存最终的寻路结果
    path.clear();
    int res = computeShortestPath();
    if (res < 0) 
    {
	    fprintf(stderr, "NO PATH TO GOAL\n");
	    return false;
    }
    list<state> n;
    list<state>::iterator i;
    state cur = s_start;
    if ((getG(s_start)>=10000)) 
    {
        fprintf(stderr, "NO PATH TO GOAL\n");
        return false;
    }

    while(cur != s_goal)
    {
        path.push_back(cur);
	getSucc(cur, n);
	if (n.empty()) 
        {
             fprintf(stderr, "NO PATH TO GOAL\n");
             return false;
	}
        //INFINITY表示不可寻路
	double cmin = INFINITY;
	double tmin = 0.0;
	state smin;

        for (i=n.begin(); i!=n.end(); i++) 
        {
            double val  = cost(cur,*i);
            double val2 = trueDist(*i,s_goal) + trueDist(s_start,*i);
            val += getG(*i);
            if (close(val,cmin))//
            {
		if (tmin > val2)
                {
		    tmin = val2;
		    cmin = val;
	       	    smin = *i;
		}
            } 
            else if (val < cmin) 
            {
                tmin = val2;
                cmin = val;
                smin = *i;
            }
        }

	n.clear();
	cur = smin;
    }

    path.push_back(s_goal);
    return true;
}


int Dstar::computeShortestPath() 
{
    list<state> s;
    list<state>::iterator i;

    if (openList.empty())
        return 1;

    int k=0;
    bool openListEmpty = openList.empty();
    while ((!openListEmpty) && (openList.top() 
		< (s_start = calculateKey(s_start))) 
		|| (getRHS(s_start) != getG(s_start))) 
    {
        if (k++ > maxSteps) 
        {
            fprintf(stderr, "At maxsteps\n");
            return -1;
        }

	state u;
	bool test = (getRHS(s_start) != getG(s_start));

        while(1) 
        {
            if (openList.empty())
                return 1;

            u = openList.top();
            openList.pop();

            if (!isValid(u))
                continue;

            if (!(u < s_start) && (!test)) 
                return 2;

            break;
        }

        ds_oh::iterator cur = openHash.find(u);
        openHash.erase(cur);

        state k_old = u;
         if (k_old < calculateKey(u)) 
        {   
            insert(u);
        }
 
        else if (getG(u) > getRHS(u)) 
        {           
            setG(u,getRHS(u));
            getPred(u,s);
            for (i=s.begin();i != s.end(); i++) 
                updateVertex(*i);
        }
        else
        {
	    setG(u, INFINITY);
	    getPred(u, s);
            for (i=s.begin();i != s.end(); i++)
                updateVertex(*i);

            updateVertex(u);
        }
    }
    return 0;
}

bool Dstar::isValid(state u) {

  ds_oh::iterator cur = openHash.find(u);
  if (cur == openHash.end()) return false;
  if (!close(keyHashCode(u), cur->second)) return false;
  return true;

}

void Dstar::updateStart(int x, int y) {

  s_start.x = x;
  s_start.y = y;

  k_m += heuristic(s_last,s_start);

  s_start = calculateKey(s_start);
  s_last  = s_start;

}


void Dstar::updateGoal(int x, int y) {

  list< pair<ipoint2, double> > toAdd;
  pair<ipoint2, double> tp;

  ds_ch::iterator i;
  list< pair<ipoint2, double> >::iterator kk;

  for(i=cellHash.begin(); i!=cellHash.end(); i++) {
    if (!close(i->second.cost, C1)) {
      tp.first.x = i->first.x;
      tp.first.y = i->first.y;
      tp.second = i->second.cost;
      toAdd.push_back(tp);
    }
  }

  cellHash.clear();
  openHash.clear();

  while(!openList.empty())
    openList.pop();

  k_m = 0;

  s_goal.x  = x;
  s_goal.y  = y;

  cellInfo tmp;
  tmp.g = tmp.rhs =  0;
  tmp.cost = C1;

  cellHash[s_goal] = tmp;

  tmp.g = tmp.rhs = heuristic(s_start,s_goal);
  tmp.cost = C1;
  cellHash[s_start] = tmp;
  s_start = calculateKey(s_start);

  s_last = s_start;

  for (kk=toAdd.begin(); kk != toAdd.end(); kk++) {
    updateCell(kk->first.x, kk->first.y, kk->second);
  }


}