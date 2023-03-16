#define hspeed 50
#define lspeed 20
class Plane
{
	float time;
	int speed;
	int w_sp; //注：此时定义的风速是整个飞行区域内的风速，与在ds.cpp中环境设置中的区域顺逆风是不相同的
	bool status; //无人机所处的状态可以分为节能状态和高能耗状态，处于高能耗状态每分钟将增加1单位油耗
	int fuel;
	int f_time;
	int distance;  //此变量记录了无人机目前所飞行的距离，并以此计算总油耗
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

	int update_fuel()  //假设每移动一单位距离耗油一单位
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