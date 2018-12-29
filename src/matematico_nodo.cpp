#include <interaccion/multiplicador.h>
#include <ros/ros.h>

// Función que realiza el servicio: x2 la entrada
bool run(interaccion::multiplicador::Request  &req, interaccion::multiplicador::Response &res)
{
   res.resultado=req.entrada * 2;
   
   ROS_INFO("Servidor procesando...");

   return true;
}

int main(int argc, char **argv)
{
  // Inicializacion de ROS y el nodo
  ros::init(argc, argv, "matematico_nodo");
  ros::NodeHandle n;

  // Levantar el servidor
  ros::ServiceServer service = n.advertiseService("multiplicar_edad", run);
  ROS_INFO("Server is up!!");
  ros::spin();

  return 0;
}
