#include "BlowfishCrypto.h"
#include <string.h>

BlowfishCrypto::BlowfishCrypto() {



}

BlowfishCrypto::~BlowfishCrypto() {
	// TODO Auto-generated destructor stub
}


void BlowfishCrypto::inicializar(string password){
	//Inicializar el vector p con los digitos de de PI
	for (int i = 0; i < 18; i++){
		this->pSubKeys[i] = DIGITS_PI_P[i];
	}
	//Inicializar las 4 cajas - s
	for (int j= 0; j < 4; j++){
		for (int i = 0; i < 256; i++){
			this->sBoxes[j][i] = DIGITS_PI_S[j][i];
		}
	}
	unsigned int len = password.length();
	DWord subkey;

	//Realizar un XOR entre el vector P y la clave.
	for (int i = 0; i < 18; i++){
		for (int k = 0; k < 4; k++){
			subkey.byte[k] = password[(i * 4 + k) % len];
		}
		this->pSubKeys[i].doble ^= subkey.doble;
	}
	//Encriptar un estring con todos 0 y setearlo en el array p
	QWord data(0,0);
	for (int i = 0 ; i < 18 ; i += 2)
	{
		data = encrypt(data);
		this->pSubKeys[i] = data.xl;
		this->pSubKeys[i + 1] = data.xr;
	}
	//El resultado, encriptarlo y ponerlo en las sBoxes
	for (int i = 0 ; i < 4 ; ++i)
	{
		for (int j = 0 ; j < 256 ; j += 2)
		{
			data = encrypt(data);
			this->sBoxes[i][j] = data.xl ;
			this->sBoxes[i][j+1] = data.xr ;
		}
	}

}





string BlowfishCrypto::encrypt(char* data, unsigned int size){
	string resultado;
	resultado.reserve(size);
	char* subData;
	int subDataSize = 0;
	for (unsigned int i = 0; i < size; i += 8){
		if (i + 8 >= size){
			subDataSize = size-i;
			subData = new char[subDataSize];
			memcpy(subData,data+i,subDataSize);
		}else{
			subDataSize = 8;
			subData = new char[subDataSize];
			memcpy(subData,data+i,subDataSize);
		}
		QWord dataQWord(subData,subDataSize);
		dataQWord = encrypt(dataQWord);
		resultado += qWordToString(dataQWord);
	}
	return resultado;
}

string BlowfishCrypto::qWordToString(QWord qword){
	string res = "        ";
	for (int i = 0; i < 4; i++){
		res[i] = qword.xl.byte[i];
	}
	for (int i = 4; i < 8; i++){
		res[i] = qword.xr.byte[i-4];
	}
	return res;
}



string BlowfishCrypto::desencrypt(string data){
	string resultado;
	resultado.reserve(data.length());
	unsigned int len = data.length();
	string subData;

	for (unsigned int i = 0; i < len; i += 8){
		if (i + 8 >= data.length()){
			subData = data.substr(i,len - i);
		}else{
			subData = data.substr(i,8);
		}
		QWord dataQWord(subData);
		dataQWord = desencrypt(dataQWord);
		resultado += qWordToString(dataQWord);
	}
	return resultado;

}

QWord BlowfishCrypto::encrypt(QWord data){

	for (int i = 0; i < 16; ++i){
		data.xl.doble = data.xl.doble ^ this->pSubKeys[i].doble;
		data.xr.doble = F(data.xl) ^ data.xr.doble;
		swap(data.xl,data.xr);
	}
	swap(data.xl,data.xr);
	data.xl.doble = data.xl.doble ^ this->pSubKeys[17].doble;
	data.xr.doble =  data.xr.doble ^ this->pSubKeys[16].doble;

	return data;
}

QWord BlowfishCrypto::desencrypt(QWord data){
	for (int i = 17; i > 1; --i){
		data.xl.doble = data.xl.doble ^ this->pSubKeys[i].doble;
		data.xr.doble = F(data.xl) ^ data.xr.doble;
		swap(data.xl,data.xr);
	}
	swap(data.xl,data.xr);
	data.xl.doble = data.xl.doble ^ this->pSubKeys[0].doble;
	data.xr.doble =  data.xr.doble ^ this->pSubKeys[1].doble;

	return data;

}

void BlowfishCrypto::swap(DWord &left, DWord &right){
	DWord aux;
	aux = left;
	left = right;
	right = aux;
}
