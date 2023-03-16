#include"dstar.h"
#include <iostream>
#include <unordered_map>
#include "plane.h"
using namespace stdext;
using namespace std;
list<state> mypath;
Dstar *dstar = new Dstar();
char draw[100][100];

int choice(char a,int n)  //此函数用于接收用户返回的各种选项
{
	int i;
	while(1)
	{
	for(i=1;i<=n;i++)
		if((a-48)==i) return i;
	cout<<"您的输入不符合标准，请重新输入";
	cin>>a;
	}
}

class Obstruct
{
protected:
	int x;
	int y;
public:
	Obstruct(int m,int n):x(m),y(n)
	{}
	virtual void set(void){}
};

class Mount:public Obstruct
{
public:
	Mount(int m,int n):Obstruct(m,n)
	{}
	void set(void)
	{
		dstar->updateCell(x,y,-1);draw[x][y]='m';
		dstar->replan();               
        mypath = dstar->getPath();
	}

};

class Hmount:public Obstruct
{
public:
	Hmount(int m,int n):Obstruct(m,n)
	{}
	void set(void)
	{
		
		dstar->updateCell(x,y,-1);draw[x][y]='m';
		dstar->updateCell(x-1,y,-1);draw[x-1][y]='m';
		dstar->updateCell(x-1,y-1,-1);draw[x-1][y-1]='m';
		dstar->updateCell(x-1,y+1,-1);draw[x-1][y+1]='m';
		dstar->updateCell(x,y+1,-1);draw[x][y+1]='m';
		dstar->updateCell(x,y-1,-1);draw[x][y-1]='m';
		dstar->updateCell(x+1,y,-1);draw[x+1][y]='m';
		dstar->updateCell(x+1,y-1,-1);draw[x+1][y-1]='m';
		dstar->updateCell(x+1,y+1,-1);draw[x+1][y+1]='m';
		dstar->replan();               
        mypath = dstar->getPath();
	}
};

class Swind:public Obstruct
{
	public:
	Swind(int m,int n):Obstruct(m,n)
	{}
	void set(void)
	{
		int k=0;
		dstar->updateCell(x,y,k);draw[x][y]='s';
		dstar->updateCell(x-1,y,k);draw[x-1][y]='s';
		dstar->updateCell(x-1,y-1,k);draw[x-1][y-1]='s';
		dstar->updateCell(x-1,y+1,k);draw[x-1][y+1]='s';
		dstar->updateCell(x,y+1,k);draw[x][y+1]='s';
		dstar->updateCell(x,y-1,k);draw[x][y-1]='s';
		dstar->updateCell(x+1,y,k);draw[x+1][y]='s';
		dstar->updateCell(x+1,y-1,k);draw[x+1][y-1]='s';
		dstar->updateCell(x+1,y+1,k);draw[x+1][y+1]='s';
		dstar->replan();               
        mypath = dstar->getPath();
	}
};
class Nwind:public Obstruct
{
	public:
	Nwind(int m,int n):Obstruct(m,n)
	{}
	void set(void)
	{
		int k=4;
		dstar->updateCell(x,y,k);draw[x][y]='n';
		dstar->updateCell(x-1,y,k);draw[x-1][y]='n';
		dstar->updateCell(x-1,y-1,k);draw[x-1][y-1]='n';
		dstar->updateCell(x-1,y+1,k);draw[x-1][y+1]='n';
		dstar->updateCell(x,y+1,k);draw[x][y+1]='n';
		dstar->updateCell(x,y-1,k);draw[x][y-1]='n';
		dstar->updateCell(x+1,y,k);draw[x+1][y]='n';
		dstar->updateCell(x+1,y-1,k);draw[x+1][y-1]='n';
		dstar->updateCell(x+1,y+1,k);draw[x+1][y+1]='n';
		dstar->replan();               
        mypath = dstar->getPath();
	}
};

class Valley:public Obstruct
{
	int dim;
	int coorx,coory;
	public:
	Valley(int m,int n,int d,int xcoor,int ycoor):Obstruct(m,n)
	{
		dim=d;
		coorx=xcoor;
		coory=ycoor;
	}
	void set(void)
	{
		int k=-1;
		int i=0;
		if(dim==1)
		{
		for(i=0;i<x;i++)
			{dstar->updateCell(coorx+i,coory,k);
		draw[coorx+i][coory]='v';
		dstar->updateCell(coorx+i,coory+2,k);
		draw[coorx+i][coory+2]='v';}
		}
		else if(dim==2)
		{
		for(i=0;i<y;i++)
			{dstar->updateCell(coorx,coory+i,k);
		     draw[coorx][coory+i]='v';
		     dstar->updateCell(coorx+2,coory+i,k);
			 draw[coorx+2][coory+i]='v';
		}
		}
		dstar->replan();               
        mypath = dstar->getPath();
	}
	
};


	


void set(Obstruct &m)  //此函数用于为所有障碍物的设置提供统一的接口，是C++中多态的应用
{
	m.set();
	cout<<"The environment is setting";
}




int main() 
{
	int x1,y1,x2,y2,count;
	int dr1,dr2,crash_x,crash_y;
	bool statu;
	int spee,fue,w_s;
    crash_x=crash_y=-1000;
	cout<<"请您设置无人机的参数"<<endl;
	cout<<"请输入无人机的初始速度（我们设定的最大飞行速度为50，最小为20，所以您的输入应该是20至50中的一个整数）";
	cin>>spee;
	cout<<"请输入无人机的初始油量（节能状态下一单位距离将花费1油耗，处于高能耗状态每单位距离将增加1单位油耗。故所需油量与您设定的起始距离以及最终规划路径有关，如果您不想考虑此变量，建议设定为10000）";
	cin>>fue;
	cout<<"请输入无人机的飞行状态（建议输入0，无人机所处的状态可以分为节能状态和高能耗状态，处于高能耗状态每单位距离将增加1单位油耗，0代表节能状态，1代表高能耗状态）";
	cin>>statu;
	cout<<"请输入整片区域的风速（建议输入1，此时定义的风速是整个飞行区域内的风速（并且默认为当前飞机速度为初始速度减去当前风速，但是飞机最终速度将处于20至50），与在环境设置中的区域顺逆风是不相同的。）";
	cin>>w_s;
	


	for(dr1=0;dr1<100;dr1++)
		for(dr2=0;dr2<100;dr2++)
			draw[dr1][dr2]=' ';
	char input;
	cout<<"                                欢迎来到无人机航线规划系统"<<endl
		<<"基本介绍：我们的目的是为您寻找最短的飞行路径，我们将一个虚拟空间分成10000份，构成一个100*100的矩形，您可以在其中任意的选择起点和终点"
		<<"（应保证起点和终点的XY坐标属于0至99的整数），并设置相应的位置为障碍物，我们将为您选择最优的路径，并简单的通过逐个坐标的显示展现最终的路线。"<<endl;
	cout<<"请输入您希望无人机所在的起点的x坐标：建议输入1";
    cin>>x1;
	if(x1>99) x1=99;
	if(x1<0) x1=0;
	cout<<"请输入您希望无人机所在的起点的y坐标：建议输入1";
    cin>>y1;
	if(y1>99) x1=99;
	if(y1<0) x1=0;
	cout<<"请输入您希望无人机所在的终点的x坐标：建议输入60";
    cin>>x2;
	if(x2>99) x1=99;
	if(x2<0) x1=0;
	cout<<"请输入您希望无人机所在的终点的y坐标：建议输入60";
    cin>>y2;
	if(y2>99) x1=99;
	if(y2<0) x1=0;



 //

 dstar->init(0,0,99,99);         
 dstar->updateStart(x1,y1);      
 dstar->replan();               
 mypath = dstar->getPath();     

 dstar->updateGoal(x2,y2);        
 dstar->replan();               
 mypath = dstar->getPath();     

	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
	cout<<"特别注意：我们的环境设置影响可以分为两个方面，一方面是对路径的规划产生影响的，另一方面还会影响无人机的飞行状态"<<endl;
	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

		char inp,pause;
	cout<<"我们有如下的几种自然环境，它们将对最终的路径规划产生不同的结果。具体的介绍如下，您可以根据自身需要进行设置"<<endl
		<<"1.小山丘，此时您所选择的此节点将处于不可通行状态，进行路径规划时将自动为您避开这些节点"<<endl
		<<"2.风，其中包含顺风向和逆风向两种，若为逆风向，则飞机经过此节点及其周围的8个节点的路径消耗将是无此障碍物"
		<<"时的四倍。若为顺风向，则近似认为飞机经过此节点及其周围的8个节点的路径消耗为0"<<endl
		<<"3.台风眼，这是我们对于极端自然环境的模拟。由于我们在路径规划时，只是选择了最短路径，而没有对路径上的极端环境进行考量。"
		<<"，当您设置对应的节点为此障碍物时，我们将模拟飞机坠落。（即在路径规划时不知道哪里有台风眼，如果最终的规划路径中有台风眼对应的节点，飞机将坠落）"<<endl
		<<"4.高山，此时您所选择的此节点及其周围的8个节点将处于不可通行状态，进行路径规划时将自动为您避开这些节点"<<endl;
	cout<<"请输入您希望设置的障碍物"<<endl
	    <<"5.河谷，河谷的设置需要提供5个值x_lengh，y_length和dim,x,y(特别注意：此时输入的长宽均应小于10，否则会出现问题;并且您设定的长与宽只有一个变量有效，另一个将被自动设置为3，"
		<<"哪个被设置成3将由您所设定的山谷取向有关)。dim表示河谷是沿x方向还是y方向，若为y方向河谷，则河谷的x方向长度为3，y方向的长度为y_length，若无人机进入河谷所在区域，此时无人机只能"
		<<"沿着y=x_length这条直线方向至少前进y_length长度；若为x方向河谷，则河谷的x方向长度为x_lengh，y方向的长度为3,此时无人机只能沿着x=y_length这条直线方向至少前进x_length长度"
		<<"(x,y)是河谷所在区域最接近（0,0）点的坐标。"<<endl;
	while(1)
	{
		    int o,p,q;
			cout<<endl<<"请输入您想要设置的环境类型";
			cin>>inp;
			cout<<"请输入您希望该环境所在的x坐标（如果是山谷则为长或宽，建议输入25）：";
            cin>>o;
	        cout<<"请输入您希望该环境所在的y坐标（如果是山谷则为长或宽，建议输入25）：";
            cin>>p;
			q=choice(inp,5);
		if(q==1||q==4) 
		{
			if(q==1) {Mount m1(o,p);set(m1);}
			else if(q==4) {Hmount m2(o,p);set(m2);}}
		else if(q==2)
		{
			char in;
			cout<<"您想设置的是顺风向还是逆风向（顺风向请输入1，逆风向输入2）"<<endl;
			cin>>in;
			if(choice(in,2)==1) {Swind n1(o,p);set(n1);}
			else if(choice(in,2)==2) {Nwind n2(o,p);set(n2);}}
		else if(q==3) {crash_x=o;crash_y=p;}
        else if(q==5) 
			{
					char dm;
					int coordx,coordy;
					cout<<"您想创建一个沿x轴方向的河谷还是一个沿y轴方向的河谷（输入1即为x方向，2即为y方向）"<<endl;
					cin>>dm;
					cout<<"请输入河谷所在区域最接近（0,0）点的x坐标（系统将根据您所提供的山谷的长宽即坐标信息，自动生成相应的山谷"<<endl;
					cin>>coordx;
					cout<<"请输入河谷所在区域最接近（0,0）点的y坐标"<<endl;
					cin>>coordy;
					if(choice(dm,2)==1)
					{Valley v1(o,p,1,coordx,coordy);set(v1);}
			        else if(choice(dm,2)==2)
					{Valley v2(o,p,2,coordx,coordy);set(v2);}
			}
		cout<<"如果您想退出设置，请输入q；否则输入任意值以进入下一次设置。"<<endl;
		cin>>pause;
		if(pause=='q') break;
		cout<<"下一次设置"<<endl;
		cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
	 }
	

 list<state>::iterator iter = mypath.begin();
        //写出结果
	for (count=0; iter != mypath.end(); iter++,count++)
	{
		cout << "x:" << iter->x << " , y:" << iter->y << endl;
	    draw[iter->x][iter->y]='*';
		if(iter->x==crash_x&&iter->y==crash_y) {cout<<"The plane is crashing"<<endl;return 0;}
	}
	cout<<"路径规划结果简单示意图"<<endl;
	cout<<"简单说明：图中*所连接的路径即为最终找到的最优路径。图中s所表示的区域代表顺风区，此时经过这些节点的花费为0。图中的n代表逆风区，经过这些节点的花费为普通区域的四倍。"
		"但是我们需要注意如果*区域与s区域临界，则说明最优路径一定经过了顺风区，此时s标识将被*所代替，逆风区同理；图中m代表障碍，即无人机无法通过这些节点。图中的v代表河谷区域"<<endl;
	for(dr1=0;dr1<100;dr1++)
	{
		for(dr2=0;dr2<100;dr2++)
		{
			printf("%c",draw[dr1][dr2]);
		}
		cout<<endl;}
	Plane p(spee,statu,fue,w_s);
	p.set_speed();
	p.set_distance(count);
	if(p.get_st()==1) 
	{
		p.update_fuel();
		if(p.update_fuel()<0) cout<<"虽然路径规划成功，但是飞机的油量不足以支持我们到达终点"<<endl;
		else cout<<"路径规划成功，飞机即将起飞。"<<endl;
	}
	else if(p.get_st()==0) 
		{
			if(p.update_fuel()<0) cout<<"虽然路径规划成功，但是飞机的油量不足以支持我们到达终点"<<endl;
			else cout<<"路径规划成功，飞机即将起飞。"<<endl;}
	cout<<"这次航行将花费"<<p.set_time()<<"单位的时间"<<endl;

	getchar();
	return 0;


}