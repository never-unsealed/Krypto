/*
 * FortunaGenerator.cpp
 */

#include <cassert>
#include <iostream>
#include <fstream>
#include "filters.h"
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "hex.h"
#include "files.h"
#include "FortunaGenerator.h"

using namespace std;

FortunaGenerator::FortunaGenerator()
{
	for (int i = 0; i < sizeof(key); i++)
	{
		key[i] = 0;
	}

	for (int i = 0; i < sizeof(cntr); i++)
	{
		cntr[i] = 0;
	}

	seeded = false;
	last_reseed = 0;
}

/**
 * @brief Erhöht den internen Zähler.
 *
 * Erhöht den internen Zähler um 1. Falls dies zu einem Überlauf führen sollte (d.h. alle 128 Bits des Zählers bereits auf 1),
 * gibt die Funktion false zurück.
 *
 * @return true, falls es durch die Inkrementierung zu keinem Überlauf kam, ansonsten false.
 */
bool FortunaGenerator::incCntr()
{
	/*************************************************************************
	 * Aufgabe 2a.
	 *************************************************************************/
	bool ret_val = true;

	byte big_endian[16];
	Integer cntr_int(cntr, 16, Integer::UNSIGNED, LITTLE_ENDIAN_ORDER);				  // CNTR-Array als Integer initialisieren (Little Endian)
	Integer u128_max("340282366920938463463374607431768211455", LITTLE_ENDIAN_ORDER); // Maximalen Wert, der in 16 Byte gespeichert werden kann, als Integer initialisieren

	if (cntr_int == u128_max) // Ist der aktuelle CNTR Wert bereits der Maximalwert?
	{
		ret_val = false; // Wenn Überlauf, setze ret_val auf false
		cntr_int = 0;
	}
	else
	{
		cntr_int++; // CNTR um 1 erhöhen.
	}

	cntr_int.Encode(big_endian, 16); // Enkodiere cntr_int wieder in ein byte-Array

	for (int i = 0; i < 16; i++)
		cntr[i] = big_endian[15 - i]; // Zu Little Endian umwandeln

	return ret_val;
}

/**
 * @brief Führt einen Reseed aus.
 *
 * Führt die Reseed Operation durch, indem ein Puffer angelegt wird und anschließend key und seed in diesen kopiert werden.
 * Über diesen Puffer wird nun eine SHA256 Prüfsumme berechnet. Cntr wird inkrementiert.
 *
 * @param Der Seed als Byte Array.
 * @param Die Größe des Seed Arrays in Bytes.
 */
void FortunaGenerator::reseed(byte *seed, unsigned int size)
{
	/*************************************************************************
	 * Aufgabe 2b.
	 *************************************************************************/

	byte *combined = new byte[size + 32]; // Neues Array dynamisch anlegen

	memcpy(combined, key, 32);		   // Kopiere den Key in den neuen Puffer
	memcpy(combined + 32, seed, size); // Kopiere den Seed nach dem Key in den Puffer

	CryptoPP::SHA256().CalculateDigest(key, combined, size + 32); // Berechne Prüfsumme über key || seed

	delete[] combined; // Combined aus Hauptspeicher löschen

	incCntr(); // Erhöhe cntr um 1

	seeded = true;
	last_reseed = getTimeStamp();
}

/**
 * @brief Generiert ein Pseudozufallsbit.
 *
 * Generiert ein Pseudozufallsbit, indem es die getByte Methode aufruft und
 * das erste Bit in dem generierten Byte zurückgibt.
 *
 * @return 1 oder 0.
 */
bool FortunaGenerator::getBit()
{
	/*************************************************************************
	 * Aufgabe 2e.
	 *************************************************************************/
	bool b = 0;

	byte random_byte = getByte(); // Generiere Zufallsbyte

	return (bool)(random_byte & 1); // Gebe entweder 1 oder 0 zurück
}

/**
 * @brief Generiert ein Pseudozufallsbyte.
 *
 * Generiert ein Pseudozufallsbyte, indem es die getBlocks Methode aufruft und
 * das erste Byte in dem generierten Block zurückgibt.
 *
 * @return Ein Pseudozufallsbyte.
 */
byte FortunaGenerator::getByte()
{
	/*************************************************************************
	 * Aufgabe 2e.
	 *************************************************************************/
	byte b = 0;
	byte block[16]; // Temporärer Block

	generateBlocks(block, 1); // Generiere einen Zufallsblock

	b = block[0]; // Extrahiere das erste Byte

	return b;
}

/**
 * @brief Generiert eine beliebige Anzahl an Pseudozufalls-Blöcken.
 *
 * Generiert eine beliebige Anzahl an Pseudozufalls-Blöcken, indem es den
 * internen Wert von CNTR für jeden Block mit AES im ECB Modus verschlüsselt und den
 * internen Schlüssel als AES Key verwendet.
 *
 * @param buffer Der Puffer, der mit Pseudozufalls-Blöcken gefüllt werden soll.
 * @param n Die Größe des Puffers in Einheiten von 16-Byte Blöcken.
 */
void FortunaGenerator::generateBlocks(byte *buffer, unsigned int n)
{
	/*************************************************************************
	 * Aufgabe 2cg.
	 *************************************************************************/

	if ((getTimeStamp() - last_reseed) > 500 || seeded == false) // Reseed alle 500ms oder wenn seeded false ist
	{
		byte seed[32];

		FILE *fd;
		fd = fopen("/dev/urandom", "r"); // Öffnen der Datei /dev/urandom im Lesemodus

		if (!fd)
		{
			cout << "fopen fehlgeschlagen" << endl;
			return;
		}

		fread(seed, 1, 32, fd); // Lese 32 Bytes
		fclose(fd);				// Schließe die Datei

		reseed(seed, 32); // Führe reseed mit den Zufallsbytes durch
	}

	ECB_Mode<AES>::Encryption aes_enc; // Erstellen des AES-Encryption Objekts im ECB Modus

	if (cntr == 0)
		std::abort(); // Ungültiger Zustand

	aes_enc.SetKey(key, sizeof(key)); // Setzen des Schluessels fuer die Operation

	for (int i = 0; i < n; i++) // Iteriere für jeden Block
	{
		ArraySource(cntr, 16, true,														 // Lese den CNTR-Array ein
					new StreamTransformationFilter(aes_enc,								 // Führe AES Verschlüsselung durch
												   new ArraySink(&buffer[i * 16], 16))); // Speichere den Ciphertext an Position i * 16 im output buffer

		incCntr(); // Erhöhe den CNTR
	}
}

uint32_t FortunaGenerator::getTimeStamp() const
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/**
 * @brief Generiert eine beliebige Anzahl an Pseudozufallsbytes.
 *
 * Generiert eine beliebige Anzahl an Pseudozufallsbytes, indem es
 * die generateBlocks Methode aufruft und die gewünschte Anzahl an Bytes
 * (auf das nächste Vielfache von 16 aufgerundet) abfragt. Die zurück gegebenen
 * Bytes werden in buffer kopiert. Ein neuer interner Schlüssel wird generiert.
 *
 * @param buffer Der Puffer, der mit Pseudozufallsbytes gefüllt werden soll.
 * @param n Die Größe des Puffers in Bytes.
 */
void FortunaGenerator::getBlock(byte *buffer, unsigned int n)
{
	/*************************************************************************
	 * Aufgabe 2d.
	 *************************************************************************/

	if (n > pow(2, 20))
		std::abort(); // N zu hoch

	unsigned int len = n + (16 - (n % 16)); // Berechne nächstes Vielfaches von 16 mit n
	byte *tmp_buf = new byte[len];			// Lege temporären Puffer im Heap an

	generateBlocks(tmp_buf, len / 16); // Generiere Blöcke

	memcpy(buffer, tmp_buf, n); // Kopiere n Byte von tmp_buf zum output Puffer

	delete[] tmp_buf; // Lösche den Heap Puffer

	generateBlocks(key, 2); // Generiere neuen Schlüssel
}

void FortunaGenerator::getBlockV0(byte *buffer, unsigned int n)
{

	unsigned int nb = n / 16 + 1;
	byte *ibuffer = new byte[16 * nb];
	byte new_key[32];

	generateBlocks(ibuffer, nb);
	generateBlocks(new_key, 2);

	memcpy(buffer, ibuffer, n);
	memcpy(key, new_key, 32);

	delete[] ibuffer;
}
