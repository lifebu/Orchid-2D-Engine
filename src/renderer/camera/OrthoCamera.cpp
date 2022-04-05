//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: OrthoCamera.cpp
// Purpose: - This Class is a Orthogonal Camera
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "OrthoCamera.h"

// Functions:
// Public functions:
OrthogonalCamera::OrthogonalCamera()
	:
	pos(0.0f, 0.0f, 0.0f),
	target(0.0f, 0.0f, 1.0f),
	right(1.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f),
	farClip(1000.0f),
	height(600),
	width(800),
	nearClip(1.0f),
	zoom(1.0f)
{
	D3DXMatrixIdentity(&viewmatrix);
	D3DXMatrixIdentity(&projectionmatrix);
}
OrthogonalCamera::~OrthogonalCamera()
{

}

void OrthogonalCamera::Initialize(const float width, const float height, const float zoom, const float nearClip, const float farClip)
{
	// 1st: Set Data Members:
	this->width = BASEWIDTH;
	// Aspect Ration Adjustment:
	this->width *= (width / height);
	this->height = BASEHEIGHT;
	this->zoom = zoom;
	this->nearClip = nearClip;
	this->farClip = farClip;

	// 2nd: Create the Orthogonal Matrix:
	D3DXMatrixOrthoLH(&projectionmatrix, this->width * this->zoom,
		this->height * this->zoom, nearClip, farClip);

	initialized = true;
	return;
}
void OrthogonalCamera::Initialize(const OrthogonalCamera& otherCam)
{
	if (otherCam.IsInitialized())
	{
		farClip = otherCam.farClip;
		height = otherCam.height;
		nearClip = otherCam.nearClip;
		pos = otherCam.pos;
		right = otherCam.right;
		target = otherCam.target;
		up = otherCam.up;
		width = otherCam.width;
		zoom = otherCam.zoom;
		initialized = otherCam.initialized;
		projectionmatrix = otherCam.projectionmatrix;
		viewmatrix = otherCam.viewmatrix;
	}
}
void OrthogonalCamera::Shutdown()
{
	initialized = false;
	return;
}

void OrthogonalCamera::PanBy(D3DXVECTOR2 amount)
{
	// 1st: Change the Position
	pos.x += amount.x;
	pos.y += amount.y;

	// 2nd: Change the Target
	target.x += amount.x;
	target.y += amount.y;

	// 3rd: Update the ViewMatrix
	D3DXMatrixLookAtLH(&viewmatrix, &pos, &target, &up);
}

void OrthogonalCamera::ZoomBy(float factor)
{
	// 1st: Change the Zoom
	zoom += factor;

	// 2nd: Don't let the Zoom Factor be negative (and 0 doesn't make sense!).
	if (zoom < 0.0f)
	{
		zoom = 0.000001f;
	}

	// 3rd: Update the ProjectionMatrix
	D3DXMatrixOrthoLH(&projectionmatrix, (float)width * zoom,
		(float)height * zoom, nearClip, farClip);
}

void OrthogonalCamera::RollBy(float Degree)
{
	// LATER!
	//// Data:
	//D3DXMATRIX rotationmatrix;
	//D3DXVECTOR3 standartTarget{ 0.0f, 0.0f, 1.0f };

	//// 1st: Build rotation matrix
	////D3DXMatrixRotationAxis(&RotationMatrix, &Target, D3DXToRadian(Degree));
	//D3DXMatrixRotationZ(&RotationMatrix, D3DXToRadian(Degree));

	//// 2nd: Transform the local vectors.
	//D3DXVec3TransformCoord(&Target, &Target, &RotationMatrix);
	//D3DXVec3TransformCoord(&Up, &Up, &RotationMatrix);
	//D3DXVec3TransformCoord(&Right, &Right, &RotationMatrix);

	//// 3rd: Perform vector regeneration
	//D3DXVec3Normalize(&Target, &Target);
	//D3DXVec3Cross(&Right, &Up, &Target);
	//D3DXVec3Normalize(&Right, &Right);
	//D3DXVec3Cross(&Up, &Target, &Right);
	//D3DXVec3Normalize(&Up, &Up);

	//// 4th: Update the ViewMatrix
	//D3DXMatrixLookAtLH(&ViewMatrix, &Pos, &Target, &Up);
}

// Getters:
const D3DXMATRIX& Camera::GetViewMatrix() const
{
	return viewmatrix;
}
const D3DXMATRIX& Camera::GetProjectionMatrix() const
{
	return projectionmatrix;
}

const float& OrthogonalCamera::GetWidth() const
{
	return width;
}
const float& OrthogonalCamera::GetHeight() const
{
	return height;
}
const D3DXVECTOR3& OrthogonalCamera::GetPos() const
{
	return pos;
}
const float& OrthogonalCamera::GetZoom() const
{
	return zoom;
}

// Setter:
void OrthogonalCamera::SetPosition(const D3DXVECTOR2 newPos)
{
	D3DXVECTOR2 panValue = {};
	panValue.x = newPos.x - pos.x;
	panValue.y = newPos.y - pos.y;
	PanBy(panValue);
}

