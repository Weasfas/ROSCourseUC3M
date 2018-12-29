#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <string>
#include <sstream>
#include <interaccion/pos_usuario.h>
#include <signal.h>

using namespace std;

// Handler para SIGINT en el caso de finalizar la ejecución del nodo con CTRL + C
void SigintHandler(int sig)
{
  ROS_INFO("\nShutting down posicion_usuario_nodo ...\n");
  ros::shutdown();
}

int main(int argc, char **argv)
{
  // Inicializacion de ROS y el nodo
  ros::init(argc, argv, "posicion_usuario_nodo");
  ros::NodeHandle n;
  
  // Publicador
  ros::Publisher pub = n.advertise<interaccion::pos_usuario>("/interaccion/posicion_usuario_topic", 1000);
  
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    int x, y, z;
	
    cout << "Please enter your position: ";
	
	string input;

	getline(cin, input); // La entrada es del formato: INT INT INT. Ej: 6 3 9

	stringstream ss(input);

	ss >> x >> y >> z;
	
	interaccion::pos_usuario msg;
	
	msg.p_x = x;
	msg.p_y = y;
	msg.p_z = z;	
	
  pub.publish(msg);

  // Se utiliza CTRL + C?
  signal(SIGINT, SigintHandler);
  ros::spinOnce();
	
  loop_rate.sleep();
  }
  return 0;
}