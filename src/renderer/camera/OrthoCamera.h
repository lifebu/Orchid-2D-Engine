//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: OrthoCamera.h
// Purpose: - This Class is a Orthogonal Camera
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Defines
#define BASEWIDTH 800;
#define BASEHEIGHT 800;

// Includes
#include <d3dx9.h>
#include "Camera.h"
#include "core/system/ErrorHandler.h"

/* An Orthogonal Camera for 2D Graphics. */
class OrthogonalCamera : public Camera
{
	// Functions:
	// Public functions:
public:
	OrthogonalCamera();
	~OrthogonalCamera();

	
	void Initialize(const float width, const float height, const float zoom, const float nearClip, const float farClip);
	void Initialize(const OrthogonalCamera& otherCam);
	/* Shutdown the Camera. */
	void Shutdown();

	// Movement:
	// Panning
	void PanBy(const D3DXVECTOR2 amount);
	// Rolling
	void RollBy(const float Degree);
	// Zoom
	void ZoomBy(const float factor);

	// Getter:
	const float& GetWidth() const;
	const float& GetHeight() const;
	const D3DXVECTOR3& GetPos() const;
	const float& GetZoom() const;

	// Setter:
	void SetPosition(const D3DXVECTOR2 newPos);


	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:

	// Orthogonal Data:
	float zoom;
	float width;
	float height;

	// Data for Both:
	float nearClip;
	float farClip;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up;
	D3DXVECTOR3 right;

};