/*
 * MensajeManager.cpp
 *
 *  Created on: 28-jul-2008
 *      Author: gsenno
 */

#include "MensajeManager.h"
#include "../compressor/CompressorBusiness.h"
#include "../../object/exceptions/RecursoInaccesibleException.h"
#include "../../object/exceptions/EspacioInsuficienteException.h"
#include "../es/EntradaSalidaManager.h"
#include "../../dao/DirectorioDAO.h"
#include "../stego/StegoFactory.h"
#include "../stego/StegoBusiness.h"
#include <sys/stat.h>
#include <list>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include "../../object/exceptions/EntidadInexistenteException.h"
using namespace std;
using namespace dao;

namespace business {

std::string MensajeManager::TMP_COMPRESSED_FILE_NAME  = __BASE_DIR__"/tmp_file";

bool compararParticionesPorIdImagen(Particion& iz, Particion& der) {
	return iz.getID_Img() < der.getID_Img();
}

string transformMensajeToString(Mensaje& mensaje) {
	return mensaje.getNombre();
}

void MensajeManager::agregarMensaje(std::string filename)
{

	/** Comprimo el mensaje **/
	FILE* file = fopen(filename.c_str(),"rb");
	if (file == NULL) {
		throw RecursoInaccesibleException();
	}
	FILE* tmpfile = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"wb");

	compressor.compress(file,tmpfile);
	fclose(tmpfile);
	fclose(file);
	/***************************/

	/**Busco imagenes**/
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
		if (EntradaSalidaManager::recursoEsAccesible(imagen.getNombre())) {
			espacioDisponible += imagen.getEspacio_libre();
			imagenesSeleccionadas.push_back(imagen);
		}
		it++;
	}

	//Si el espacioDisponible no alcanza, hay que escanear los directorios en busca de nuevas imagenes
	if (espacioDisponible < tamanioMensaje) {

		list<Directorio> directorios = directorioManager.getDirectorioDao().getDirsSortedByFechaModif();

		for (list<Directorio>::iterator iter = directorios.begin(); iter != directorios.end(); iter++) {
			Directorio& directorio = *iter;

			util::Date fechaModificacionAlmacenada = directorio.getFechaUltimaModificacion();
			directorioManager.actualizarFechaDeModificacion(directorio);
 			if (fechaModificacionAlmacenada < directorio.getFechaUltimaModificacion()) {

 				list<Imagen> imagenesEnDirectorio = imagenDao.getImgsByDirectorio(directorio.getID());
				DirectorioIteradorImagenes iteradorDirectorio = directorioManager.obtenerIteradorDeImagenes(directorio);
				while ((iteradorDirectorio.hasNext())) {

					std::string pathImagen = iteradorDirectorio.next();
					if (EntradaSalidaManager::recursoEsAccesible(pathImagen)) {
						Imagen imagenEnDir(pathImagen);
						list<Imagen>::iterator listBeginning = imagenesEnDirectorio.begin();
						list<Imagen>::iterator listEnding = imagenesEnDirectorio.end();
						if (find(listBeginning,listEnding,imagenEnDir) == listEnding) {
							if (directorioManager.agregarImagenEnDirectorio(directorio,imagenEnDir)) {
								imagenesSeleccionadas.push_back(imagenEnDir);
								if ((espacioDisponible += imagenEnDir.getTamanio()) >= tamanioMensaje) {
									break;
								}
							}
						}
					}
				}
				if (espacioDisponible >= tamanioMensaje)
					break;
			}

		}

	}

	if (espacioDisponible > tamanioMensaje) {
		Mensaje mensaje(filename,tamanioMensaje,imagenesSeleccionadas.size());

		mensajeDao.insert(mensaje);
		trieDao.insertCadena(MENSAJES,filename,mensaje.getID());

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

			std::string tiraDeBits("");
			for (unsigned int i = 0; i < streamsize  ; i++) {
				for (int j = 0; j < 8; j++)
					if ((buffer[i])&(1<<j))
						tiraDeBits.append("1");
					else
						tiraDeBits.append("0");
			}

			list<Particion> particionesEnImagen = particionDao.getPartsByImg(imagen.getID());
			StegoBusiness* stego = StegoFactory::newInstance((*it2).getNombre());

			if (particionesEnImagen.size() > 0) {
				for(list<Particion>::iterator iterPart = particionesEnImagen.begin(); iterPart != particionesEnImagen.end(); iterPart++) {
					Particion& particion = *iterPart;
					if (particion.isLibre()) {
						Particion nuevaParticion(imagen.getID(),mensaje.getID(),numeroDeParticion,
								particion.getBit_inicio(),particion.getLongitud(),false);
						particionDao.borrar(particion);
						particionDao.insert(nuevaParticion);
						stego->setMessage(nuevaParticion.getBit_inicio(),tiraDeBits.substr(0,nuevaParticion.getLongitud()));
						tiraDeBits = tiraDeBits.substr(nuevaParticion.getLongitud());
					}
				}
			}
			if (tiraDeBits.size() > 0) {
				Particion particion(imagen.getID(),mensaje.getID(),numeroDeParticion,
					imagen.getProximo_bit_libre(),tiraDeBits.size(),false);
				particionDao.insert(particion);

				imagenDao.updateProxBitLibre(imagen.getID(),stego->setMessage(particion.getBit_inicio(),tiraDeBits));
			}

			imagenDao.updateEspacioLibre(imagen.getID(),imagen.getEspacio_libre()-streamsize);

			streamsize = tamanioMensaje - streamsize;
			numeroDeParticion++;
			delete buffer;
		}
	}
	else {
		throw EspacioInsuficienteException();
	}

	remove(TMP_COMPRESSED_FILE_NAME.c_str());

}

void MensajeManager::quitarMensajesEnDirectorio(std::string dirpath) {
	unsigned int dirId = trieDao.getIndice(DIRECTORIOS,dirpath);

	if (dirId != 0) {
		Directorio* directory = directorioManager.getDirectorioDao().getDirById(dirId);

		list<Imagen> imagenes = imagenDao.getImgsByDirectorio(directory->getID());

		for(list<Imagen>::iterator it = imagenes.begin(); it != imagenes.end(); it++) {
			list<Particion> particiones = particionDao.getPartsByImg((*it).getID());
			for (list<Particion>::iterator it2 = particiones.begin(); it2 != particiones.end(); it2++) {
				Particion& particion = *it2;
				const Mensaje& mensaje = mensajeDao.getMsjById(particion.getID_Txt());
				trieDao.deleteCadena(MENSAJES,mensaje.getNombre());
				mensajeDao.borrar(mensaje);
			}
		}
		delete directory;

	}
	else
		throw EntidadInexistenteException();
}

void MensajeManager::quitarMensaje(std::string& filename)
{
	int mensajeId = trieDao.getIndice(MENSAJES,filename);
	if (mensajeId == 0)
		throw EntidadInexistenteException();
	else {
		Mensaje mensaje = mensajeDao.getMsjById(mensajeId);
		list<Particion> particiones = particionDao.getPartsByTxt(mensaje.getID());

		if (particiones.size() > 0) {
			particiones.sort(compararParticionesPorIdImagen);
			Imagen imagen = imagenDao.getImgById((*particiones.begin()).getID_Img());
			for (list<Particion>::iterator it = particiones.begin(); it != particiones.end(); it++) {
				Particion& particion = *it;
				if (particion.getID_Img() != imagen.getID())
					imagen = imagenDao.getImgById(particion.getID_Img());
				imagenDao.updateEspacioLibre(imagen.getID(),imagen.getEspacio_libre()+particion.getLongitud());
				Particion particionNueva(imagen.getID(),0,0,particion.getBit_inicio(),particion.getLongitud(),true);
				particionDao.borrar(particion);
				particionDao.insert(particionNueva);
			}
		}
		mensajeDao.borrar(mensaje);
		trieDao.deleteCadena(MENSAJES,mensaje.getNombre());
	}
}



void MensajeManager::obtenerMensaje(std::string filename, std::string destino)
{
	int mensajeId = trieDao.getIndice(MENSAJES,filename);

	if (mensajeId == 0)
		throw EntidadInexistenteException();
	else {
		FILE* salida = fopen(destino.c_str(),"w+b");

		if (salida == NULL) {
			throw RecursoInaccesibleException();
		}

		fstream fromImage(TMP_COMPRESSED_FILE_NAME.c_str(), fstream::binary | fstream::out);

		Mensaje mensaje = mensajeDao.getMsjById(mensajeId);

		list<Particion> particiones = particionDao.getPartsByTxt(mensaje.getID());

		//Ordeno las particiones por posicion.
		particiones.sort();

		for (list<Particion>::iterator it = particiones.begin(); it != particiones.end(); it++) {
			Particion& particion = *it;
			const Imagen& imagen = imagenDao.getImgById(particion.getID_Img());
			StegoBusiness* stego = StegoFactory::newInstance(imagen.getNombre());
			const string& chunk = stego->getMessage(particion.getBit_inicio(),particion.getLongitud()*8);

			string encodedMessage("");
			const char* buffer = chunk.c_str();

			for (unsigned int i = 0; i < chunk.size(); i+=8) {
				unsigned char byte = 0x0;
				for (int j = 0; j < 8; j++) {
					if (memcmp(buffer+i+j,"1",1) == 0)
						byte = byte | (1<<j);
				}
				encodedMessage.push_back(byte);
			}

			fromImage.write(encodedMessage.c_str(),encodedMessage.size());
		}
		fromImage.close();

		FILE* tmp_file = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"rb");

		compressor.decompress(tmp_file,salida);
		fclose(tmp_file);
		remove(TMP_COMPRESSED_FILE_NAME.c_str());
		fclose(salida);
	}
}

list<string> MensajeManager::getMensajes()
{
	//TODO::GET ALL MENSAJES EN EL DAO
//	list<Mensaje> mensajes = mensajeDao.
//	directorios.sort(compararDirectoriosPorPath);
//	list<string> nombres;
//	nombres.resize(directorios.size());
//	transform(directorios.begin(),directorios.end(),nombres.begin(),transformDirectorioToString);
//	return nombres;
}


MensajeManager::~MensajeManager() {
	// TODO Auto-generated destructor stub
}
}
