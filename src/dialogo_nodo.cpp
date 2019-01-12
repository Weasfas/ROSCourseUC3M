#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#include <vector>
#include <interaccion/inf_personal_usuario.h>
#include <interaccion/pos_usuario.h>
#include <interaccion/usuario.h>
#include <interaccion/multiplicador.h>
#include <std_msgs/Bool.h>

using namespace std;

// Template para convertir numero en entero: No se muy bien porque pero std::to_string (c++11) no compila correctamente
template<class T>
std::string toString(const T &num){
	std::ostringstream stream;
	stream << num;
	return stream.str();
}

// Variables globales para la información compacta del usuario recibida y el contador para el temporizador
interaccion::usuario info;
int contador = -100;
bool procesar = false;

// Callback del empaquetador
void empaquetador_callback(const interaccion::usuario& msg)
{
  info = msg;
	procesar = true;
  ROS_INFO("Recibida iteraccion de usuario...");
}

// Callback para el temporizador
void clock_callback(const std_msgs::Bool& msg)
{
  if(msg.data){
  	ROS_INFO("---------------------STILL ALIVE---------------------");
	}

}

int main(int argc, char **argv)
{
	// Inicializacion de ROS y el nodo
	ros::init(argc, argv, "dialogo_nodo"); 
	ros::NodeHandle nh;

	// Publicadores
	ros::Publisher myPub;
	ros::Subscriber timeClock;

	// Subscriptor
	ros::Subscriber empaquetador;

	empaquetador = nh.subscribe("interaccion/user_topic", 1000, &empaquetador_callback); // Inf. usuario
  ros::ServiceClient client = nh.serviceClient<interaccion::multiplicador>("multiplicar_edad"); // Servicio Edad x 2
	timeClock = nh.subscribe("interaccion/still_alive", 1000, &clock_callback); // Mensaje del temporizador
  interaccion::multiplicador srv; // Servidor
	
	ros::Rate loop_rate(10);

	// Texto del comando para espeak
	string text = "default";

	while(ros::ok()){

	  string name = info.infPersonal.nombre;
	  int age = info.infPersonal.edad;

	  srv.request.entrada = age;
		if(client.call(srv))
 	  {
		  age = srv.response.resultado;
		  ROS_INFO("Cliente registra la edad...");
 	  }
		else
		{
  	  ROS_ERROR("Fallo en el cliente...");
 		  return 1;
  	}

	  vector<string> lang = info.infPersonal.idiomas;
	  ROS_INFO(" Informacion del usuario... ");
	  ROS_INFO(" [Name: %s] ", name.c_str());
	  ROS_INFO(" [Age: %d] ", age);
	  
	  for(int ii = 0; ii < lang.size(); ++ii){
			ROS_INFO(" [Idioma %d --> %s] ", ii, lang[ii].c_str());
	  }

	  string emot = info.emocion;
	  ROS_INFO(" Emocion del usuario --> [%s] ", emot.c_str());
	  
	  int x = info.posicion.p_x;
	  int y = info.posicion.p_y;
	  int z = info.posicion.p_z;
	  
	  ROS_INFO(" Posicion del usuario --> [PosX: %d][PosY: %d][PosZ: %d] ", x , y, z);

		// Variable para contener el topic dinámico
	  string topc;
	  std_msgs::String time_msg;

		// Cuando el contador llega a 100 se empieza a publicar en otro topic para "activar" el temporizador
	  if(contador < 0)
	  {
			topc = "interaccion/start_topic";
			time_msg.data = "start_topic";
			++contador;
	  }
		else
	  {
			time_msg.data = "reset_topic";
			topc = "interaccion/reset_topic";
	  }

	  myPub = nh.advertise<std_msgs::String>(topc, 1000);
		myPub.publish(time_msg);


		if(procesar)
		{
			text = "Hola " + name + " veo que estas " + emot + ", tu posicion es " + toString(x) + " en X " + toString(y) + " en Y " + toString(z) + " en Z";

			// Llamada a espeak
			string command = "espeak -v es \"" + text + "\"";
			system(command.c_str());

			procesar = false;
		}

	  ros::spinOnce();

	  loop_rate.sleep();
	}

	return 0; 
}