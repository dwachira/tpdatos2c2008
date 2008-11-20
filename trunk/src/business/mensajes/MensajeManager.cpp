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
#include "../../object/exceptions/ImagenFaltanteException.h"
#include "../../object/exceptions/EntidadYaExistenteException.h"
#include "../crypto/blowfish/BlowfishCrypto.h"
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
	if (trieDao.getIndice(MENSAJES,filename) != 0) {
		throw EntidadYaExistenteException();
	}

	/** Comprimo el mensaje **/
	FILE* file = fopen(filename.c_str(),"rb");
	if (file == NULL) {
		throw RecursoInaccesibleException();
	}
	FILE* tmpfile = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"wb");

	CompressorBusiness compressor;
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
			util::Date* fechaModificacionActual = directorioManager.getFechaModificacionActual(directorio);
			if (fechaModificacionActual != NULL) {
				util::Date fechaModificacionAlmacenada = directorio.getFechaUltimaModificacion();
				if (fechaModificacionAlmacenada < *fechaModificacionActual) {

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
						//Ahora que recorrimos todo el directorio, actualizamos la fecha de modificacion.
						directorioManager.actualizarFechaDeModificacion(directorio);
					}
					if (espacioDisponible >= tamanioMensaje)
						break;
				}
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
			memset(buffer,'\0',streamsize);
			filestrm.read(buffer,streamsize);

			//Si la imagen es Jpeg, encriptamos el msg para mas seguridad.
			if (imagen.isJpg()) {
				BlowfishCrypto crypto;
				crypto.inicializar(userPass);
				string encrypted = crypto.encrypt(buffer,streamsize);
				delete buffer;
				streamsize = encrypted.size();
				buffer = new char[streamsize];
				memset(buffer,'\0',streamsize);
				memcpy(buffer,encrypted.data(),streamsize);
			}

			int bytesLeft = streamsize;

			list<Particion> particionesEnImagen = particionDao.getPartsByImg(imagen.getID());

			if (particionesEnImagen.size() > 0) {
				for(list<Particion>::iterator iterPart = particionesEnImagen.begin(); iterPart != particionesEnImagen.end(); iterPart++) {
					Particion& particion = *iterPart;
					if (particion.isLibre()) {
						int newPartitionSize = (streamsize<particion.getLongitud()) ? streamsize : particion.getLongitud();
						Particion nuevaParticion(imagen.getID(),mensaje.getID(),numeroDeParticion++,
								particion.getBit_inicio(),newPartitionSize,false);
						particionDao.borrar(particion);
						particionDao.insert(nuevaParticion);
						StegoBusiness* stego = StegoFactory::newInstance((*it2).getNombre());
						stego->setMessage(nuevaParticion.getBit_inicio(),buffer,nuevaParticion.getLongitud());
						bytesLeft -= nuevaParticion.getLongitud();
						delete stego;
					}
				}
			}
			if (bytesLeft > 0) {
				Particion particion(imagen.getID(),mensaje.getID(),numeroDeParticion++,
					imagen.getProximo_bit_libre(),bytesLeft,false);
				particionDao.insert(particion);
				StegoBusiness* stego = StegoFactory::newInstance((*it2).getNombre());
				imagenDao.updateProxBitLibre(imagen.getID(),stego->setMessage(particion.getBit_inicio(),buffer+streamsize-bytesLeft,bytesLeft));
				delete stego;
			}

			imagenDao.updateEspacioLibre(imagen.getID(),imagen.getEspacio_libre()-streamsize);
			imagenDao.updateHashValue(imagen.getID(),hasheador.getHashFromFile(imagen.getNombre()));
			imagenDao.updateFecha(imagen.getID(),Date());
			streamsize = tamanioMensaje - streamsize;
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
			StegoBusiness* stego = NULL;
			if (EntradaSalidaManager::recursoEsAccesible(imagen.getNombre()))
				stego = StegoFactory::newInstance(imagen.getNombre());
			else {
				Imagen* imagenBuscada = directorioManager.buscarImagenMovida(imagen);
				if (imagenBuscada != NULL) {
					stego = StegoFactory::newInstance(imagenBuscada->getNombre());
					imagenDao.updateNombre(imagen.getID(),imagenBuscada->getNombre());
					imagenDao.updateDirectorio(imagen.getID(),imagenBuscada->getID());
					trieDao.deleteCadena(IMAGENES,imagen.getNombre());
					trieDao.insertCadena(IMAGENES,imagenBuscada->getNombre(),imagen.getID());
				}
			}

			if (stego == NULL) {
				throw ImagenFaltanteException();
			}

			string chunk = stego->getMessage(particion.getBit_inicio(),particion.getLongitud());

			//Si es jpg, hay que desencriptar.
			if (imagen.isJpg()) {
				BlowfishCrypto crypto;
				crypto.inicializar(userPass);
				chunk = crypto.desencrypt(chunk);
			}

			fromImage.write(chunk.c_str(),chunk.size());
			delete stego;
		}
		fromImage.close();

		FILE* tmp_file = fopen(TMP_COMPRESSED_FILE_NAME.c_str(),"rb");

		CompressorBusiness compressor;
		compressor.decompress(tmp_file,salida);
		fclose(tmp_file);
		remove(TMP_COMPRESSED_FILE_NAME.c_str());
		fclose(salida);
	}
}

list<string> MensajeManager::getMensajes()
{
	list<Mensaje> mensajes = mensajeDao.getAllMsjs();
	list<string> nombres;
	nombres.resize(mensajes.size());
	transform(mensajes.begin(),mensajes.end(),nombres.begin(),transformMensajeToString);
	return nombres;
}


MensajeManager::~MensajeManager() {
	// TODO Auto-generated destructor stub
}
}
