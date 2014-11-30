#include "Small_Scene_Render.h"

void drawVertices() {
	D3DXMATRIX rotMatrix;
	D3DXMATRIX transMatrix;
	D3DXMATRIX worldMatrix;
	static float rotAngle = 0.0f;

	D3DXMatrixRotationY(&rotMatrix, D3DXToRadian(rotAngle)); //rotate the object at a constant angle
	D3DXMatrixTranslation(&transMatrix, 2, 0, 0); //translate the object 2 units along the x axis
	D3DXMatrixMultiply(&worldMatrix, &rotMatrix, &transMatrix);
	d3d_device->SetTransform(D3DTS_WORLD, &worldMatrix); //set the total transformation matrix

	//draw surroundings
	d3d_device->SetStreamSource(0, virtex_buffer, 0, sizeof(Vertex)); //set the virtex stream source
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MAX_TRIANGLES);

	//draw the character now
	d3d_device->SetStreamSource(0, character_buffer, 0, sizeof(Vertex));
	d3d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, MAX_TRIANGLES);
	
	rotAngle += .07;
}

