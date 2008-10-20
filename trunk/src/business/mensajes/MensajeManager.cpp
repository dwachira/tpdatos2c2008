/*
 * MensajeManager.cpp
 *
 *  Created on: 28-jul-2008
 *      Author: gsenno
 */

#include "MensajeManager.h"
#include <sys/stat.h>
#include <list>
#include <sys/types.h>
using namespace std;

void MensajeManager::agregarMensaje(std::string filename)
{
	ifstream filestrm(filename.c_str());
	if (filestrm.fail()) {
		//No se pudo abrir el archivo
		return;
	}

	filestrm.seekg (0, ios::end);
	int tamanioMensaje = filestrm.tellg();
	filestrm.seekg (0, ios::beg);

	list<Imagen> imagenesDisponibles = managerDAO.getImagenDAO().getImgsSortedByEspacioLibre();
	list<Imagen> imagenesSeleccionadas;

	int espacioDisponible = 0;
	list<Imagen>::iterator it = imagenesDisponibles.begin();

	while ((it != imagenesDisponibles.end())&&(espacioDisponible < tamanioMensaje)) {
		Imagen& imagen = (*it);
		espacioDisponible += imagen.getEspacio_libre();
		imagenesSeleccionadas.push_back(imagen);
	}

	if (espacioDisponible < tamanioMensaje) {
		//Escanear directorios en busca de imagenes
	}

	//volver a calcular el espacio disponible

	if (espacioDisponible > tamanioMensaje) {
		Mensaje mensaje;
		mensaje.setNombre(filename);
		mensaje.setTamanio(tamanioMensaje);
		mensaje.setCant_partes(imagenesSeleccionadas.size());

		managerDAO.getMensajeDAO().insert(mensaje);

		int streamsize = tamanioMensaje;
		for (list<Imagen>::iterator it2 = imagenesSeleccionadas.begin(); it2 != imagenesSeleccionadas.end(); it2++) {
			int espacioEnImagen = (*it2).getEspacio_libre();
			if (streamsize >= espacioEnImagen)
				streamsize = espacioEnImagen;
			char* buffer = new char[streamsize];
			filestrm.read(buffer,streamsize);
			//stego con imagen y buffer
			//creo particion y asigno bit_inicio y posicion
			//update de espacio libre y hash_value en imagen
			//insert de particion
			streamsize = tamanioMensaje - streamsize;
		}
	}
	else {
		//throw espacio insuficiente
	}


}

void MensajeManager::quitarMensaje(std::string filename)
{
//	Mensaje& mensaje = managerDAO.getMensajeDAO()->getMensajeByNombre(filename);
//	if (mensaje == NULL) {
//		//NO EXISTE EL MENSAJE
//	}
//	else {
//		ParticionDAO* particionDAO = managerDAO.getParticionDAO();
//		list<Particion> particiones = particionDAO->getPartsByTxt(mensaje.getId());
//		for (list<Particion>::iterator it = particiones.begin(); it != particiones.end();  it++) {
//			Imagen& imagen = managerDAO.getImagenDAO()->getImgById((*it).getID_Img());
//			imagen.setEspacio_libre(imagen.getEspacio_libre()+(*it).getLongitud());
//			//RECUPERAR ESPACIO EN IMAGEN
//			particionDAO->drop(*it);
//		}
//		managerDAO.getMensajeDAO()->drop(mensaje);
//	}
}



MensajeManager::~MensajeManager() {
	// TODO Auto-generated destructor stub
}
