#define hspeed 50
#define lspeed 20
class Plane
{
	float time;
	int speed;
	int w_sp; //ע����ʱ����ķ������������������ڵķ��٣�����ds.cpp�л��������е�����˳����ǲ���ͬ��
	bool status; //���˻�������״̬���Է�Ϊ����״̬�͸��ܺ�״̬�����ڸ��ܺ�״̬ÿ���ӽ�����1��λ�ͺ�
	int fuel;
	int f_time;
	int distance;  //�˱�����¼�����˻�Ŀǰ�����еľ��룬���Դ˼������ͺ�
public:
	Plane(int speed,bool status,int fuel,int w_sp,int f_r=200)
	{
		this->speed=speed;
		this->status=status;
		this->fuel=fuel;
		this->w_sp=w_sp;
	}
	void set_speed(void)
	{
		if(speed>50) speed=50;
		if(speed<20) speed=20;
	
	}

	int update_fuel()  //����ÿ�ƶ�һ��λ�������һ��λ
	{
		fuel=fuel-distance;
		return fuel;
	}

	void set_distance(int dis)
	{
		distance=dis;
	}
	bool get_st(void)
	{
		return status;}
	float set_time()
	{
		time=(float)distance/speed;
		return time;
	}

};