#pragma once
#include <glm/gtc/matrix_transform.hpp>

//promena pro ulozeni a praci s daty s vyskove mapy
unsigned char * data;
//sirka a delka heightmapy
int terrainx = 300;
int terrainy = 300;

class Terrain
{
private:
	int w; //sirka terenu
	int l; //delka terenu
	float** hs; //Vyska terenu = dvoudimenzionalni array hs[l][w]
	glm::fvec3** normaly;
	bool computedNormals; //Jestli jsou normaly aktualni
public:
	//konstruktor, alokuje pamet
	Terrain(int w2, int l2)
	{
		w = w2;
		l = l2;

		hs = new float*[l];
		for (int i = 0; i < l; i++) {
			hs[i] = new float[w];
		}

		normaly = new glm::vec3*[l];
		for (int i = 0; i < l; i++) {
			normaly[i] = new glm::vec3[w];
		}

		computedNormals = false;
	}

	//destruktor, uvolnuje pamet
	~Terrain()
	{
		for (int i = 0; i < l; i++) {
			delete[] hs[i];
		}
		delete[] hs;

		for (int i = 0; i < l; i++) {
			delete[] normaly[i];
		}
		delete[] normaly;
	}

	//pom. fce, vraci sirku
	int width()
	{
		return w;
	}

	//pom. fce, vraci delku
	int length()
	{
		return l;
	}

	//nastavi vysku v (z, x) na y
	void setHeight(int x, int z, float y)
	{
		hs[z][x] = y;
		computedNormals = false;
	}

	//Vraci vysku v (z, x)
	float getHeight(int x, int z)
	{
		return hs[z][x];
	}

	//Jestli nejsou normaly aktualni, spocita je
	void computeNormals() {
		if (computedNormals) {
			return;
		}
		glm::fvec3** normaly2 = new glm::fvec3*[l];
		for (int i = 0; i < l; i++) {
			normaly2[i] = new glm::fvec3[w];
		}
		//pocitani normal
		for (int z = 0; z < l; z++) {
			for (int x = 0; x < w; x++) { //pro kazdy bod terenu udelej...
				glm::fvec3 suma(0, 0, 0);
				float scale = 5.0 / 60.0;
				if (((z + 1) < l) && ((x + 1) < w))
					suma += (glm::cross(glm::fvec3(0.0f, hs[z + 1][x] - hs[z][x], scale), glm::fvec3(scale, hs[z][x + 1] - hs[z][x], 0.0f)));
				if (((x + 1)<w) && (z>0))
					suma += (glm::cross(glm::fvec3(scale, hs[z][x + 1] - hs[z][x], 0.0f), glm::fvec3(scale, hs[z - 1][x + 1] - hs[z][x], -scale)));
				if ((z>0) && ((x + 1) < w))
					suma += (glm::cross(glm::fvec3(scale, hs[z - 1][x + 1] - hs[z][x], -scale), glm::fvec3(0.0f, hs[z - 1][x] - hs[z][x], -scale)));
				if ((z > 0) && (x > 0))
					suma += (glm::cross(glm::fvec3(0.0f, hs[z - 1][x] - hs[z][x], -scale), glm::fvec3(-scale, hs[z][x - 1] - hs[z][x], 0.0f)));
				if (((z + 1) < l) && (x > 0))
					suma += (glm::cross(glm::fvec3(-scale, hs[z][x - 1] - hs[z][x], 0.0f), glm::fvec3(-scale, hs[z + 1][x - 1] - hs[z][x], scale)));
				if (((z + 1) < l) && (x > 0))
					suma += (glm::cross(glm::fvec3(-scale, hs[z + 1][x - 1] - hs[z][x], scale), glm::fvec3(0.0f, hs[z + 1][x] - hs[z][x], scale)));
				normaly[z][x] = glm::normalize(suma);
			}
		}
		//uvolnuji alokovanou pamet
		for (int i = 0; i < l; i++) {
			delete[] normaly2[i];
		}
		delete[] normaly2;
		computedNormals = true;
	}

	//vrati normalu na pozici (x,z)
	glm::fvec3 getNormaly(int x, int z) {
		if (!computedNormals) {
			computeNormals();
		}
		return normaly[z][x];
	}
};

//funkce pro nacteni .bmp obrazku (pro vyskovou mapu a textury)
unsigned char* loadTexture(const char * filename, int sirka_obr, int delka_obr)
{
	unsigned char * datavar;
	unsigned char * dataX;
	FILE * file;
	file = fopen(filename, "rb");
	if (file == NULL)
	{
		perror("Soubor se nepodarilo nacist");
		return 0;
	}
	datavar = (unsigned char *)malloc((sirka_obr * delka_obr * 3) + 54);
	fread(datavar, (sirka_obr * delka_obr * 3) + 54, 1, file);
	fclose(file);
	//.bmp ma prvnich 54 bitu hlavicku. Tu potrebuji zahodit
	dataX = (unsigned char *)malloc((sirka_obr * delka_obr * 3));
	for (int f = 54; f < ((sirka_obr*delka_obr * 3) + 54); f++)
	{
		dataX[f - 54] = datavar[f];
	}
	//.bmp uklada veci ve formatu BGR, je tedy treba to otocit na RGB
	for (int i = 0; i < (sirka_obr * delka_obr); ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = dataX[index];
		R = dataX[index + 2];

		dataX[index] = R;
		dataX[index + 2] = B;

	}
	
	return dataX;
}

//funkce pro nacteni terenu do datove struktury
Terrain* loadTerrain(const char* filename, float height) {
	data = loadTexture(filename, terrainx, terrainy);
	Terrain* t = new Terrain(terrainx, terrainy);
	for (int y = 0; y < terrainy; y++) {
		for (int x = 0; x < terrainx; x++) {
			unsigned char color = data[(x + y * terrainx) * 3];	//to nasobeni trema tam je jen proto, abych vzal z kazdeho pixelu jen jednu barevnou slozku. Grayscale ma vzdy vsechny polozky uplne stejne (R=G=B) tzn. staci mi ta jedna
			float h = height * ((color / 255.0f) - 0.5f); //color je 0-255, tzn. vysledek zavorky bude v rozmezi -0.5 az 0.5 tzn. pokud bude height 20, pak vysledne vysky budou v rozmezi -10 az 10
			t->setHeight(x, y, h); //v hs[y][x] je ted hodnota h. Tzn. kazdy bod terenu ma ted prirazenou vysku
		}
	}
	t->computeNormals(); //vypocita normaly		
	return t;
}