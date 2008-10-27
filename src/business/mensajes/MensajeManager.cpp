/*
 * MensajeManager.cpp
 *
 *  Created on: 28-jul-2008
 *      Author: gsenno
 */

#include "MensajeManager.h"
#include "../compressor/CompressorBusiness.h"
#include "../stego/StegoFactory.h"
#include "../stego/StegoBusiness.h"
#include <sys/stat.h>
#include <list>
#include <cstdlib>
#include <sys/types.h>
using namespace std;

namespace business {

std::string MensajeManager::TMP_COMPRESSED_FILE_NAME  = "tmp_file";

void MensajeManager::agregarMensaje(std::string filename)
{
//	CompressorBusiness compressor;
//	FILE* file = fopen(filename.c_str(),"rb");
//	FILE* tmpfile = fopen(TMP_FILE_NAME,"w+b");
//
//	compressor.compress(file,tmpfile);
	ifstream filestrm(filename.c_str());
	if (filestrm.fail()) {
		//No se pudo abrir el archivo
		return;
	}

	filestrm.seekg (0, ios::end);
	unsigned int tamanioMensaje = filestrm.tellg();
	filestrm.seekg (0, ios::beg);

	list<Imagen> imagenesDisponibles = managerDAO.getImagenDAO().getImgsSortedByEspacioLibre();
	list<Imagen> imagenesSeleccionadas;

	unsigned int espacioDisponible = 0;
	list<Imagen>::iterator it = imagenesDisponibles.begin();

	while ((it != imagenesDisponibles.end())&&(espacioDisponible < tamanioMensaje)) {
		Imagen& imagen = (*it);
		espacioDisponible += imagen.getEspacio_libre();
		imagenesSeleccionadas.push_back(imagen);
	}

//	if (espacioDisponible < tamanioMensaje) {
//		buscarImagenes()
//	}

	//volver a calcular el espacio disponible

	if (espacioDisponible > tamanioMensaje) {
		Mensaje mensaje;
		//TODO::REMOVE SETID
		mensaje.setID(0);
		mensaje.setNombre(filename);
		mensaje.setTamanio(tamanioMensaje);
		mensaje.setCant_partes(imagenesSeleccionadas.size());

		managerDAO.getMensajeDAO().insert(mensaje);

		unsigned int streamsize = tamanioMensaje;
		unsigned int numeroDeParticion = 0;
		for (list<Imagen>::iterator it2 = imagenesSeleccionadas.begin(); it2 != imagenesSeleccionadas.end(); it2++) {
			Imagen& imagen = *it2;
			unsigned int espacioEnImagen = imagen.getEspacio_libre();
			if (streamsize >= espacioEnImagen) {
				streamsize = espacioEnImagen;
			}
			char* buffer = new char[streamsize];

			filestrm.read(buffer,streamsize);

			Particion particion;
			particion.setBit_inicio(imagen.getProximo_bit_libre());
			particion.setID_Img(imagen.getID());
			particion.setID_Txt(mensaje.getID());
			particion.setPosicion(numeroDeParticion);
			particion.setLongitud(streamsize);
			particion.ocupar();

			StegoBusiness* stego = StegoFactory::newInstance((*it2).getNombre());

			imagen.setProximo_bit_libre(stego->setMessage(particion.getBit_inicio(),buffer));
			imagen.setEspacio_libre(imagen.getEspacio_libre()-streamsize);
			//imagen.setHash_value()

			managerDAO.getParticionDAO().insert(particion);
			//managerDAO.getImagenDAO().upate(imagen);

			streamsize = tamanioMensaje - streamsize;
			numeroDeParticion++;
			delete buffer;
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



void MensajeManager::obtenerMensaje(std::string filename, std::string destino)
{
	fstream salida(destino.c_str(), fstream::binary | fstream::out);

	MensajeDAO mensajeDAO = managerDAO.getMensajeDAO();
	ParticionDAO particionDAO = managerDAO.getParticionDAO();
	ImagenDAO imagenDAO = managerDAO.getImagenDAO();

	Mensaje mensaje = mensajeDAO.getMsjById(0);

	//TODO:: Chequear si vienen ordenadas por posicion
	list<Particion> particiones = particionDAO.getPartsByTxt(mensaje.getID());

	for (list<Particion>::iterator it = particiones.begin(); it != particiones.end(); it++) {
		Particion& particion = *it;
		const Imagen& imagen = imagenDAO.getImgById(particion.getID_Img());
		StegoBusiness* stego = StegoFactory::newInstance(imagen.getNombre());
		const std::string& chunk = stego->getMessage(particion.getBit_inicio(),particion.getLongitud());
		salida.write(chunk.c_str(),particion.getLongitud());
	}

}

MensajeManager::~MensajeManager() {
	// TODO Auto-generated destructor stub
}
}
