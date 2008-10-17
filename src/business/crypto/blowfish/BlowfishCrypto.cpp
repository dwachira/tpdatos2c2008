#include "BlowfishCrypto.h"

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
	string subkey = "";

	//Realizar un XOR entre el vector P y la clave.
	for (int i = 0; i < 18; i++){
		for (int k = 0; k < 4; k++){
			subkey[k] = password[(i * 4 + k) % len];
		}
		this->pSubKeys[i].doble ^= ((DWord)subkey.c_str()).doble;
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

QWord BlowfishCrypto::encrypt(QWord data){
	DWord xl = data.xl;
	DWord xr = data.xr;

	xl.doble ^= this->pSubKeys[0].doble;
	for (int i = 0; i < 16; ++i){
		xl.doble = xl.doble ^ this->pSubKeys[i].doble;
		xr.doble = F(xl) ^ xr.doble;
		swap(xl,xr);
	}
	swap(xl,xr);
	data.xl.doble = xl.doble ^ this->pSubKeys[18].doble;
	data.xr.doble =  xr.doble ^ this->pSubKeys[17].doble;

	return data;
}

void BlowfishCrypto::swap(DWord left, DWord right){
	DWord aux;
	aux = left;
	left = right;
	right = aux;
}

string BlowfishCrypto::encrypt(string data){
	string resultado;
	unsigned int len = data.length();
	string subData;
	for (unsigned int i = 1; i < len; i += 8){
		if (i + 8 >= data.length()){
			subData = data.substr(i,len - i);
		}else{
			subData = data.substr(i,8);
		}
		QWord dataQWord(subData.c_str());
		dataQWord = encrypt(dataQWord);
		resultado += string(dataQWord.xl.toString);

	}
	return resultado;
}

string BlowfishCrypto::desencrypt(string password, string data){
return "";
}
