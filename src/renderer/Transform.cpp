//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Transform.cpp
// Purpose: - This Class is a Transform Class, it is used by every
//			- object to move around in 3D Space
//////////////////////////////////////////////////////////////////

// Includes
#include "Transform.h"

// Functions:
// Public functions:
Transform::Transform()
	:
	pos(0.0f, 0.0f, 0.0f),
	rot(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	transMatrixDirty(false),
	rotMatrixDirty(false),
	scaleMatrixDirty(false)
{
	D3DXMatrixIdentity(&transMatrix);
	D3DXMatrixIdentity(&rotMatrix);
	D3DXMatrixIdentity(&scaleMatrix);
	D3DXMatrixIdentity(&transformMatrix);
}

void Transform::LookAt(D3DXVECTOR3 target)
{
	// How to do this?
}

// Getter:
D3DXVECTOR3 Transform::GetPos()
{
	return pos;
}
D3DXVECTOR3 Transform::GetRot()
{
	return rot;
}
D3DXVECTOR3 Transform::GetScale()
{
	return scale;
}
D3DXMATRIX Transform::GetTransformMatrix()
{
	if (!transMatrixDirty && !rotMatrixDirty && !scaleMatrixDirty)
	{
		// None of this is dirty so you can just return the old TransformMatrix.
		return transformMatrix;
	}

	if (transMatrixDirty)
	{
		D3DXMatrixTranslation(&transMatrix, pos.x, pos.y, pos.z);
		transMatrixDirty = false;
	}
	if (rotMatrixDirty)
	{
		D3DXMatrixRotationYawPitchRoll(&rotMatrix, rot.x, rot.y, rot.z);
		rotMatrixDirty = false;
	}
	if (scaleMatrixDirty)
	{
		D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
		scaleMatrixDirty = false;
	}

	transformMatrix = scaleMatrix * rotMatrix * transMatrix;
	return transformMatrix;
}
D3DXVECTOR3 Transform::GetTarget()
{
	D3DXVECTOR3 target{ 0.0f, 0.0f, 1.0f };
	D3DXMATRIX rotationMatrix = *D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.x, rot.y, rot.z);
	D3DXVec3TransformCoord(&target, &target, &rotationMatrix);
	D3DXVec3Normalize(&target, &target);

	return target;
}
D3DXVECTOR3 Transform::GetUp()
{
	D3DXVECTOR3 up{ 0.0f, 1.0f, 0.0f };
	D3DXMATRIX rotationMatrix = *D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.x, rot.y, rot.z);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	D3DXVec3Normalize(&up, &up);

	return up;
}
D3DXVECTOR3 Transform::GetRight()
{
	D3DXVECTOR3 right{ 1.0f, 0.0f, 0.0f };
	D3DXMATRIX rotationMatrix = *D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.x, rot.y, rot.z);
	D3DXVec3TransformCoord(&right, &right, &rotationMatrix);
	D3DXVec3Normalize(&right, &right);

	return right;
}

// Setter:
void Transform::SetPos(D3DXVECTOR3 newPos)
{
	this->pos = newPos;
	transMatrixDirty = true;
}
void Transform::SetRot(D3DXVECTOR3 newRot)
{
	this->rot = newRot;
	rotMatrixDirty = true;
}
void Transform::SetScale(D3DXVECTOR3 newScale)
{
	this->scale = newScale;
	scaleMatrixDirty = true;
}

// Adder:
void Transform::PanBy(D3DXVECTOR3 amount, Space space)
{
	if (space == local)
	{
		// Movement in Local Space:
		// 1st: Create the local Vectors and rotate them
		D3DXVECTOR3 target = GetTarget();
		D3DXVECTOR3 up = GetUp();
		D3DXVECTOR3 right = GetRight();

		// 2nd: Perform Vector regeneration
		D3DXVec3Normalize(&target, &target);
		D3DXVec3Cross(&right, &up, &target);
		D3DXVec3Normalize(&right, &right);
		D3DXVec3Cross(&up, &target, &right);
		D3DXVec3Normalize(&up, &up);

		// 3rd: Move the Object along those local Vectors:
		pos = pos + (right * amount.x);
		pos = pos + (up * amount.y);
		pos = pos + (target * amount.z);

		transMatrixDirty = true;

	}
	else
	{
		// Movement in Global Space:
		this->pos += amount;
		transMatrixDirty = true;
	}
}
void Transform::ScaleBy(D3DXVECTOR3 amount, Space space)
{
	if (space == local)
	{
		// Scaling in Local Space:
		this->scale += amount;
		scaleMatrixDirty = true;

	}
	else
	{
		// Scaling in Global Space:
		this->scale += amount;
		scaleMatrixDirty = true;
	}
}
void Transform::RotateBy(D3DXVECTOR3 amount, Space space)
{
	// TODO: Whats the difference between local rotation and global rotation?
	if (space == local)
	{
		// Rotation in Local Space:
		this->rot += amount;
		rotMatrixDirty = true;

	}
	else
	{
		// Rotation in Global Space:
		this->rot += amount;
		rotMatrixDirty = true;
	}
}
