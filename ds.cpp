#include"dstar.h"
#include <iostream>
#include <unordered_map>
#include "plane.h"
using namespace stdext;
using namespace std;
list<state> mypath;
Dstar *dstar = new Dstar();
char draw[100][100];

int choice(char a,int n)  //�˺������ڽ����û����صĸ���ѡ��
{
	int i;
	while(1)
	{
	for(i=1;i<=n;i++)
		if((a-48)==i) return i;
	cout<<"�������벻���ϱ�׼������������";
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


	


void set(Obstruct &m)  //�˺�������Ϊ�����ϰ���������ṩͳһ�Ľӿڣ���C++�ж�̬��Ӧ��
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
	cout<<"�����������˻��Ĳ���"<<endl;
	cout<<"���������˻��ĳ�ʼ�ٶȣ������趨���������ٶ�Ϊ50����СΪ20��������������Ӧ����20��50�е�һ��������";
	cin>>spee;
	cout<<"���������˻��ĳ�ʼ����������״̬��һ��λ���뽫����1�ͺģ����ڸ��ܺ�״̬ÿ��λ���뽫����1��λ�ͺġ����������������趨����ʼ�����Լ����չ滮·���йأ���������뿼�Ǵ˱����������趨Ϊ10000��";
	cin>>fue;
	cout<<"���������˻��ķ���״̬����������0�����˻�������״̬���Է�Ϊ����״̬�͸��ܺ�״̬�����ڸ��ܺ�״̬ÿ��λ���뽫����1��λ�ͺģ�0�������״̬��1������ܺ�״̬��";
	cin>>statu;
	cout<<"��������Ƭ����ķ��٣���������1����ʱ����ķ������������������ڵķ��٣�����Ĭ��Ϊ��ǰ�ɻ��ٶ�Ϊ��ʼ�ٶȼ�ȥ��ǰ���٣����Ƿɻ������ٶȽ�����20��50�������ڻ��������е�����˳����ǲ���ͬ�ġ���";
	cin>>w_s;
	


	for(dr1=0;dr1<100;dr1++)
		for(dr2=0;dr2<100;dr2++)
			draw[dr1][dr2]=' ';
	char input;
	cout<<"                                ��ӭ�������˻����߹滮ϵͳ"<<endl
		<<"�������ܣ����ǵ�Ŀ����Ϊ��Ѱ����̵ķ���·�������ǽ�һ������ռ�ֳ�10000�ݣ�����һ��100*100�ľ��Σ������������������ѡ�������յ�"
		<<"��Ӧ��֤�����յ��XY��������0��99������������������Ӧ��λ��Ϊ�ϰ�����ǽ�Ϊ��ѡ�����ŵ�·�������򵥵�ͨ������������ʾչ�����յ�·�ߡ�"<<endl;
	cout<<"��������ϣ�����˻����ڵ�����x���꣺��������1";
    cin>>x1;
	if(x1>99) x1=99;
	if(x1<0) x1=0;
	cout<<"��������ϣ�����˻����ڵ�����y���꣺��������1";
    cin>>y1;
	if(y1>99) x1=99;
	if(y1<0) x1=0;
	cout<<"��������ϣ�����˻����ڵ��յ��x���꣺��������60";
    cin>>x2;
	if(x2>99) x1=99;
	if(x2<0) x1=0;
	cout<<"��������ϣ�����˻����ڵ��յ��y���꣺��������60";
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
	cout<<"�ر�ע�⣺���ǵĻ�������Ӱ����Է�Ϊ�������棬һ�����Ƕ�·���Ĺ滮����Ӱ��ģ���һ���滹��Ӱ�����˻��ķ���״̬"<<endl;
	cout<<"--------------------------------------------------------------------------------------------------------"<<endl;

		char inp,pause;
	cout<<"���������µļ�����Ȼ���������ǽ������յ�·���滮������ͬ�Ľ��������Ľ������£������Ը���������Ҫ��������"<<endl
		<<"1.Сɽ�𣬴�ʱ����ѡ��Ĵ˽ڵ㽫���ڲ���ͨ��״̬������·���滮ʱ���Զ�Ϊ���ܿ���Щ�ڵ�"<<endl
		<<"2.�磬���а���˳�������������֣���Ϊ�������ɻ������˽ڵ㼰����Χ��8���ڵ��·�����Ľ����޴��ϰ���"
		<<"ʱ���ı�����Ϊ˳�����������Ϊ�ɻ������˽ڵ㼰����Χ��8���ڵ��·������Ϊ0"<<endl
		<<"3.̨���ۣ��������Ƕ��ڼ�����Ȼ������ģ�⡣����������·���滮ʱ��ֻ��ѡ�������·������û�ж�·���ϵļ��˻������п�����"
		<<"���������ö�Ӧ�Ľڵ�Ϊ���ϰ���ʱ�����ǽ�ģ��ɻ�׹�䡣������·���滮ʱ��֪��������̨���ۣ�������յĹ滮·������̨���۶�Ӧ�Ľڵ㣬�ɻ���׹�䣩"<<endl
		<<"4.��ɽ����ʱ����ѡ��Ĵ˽ڵ㼰����Χ��8���ڵ㽫���ڲ���ͨ��״̬������·���滮ʱ���Զ�Ϊ���ܿ���Щ�ڵ�"<<endl;
	cout<<"��������ϣ�����õ��ϰ���"<<endl
	    <<"5.�ӹȣ��ӹȵ�������Ҫ�ṩ5��ֵx_lengh��y_length��dim,x,y(�ر�ע�⣺��ʱ����ĳ����ӦС��10��������������;�������趨�ĳ����ֻ��һ��������Ч����һ�������Զ�����Ϊ3��"
		<<"�ĸ������ó�3���������趨��ɽ��ȡ���й�)��dim��ʾ�ӹ�����x������y������Ϊy����ӹȣ���ӹȵ�x���򳤶�Ϊ3��y����ĳ���Ϊy_length�������˻�����ӹ��������򣬴�ʱ���˻�ֻ��"
		<<"����y=x_length����ֱ�߷�������ǰ��y_length���ȣ���Ϊx����ӹȣ���ӹȵ�x���򳤶�Ϊx_lengh��y����ĳ���Ϊ3,��ʱ���˻�ֻ������x=y_length����ֱ�߷�������ǰ��x_length����"
		<<"(x,y)�Ǻӹ�����������ӽ���0,0��������ꡣ"<<endl;
	while(1)
	{
		    int o,p,q;
			cout<<endl<<"����������Ҫ���õĻ�������";
			cin>>inp;
			cout<<"��������ϣ���û������ڵ�x���꣨�����ɽ����Ϊ�������������25����";
            cin>>o;
	        cout<<"��������ϣ���û������ڵ�y���꣨�����ɽ����Ϊ�������������25����";
            cin>>p;
			q=choice(inp,5);
		if(q==1||q==4) 
		{
			if(q==1) {Mount m1(o,p);set(m1);}
			else if(q==4) {Hmount m2(o,p);set(m2);}}
		else if(q==2)
		{
			char in;
			cout<<"�������õ���˳�����������˳����������1�����������2��"<<endl;
			cin>>in;
			if(choice(in,2)==1) {Swind n1(o,p);set(n1);}
			else if(choice(in,2)==2) {Nwind n2(o,p);set(n2);}}
		else if(q==3) {crash_x=o;crash_y=p;}
        else if(q==5) 
			{
					char dm;
					int coordx,coordy;
					cout<<"���봴��һ����x�᷽��ĺӹȻ���һ����y�᷽��ĺӹȣ�����1��Ϊx����2��Ϊy����"<<endl;
					cin>>dm;
					cout<<"������ӹ�����������ӽ���0,0�����x���꣨ϵͳ�����������ṩ��ɽ�ȵĳ���������Ϣ���Զ�������Ӧ��ɽ��"<<endl;
					cin>>coordx;
					cout<<"������ӹ�����������ӽ���0,0�����y����"<<endl;
					cin>>coordy;
					if(choice(dm,2)==1)
					{Valley v1(o,p,1,coordx,coordy);set(v1);}
			        else if(choice(dm,2)==2)
					{Valley v2(o,p,2,coordx,coordy);set(v2);}
			}
		cout<<"��������˳����ã�������q��������������ֵ�Խ�����һ�����á�"<<endl;
		cin>>pause;
		if(pause=='q') break;
		cout<<"��һ������"<<endl;
		cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
	 }
	

 list<state>::iterator iter = mypath.begin();
        //д�����
	for (count=0; iter != mypath.end(); iter++,count++)
	{
		cout << "x:" << iter->x << " , y:" << iter->y << endl;
	    draw[iter->x][iter->y]='*';
		if(iter->x==crash_x&&iter->y==crash_y) {cout<<"The plane is crashing"<<endl;return 0;}
	}
	cout<<"·���滮�����ʾ��ͼ"<<endl;
	cout<<"��˵����ͼ��*�����ӵ�·����Ϊ�����ҵ�������·����ͼ��s����ʾ���������˳��������ʱ������Щ�ڵ�Ļ���Ϊ0��ͼ�е�n�����������������Щ�ڵ�Ļ���Ϊ��ͨ������ı���"
		"����������Ҫע�����*������s�����ٽ磬��˵������·��һ��������˳��������ʱs��ʶ����*�����棬�����ͬ��ͼ��m�����ϰ��������˻��޷�ͨ����Щ�ڵ㡣ͼ�е�v����ӹ�����"<<endl;
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
		if(p.update_fuel()<0) cout<<"��Ȼ·���滮�ɹ������Ƿɻ�������������֧�����ǵ����յ�"<<endl;
		else cout<<"·���滮�ɹ����ɻ�������ɡ�"<<endl;
	}
	else if(p.get_st()==0) 
		{
			if(p.update_fuel()<0) cout<<"��Ȼ·���滮�ɹ������Ƿɻ�������������֧�����ǵ����յ�"<<endl;
			else cout<<"·���滮�ɹ����ɻ�������ɡ�"<<endl;}
	cout<<"��κ��н�����"<<p.set_time()<<"��λ��ʱ��"<<endl;

	getchar();
	return 0;


}