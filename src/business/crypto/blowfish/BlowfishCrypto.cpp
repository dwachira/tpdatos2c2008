#include "BlowfishCrypto.h"

BlowfishCrypto::BlowfishCrypto() {
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


}

BlowfishCrypto::~BlowfishCrypto() {
	// TODO Auto-generated destructor stub
}


void BlowfishCrypto::inicializar(string password){
	unsigned int len = password.length();
	string subkey = 0;

	//Realizar un XOR entre el vector P y la clave.
	for (int i = 0; i < 18; i++){
		for (int k = 0; k < 4; k++){
			subkey[k] = password[(i * 4 + k) % len];
		}
		this->pSubKeys[i].dword ^= ((Doble)subkey.c_str()).dword;
	}
	QWord data(0,0);



	for (int i = 0 ; i < 18 ; i += 2)
	{
		data = encrypt(data);
		this->pSubKeys[i] = data.xl;
		this->pSubKeys[i + 1] = data.xr;
	}

	for (int i = 0 ; i < 4 ; ++i)
	{
		for (int j = 0 ; j < 256 ; j += 2)
		{
			data = encrypt(data);
			this->sBoxes[j][i] = data.xl ;
			this->sBoxes[j][i+1] = data.xr ;
		}
	}

}

QWord BlowfishCrypto::encrypt(QWord data){
	Doble xl = data.xl;
	Doble xr = data.xr;

	xl.dword ^= this->pSubKeys[0].dword;
	for (int i = 0; i < 16; ++i){
		xl.dword = xl.dword ^ this->pSubKeys[i].dword;
		xr.dword = F(xl) ^ xr.dword;
		swap(xl,xr);
	}
	swap(xl,xr);
	 data.xl.dword = xl.dword ^ this->pSubKeys[18].dword;
	data.xr.dword =  xr.dword ^ this->pSubKeys[17].dword;

	return data;
}

void BlowfishCrypto::swap(Doble left, Doble right){
	Doble aux;
	aux = left;
	left = right;
	right = aux;
}

string BlowfishCrypto::desencrypt(string password, string data){
return "";
}
