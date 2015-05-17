#include "./headers/textures.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
using namespace std;
GLuint RectTexture;
GLuint TriTexture;
GLuint BackTexture;
GLuint CircTexture;
string names[100];
int counter=1;
int maximum=1;
static GLuint LoadTexture(char* filename)
{
    GLuint texture = SOIL_load_OGL_texture(
				filename,
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_POWER_OF_TWO
				| SOIL_FLAG_MIPMAPS
				| SOIL_FLAG_MULTIPLY_ALPHA
				| SOIL_FLAG_COMPRESS_TO_DXT
				| SOIL_FLAG_DDS_LOAD_DIRECT
				| SOIL_FLAG_INVERT_Y
				);

    if (texture == 0)
         cout<<"Texture Load Error: " + string(filename);

		glEnable(GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}
void listFiles(const char* path )
{
   DIR* dirFile = opendir( path );
   if ( dirFile ) 
   {
      struct dirent* hFile;
      while (( hFile = readdir( dirFile )) != NULL ) 
      {
         if ( !strcmp( hFile->d_name, "."  )) continue;
         if ( !strcmp( hFile->d_name, ".." )) continue;

         // dirFile.name is the name of the file. Do whatever string comparison 
         // you want here. Something like:
         if ( strstr( hFile->d_name, "" )){
            names[counter++]=hFile->d_name;
        }
      } 
      closedir( dirFile );
   }
}
void LoadAllTextures(){
  RectTexture = LoadTexture((char*)"../img/rectangle.png");
  TriTexture = LoadTexture((char *)"../img/rectangle.png");
  BackTexture = LoadTexture((char *)"../img/background.bmp");
  CircTexture = LoadTexture((char *)"../img/rectangle.png");
}
void maxFinder(){
	int max=0;
	for(int i=0;i<counter;i++){
		int a = atoi(names[i].c_str());
		if(a>max)
			max=a;
	}
	maximum=max+1;
}
void screenshot(){
	listFiles("../screenshots/");
	maxFinder();
	char name[10];
	sprintf(name,"../screenshots/%d",maximum);
	int save_result = SOIL_save_screenshot
	(
		name,
		SOIL_SAVE_TYPE_BMP,
		0, 0, 1360, 768
	);
}