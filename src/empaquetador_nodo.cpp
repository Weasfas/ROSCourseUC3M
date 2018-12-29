#include "ros/ros.h"
#include "std_msgs/String.h"
#include <vector>
#include <string>
#include <interaccion/inf_personal_usuario.h>
#include <interaccion/pos_usuario.h>
#include <interaccion/usuario.h>

using namespace std;

// Variables a guardar
string name;
int edad;
vector<string> idiomas;

// Variables booleanas para implementar las restricciones del paso de argumentos por el usuario
bool pub_inf = false;
bool pub_pos = false;
bool pub_emot = false;

string emocion_usuario;
int x, y, z;

// Callback para la emocion del usuario
void emocion_callback(const std_msgs::String&  msg)
{
  emocion_usuario = msg.data;
  pub_emot = true;
  ROS_INFO("Recibida emocion de usuario...");
}

// Callback para la informacion personal del usuario
void inf_personal_usuario_callback(const interaccion::inf_personal_usuario& msg)
{
  name = msg.nombre;
  edad = msg.edad;
  idiomas = msg.idiomas;
  pub_inf = true;
  ROS_INFO("Recibida informacion de usuario...");
}

// Callback para la posicion del usuario
void posicion_usuario_callback(const interaccion::pos_usuario& msg)
{
  x= msg.p_x;
  y= msg.p_y;
  z= msg.p_z;
  pub_pos = true;
  ROS_INFO("Recibida posicion de usuario...");
}

int main(int argc, char **argv)
{
	// Inicializacion de ROS y el nodo
	ros::init(argc, argv, "empaquetador_nodo"); 
	ros::NodeHandle nh;

	// Subscriptores
	ros::Subscriber informacion_personal, posicion_usuario, emocion;

	// Publicador
	ros::Publisher myPub = nh.advertise<interaccion::usuario>("interaccion/user_topic", 1000);

	emocion = nh.subscribe("/interaccion/emocion_usuario_topic", 1000, &emocion_callback);
	informacion_personal = nh.subscribe("/interaccion/informacion_personal_topic", 1000, &inf_personal_usuario_callback);
	posicion_usuario = nh.subscribe("/interaccion/posicion_usuario_topic", 1000, &posicion_usuario_callback);
	
	ros::Rate loop_rate(10);

	while(ros::ok()){

	interaccion::usuario msg;

	msg.infPersonal.nombre = name;
	msg.infPersonal.edad = edad;
	msg.infPersonal.idiomas = idiomas;

	msg.emocion = emocion_usuario;

	msg.posicion.p_x = x;
	msg.posicion.p_y = y;
	msg.posicion.p_z = z;

	// No permitir la publicacion del mensage si no se ha completado
	if(pub_inf && pub_pos && pub_emot)
	{
		myPub.publish(msg);
		pub_inf  = false;
		pub_pos = false;
		pub_emot = false;	
	}

	ros::spinOnce();

	loop_rate.sleep();
	}
	return 0; 
}