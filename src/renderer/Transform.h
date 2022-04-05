//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Transform.h
// Purpose: - This Class is a Transform Class, it is used by every
//			- object to move around in 3D Space
//////////////////////////////////////////////////////////////////


// TODO:
// - Rotate around Axis (user defined axis)
// - rotation local = around local vectors
// - rotation global = around global vectors
// - scale local = scale in direction of local vectors
// - scale global = scale in direction of global vectors

// Preprocessor Directives:
#pragma once

// Includes:
#include <d3dx9.h>
#include "Space.h"


struct Transform
{
	// Functions:
	// Public functions:
public:
	Transform();

	void LookAt(D3DXVECTOR3 target);

	// Getter:
	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3 GetScale();
	D3DXMATRIX GetTransformMatrix();
	D3DXVECTOR3 GetTarget();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();

	// Setter:
	void SetPos(D3DXVECTOR3 newPos);
	void SetRot(D3DXVECTOR3 newRot);
	void SetScale(D3DXVECTOR3 newScale);

	// Adder:
	void PanBy(D3DXVECTOR3 amount, Space space = local);
	void ScaleBy(D3DXVECTOR3 amount, Space space = local);
	void RotateBy(D3DXVECTOR3 amount, Space space = local);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* the components of pos define the movement on the 3 global axis. */
	D3DXVECTOR3 pos;
	/* the components of rot define the rotation around the 3 global axis. */
	D3DXVECTOR3 rot;
	/* the components of scale define the scaling on the 3 local axis. */
	D3DXVECTOR3 scale;
	/* the matrix for the whole transformation */
	D3DXMATRIX transformMatrix;
	/* the translation matrix. */
	D3DXMATRIX transMatrix;
	/* Is the translation matrix dirty? */
	bool transMatrixDirty;
	/* the rotation matrix. */
	D3DXMATRIX rotMatrix;
	/* Is the rotation matrix dirty? */
	bool rotMatrixDirty;
	/* the scale matrix.*/
	D3DXMATRIX scaleMatrix;
	/* Is the scale matrix dirty? */
	bool scaleMatrixDirty;

};