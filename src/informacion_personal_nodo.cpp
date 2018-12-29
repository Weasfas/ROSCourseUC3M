#include "ros/ros.h"
#include <std_msgs/String.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <interaccion/inf_personal_usuario.h>
#include <signal.h>

using namespace std;

// Handler para SIGINT en el caso de finalizar la ejecución del nodo con CTRL + C
void SigintHandler(int sig)
{
  ROS_INFO("\nShutting down inf_personal_usuario ...\n");
  ros::shutdown();
}

int main(int argc, char **argv)
{
  // Inicializacion de ROS y el nodo
  ros::init(argc, argv, "informacion_personal_nodo");
  ros::NodeHandle n;
  
  // Publicador
  ros::Publisher pub = n.advertise<interaccion::inf_personal_usuario>("/interaccion/informacion_personal_topic", 1000);
  
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
	string name = "";
	int edad = 0;
	vector<string> idiomas;
	
	cout << "Please enter you name: ";
	cin >> name;
	
	cout << "Please enter your age: ";
	cin >> edad;

	string input;
	bool entr = true; // Control de la entrada

	// Posibles idiomas del usuario: Introducir todos los que se quieran y parar con "done"
	do
	{
       	cout << "Please enter your languages: ";
		cin >> input;
		
		if(strcmp(input.c_str(),"done")==0)
		{
			entr = false;
		}
		else
		{
			idiomas.push_back(input);
		}
		
	} while(entr);

	interaccion::inf_personal_usuario msg;
	
	msg.nombre = name;
	msg.edad = edad;
	msg.idiomas = idiomas;
	
    pub.publish(msg);

	// Se utiliza CTRL + C?
	signal(SIGINT, SigintHandler);
    ros::spinOnce();
	
    loop_rate.sleep();
  }
  return 0;
}