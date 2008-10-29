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
#include <string.h>
#include <string>
#include <iostream>
#include <sys/types.h>
using namespace std;

namespace business {

std::string MensajeManager::TMP_COMPRESSED_FILE_NAME  = "tmp_file";

void MensajeManager::agregarMensaje(std::string filename)
{

	FILE* file = fopen(filename.c_str(),"rb");
	if (file == NULL) {
		//No se pudo abrir el archivo.
		return;
	}
	FILE* tmpfile = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"wb");

	compressor.compress(file,tmpfile);
	fclose(tmpfile);
	fclose(file);

	ifstream filestrm(TMP_COMPRESSED_FILE_NAME.c_str());
	filestrm.seekg (0, ios::end);
	unsigned int tamanioMensaje = filestrm.tellg();
	filestrm.seekg (0, ios::beg);

	list<Imagen> imagenesDisponibles = imagenDao.getImgsSortedByEspacioLibre();
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

		mensajeDao.insert(mensaje);

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
			std::string mensajeComprimido(buffer);
			std::cout<<mensajeComprimido<<std::endl;
			std::string tiraDeBits("");
			for (int i = 0; i < streamsize  ; i++) {
				for (int j = 0; j < 8; j++)
					if ((buffer[i])&(1<<j))
						tiraDeBits.append("1");
					else
						tiraDeBits.append("0");
			}
			std::cout<<tiraDeBits<<std::endl;
			Particion particion;
			particion.setBit_inicio(imagen.getProximo_bit_libre());
			particion.setID_Img(imagen.getID());
			particion.setID_Txt(mensaje.getID());
			particion.setPosicion(numeroDeParticion);
			particion.setLongitud(streamsize);
			particion.ocupar();

			StegoBusiness* stego = StegoFactory::newInstance((*it2).getNombre());

			imagen.setProximo_bit_libre(stego->setMessage(particion.getBit_inicio(),tiraDeBits));
			imagen.setEspacio_libre(imagen.getEspacio_libre()-streamsize);
			//imagen.setHash_value()

			particionDao.insert(particion);
			//managerDAO.getImagenDAO().upate(imagen);

			streamsize = tamanioMensaje - streamsize;
			numeroDeParticion++;
			delete buffer;
		}
	}
	else {
		//throw espacio insuficiente
	}

	remove(TMP_COMPRESSED_FILE_NAME.c_str());

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
	fstream fromImage(TMP_COMPRESSED_FILE_NAME.c_str(), fstream::binary | fstream::out);

	Mensaje mensaje = mensajeDao.getMsjById(0);

	//TODO:: Chequear si vienen ordenadas por posicion
	list<Particion> particiones = particionDao.getPartsByTxt(mensaje.getID());

	for (list<Particion>::iterator it = particiones.begin(); it != particiones.end(); it++) {
		Particion& particion = *it;
		const Imagen& imagen = imagenDao.getImgById(particion.getID_Img());
		StegoBusiness* stego = StegoFactory::newInstance(imagen.getNombre());
		const string& chunk = stego->getMessage(particion.getBit_inicio(),particion.getLongitud()*8);
		std::cout<<chunk<<std::endl;
		string encodedMessage("");
		const char* buffer = chunk.c_str();
		for (int i = 0; i < chunk.size(); i+=8) {
			char byte = 0x0;
			for (int j = 0; j < 7; j++) {
				if (memcmp(buffer+i+j,"1",1) == 0)
					byte = byte | (1<<j);
			}
			encodedMessage.push_back(byte);
		}
		cout<<encodedMessage<<endl;
		fromImage.write(encodedMessage.c_str(),encodedMessage.size());
	}
	fromImage.close();

	FILE* tmp_file = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"rb");
	FILE* salida = fopen(destino.c_str(),"w+b");

	compressor.decompress(tmp_file,salida);
	fclose(tmp_file);
	remove(TMP_COMPRESSED_FILE_NAME.c_str());
	fclose(salida);
}

MensajeManager::~MensajeManager() {
	// TODO Auto-generated destructor stub
}
}
