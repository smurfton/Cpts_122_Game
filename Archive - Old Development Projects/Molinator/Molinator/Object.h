class Object {
public:
	Object();
	~Object();

	//getters
	Position getPosition();
	Position getOrientation();
	wchar_t *getModelFile();
	wchar_t *getTextureFile();
	float getScale();
	LPDIRECT3DVERTEXBUFFER9 &getBuffer();

	//setters
	void setPosition(Position newPosition);
	void setOrientation(Position newOrientation);
	void setModelFile(wchar_t * fileName);
	void setTextureFile(wchar_t * fileName);
	void setScale(float newScale);
	void setVertices();
	void setTransformedVertices();

	//general functionality
	void createTexture(LPDIRECT3DDEVICE9 &device);
	void createBuffer(LPDIRECT3DDEVICE9 &device); 
	void loadBuffer(LPDIRECT3DDEVICE9 &device);
	void loadTexture(LPDIRECT3DDEVICE9 &device);
private:
	wchar_t modelFileName[MAX_FILE_LENGTH], textureFileName[MAX_FILE_LENGTH];
	LPDIRECT3DVERTEXBUFFER9 buffer;
	IDirect3DTexture9 *pTexture;
	Position location, orientation; //orientation is the transformed normal, normal is the current normal (1,0,0)
	vector<Vertex> vertices, transformed;
	float size;
};