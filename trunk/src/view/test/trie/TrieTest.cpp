/*
 * TrieTest.cpp
 *
 *  Created on: 29/10/2008
 *      Author: maxi
 */

#include "TrieTest.h"

TrieTest::TrieTest() {
	// TODO Auto-generated constructor stub

}

TrieTest::~TrieTest() {
	// TODO Auto-generated destructor stub
}

int TrieTest::test(){
	Trie trie;
	trie.insertCadena(" espaciado",1);
	trie.insertCadena("  espaciadomasivo",2);
	trie.insertCadena("minguito",3);
	trie.insertCadena("minimo",4);
	trie.insertCadena("milhojas",10);
	std::cout << "Cadena 'minimo' - 4: " << trie.getIndice("minimo") << std::endl;
	trie.deleteCadena("minimo");
	std::cout << "Cadena 'minimo' - 0: " << trie.getIndice("minimo") << std::endl;
	trie.insertCadena("boca 1 - river 0",8);
	trie.insertCadena("coco se fue",18);
	trie.insertCadena("choco",110);
	trie.insertCadena("y Miguel donde esta?",11);
	trie.insertCadena("/home/maxi/FreeImage3110/FreeImage/",31);
	trie.insertCadena("/home/maxi/FreeImage3110",51);
	trie.insertCadena("/home/maxi/FreeImage3110/Freezeee/",551);
	std::cout << "Cadena 'choco'- 110: " << trie.getIndice("choco") << std::endl;
	std::cout << "Cadena 'minim'- 0: " << trie.getIndice("minim") << std::endl;
	std::cout << "Cadena 'coco'- 0: " << trie.getIndice("coco") << std::endl;
	std::cout << "Cadena '/home/maxi/FreeImage3110/Freezeee/' - 551: " << trie.getIndice("/home/maxi/FreeImage3110/Freezeee/") << std::endl;
	std::cout << "Cadena 'y Miguel donde esta?' - 11: " << trie.getIndice("y Miguel donde esta?") << std::endl;
	trie.deleteCadena("y Miguel donde esta?");
	std::cout << "Cadena 'y Miguel donde esta?' - 0: " << trie.getIndice("y Miguel donde esta?") << std::endl;
	return 0;
}
