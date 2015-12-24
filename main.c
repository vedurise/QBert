#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

#include <math.h>
using namespace std;
//#include <MMSystem.h>
#include <windows.h>
#include "freeimage.h"
//#define COMPATIBILITY
//#define MODERN
//#ifdef MODERN
// include GLEW
#include "glew/glew.h"
// include GLFW
#include "glfw/glfw3.h"
GLFWwindow* window;

// include GLM
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

enum directions
{
	LEFT, RIGHT, TOP, BOTTOM
};
enum screens
{
	startScreen, gameScreen, gameOverScreen
};
screens currScreen;
bool showLeftDisc = true;
bool showRightDisc = true;
bool isTransition = false;
bool isTransitionMusic = false;
int transitionInd = 1;
char *timeText = new char[50];
char *scoreText = new char[10];
char *livesText = new char[20];
char *levelText = new char[20];
char *welcomeText = "Welcome to QBert Game!";
char *gameOverText = "Game Done, Press Enter to restart!";
char *pressEnter = "Press Enter to Start :)";
bool startsScreenPlaySound = false;
bool endScreenPlaySound = false;
const int BlkLevel = 7;
double frameRateToShow = 0.0, currentTime = 0.0, frameRate = 0, frameRateTime = 0, enemyRate = 0.8, myCharVisit = 0.0;
const float moveX = 4.25;
const float moveY = 5.25;
const float moveZ = 4.9;
const float qBertX = 0.0;
const float qBertY = 5.0;
const float qBertZ = 2.0;
int tileCount = 0;
int lives = 3;
int level = 1;
int totalLevels = 3;
bool upToggle;
bool enterToggle;
bool downToggle;
bool rightToggle;
bool leftToggle;
int charLoc = 1;
int globalTexNum;
unsigned int bitArr_Draw = 0;
unsigned int visitArr = 1;
int score = 0;
const int scoreInc = 25;
int currLevel = 1;
char *temp = new char[150];
char *mtlFileName = new char[100];
char stupidBuffer[1000];
int currTexture = -1;
char *PrevName = new char[100];
bool isCube;
GLuint MatrixID;
GLuint currTextureId;
GLuint eyeLocId;
GLuint lightLocId;
GLuint my_text_Id;
GLuint my_text_VertexBuff_Id;
GLuint my_text_UVBuff_Id;
GLuint currElement_F_Id;
GLuint currElement_V_Id;
float currElement;
GLuint Ka_Id;
GLuint Kd_Id;
GLuint Ks_Id;
GLuint Ns_Id;
GLuint La_Id;
GLuint Ld_Id;
GLuint Ls_Id;
vector<glm::vec3> gridChangeVals_Top;
vector<glm::vec3> gridChangeVals_Right;
vector<glm::vec3> gridChangeVals_Movement;
glm::vec3 eyeLoc;
glm::vec3 eyeLoc_Original;
glm::vec3 lookAtLoc;
glm::vec3 upVec;
glm::vec3 lightLoc;
glm::vec3 La;
glm::vec3 Ld;
glm::vec3 Ls;
glm::mat4 MVP;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 ScaleMtx;
glm::mat4 RotateMtx;
glm::mat4 TransMtx;
glm::vec3 rotAxis;
float rotAngle;
float scaleVal;
float transX;
float transY;
float transZ;
char prevKey;
GLuint showTexId;
float showTex;
std::vector< glm::vec3 > AllVertices;
std::vector<glm::vec3> AllVns;
std::vector<glm::vec2> AllVts;
const int maxTextures = 16;
const int maxLevels = 3;
float x = -4;
float prev_x;
float y = 0;
float k = 1;

char * path;

struct  myPoint
{
	public: float x;
	public: float y;
	public: float z;
		//add colors later.
};

struct mtl_block
{
	char newmtlName[50];
	float Ns;
	struct myPoint Ka;
	struct myPoint Ks;
	struct myPoint Kd;
	char map_Kd[50];
};

struct eachMtlBlk
{
	GLuint vertexBuffer;
	GLuint UVBuffer;
	GLuint vertexNormals;
	GLuint my_texture_Id;
	std::vector<glm::vec3> FinalVerts;
	std::vector<glm::vec2> FinalVts;
	std::vector<glm::vec3> FinalVns;
	struct mtl_block myMtlBlock;
};

vector<eachMtlBlk> eachMtlBlk_List(maxTextures);
std::vector<glm::vec3> tempVect;
struct ShaderInfo 
{
	GLuint programId;
	GLuint vertexArray;

};
struct myTrans
{
	glm::mat4 Model;
	glm::mat4 Model_Res;
	glm::mat4 ScaleMtx;
	glm::mat4 RotateMtx;
	glm::mat4 TransMtx;
	glm::mat4 MVP;
	glm::vec3 rotAxis;
	float rotAngle;
	float scaleVal;
	float transX;
	float transY;
	float transZ;

};
struct myNode
{
	int id;
	GLfloat min_x;
	GLfloat min_y;
	GLfloat min_z;
	GLfloat max_x;
	GLfloat max_y;
	GLfloat max_z;
	glm::vec3 size;
	glm::vec3 center;
	glm::mat4 transform;
	int traverseId;
	char objFile[100];
	struct myTrans modelTrans;
	int numChildren;
	vector<eachMtlBlk> eachMtlBlk_List;
	struct myNode *bottom;
	struct myNode *right;
	struct myNode *top;
	struct myNode *left;
};
enum EnemyType
{
	RED, BLUE, SNAKE, YELLOW, OTHERS
};
struct myEnemyNode
{
	struct myNode myEnemy;
	struct myNode *myEnemyIndex;
	int startTimer;
	double frameTimer;
	bool isAlive;
	EnemyType enemyType;
};
struct myPoint gridLocIndices[BlkLevel * (BlkLevel + 1) / 2 + 2];
struct myNode myRootNode;
struct myNode myDiscs[2];
struct myNode myCharacter;
struct myEnemyNode redEnemy, redEnemy1, blueEnemy, snakeEnemy, redEnemy2;
void loadAnObj(char *path);
void loadRec(char *path, struct myNode &tempNode, int tempBlkLevel);
void initParamsofNode(struct myNode &tempNode, float scalVal, float rotAng, float transx, float transy, float transz);
void loadEachObj(char *path, struct myNode& tempNode);
struct myNode *load_NodeArr[BlkLevel * (BlkLevel + 1)/ 2 + 2];
bool drawArrBool[BlkLevel * (BlkLevel + 1) / 2 + 2];
GLuint textureSampler[maxTextures];
/**
* Load and runtime compile GLSL shaders.
*/
GLuint LoadShaders( const char* vertex_file_path, const char* fragment_file_path ){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


/**
* Program initialization.
*/
void init(ShaderInfo& info)
{
	int gridInd_temp = 1;
	for (int q = 0; q < BlkLevel; q++)
	{
		for (int w = 0; w <= q; w++)
		{
			gridLocIndices[gridInd_temp].x = q + 1;
			gridLocIndices[gridInd_temp++].y = w + 1;
		}
	}

	gridChangeVals_Top.reserve(3);
	gridChangeVals_Top.push_back(glm::vec3(0.0, 0.0, 0.8));
	gridChangeVals_Top.push_back(glm::vec3(0.0, 0.8, 0.0));
	gridChangeVals_Top.push_back(glm::vec3(0.8, 0.0, 0.0));

	gridChangeVals_Right.reserve(3);
	gridChangeVals_Right.push_back(glm::vec3(0.1, 0.1, 0.35));
	gridChangeVals_Right.push_back(glm::vec3(0.1, 0.35, 0.1));
	gridChangeVals_Right.push_back(glm::vec3(0.35, 0.1, 0.1));

	gridChangeVals_Movement.reserve(3);
	gridChangeVals_Movement.push_back(glm::vec3(0.0, 0.8, 0.8));
	gridChangeVals_Movement.push_back(glm::vec3(0.8, 0.8, 0.0));
	gridChangeVals_Movement.push_back(glm::vec3(0.8, 0.0, 0.8));

	upToggle = downToggle = rightToggle = leftToggle = enterToggle = true;
	for (int o = 0; o < BlkLevel * (BlkLevel + 1) / 2 + 2; o++)
	{
		drawArrBool[o] = false;
		load_NodeArr[o] = NULL;
	}
	prevKey = 0;
	cout << "In Init";
	x = -4;
	glGenVertexArrays(1, &info.vertexArray);
	glBindVertexArray(info.vertexArray);

	// create and compile our GLSL program from the shaders
	info.programId = LoadShaders("shaders/vs.glsl", "shaders/fs.glsl");


	MatrixID = glGetUniformLocation(info.programId, "MVP");
	currTextureId = glGetUniformLocation(info.programId, "currTex");
	eyeLocId = glGetUniformLocation(info.programId, "eyeLoc");
	lightLocId = glGetUniformLocation(info.programId, "lightLoc");
	Ks_Id = glGetUniformLocation(info.programId, "Ks");
	Ka_Id = glGetUniformLocation(info.programId, "Ka");
	Kd_Id = glGetUniformLocation(info.programId, "Kd");
	Ls_Id = glGetUniformLocation(info.programId, "Ls");
	Ld_Id = glGetUniformLocation(info.programId, "Ld");
	La_Id = glGetUniformLocation(info.programId, "La");
	Ns_Id = glGetUniformLocation(info.programId, "Ns");
	currElement_F_Id = glGetUniformLocation(info.programId, "currElement_F");
	currElement_V_Id = glGetUniformLocation(info.programId, "currElement_V");
	showTexId = glGetUniformLocation(info.programId, "showTex");

	La = glm::vec3(1.0, 1.0, 1.0);
	Ld = glm::vec3(0.5, 0.5, 0.5);
	Ls = glm::vec3(0.5, 0.5, 0.5);

	showTex = 0.5;
	textureSampler[0] = glGetUniformLocation(info.programId, "myTextureSampler0");
	textureSampler[1] = glGetUniformLocation(info.programId, "myTextureSampler1");
	textureSampler[2] = glGetUniformLocation(info.programId, "myTextureSampler2");
	textureSampler[3] = glGetUniformLocation(info.programId, "myTextureSampler3");
	textureSampler[4] = glGetUniformLocation(info.programId, "myTextureSampler4");
	textureSampler[5] = glGetUniformLocation(info.programId, "myTextureSampler5");
	textureSampler[6] = glGetUniformLocation(info.programId, "myTextureSampler6");
	textureSampler[7] = glGetUniformLocation(info.programId, "myTextureSampler7");
	textureSampler[8] = glGetUniformLocation(info.programId, "myTextureSampler8");
	textureSampler[9] = glGetUniformLocation(info.programId, "myTextureSampler9");
	textureSampler[10] = glGetUniformLocation(info.programId, "myTextureSampler10");
	textureSampler[11] = glGetUniformLocation(info.programId, "myTextureSampler11");
	textureSampler[12] = glGetUniformLocation(info.programId, "myTextureSampler12");
	textureSampler[13] = glGetUniformLocation(info.programId, "myTextureSampler13");
	textureSampler[14] = glGetUniformLocation(info.programId, "myTextureSampler14");
	textureSampler[15] = glGetUniformLocation(info.programId, "myTextureSampler15");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(10.0f, 3.0f / 3.0f, 0.1f, 5000.0f);
	eyeLoc = glm::vec3(0.0, 300.0, 300.0);
	eyeLoc_Original = glm::vec3(0.0, 300.0, 300.0);
	lightLoc = glm::vec3(0.0, 0.0, 30.0);
	lookAtLoc = glm::vec3(0, -30.0, 0);
	upVec = glm::vec3(0, 1, 0);
	// Camera matrix 
	View = glm::lookAt(
		eyeLoc,
		lookAtLoc,
		upVec
		);

	//generate buffers for text

	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType("Holstein.DDS", 0), "Holstein.DDS");
	FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &my_text_Id);
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, my_text_Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned int width = FreeImage_GetWidth(pImage);
	unsigned int height = FreeImage_GetHeight(pImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(bitmap);

	glGenBuffers(1, &my_text_VertexBuff_Id);
	glGenBuffers(1, &my_text_UVBuff_Id);

	// end
	//start: load character
	initParamsofNode(myCharacter, 6.0, -45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit.obj";
	isCube = false;
	loadEachObj(path, myCharacter);
	//end

	//load discs
	initParamsofNode(myDiscs[0], 2.0, -45.0, -22.0, -15.0, 19.6);
	path = "disc.obj";
	isCube = true;
	loadEachObj(path, myDiscs[0]);

	initParamsofNode(myDiscs[1], 2.0, -45.0, 22.0, -15.0, 19.6);
	loadEachObj(path, myDiscs[1]);
	//end

	initParamsofNode(myRootNode, 3.0, -45.0, 0.0, 0.0, 0.0);
	myRootNode.id = 1;
	path = "cube.obj";
	isCube = true;
	myRootNode.top = myRootNode.left = NULL;
	loadRec(path, myRootNode, 1);
	load_NodeArr[1] = &myRootNode;
	//loadAnObj(path);

	//load red enemy
	initParamsofNode(redEnemy.myEnemy, 6.0, -45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit_Red.obj";
	isCube = false;
	loadEachObj(path, redEnemy.myEnemy);
	redEnemy.myEnemyIndex = &myRootNode;
	redEnemy.startTimer = rand() % 5;
	redEnemy.isAlive = false;
	redEnemy.enemyType = RED;
	//end

	//load red enemy
	initParamsofNode(redEnemy1.myEnemy, 6.0, -45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit_Red.obj";
	isCube = false;
	loadEachObj(path, redEnemy1.myEnemy);
	redEnemy1.myEnemyIndex = &myRootNode;
	redEnemy1.startTimer = rand() % 5;
	redEnemy1.isAlive = false;
	redEnemy1.enemyType = RED;
	//end

	//load blue enemy
	initParamsofNode(blueEnemy.myEnemy, 6.0, -45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit_Blue.obj";
	isCube = false;
	loadEachObj(path, blueEnemy.myEnemy);
	blueEnemy.myEnemyIndex = &myRootNode;
	blueEnemy.startTimer = rand() % 5;
	blueEnemy.isAlive = false;
	blueEnemy.enemyType = BLUE;
	//end

	//load snake enemy
	initParamsofNode(snakeEnemy.myEnemy, 5.0, 45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit_Snake.obj";
	isCube = false;
	loadEachObj(path, snakeEnemy.myEnemy);
	snakeEnemy.myEnemyIndex = &myRootNode;
	snakeEnemy.startTimer = 200;
	snakeEnemy.isAlive = false;
	snakeEnemy.enemyType = SNAKE;
	//end

	//load sidewise enemy
	initParamsofNode(redEnemy2.myEnemy, 6.0, -45.0, qBertX, qBertY, qBertZ);
	path = "Rabbit_Yellow.obj";
	isCube = false;
	loadEachObj(path, redEnemy2.myEnemy);
	redEnemy2.myEnemyIndex = &myRootNode;
	redEnemy2.startTimer = rand() % 5;
	redEnemy2.isAlive = false;
	redEnemy2.enemyType = YELLOW;
	//end

	//set the cubes property based on level
	for (int we = 1; we <= BlkLevel * (BlkLevel + 1) / 2; we++)
	{
		load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Top[level - 1].x;
		load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Top[level - 1].y;
		load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Top[level - 1].z;

		load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.x = gridChangeVals_Right[level - 1].x;
		load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.y = gridChangeVals_Right[level - 1].y;
		load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.z = gridChangeVals_Right[level - 1].z;
	}
}
void recalcMVP_Loc(struct myNode *tempNode)
{
	//View = glm::lookAt(eyeLoc, lookAtLoc, upVec);
	tempNode->modelTrans.TransMtx = glm::translate(tempNode->modelTrans.transX, tempNode->modelTrans.transY, tempNode->modelTrans.transZ);
	tempNode->modelTrans.Model = tempNode->modelTrans.TransMtx * tempNode->modelTrans.RotateMtx * tempNode->modelTrans.ScaleMtx;
	tempNode->modelTrans.MVP = Projection * View * tempNode->modelTrans.Model;
}
void initParamsofNode(struct myNode &tempNode, float scalVal, float rotAng, float transx, float transy, float transz)
{
	tempNode.modelTrans.scaleVal = scalVal;
	tempNode.modelTrans.transX = transx;
	tempNode.modelTrans.transY = transy;
	tempNode.modelTrans.transZ = transz;
	tempNode.modelTrans.rotAxis = glm::vec3(0.0, 1.0, 0.0);
	tempNode.modelTrans.rotAngle = rotAng;
	tempNode.modelTrans.RotateMtx = glm::rotate(tempNode.modelTrans.rotAngle, tempNode.modelTrans.rotAxis);
	tempNode.modelTrans.ScaleMtx = glm::scale(vec3(tempNode.modelTrans.scaleVal));
	recalcMVP_Loc(&tempNode);
}
void loadRec(char *path, struct myNode &tempNode, int tempBlkLevel)
{
	loadEachObj(path, tempNode);
	if (tempBlkLevel < BlkLevel)
	{
		//create child nodes
		for (int i = 0; i < 2; i++)
		{
			if (load_NodeArr[tempNode.id + tempBlkLevel + i] == NULL)
			{
				struct myNode *tempChild = new myNode();
				tempChild->id = tempNode.id + tempBlkLevel + i;
				tempChild->modelTrans.rotAngle = -45;
				tempChild->modelTrans.scaleVal = 3.0f;
				tempChild->modelTrans.rotAxis = glm::vec3(0.0, 1.0, 0.0);
				tempChild->modelTrans.rotAngle = -45.0;
				if (i == 0)
				{
					tempChild->modelTrans.transX = tempNode.modelTrans.transX - moveX;
					tempChild->modelTrans.transY = tempNode.modelTrans.transY - moveY;
					tempChild->modelTrans.transZ = tempNode.modelTrans.transZ + moveZ;
					tempNode.bottom = tempChild;
					tempChild->top = &tempNode;
					tempChild->left = NULL;
				}
				else
				{
					tempChild->modelTrans.transX = tempNode.modelTrans.transX + moveX;
					tempChild->modelTrans.transY = tempNode.modelTrans.transY - moveY;
					tempChild->modelTrans.transZ = tempNode.modelTrans.transZ + moveZ;
					tempNode.right = tempChild;
					tempChild->left = &tempNode;
					tempChild->top = NULL;
				}
				tempChild->modelTrans.RotateMtx = glm::rotate(tempChild->modelTrans.rotAngle, tempChild->modelTrans.rotAxis);
				tempChild->modelTrans.ScaleMtx = glm::scale(vec3(tempChild->modelTrans.scaleVal));
				recalcMVP_Loc(tempChild);
				loadRec(path, *tempChild, tempBlkLevel + 1);
				load_NodeArr[tempNode.id + tempBlkLevel + i] = tempChild;
			}
			else
			{
				struct myNode *trex = load_NodeArr[tempNode.id + tempBlkLevel + i];
				if (i == 0)
				{
					trex->top = &tempNode;
					tempNode.bottom = trex;
				}
				else
				{
					trex->left = &tempNode;
					tempNode.right = trex;
				}
			}
		}
	}
	else
	{
		//null inits
		tempNode.bottom = NULL;
		tempNode.right = NULL;
	}
}
void loadEachObj(char *path, struct myNode& tempNode)
{
	//vector<eachMtlBlk> eachMtlBlk_List(maxTextures);
	tempNode.eachMtlBlk_List.resize(maxTextures);
	tempNode.max_x = tempNode.max_y = tempNode.max_z = -1000000.0;
	tempNode.min_x = tempNode.min_y = tempNode.min_z = 1000000.0;
	FILE * file = std::fopen(path, "r");
	if (file == NULL)
	{
		printf("Unable to open the file !\n");
		std::exit(1);
	}
	int currTexture = -1;
	char *PrevName = new char[100];
	while (fscanf(file, "%s", temp) != EOF)
	{
		//read line 
		if (strcmp(temp, "usemtl") == 0)
		{
			fscanf(file, "%s\n", temp);
			if (strlen(PrevName) == 0 || strcmp(PrevName, temp) != 0)
				currTexture++;
			strcpy(PrevName, temp);
			strcpy(tempNode.eachMtlBlk_List[currTexture].myMtlBlock.newmtlName, temp);
		}
		else if (strcmp(temp, "mtllib") == 0)
		{
			fscanf(file, "%s\n", mtlFileName);
		}
		else if (strcmp(temp, "v") == 0)
		{
			//create new GLfloat
			glm::vec3 vertex;
			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			if (tempNode.max_x < vertex.x) tempNode.max_x = vertex.x;
			if (tempNode.max_y < vertex.y) tempNode.max_y = vertex.y;
			if (tempNode.max_z < vertex.z) tempNode.max_z = vertex.z;
			if (tempNode.min_x > vertex.x) tempNode.min_x = vertex.x;
			if (tempNode.min_y > vertex.y) tempNode.min_y = vertex.y;
			if (tempNode.min_z > vertex.z) tempNode.min_z = vertex.z;
			AllVertices.push_back(vertex);
		}
		else if (strcmp(temp, "vt") == 0)
		{
			//create new GLfloat
			glm::vec2 vt;
			fscanf(file, "%f %f", &vt.x, &vt.y);
			AllVts.push_back(vt);
		}
		else if (strcmp(temp, "vn") == 0)
		{
			//create new GLfloat
			glm::vec3 vn;
			fscanf(file, "%f %f %f", &vn.x, &vn.y, &vn.z);
			AllVns.push_back(vn);
		}
		else if (strcmp(temp, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = 0;
			if (!isCube)
				matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			else
				matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
			if (matches == 9)
			{
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[0] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[1] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[2] - 1]);

				tempNode.eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[0] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[1] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[2] - 1]);

				tempNode.eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[0] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[1] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[2] - 1]);
			}
			else if (matches == 3)
			{
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[0] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[1] - 1]);
				tempNode.eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[2] - 1]);
			}
			else
			{
				printf("File can't be read by our simple parser :-(\n");
				std::exit(1);
			}
		}
		else
		{
			fgets(stupidBuffer, 1000, file);
		}
	}
	AllVertices.clear();
	AllVns.clear();
	AllVts.clear();
	tempNode.eachMtlBlk_List.resize(currTexture + 1);
	tempNode.size = glm::vec3(tempNode.max_x - tempNode.min_x, tempNode.max_y - tempNode.min_y, tempNode.max_z - tempNode.min_z);
	tempNode.center = glm::vec3((tempNode.min_x + tempNode.max_x) / 2, (tempNode.min_y + tempNode.max_y) / 2, (tempNode.min_z + tempNode.max_z) / 2);
	tempNode.transform = glm::translate(glm::mat4(1), tempNode.center)* glm::scale(glm::mat4(1), tempNode.size);

	for (int i = 0; i < tempNode.eachMtlBlk_List.size(); i++)
	{
		glGenBuffers(1, &tempNode.eachMtlBlk_List[i].vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].FinalVerts.size() * sizeof(glm::vec3), &tempNode.eachMtlBlk_List[i].FinalVerts[0], GL_STATIC_DRAW);

		if (!isCube)
		{
			glGenBuffers(1, &tempNode.eachMtlBlk_List[i].UVBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].UVBuffer);
			glBufferData(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].FinalVts.size() * sizeof(glm::vec2), &tempNode.eachMtlBlk_List[i].FinalVts[0], GL_STATIC_DRAW);

			glGenBuffers(1, &tempNode.eachMtlBlk_List[i].vertexNormals);
			glBindBuffer(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].vertexNormals);
			glBufferData(GL_ARRAY_BUFFER, tempNode.eachMtlBlk_List[i].FinalVns.size() * sizeof(glm::vec3), &tempNode.eachMtlBlk_List[i].FinalVns[0], GL_STATIC_DRAW);
		}
	}
	//load mtl file and then, load texture file
	FILE *file_mtl = fopen(mtlFileName, "r");
	if (file_mtl == NULL)
	{
		cout << mtlFileName << ":Unable to open the file,. Exiting";
		exit;
	}

	while (fscanf(file_mtl, "%s", temp) != EOF) //only the first string till space is read.
	{
		if (strcmp(temp, "newmtl") == 0)
		{
			fscanf(file_mtl, " %s\n", temp);
			for (int j = 0; j < tempNode.eachMtlBlk_List.size(); j++)
			{
				if (strcmp(temp, tempNode.eachMtlBlk_List[j].myMtlBlock.newmtlName) == 0)
				{
					fscanf(file_mtl, "%s %f", temp, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ns);
					fscanf(file_mtl, "%s %f %f %f", temp, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ka.x, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ka.y, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ka.z);
					fscanf(file_mtl, "%s %f %f %f", temp, &tempNode.eachMtlBlk_List[j].myMtlBlock.Kd.x, &tempNode.eachMtlBlk_List[j].myMtlBlock.Kd.y, &tempNode.eachMtlBlk_List[j].myMtlBlock.Kd.z);
					fscanf(file_mtl, "%s %f %f %f", temp, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ks.x, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ks.y, &tempNode.eachMtlBlk_List[j].myMtlBlock.Ks.z);
					fscanf(file_mtl, "%s %s", temp, &tempNode.eachMtlBlk_List[j].myMtlBlock.map_Kd);
					break;
				}
			}
		}
		else
		{
			fgets(stupidBuffer, 1000, file_mtl);
		}
	}

	for (int i = 0; i < tempNode.eachMtlBlk_List.size(); i++)
	{
		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(tempNode.eachMtlBlk_List[i].myMtlBlock.map_Kd, 0), tempNode.eachMtlBlk_List[i].myMtlBlock.map_Kd);
		FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &tempNode.eachMtlBlk_List[i].my_texture_Id);
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, tempNode.eachMtlBlk_List[i].my_texture_Id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned int width = FreeImage_GetWidth(pImage);
		unsigned int height = FreeImage_GetHeight(pImage);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(bitmap);
	}
}
//void loadAnObj(char *path)
//{
//	FILE * file = std::fopen(path, "r");
//	if (file == NULL)
//	{
//		printf("Unable to open the file !\n");
//		std::exit;
//	}
//	while (fscanf(file, "%s", temp) != EOF)
//	{
//		//read line 
//		if (strcmp(temp, "usemtl") == 0)
//		{
//			fscanf(file, "%s\n", temp);
//			if (strlen(PrevName) == 0 || strcmp(PrevName, temp) != 0)
//				currTexture++;
//			strcpy(PrevName, temp);
//			strcpy(eachMtlBlk_List[currTexture].myMtlBlock.newmtlName, temp);
//		}
//		else if (strcmp(temp, "mtllib") == 0)
//		{
//			fscanf(file, "%s\n", mtlFileName);
//		}
//		else if (strcmp(temp, "v") == 0)
//		{
//			//create new GLfloat
//			glm::vec3 vertex;
//			fscanf(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
//			AllVertices.push_back(vertex);
//		}
//		else if (strcmp(temp, "vt") == 0)
//		{
//			//create new GLfloat
//			glm::vec2 vt;
//			fscanf(file, "%f %f", &vt.x, &vt.y);
//			AllVts.push_back(vt);
//		}
//		else if (strcmp(temp, "vn") == 0)
//		{
//			//create new GLfloat
//			glm::vec3 vn;
//			fscanf(file, "%f %f %f", &vn.x, &vn.y, &vn.z);
//			AllVns.push_back(vn);
//		}
//		else if (strcmp(temp, "f") == 0)
//		{
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = 0;
//			if (!isCube)
//				matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//			else
//				matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
//			if (matches == 9)
//			{
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[0] - 1]);
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[1] - 1]);
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[2] - 1]);
//
//				eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[0] - 1]);
//				eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[1] - 1]);
//				eachMtlBlk_List[currTexture].FinalVts.push_back(AllVts[uvIndex[2] - 1]);
//
//				eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[0] - 1]);
//				eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[1] - 1]);
//				eachMtlBlk_List[currTexture].FinalVns.push_back(AllVns[normalIndex[2] - 1]);
//			}
//			else if (matches == 3)
//			{
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[0] - 1]);
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[1] - 1]);
//				eachMtlBlk_List[currTexture].FinalVerts.push_back(AllVertices[vertexIndex[2] - 1]);
//			}
//			else
//			{
//				printf("File can't be read by our simple parser :-(\n");
//				std::exit(1);
//			}
//		}
//		else
//		{
//			fgets(stupidBuffer, 1000, file);
//		}
//	}
//	eachMtlBlk_List.resize(currTexture + 1);
//	for (int i = 0; i < eachMtlBlk_List.size(); i++)
//	{
//		glGenBuffers(1, &eachMtlBlk_List[i].vertexBuffer);
//		glBindBuffer(GL_ARRAY_BUFFER, eachMtlBlk_List[i].vertexBuffer);
//		glBufferData(GL_ARRAY_BUFFER, eachMtlBlk_List[i].FinalVerts.size() * sizeof(glm::vec3), &eachMtlBlk_List[i].FinalVerts[0], GL_STATIC_DRAW);
//
//		if (!isCube)
//		{
//			glGenBuffers(1, &eachMtlBlk_List[i].UVBuffer);
//			glBindBuffer(GL_ARRAY_BUFFER, eachMtlBlk_List[i].UVBuffer);
//			glBufferData(GL_ARRAY_BUFFER, eachMtlBlk_List[i].FinalVts.size() * sizeof(glm::vec2), &eachMtlBlk_List[i].FinalVts[0], GL_STATIC_DRAW);
//
//			glGenBuffers(1, &eachMtlBlk_List[i].vertexNormals);
//			glBindBuffer(GL_ARRAY_BUFFER, eachMtlBlk_List[i].vertexNormals);
//			glBufferData(GL_ARRAY_BUFFER, eachMtlBlk_List[i].FinalVns.size() * sizeof(glm::vec3), &eachMtlBlk_List[i].FinalVns[0], GL_STATIC_DRAW);
//		}
//	}
//	//load mtl file and then, load texture file
//	FILE *file_mtl = fopen(mtlFileName, "r");
//	if (file_mtl == NULL)
//	{
//		cout << mtlFileName << ":Unable to open the file,. Exiting";
//		exit;
//	}
//
//	while (fscanf(file_mtl, "%s", temp) != EOF) //only the first string till space is read.
//	{
//		if (strcmp(temp, "newmtl") == 0)
//		{
//			fscanf(file_mtl, " %s\n", temp);
//			for (int j = 0; j < eachMtlBlk_List.size(); j++)
//			{
//				if (strcmp(temp, eachMtlBlk_List[j].myMtlBlock.newmtlName) == 0)
//				{
//					fscanf(file_mtl, "%s %f", temp, &eachMtlBlk_List[j].myMtlBlock.Ns);
//					fscanf(file_mtl, "%s %f %f %f", temp, &eachMtlBlk_List[j].myMtlBlock.Ka.x, &eachMtlBlk_List[j].myMtlBlock.Ka.y, &eachMtlBlk_List[j].myMtlBlock.Ka.z);
//					fscanf(file_mtl, "%s %f %f %f", temp, &eachMtlBlk_List[j].myMtlBlock.Kd.x, &eachMtlBlk_List[j].myMtlBlock.Kd.y, &eachMtlBlk_List[j].myMtlBlock.Kd.z);
//					fscanf(file_mtl, "%s %f %f %f", temp, &eachMtlBlk_List[j].myMtlBlock.Ks.x, &eachMtlBlk_List[j].myMtlBlock.Ks.y, &eachMtlBlk_List[j].myMtlBlock.Ks.z);
//					fscanf(file_mtl, "%s %s", temp, &eachMtlBlk_List[j].myMtlBlock.map_Kd);
//					break;
//				}
//			}
//		}
//		else
//		{
//			fgets(stupidBuffer, 1000, file_mtl);
//		}
//	}
//
//	for (int i = 0; i < eachMtlBlk_List.size(); i++)
//	{
//		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(eachMtlBlk_List[i].myMtlBlock.map_Kd, 0), eachMtlBlk_List[i].myMtlBlock.map_Kd);
//		FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
//
//		glEnable(GL_TEXTURE_2D);
//		glGenTextures(1, &eachMtlBlk_List[i].my_texture_Id);
//		// Typical Texture Generation Using Data From The Bitmap
//		glBindTexture(GL_TEXTURE_2D, eachMtlBlk_List[i].my_texture_Id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		unsigned int width = FreeImage_GetWidth(pImage);
//		unsigned int height = FreeImage_GetHeight(pImage);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
//
//		FreeImage_Unload(bitmap);
//	}
//
//}
void checkKeyboard()
{
	//modify the vals here, based on keys.
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && prevKey != 'Z')
	{
		cout << "Scale Up" << endl;
		scaleVal += 0.01;
		prevKey = 'Z';
	}
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && prevKey != 'X')
	{
		cout << "Scale Down" << endl;
		scaleVal -= 0.01;
		//prevKey = 'X';
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		cout << "Y+" << endl;
		transY += 0.05;
		//prevKey = 'U';
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
	{
		cout << "Y-" << endl;
		transY -= 0.05;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		cout << "X+" << endl;
		transX += 0.05;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS )
	{
		cout << "X-" << endl;
		transX -= 0.05;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS )
	{
		cout << "Z+" << endl;
		transZ += 0.05;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS )
	{
		cout << "Z-" << endl;
		transZ -= 0.05;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cout << "Rot+" << endl;
		rotAngle ++;
		//prevKey = 'D';
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cout << "Rot-" << endl;
		rotAngle --;
		//prevKey = 'D';
	}
	else
	{
		prevKey = 0;
	}
	scaleVal = scaleVal < 0 ? 0 : scaleVal;
	TransMtx = glm::translate(transX, transY, transZ);
	RotateMtx = glm::rotate(rotAngle,rotAxis);
	ScaleMtx = glm::scale(vec3(scaleVal));
	Model = TransMtx * RotateMtx * ScaleMtx;
	MVP = Projection * View * Model;
}
/**
* Main 3D rendering function.
*/
void render3D(ShaderInfo& info, struct myNode &currNode, float currEl_V, float currEl_F)
{
	//checkKeyboard();
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &currNode.modelTrans.MVP[0][0]);
	//currElement = 0;
	glUniform1f(currElement_V_Id, currEl_V);
	glUniform1f(currElement_F_Id, currEl_F);
	//for each mesh/ a bmp, draw once with out clearing .
	for (int i = 0; i < currNode.eachMtlBlk_List.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + globalTexNum);
		glBindTexture(GL_TEXTURE_2D, currNode.eachMtlBlk_List[i].my_texture_Id);
		glUniform1i(textureSampler[globalTexNum], globalTexNum);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, currNode.eachMtlBlk_List[i].vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, currNode.eachMtlBlk_List[i].UVBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, currNode.eachMtlBlk_List[i].vertexNormals);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform1f(currTextureId, globalTexNum++);
		glUniform3f(eyeLocId,eyeLoc.r, eyeLoc.g, eyeLoc.b);
		glUniform3f(lightLocId, lightLoc.r, lightLoc.g, lightLoc.b);
		glUniform3f(Kd_Id, currNode.eachMtlBlk_List[i].myMtlBlock.Kd.x, currNode.eachMtlBlk_List[i].myMtlBlock.Kd.y, currNode.eachMtlBlk_List[i].myMtlBlock.Kd.z);
		glUniform3f(Ka_Id, currNode.eachMtlBlk_List[i].myMtlBlock.Ka.x, currNode.eachMtlBlk_List[i].myMtlBlock.Ka.y, currNode.eachMtlBlk_List[i].myMtlBlock.Ka.z);
		glUniform3f(Ks_Id, currNode.eachMtlBlk_List[i].myMtlBlock.Ks.x, currNode.eachMtlBlk_List[i].myMtlBlock.Ks.y, currNode.eachMtlBlk_List[i].myMtlBlock.Ks.z);
		glUniform3f(La_Id, La.r, La.g, La.b);
		glUniform3f(Ld_Id, Ld.r, Ld.g, Ld.b);
		glUniform3f(Ls_Id, Ls.r, Ls.g, Ls.b);
		glUniform1f(Ns_Id, currNode.eachMtlBlk_List[i].myMtlBlock.Ns);
		glUniform1f(showTexId, showTex);

		//glDrawElements(GL_TRIANGLES, eachMtlBlk_List[i].FinalVerts.size(), GL_UNSIGNED_SHORT, (void *)0);
		glDrawArrays(GL_TRIANGLES, 0, currNode.eachMtlBlk_List[i].FinalVerts.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

//create a circular view of the obj.
void updateView()
{
	Projection = glm::perspective(10.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	//x2 + y2 = r2 - circle equation.
	if (x > 249 || x < -249)
	{
		k = -k;
	}
	prev_x = x;
	x += k;
	y = std::sqrt(62500 - std::pow(x, 2));
	if (prev_x > x)
		y *= -1;
	cout << x << "-" << y << "\n";
	// Camera matrix
	View = glm::lookAt(
		glm::vec3(x, 4, y), // Camera is at (4,3,-3), in World Space
		glm::vec3(0, 4, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	//Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View *Model; // Remember, matrix multiplication is the other way around
}

void drawDFS(ShaderInfo& info, struct myNode& tempNode, int tempBlkLevel)
{
	render3D(info, tempNode, 0, 0);
	if (tempBlkLevel < BlkLevel)
	{
		for (int i = 0; i < 2; i++)
		{
			if (((bitArr_Draw >> (tempNode.id + tempBlkLevel + i)) & 1) == 0)
			//if (!drawArrBool[tempNode.id + tempBlkLevel + i])
			{
				bitArr_Draw |= (1 << (tempNode.id + tempBlkLevel + i));
				//drawArrBool[tempNode.id + tempBlkLevel + i] = true;
				//cout << tempNode.id << "-" << tempBlkLevel << "-" << i << endl;
				//if ((tempNode.id + tempBlkLevel + i) == 3)
				//	cout << "";
				////[debug] print the bits 
				//for (int p = 0; p < 63; p++)
				//	printf("%2d", ((bitArr_Draw >> p) & 1));
				//cout << endl;
				//for (int p = 0; p < 63; p++)
				//	printf("%2d", p);
				//cout << endl;
				drawDFS(info, *load_NodeArr[tempNode.id + tempBlkLevel + i], tempBlkLevel + 1);
			}
		}
	}
}
void drawFrameRateNTime(const char * text, int x, int y, int size, ShaderInfo& info)
{
	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i<length; i++){

		glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = 1.0 - (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y - 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y - 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, my_text_VertexBuff_Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, my_text_UVBuff_Id);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(info.programId);

	currElement = 1;
	glUniform1f(currElement_V_Id, currElement);
	currElement = 2;
	glUniform1f(currElement_F_Id, currElement);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, my_text_Id);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureSampler[0], 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, my_text_VertexBuff_Id);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, my_text_UVBuff_Id);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}
void updateCharLoc(myNode &tempChar, directions myDir)
{
	//change color of tile and update score
	if (((visitArr >> charLoc) & 1) == 0)
	{
		score += scoreInc;
		tileCount++;
		visitArr |= (1 << charLoc);
	}
	struct myNode *tempNode = load_NodeArr[charLoc];
	tempNode->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Movement[level - 1].x;
	tempNode->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Movement[level - 1].y;
	tempNode->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Movement[level - 1].z;
	if (myDir == TOP)
	{
		tempChar.modelTrans.transX += moveX;
		tempChar.modelTrans.transY += moveY;
		tempChar.modelTrans.transZ -= moveZ;
		eyeLoc.x += moveX;
		eyeLoc.y += moveY;
		eyeLoc.z -= moveZ;
	}
	else if (myDir == BOTTOM)
	{
		tempChar.modelTrans.transX -= moveX;
		tempChar.modelTrans.transY -= moveY;
		tempChar.modelTrans.transZ += moveZ;
		eyeLoc.x -= moveX;
		eyeLoc.y -= moveY;
		eyeLoc.z += moveZ;
	}
	else if (myDir == RIGHT)
	{
		tempChar.modelTrans.transX += moveX;
		tempChar.modelTrans.transY -= moveY;
		tempChar.modelTrans.transZ += moveZ;
		eyeLoc.x += moveX;
		eyeLoc.y -= moveY;
		eyeLoc.z += moveZ;
	}
	else if (myDir == LEFT)
	{
		tempChar.modelTrans.transX -= moveX;
		tempChar.modelTrans.transY += moveY;
		tempChar.modelTrans.transZ -= moveZ;
		eyeLoc.x -= moveX;
		eyeLoc.y += moveY;
		eyeLoc.z -= moveZ;
	}
	//eyeLoc.x * 30;
	//eyeLoc.y * 30;
	//eyeLoc.z * 30;
	recalcMVP_Loc(&tempChar);

}
void resetEnemy(struct myEnemyNode& tempNode)
{
	tempNode.isAlive = false;
	tempNode.myEnemy.id = 1;
	if (tempNode.enemyType == SNAKE)
	{
		tempNode.startTimer = 200;
	}
	else
	{
		tempNode.startTimer = rand() % 5;
	}
	tempNode.myEnemyIndex = &myRootNode;
	tempNode.myEnemy.modelTrans.transX = qBertX;
	tempNode.myEnemy.modelTrans.transY = qBertY;
	tempNode.myEnemy.modelTrans.transZ = qBertZ;
	recalcMVP_Loc(&tempNode.myEnemy);
}
void moveToTop()
{
	//reset position of char
	if (((visitArr >> charLoc) & 1) == 0)
	{
		score += scoreInc;
		tileCount++;
		visitArr |= (1 << charLoc);
	}
	load_NodeArr[1]->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Movement[level - 1].x;
	load_NodeArr[1]->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Movement[level - 1].y;
	load_NodeArr[1]->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Movement[level - 1].z;

	myCharacter.modelTrans.transX = qBertX;
	myCharacter.modelTrans.transY = qBertY;
	myCharacter.modelTrans.transZ = qBertZ; 
	myCharacter.id = 1;
	eyeLoc = eyeLoc_Original;
	recalcMVP_Loc(&myCharacter);
}
void moveQBert(myNode &myChar)
{
	//get cube of the current location
	struct myNode *tempNode = load_NodeArr[charLoc];
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && upToggle)
	{
		upToggle = false;
		if (charLoc == 15 && showRightDisc)
		{
			PlaySound(TEXT("sounds\\Ridethedisk.wav"), NULL, SND_ASYNC | SND_FILENAME);
			showRightDisc = false;
			charLoc = 1;
			moveToTop();
			//reset others if required
		}
		else if (tempNode->top != NULL)
		{
			PlaySound(TEXT("sounds\\jump.wav"), NULL, SND_ASYNC | SND_FILENAME);
			charLoc = tempNode->top->id;
			updateCharLoc(myChar, TOP);
		}
		else
		{
			//bham
			lives--;
			PlaySound(TEXT("sounds\\QbertOvertheedge.wav"), NULL, SND_ASYNC | SND_FILENAME);
			resetEnemy(redEnemy1);
			resetEnemy(redEnemy);
			resetEnemy(blueEnemy);
			resetEnemy(snakeEnemy);
			if (level > 1)
				resetEnemy(redEnemy2);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && downToggle)
	{
		downToggle = false;
		if (tempNode->bottom != NULL)
		{
			PlaySound(TEXT("sounds\\jump.wav"), NULL, SND_ASYNC | SND_FILENAME);
			charLoc = tempNode->bottom->id;
			updateCharLoc(myChar, BOTTOM);
		}
		else
		{
			//bham
			lives--;
			PlaySound(TEXT("sounds\\QbertOvertheedge.wav"), NULL, SND_ASYNC | SND_FILENAME);
			resetEnemy(redEnemy1);
			resetEnemy(redEnemy);
			resetEnemy(blueEnemy);
			resetEnemy(snakeEnemy);
			if (level > 1)
				resetEnemy(redEnemy2);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && rightToggle)
	{
		rightToggle = false;
		if (tempNode->right != NULL)
		{
			PlaySound(TEXT("sounds\\jump.wav"), NULL, SND_ASYNC | SND_FILENAME);
			charLoc = tempNode->right->id;
			updateCharLoc(myChar, RIGHT);
		}
		else
		{
			//bham
			lives--;
			PlaySound(TEXT("sounds\\QbertOvertheedge.wav"), NULL, SND_ASYNC | SND_FILENAME);
			resetEnemy(redEnemy1);
			resetEnemy(redEnemy);
			resetEnemy(blueEnemy);
			resetEnemy(snakeEnemy);
			if (level > 1)
				resetEnemy(redEnemy2);
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && leftToggle)
	{
		leftToggle = false;
		if (charLoc == 11 && showLeftDisc)
		{
			PlaySound(TEXT("sounds\\Ridethedisk.wav"), NULL, SND_ASYNC | SND_FILENAME);
			//reset position of char
			showLeftDisc = false;
			charLoc = 1;
			moveToTop();
			//reset others if required
		}
		else if (tempNode->left != NULL)
		{
			PlaySound(TEXT("sounds\\jump.wav"), NULL, SND_ASYNC | SND_FILENAME);
			charLoc = tempNode->left->id;
			updateCharLoc(myChar, LEFT);
		}
		else
		{
			//bham
			lives--;
			PlaySound(TEXT("sounds\\QbertOvertheedge.wav"), NULL, SND_ASYNC | SND_FILENAME);
			resetEnemy(redEnemy1);
			resetEnemy(redEnemy);
			resetEnemy(blueEnemy);
			resetEnemy(snakeEnemy);
			if (level > 1)
				resetEnemy(redEnemy2);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		leftToggle = true;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		rightToggle = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		upToggle = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		downToggle = true;
}
void updateSnakeEnemy(struct myEnemyNode& tempNode)
{
	PlaySound(TEXT("sounds\\Ahop.wav"), NULL, SND_ASYNC | SND_FILENAME);

	//look for myCharacter and find shortest path to it.
	int min = 100, pos_min= 100, neg_max = -100, tempDiff;
	directions resDir;
	myNode *posNode = NULL, *negNode = NULL, *resNod = NULL;
	if (gridLocIndices[tempNode.myEnemy.id].x >= gridLocIndices[charLoc].x)
	{
		//left or top
		if (gridLocIndices[tempNode.myEnemy.id].y > gridLocIndices[charLoc].y)
		{
			//left
			resDir = LEFT;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->left;
		}
		else
		{
			//top
			resDir = TOP;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->top;
		}
	}
	else
	{
		if (gridLocIndices[tempNode.myEnemy.id].y >= gridLocIndices[charLoc].y)
		{
			//bottom
			resDir = BOTTOM;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->bottom;
		}
		else
		{
			//right
			resDir = RIGHT;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->right;
		}
	}
	tempNode.myEnemy.id = tempNode.myEnemyIndex->id;
	//use pos
	if (resDir == TOP)
	{
		tempNode.myEnemy.modelTrans.transX += moveX;
		tempNode.myEnemy.modelTrans.transY += moveY;
		tempNode.myEnemy.modelTrans.transZ -= moveZ;
	}
	else if (resDir == BOTTOM)
	{
		tempNode.myEnemy.modelTrans.transX -= moveX;
		tempNode.myEnemy.modelTrans.transY -= moveY;
		tempNode.myEnemy.modelTrans.transZ += moveZ;
	}
	else if (resDir == RIGHT)
	{
		tempNode.myEnemy.modelTrans.transX += moveX;
		tempNode.myEnemy.modelTrans.transY -= moveY;
		tempNode.myEnemy.modelTrans.transZ += moveZ;
	}
	else if (resDir == LEFT)
	{
		tempNode.myEnemy.modelTrans.transX -= moveX;
		tempNode.myEnemy.modelTrans.transY += moveY;
		tempNode.myEnemy.modelTrans.transZ -= moveZ;
	}
	//tempNode.myEnemyIndex = resNod;
	//tempNode.prevId = tempNode.myEnemy.id;
	//tempNode.myEnemy.id = resNod->id;
	recalcMVP_Loc(&tempNode.myEnemy);
}
void updateEnemy(struct myEnemyNode& tempNode)
{
	//falls down only; check bottom and right
	if (tempNode.myEnemyIndex->bottom != NULL && tempNode.myEnemyIndex->right != NULL)
	{
		//choose from bottom or right
		if (rand() % 2 == 0)
		{
			tempNode.myEnemy.modelTrans.transX -= moveX;
			tempNode.myEnemy.modelTrans.transY -= moveY;
			tempNode.myEnemy.modelTrans.transZ += moveZ;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->bottom;
			tempNode.myEnemy.id = tempNode.myEnemyIndex->id;
		}
		else 
		{
			tempNode.myEnemy.modelTrans.transX += moveX;
			tempNode.myEnemy.modelTrans.transY -= moveY;
			tempNode.myEnemy.modelTrans.transZ += moveZ;
			tempNode.myEnemyIndex = tempNode.myEnemyIndex->right;
			tempNode.myEnemy.id = tempNode.myEnemyIndex->id;
		}
		recalcMVP_Loc(&tempNode.myEnemy);
	}
	else
	{
		//set start time and make dead.
		if (tempNode.enemyType == BLUE)
		{
			//make snake out of it
			snakeEnemy.myEnemy.modelTrans.transX = tempNode.myEnemy.modelTrans.transX;
			snakeEnemy.myEnemy.modelTrans.transY = tempNode.myEnemy.modelTrans.transY;
			snakeEnemy.myEnemy.modelTrans.transZ = tempNode.myEnemy.modelTrans.transZ;
			snakeEnemy.myEnemy.id = tempNode.myEnemy.id;
			snakeEnemy.myEnemyIndex = tempNode.myEnemyIndex;
			tempNode.startTimer = 2000;
			snakeEnemy.isAlive = true;
			snakeEnemy.startTimer = -1;
			recalcMVP_Loc(&snakeEnemy.myEnemy);
		}
		else
			resetEnemy(tempNode);
	}
}
void showStartScreen(ShaderInfo& info)
{
	if (!startsScreenPlaySound)
	{
		PlaySound(TEXT("sounds\\Gamemusic.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
		startsScreenPlaySound = true;
		endScreenPlaySound = false;
	}
	drawFrameRateNTime(welcomeText, 75, 370, 30, info);
	drawFrameRateNTime(pressEnter, 100, 220, 22, info);
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && enterToggle)
	{
		currScreen = gameScreen;
		enterToggle = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterToggle = true;
}
void showGameScreen(ShaderInfo& info)
{
	if (snakeEnemy.isAlive)
		blueEnemy.startTimer = 2000;
	if (currentTime - redEnemy2.frameTimer >= 1.0 && level > 1)
	{
		if (redEnemy2.startTimer >= 0)
		{
			redEnemy2.isAlive = false;
			redEnemy2.startTimer--;
		}
		else
		{
			redEnemy2.isAlive = true;
			//update the location
			updateEnemy(redEnemy2);
		}
		redEnemy2.frameTimer = currentTime;
	}
	if (currentTime - redEnemy1.frameTimer >= 1.0)
	{
		if (redEnemy1.startTimer >= 0)
		{
			redEnemy1.isAlive = false;
			redEnemy1.startTimer--;
		}
		else
		{
			redEnemy1.isAlive = true;
			//update the location
			updateEnemy(redEnemy1);
		}
		redEnemy1.frameTimer = currentTime;
	}
	if (currentTime - redEnemy.frameTimer >= 1.0)
	{
		if (redEnemy.startTimer >= 0)
		{
			redEnemy.isAlive = false;
			redEnemy.startTimer--;
		}
		else
		{
			redEnemy.isAlive = true;
			//update the location
			updateEnemy(redEnemy);
		}
		redEnemy.frameTimer = currentTime;
	}
	if (currentTime - snakeEnemy.frameTimer >= 1.0)
	{
		if (snakeEnemy.startTimer >= 0)
		{
			snakeEnemy.isAlive = false;
			snakeEnemy.startTimer--;
		}
		else
		{
			snakeEnemy.isAlive = true;
			//update the location
			updateSnakeEnemy(snakeEnemy);
		}
		snakeEnemy.frameTimer = currentTime;
	}
	if (currentTime - blueEnemy.frameTimer >= 1.0)
	{
		if (blueEnemy.startTimer >= 0)
		{
			blueEnemy.isAlive = false;
			blueEnemy.startTimer--;
		}
		else
		{
			blueEnemy.isAlive = true;
			//update the location
			updateEnemy(blueEnemy);
		}
		blueEnemy.frameTimer = currentTime;
	}
	if (redEnemy1.isAlive && charLoc == redEnemy1.myEnemy.id ||
		redEnemy.isAlive && charLoc == redEnemy.myEnemy.id || 
		blueEnemy.isAlive && charLoc == blueEnemy.myEnemy.id ||
		snakeEnemy.isAlive && charLoc == snakeEnemy.myEnemy.id ||
		redEnemy2.isAlive && charLoc == redEnemy2.myEnemy.id)
	{
		lives--;
		PlaySound(TEXT("sounds\\Sware1.wav"), NULL, SND_ASYNC | SND_FILENAME);
		//reset enemy locs
		resetEnemy(redEnemy1);
		resetEnemy(redEnemy);
		resetEnemy(blueEnemy);
		resetEnemy(snakeEnemy);
		if (level > 1)
			resetEnemy(redEnemy2);
	}
	if (currentTime - myCharVisit >= 0.2)
	{
		moveQBert(myCharacter);
		myCharVisit = currentTime;
	}
	render3D(info, myCharacter, 0, 0);
	if (redEnemy2.isAlive && level > 1)
		render3D(info, redEnemy2.myEnemy, 0, 0);
	if (redEnemy1.isAlive)
		render3D(info, redEnemy1.myEnemy, 0, 0);
	if (redEnemy.isAlive)
		render3D(info, redEnemy.myEnemy, 0, 0);
	if (blueEnemy.isAlive)
		render3D(info, blueEnemy.myEnemy, 0, 0);
	if (snakeEnemy.isAlive)
		render3D(info, snakeEnemy.myEnemy, 0, 0);
	if(showLeftDisc)
		render3D(info, myDiscs[0], 0, 0);
	if (showRightDisc)
		render3D(info, myDiscs[1], 0, 0);
	drawDFS(info, myRootNode, 1);
	if (isTransition)
	{
		if (isTransitionMusic)
		{
			isTransitionMusic = false;
			PlaySound(TEXT("sounds\\trans.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		if (transitionInd <= BlkLevel * (BlkLevel + 1) / 2)
		{
			load_NodeArr[transitionInd]->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Top[level - 1].x;
			load_NodeArr[transitionInd]->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Top[level - 1].y;
			load_NodeArr[transitionInd]->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Top[level - 1].z;
			transitionInd++;
			Sleep(70);
		}
		else
		{
			isTransition = false;
			transitionInd = 1;
		}
	}
	if (tileCount >= BlkLevel * (BlkLevel + 1) / 2)
	{
		//go to next level;
		level++;
		isTransition = true;
		isTransitionMusic = true;
		if (level > maxLevels)
		{
			//end game
			currScreen = gameOverScreen;
		}
		else
		{
			//change colors of cubes
			//set the cubes property based on level
			//for (int we = 1; we <= BlkLevel * (BlkLevel + 1) / 2; we++)
			{
				//load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Top[level - 1].x;
				//load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Top[level - 1].y;
				//load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Top[level - 1].z;

				//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.x = gridChangeVals_Right[level - 1].x;
				//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.y = gridChangeVals_Right[level - 1].y;
				//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.z = gridChangeVals_Right[level - 1].z;
			}
		}
		showRightDisc = showLeftDisc = true;
		myCharacter.modelTrans.transX = qBertX;
		myCharacter.modelTrans.transY = qBertY;
		myCharacter.modelTrans.transZ = qBertZ;
		myCharacter.id = 1;
		eyeLoc = eyeLoc_Original;
		recalcMVP_Loc(&myCharacter);
		charLoc = 1;
		//reset enemies here now
		tileCount = 0;
		visitArr = 1;
		//reset players, creatures, present new board.
		resetEnemy(redEnemy);
		resetEnemy(redEnemy1);
		resetEnemy(blueEnemy);
		resetEnemy(snakeEnemy);
		if (level > 1)
			resetEnemy(redEnemy2);
	}

	sprintf(scoreText, "%s", "Score: ");
	sprintf(scoreText + 7, "%d", score);
	drawFrameRateNTime(scoreText, 30, 570, 15, info);

	sprintf(levelText, "%s", "Level: ");
	sprintf(levelText + 7, "%d", level);
	drawFrameRateNTime(levelText, 200, 570, 15, info);

	sprintf(livesText, "%s", "Lives Rem: ");
	sprintf(livesText + 11, "%d", lives);
	drawFrameRateNTime(livesText, 450, 570, 15, info);

	bitArr_Draw = 0;
	globalTexNum = 0;
}
void showGameOverScreen(ShaderInfo& info)
{
	if (!endScreenPlaySound)
	{
		PlaySound(TEXT("sounds\\Gameendmusic.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
		endScreenPlaySound = true;
		startsScreenPlaySound = false;
	}
	drawFrameRateNTime(gameOverText, 30, 370, 20, info);
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && enterToggle)
	{
		myCharacter.modelTrans.transX = qBertX;
		myCharacter.modelTrans.transY = qBertY;
		myCharacter.modelTrans.transZ = qBertZ;
		myCharacter.id = 1;
		eyeLoc = eyeLoc_Original;
		recalcMVP_Loc(&myCharacter);
		showRightDisc = showLeftDisc = true;
		//reset items here
		tileCount = 0;
		visitArr = 1;
		score = 0;
		level = 1;
		charLoc = 1;
		lives = 3;
		//change colors of cubes
		//set the cubes property based on level
		for (int we = 1; we <= BlkLevel * (BlkLevel + 1) / 2; we++)
		{
			load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.x = gridChangeVals_Top[level - 1].x;
			load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.y = gridChangeVals_Top[level - 1].y;
			load_NodeArr[we]->eachMtlBlk_List[0].myMtlBlock.Ka.z = gridChangeVals_Top[level - 1].z;

			//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.x = gridChangeVals_Right[level - 1].x;
			//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.y = gridChangeVals_Right[level - 1].y;
			//load_NodeArr[we]->eachMtlBlk_List[2].myMtlBlock.Ka.z = gridChangeVals_Right[level - 1].z;
		}
		//reset enemies now
		resetEnemy(redEnemy);
		resetEnemy(redEnemy1);
		resetEnemy(blueEnemy);
		resetEnemy(snakeEnemy);
		if (level > 1)
			resetEnemy(redEnemy2);

		currScreen = startScreen;
		lives = 3;
		enterToggle = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		enterToggle = true;
}
/**
* Program Entry Point.
*/
int main( int argc, char** argv ) 
{
	
	currScreen = startScreen;
	ShaderInfo info;
	cout << "Main Func";
	// initialize GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW!\n" );
		return -1;
	}

	glfwWindowHint( GLFW_SAMPLES, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 900, "QBert - The Game.", NULL, NULL );
	if( window == NULL ) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	// initialize GLEW
	glewExperimental = true; // Needed for core profile
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	// ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );

	// set the GL clear color
	glClearColor( 0.f, 0.f, 0.0f, 0.0f );


	//// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	// load shaders and bind arrays
	info.programId = NULL;
	info.vertexArray = NULL;
//	info.vertexBuffer = NULL;
	init( info );
	globalTexNum = 0;

	frameRateTime = redEnemy.frameTimer = redEnemy1.frameTimer = blueEnemy.frameTimer = redEnemy2.frameTimer = glfwGetTime();
	glUseProgram(info.programId);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//updateView();
		frameRate++;
		currentTime = glfwGetTime();
		if (currentTime - frameRateTime >= 1.0)
		{
			//cout << frameRate << " Frame Rate" << endl;
			frameRateToShow = frameRate;
			frameRate = 0;
			frameRateTime = glfwGetTime();
		}
		sprintf(timeText, "%f", frameRateToShow);
		sprintf(timeText + 5, "%s", "fps");
		drawFrameRateNTime(timeText, 650, 570, 15, info);

		if (lives < 1)
			currScreen = gameOverScreen;
		switch (currScreen)
		{
		case startScreen: showStartScreen(info);
			break;
		case gameScreen: showGameScreen(info);
			break;
		case gameOverScreen: showGameOverScreen(info);
			break;
		}
		//for (int o = 0; o < BlkLevel * (BlkLevel + 1) / 2 + 2; o++)
		//{
		//	drawArrBool[o] = false;
		//}
		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	//	break;
	} // check if the ESC key was pressed or the window was closed
	while( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose( window ) == 0 );
	//int r;
	//cin >> r;
	// close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
//#endif
