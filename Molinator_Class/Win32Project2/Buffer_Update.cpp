#include "Small_Scene_Render.h"

void updateCharacter() {
	vector<Vertex> playerT;
	//apply transformations
	transformVector(playerOrientation, player, playerT);

	void *pVoid;
	character_buffer->Lock(0,0, (void **)&pVoid, 0);
	memcpy(pVoid, &playerT[0], playerT.size()*sizeof(Vertex));
	character_buffer->Unlock();
}

void updateCamera() {
	D3DXMATRIX viewMatrix;
	D3DXMATRIX lensMatrix;

	D3DXMatrixLookAtLH(&viewMatrix,
					   &D3DXVECTOR3 (playerOrientation.location.x-10, playerOrientation.location.y+5, playerOrientation.location.z),    // the camera position
					   &D3DXVECTOR3 (playerOrientation.location.x, playerOrientation.location.y, playerOrientation.location.z),    // the look-at position
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction

	d3d_device->SetTransform(D3DTS_VIEW, &viewMatrix); //set the camera view point

	D3DXMatrixPerspectiveFovLH(&lensMatrix, //Set the camera lens
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane
	d3d_device->SetTransform(D3DTS_PROJECTION, &lensMatrix); //set the camera lens 
}