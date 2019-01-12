#include "ros/ros.h"
#include "std_msgs/String.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <string>
#include <vector>
#include <std_msgs/Bool.h>

using namespace std;
using namespace boost::posix_time;

typedef boost::date_time::c_local_adjustor<ptime> local_adj;

// Variables globales para el temporizador
std_msgs::String info;
ros::Time callbackMsgTime(0.0);

// Clase para poder publicar desde el Calback del temporizador
class ClockTimer
{
	public:

	ClockTimer()
	{
		ros::NodeHandle nh;

		myPub = nh.advertise<std_msgs::Bool>("interaccion/still_alive", 1000);
		myTimer = nh.createTimer(ros::Duration(10), &ClockTimer::timer_callback, this);
	}

	void timer_callback(const ros::TimerEvent& event)
	{
  		//ROS_INFO("-------------- TIMER EVENT -----------------");

 		msg.data = true;

 		myPub.publish(msg);

	}

	protected:
		ros::Publisher myPub;
		ros::Timer myTimer;

		std_msgs::Bool msg;

};

// Callback para la inicializacion del timer
void message_callback(const std_msgs::String& msg)
{
  info = msg;
  callbackMsgTime = ros::Time::now();
 // ROS_INFO("Recibida init timer...");
}

int main(int argc, char **argv)
{
	// Inicializacion de ROS y el nodo
	ros::init(argc, argv, "reloj_nodo");
	ros::NodeHandle nh;

	string topc = info.data;

	// Topic dinamico
	if(strcmp(topc.c_str(),"start_topic")==0)
	{
		topc = "interaccion/start_topic";
	}
	else
	{
		topc = "interaccion/reset_topic";
	}
	
	// Subscriptor
	ros::Subscriber subs = nh.subscribe(topc, 1000, &message_callback);

	// Temporizador
	ClockTimer tmr;

	ros::Rate loop_rate(3);

	while(ros::ok()){

	// Tiempo local y en UTC
	ptime UTCTime = second_clock::universal_time();
	ptime localTime = local_adj::utc_to_local(UTCTime);
	
	ROS_INFO("Current local date: %s", to_simple_string(localTime).c_str());
	ROS_INFO("Current UTC date: %s", to_simple_string(UTCTime).c_str());

	ros::Time currentTime = ros::Time::now();
	if(callbackMsgTime.toSec() != 0.0)
	{
		// Se utiliza el tiempo resultante de la resta del tiempo actual con el último registrado en el callback
		ros::Duration elapsedTime = currentTime - callbackMsgTime;
		ROS_INFO("Han pasado %f segundos desde el ultimo msg", elapsedTime.toSec());
	}
	else
	{
		// Durante el inicio de la ejecución no se reciben mensajes ya que el timer no esta "activado"
		ROS_INFO("No se han recibido msgs todavia...");
	}

	ros::spinOnce();

	loop_rate.sleep();
	}
	return 0; 
}