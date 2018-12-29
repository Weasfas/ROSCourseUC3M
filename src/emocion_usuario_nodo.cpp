#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>
#include <string>
#include <signal.h>

using namespace std;

// Handler para SIGINT en el caso de finalizar la ejecución del nodo con CTRL + C
void SigintHandler(int sig)
{
  ROS_INFO("\nShutting down emocion_usuario_nodo ...\n");
  ros::shutdown();
}

int main(int argc, char **argv)
{
  // Inicializacion de ROS y el nodo
  ros::init(argc, argv, "emocion_usuario_nodo");
  ros::NodeHandle n;

  // Publicador
  ros::Publisher pub = n.advertise<std_msgs::String>("/interaccion/emocion_usuario_topic", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    // Variables para contener la emocion entrada por el usuario
    std_msgs::String msg;
	  string aux;
	
	  cout << "Please enter you emotion: ";

    getline(cin, aux);
    stringstream ss(aux);
    msg.data = ss.str();

    pub.publish(msg);

    // Se utiliza CTRL + C?
    signal(SIGINT, SigintHandler);
    ros::spinOnce();
	
    loop_rate.sleep();
  }
  return 0;
}