//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Graphics.cpp
// Purpose: - This Class is responsible for the Graphic.
//			- It uses Direct3D 9.0c.
//////////////////////////////////////////////////////////////////

// Includes:
#include "Graphics.h"

// Functions:
// Public Functions:
Graphics::Graphics(const SETTINGS& Settings, const HWND hWnd)
	:
	settings(Settings),
	hWnd(hWnd),
	d3dDevice(nullptr),
	direct3D(nullptr),
	activeTexture(nullptr),
	font(),
	renderedTiles(0),
	useDebugCam(false),
	errorHandler(ErrorHandler::GetInstance())
{
}
Graphics::~Graphics()
{
}

void Graphics::Initialize()
{
	errorHandler->WriteLog("Initializing Graphicsystem ...");
	HRESULT result;

	// 1st: Create the Parameters to create the Device.
	presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	presentParameters.BackBufferCount = settings.bufferCount - 1;
	presentParameters.BackBufferFormat = settings.backbufferFormat;
	presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	presentParameters.BackBufferHeight = settings.resolutionHeight;
	presentParameters.BackBufferWidth = settings.resolutionWidth;
	//presentParameters.EnableAutoDepthStencil = true;
	/* Don't use Depthbuffering (not used for 2D Games).*/
	presentParameters.EnableAutoDepthStencil = false;
	presentParameters.Flags = settings.flags;
	presentParameters.FullScreen_RefreshRateInHz = settings.fullscreen_RefreshRateInHz;
	presentParameters.hDeviceWindow = hWnd;
	presentParameters.MultiSampleQuality = settings.multisampleQuality;
	presentParameters.MultiSampleType = settings.multisampleType;
	if (settings.vSync)
	{
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.Windowed = settings.windowed;

	D3DDEVTYPE DeviceType;
	if (settings.hardwareRendering)
	{
		DeviceType = D3DDEVTYPE_HAL;
	}
	else
	{
		DeviceType = D3DDEVTYPE_REF;
	}

	DWORD BehaviourFlags;
	if (settings.hardwareRendering)
	{
		BehaviourFlags = D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		BehaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 2nd: Create the Direct3D Interface
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3D == nullptr)
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Create a Direct3D Interface");
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3rd: Create the Device Interface.
	result = direct3D->CreateDevice(D3DADAPTER_DEFAULT,
		DeviceType,
		hWnd,
		BehaviourFlags,
		&presentParameters,
		&d3dDevice);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Create the D3DDevice Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Set the StandartValues for the device
	SetDeviceStandarts();

	// 5th: Initialized the Font:
	font.Initialize(d3dDevice, "Arial", 20, false, false);

	// 6th: Initialize the standart Camera:
	activeCamera.Initialize((float)settings.resolutionWidth, (float)settings.resolutionHeight, 0.19f, 1.0f, 1000.0f);
	//activeCamera.PanBy({ -918.0f, -423.0f });
	activeCamera.PanBy({ -700.0f, -344.0f });

	// 7th: Initialize the Debug Camera:
	debugCamera.Initialize((float)settings.resolutionWidth, (float)settings.resolutionHeight, 0.19f, 1.0f, 1000.0f);
	//debugCamera.PanBy({ -918.0f, -423.0f });
	debugCamera.PanBy({ -700.0f, -344.0f });
	
	// Temporarly
	drawDebugInfo = true;

	errorHandler->WriteLog("Graphicsystem Initialized.");

	return;
}
void Graphics::Shutdown()
{
	// No Pointers or shutdownable subsystems, so not needed!
	errorHandler->WriteLog("Shutdown Graphicsystem ...");


	// 1st: Shutdown the Font:
	if (font.Initialized())
	{
		font.Shutdown();
	}

	// 2nd: Shutdown the  D3DDevice Interface.
	if (d3dDevice != NULL)
	{
		d3dDevice->Release();
	}
	d3dDevice = nullptr;

	// 3rd: Shutdown the Direct3D Interface.
	if (direct3D != NULL)
	{
		direct3D->Release();
	}
	direct3D = nullptr;


	activeTexture = nullptr;

	//// Test:
	//areaActiveCamera.Shutdown();

	errorHandler->WriteLog("Graphicsystem Shutdown successful.");

	return;
}

void Graphics::BeginFrame()
{
	HRESULT result;

	//1st: Clear the Screen
	// No D3DCLEAR_ZBUFFER, because this 2D Framework does not use a Z-Stencil-Buffer.
	result = d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER*/, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.BeginFrame(): Failed to Clear the BackBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3rd: Begin Drawing
	result = d3dDevice->BeginScene();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.BeginFrame(): Failed to Begin the 3D Scene: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Set active Camera.
	if (!useDebugCam)
	{
		if (activeCamera.IsInitialized())
		{
			// Set the projection- and Viewmatrix.
			result = d3dDevice->SetTransform(D3DTS_VIEW, &activeCamera.GetViewMatrix());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphic.Initialize(): Failed to Set the ViewMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
			result = d3dDevice->SetTransform(D3DTS_PROJECTION, &activeCamera.GetProjectionMatrix());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphic.Initialize(): Failed to Set the ProjectionMatrix: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
		}
	}
	else  // Use Debug Cam for Rendering
	{
		if (debugCamera.IsInitialized())
		{
			// Set the projection- and Viewmatrix.
			result = d3dDevice->SetTransform(D3DTS_VIEW, &debugCamera.GetViewMatrix());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphic.Initialize(): Failed to Set the ViewMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
			result = d3dDevice->SetTransform(D3DTS_PROJECTION, &debugCamera.GetProjectionMatrix());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphic.Initialize(): Failed to Set the ProjectionMatrix: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
		}
	}

	// Draw Debug info if activated:
	if (drawDebugInfo)
	{
	}

	return;
}
void Graphics::EndFrame()
{
	HRESULT result;

	// 1st: Render all Texts
	// 1.1.: how many tiles rendered?
	char buffer[256];
	sprintf(buffer, "Amount of Tiles Rendered: %d", renderedTiles);
	font.Write(buffer, 0, 0, D3DCOLOR_XRGB(255, 255, 255));

	// 1.2.: Which Camera is active?
	if (useDebugCam)
	{
		sprintf(buffer, "Debug Camera active");
		font.Write(buffer, 0, 22, D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		sprintf(buffer, "Normal Camera active");
		font.Write(buffer, 0, 22, D3DCOLOR_XRGB(255, 255, 255));
	}

	// 1.3. Render every other text, that the game added.
	if (TextDrawQueue.size() > 0)
	{
		int a = 10;
	}
	for (auto& entry : TextDrawQueue)
	{
		font.Write(entry.string, entry.x, entry.y, entry.color);
	}
	TextDrawQueue.clear();


	// 2nd: End Drawing
	result = d3dDevice->EndScene();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.EndFrame(): Failed to End the 3D Scene: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 3rd: Present the whole Screen.
	result = d3dDevice->Present(NULL, NULL, NULL, NULL);
	if (FAILED(result) && result != D3DERR_DEVICELOST)
	{

		char buffer[256];
		sprintf(buffer, "Graphic.EndFrame(): Failed to Present the Scene: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	else if (result == D3DERR_DEVICELOST)
	{
		// 4th: The Device was lost!!! So do stuff!
		do
		{
			// 4.1 Loop until we get Device not Rest.
			result = d3dDevice->TestCooperativeLevel();
			Sleep(10);

		} while (result != D3DERR_DEVICENOTRESET);

		// 4.2. Temporarly release the textures
		ResourceManager::GetInstance()->ReleaseTextures();

		// 4.3 Create new Parameters.
		D3DPRESENT_PARAMETERS parameters;
		parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		parameters.BackBufferCount = settings.bufferCount - 1;
		parameters.BackBufferFormat = settings.backbufferFormat;
		parameters.BackBufferFormat = D3DFMT_X8R8G8B8;
		parameters.BackBufferHeight = settings.resolutionHeight;
		parameters.BackBufferWidth = settings.resolutionWidth;
		parameters.EnableAutoDepthStencil = true;
		parameters.Flags = settings.flags;
		parameters.FullScreen_RefreshRateInHz = settings.fullscreen_RefreshRateInHz;
		parameters.hDeviceWindow = hWnd;
		parameters.MultiSampleQuality = settings.multisampleQuality;
		parameters.MultiSampleType = settings.multisampleType;
		if (settings.vSync)
		{
			parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
		else
		{
			parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		parameters.Windowed = settings.windowed;

		D3DDEVTYPE DeviceType;
		if (settings.hardwareRendering)
		{
			DeviceType = D3DDEVTYPE_HAL;
		}
		else
		{
			DeviceType = D3DDEVTYPE_REF;
		}

		DWORD BehaviourFlags;
		if (settings.hardwareRendering)
		{
			BehaviourFlags = D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else
		{
			BehaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		// 4.4 Wait for D3D_OK
		do
		{
			// 4.5 Reset the Device
			d3dDevice->Reset(&parameters);


			// 4.6 Loop until we get Device not Rest.
			result = d3dDevice->TestCooperativeLevel();
			Sleep(10);
		} while (result != D3D_OK);

		// 4.7 Now that the Device is resettet and accessible you need to restore the settings:
		SetDeviceStandarts();
		activeTexture = nullptr;

		// 4.8. After that recreat all textures
		ResourceManager::GetInstance()->ReloadTextures();
	}

	renderedTiles = 0;

	return;
}

void Graphics::RenderSprite(const Static_Sprite& sprite)
{
	// Data
	HRESULT result;
	LPDIRECT3DTEXTURE9 wantedTexture;

	if (!sprite.IsInitialized())
	{
		// 1st: It is not initialized, so you can't draw it!
		return;
	}

	// 2nd: Set Vertexbuffer Streamsource (The Stream where the Draw Functions gets its Data).
	result = d3dDevice->SetStreamSource(0,								// StreamNumber 
										   sprite.GetVertexBuffer(),		// The Vertexbuffer to be bound to the Stream	
										   0,								// Offset in the Vertexbuffer (unused here).
										   sizeof(Vertex));					// the size for one Vertex.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the Streamsource: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3rd: Set the Indices (So the Draw Function knows which Indices you want to use).
	result = d3dDevice->SetIndices(sprite.GetIndexBuffer());
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the indices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Set the WorldMatrix
	D3DXMATRIX WorldMatrix;
	WorldMatrix = (sprite.GetScalingMatrix() * sprite.GetRotationMatrix()) * sprite.GetTranslationMatrix();

	result = d3dDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the WorldMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 5th: Set the Texture:
	// Only set the Texture if the current used texture is not the same!
	wantedTexture = sprite.GetTexture();
	if (wantedTexture != activeTexture)
	{
		result = d3dDevice->SetTexture(0, wantedTexture);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Graphics.RenderSprite(): Failed to set the Texture: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}
		activeTexture = wantedTexture;
	}
	

	// 6th: Draw the Primitive
	result = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,			// How to interpret the Data (interpret as single triangles, set of 3 vertices).
												0,							// Basisindex for the Indices (usually 0)
												0,							// The smallest Index used
												4,							// Difference +1 between smallest and highest index used (e.g. Vertexnumber).
												0,							// The index DirectX beginns to draw (here 0, beginn at first index)
												2);							// How many primitives have to be drawn
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to draw the IndexedPrimitive: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
}
void Graphics::RenderSprite(const Animated_Sprite& sprite)
{
	// Data
	HRESULT result;

	if (!sprite.IsInitialized())
	{
		// 1st: It is not initialized, so you can't draw it!
		return;
	}

	// 2nd: Set Vertexbuffer Streamsource (The Stream where the Draw Functions gets its Data).
	result = d3dDevice->SetStreamSource(0,								// StreamNumber 
		sprite.GetVertexBuffer(),		// The Vertexbuffer to be bound to the Stream	
		0,								// Offset in the Vertexbuffer (unused here).
		sizeof(Vertex));					// the size for one Vertex.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the Streamsource: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3rd: Set the Indices (So the Draw Function knows which Indices you want to use).
	result = d3dDevice->SetIndices(sprite.GetIndexBuffer());
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the indices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Set the WorldMatrix
	D3DXMATRIX WorldMatrix;
	WorldMatrix = (sprite.GetScalingMatrix() * sprite.GetRotationMatrix()) * sprite.GetTranslationMatrix();

	result = d3dDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to set the WorldMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 5th: Set the Texture:
	// Only set the Texture if the current used texture is not the same!
	LPDIRECT3DTEXTURE9 wantedTexture = sprite.GetTexture();
	if (wantedTexture != activeTexture)
	{
		result = d3dDevice->SetTexture(0, wantedTexture);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Graphics.RenderSprite(): Failed to set the Texture: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}
		activeTexture = wantedTexture;
	}


	// 6th: Draw the Primitive
	result = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,			// How to interpret the Data (interpret as single triangles, set of 3 vertices).
		0,							// Basisindex for the Indices (usually 0)
		0,							// The smallest Index used
		4,							// Difference +1 between smallest and highest index used (e.g. Vertexnumber).
		0,							// The index DirectX beginns to draw (here 0, beginn at first index)
		2);							// How many primitives have to be drawn
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphics.RenderSprite(): Failed to draw the IndexedPrimitive: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
}
void Graphics::RenderTilemap(const Tilemap& tilemap)
{

	if (!tilemap.IsInitialized())
	{
		// It is not initialized, so you can't draw it!
		return;
	}

	// Data
	const std::vector<Tileset>& tilesets = tilemap.GetTilesets();

	// 1st: Define the Viewfrustum
	D3DXVECTOR3 CamViewfrustrumLU;
	CamViewfrustrumLU.x = activeCamera.GetPos().x - ((activeCamera.GetWidth() * activeCamera.GetZoom()) / 2.0f);
	CamViewfrustrumLU.y = activeCamera.GetPos().y + ((activeCamera.GetHeight() * activeCamera.GetZoom()) / 2.0f);
	CamViewfrustrumLU.z = 0.0f;

	D3DXVECTOR3 camViewfrustrumRD;
	camViewfrustrumRD.x = activeCamera.GetPos().x + ((activeCamera.GetWidth() * activeCamera.GetZoom()) / 2.0f);
	camViewfrustrumRD.y = activeCamera.GetPos().y - ((activeCamera.GetHeight() * activeCamera.GetZoom()) / 2.0f);
	camViewfrustrumRD.z = 0.0f;

	D3DXVECTOR3 mapTopLeftCorner;
	mapTopLeftCorner.x = 0.0f - (((float)(tilemap.GetMapWidth() * tilemap.GetTileWidth())) / 2.0f);
	mapTopLeftCorner.y = 0.0f + (((float)(tilemap.GetMapHeight() * tilemap.GetTileHeight())) / 2.0f);
	mapTopLeftCorner.z = 0.0f;

	D3DXVECTOR3 differenceVectorToCamLU;
	differenceVectorToCamLU = CamViewfrustrumLU - mapTopLeftCorner;
	differenceVectorToCamLU.y = -differenceVectorToCamLU.y;

	D3DXVECTOR3 differenceVectorToCamRD;
	differenceVectorToCamRD = camViewfrustrumRD - mapTopLeftCorner;
	differenceVectorToCamRD.y = -differenceVectorToCamRD.y;

	float temp = differenceVectorToCamLU.x / (float)tilemap.GetTileWidth();
	if (temp < 0)
	{
		temp = 0;
	}
	else if (temp >((float)tilemap.GetMapWidth() - 2.0f))
	{
		temp = ((float)tilemap.GetMapWidth() - 2.0f);
	}
	UINT TileXStart = (UINT)temp;

	temp = differenceVectorToCamLU.y / (float)tilemap.GetTileHeight();
	if (temp < 0)
	{
		temp = 0;
	}
	else if (temp > ((float)tilemap.GetMapHeight() - 2.0f))
	{
		temp = ((float)tilemap.GetMapHeight() - 2.0f);
	}
	UINT TileYStart = (UINT)(temp);

	temp = (differenceVectorToCamRD.x / (float)tilemap.GetTileWidth()) + 0.5f;
	if (temp < 0)
	{
		temp = 0;
	}
	else if (temp > ((float)tilemap.GetMapWidth() - 2.0f))
	{
		temp = ((float)tilemap.GetMapWidth() - 2.0f);
	}
	UINT TileXEnd = (UINT)(temp) + 1;

	temp = (differenceVectorToCamRD.y / (float)tilemap.GetTileHeight()) + 0.5f;
	if (temp < 0)
	{
		temp = 0;
	}
	else if (temp > ((float)tilemap.GetMapHeight() - 2.0f))
	{
		temp = ((float)tilemap.GetMapHeight() - 2.0f);
	}
	UINT TileYEnd = (UINT)(temp) + 1;


	// 2nd: Render all backgroundImageLayers:
	RenderImageLayers(tilemap.GetBackgroundImageLayers());

	// 3rd: Render all backgroundTileLayers:
	RenderTileLayers(tilemap.GetBackgroundTileLayers(), tilesets, tilemap, TileXStart, TileXEnd, TileYStart, TileYEnd);

	// 4th: Render all backgroundCollisionLayers;
	// TODO:
	
	// 5th: Render all ObjectLayers;
	RenderObjectLayers(tilemap.GetObjectLayer());

	// 6th: Render all foregroundTileLayers:
	RenderTileLayers(tilemap.GetForegroundTileLayers(), tilesets, tilemap, TileXStart, TileXEnd, TileYStart, TileYEnd);

	// 7th: Render all effectImageLayers:
	RenderImageLayers(tilemap.GetEffectImageLayers());

}
void Graphics::RenderTileLayers(const std::vector<Tilelayer>& tilelayers, const std::vector<Tileset>& tilesets, const Tilemap& tilemap,
	const UINT& tileXStart, const UINT& tileXEnd, const UINT& tileYStart, const UINT& tileYEnd)
{
	HRESULT result;

	if (tilelayers.size() > 0)
	{
		for (auto& layerentry : tilelayers)
		{
			// 1st: Iterate through every tile.
			const std::vector<Tile>& tiles = layerentry.getTiles();
			for (UINT x = tileXStart; x <= tileXEnd; x++)
			{
				for (UINT y = tileYStart; y <= tileYEnd; y++)
				{
					Tile currentTile = tiles.at(y * tilemap.GetMapWidth() + x);
					if (!currentTile.GetGID() == 0)
					{
						// This is a tile with a graphic!
						const Tileset* usedTileset = nullptr;
						const UINT& tileGID = currentTile.GetGID();

						// 1.2. Get the tileset to this ID!
						for (auto& tilesetentry : tilesets)
						{
							if (tilesetentry.IsGIDInTilesetRange(tileGID))
							{
								usedTileset = &tilesetentry;
							}
						}

						// 1.3.  Set Vertexbuffer Streamsource (The Stream where the Draw Functions gets its Data).
						result = d3dDevice->SetStreamSource(0,								// StreamNumber 
															  currentTile.GetVertexBuffer(),	// The Vertexbuffer to be bound to the Stream	
															  0,								// Offset in the Vertexbuffer (unused here).
															  sizeof(Vertex));					// the size for one Vertex.
						if (FAILED(result))
						{
							char buffer[256];
							sprintf(buffer, "Graphics.RenderTileLayers(): Failed to set the Streamsource: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
							errorHandler->CreateFatalError(buffer);
							return;
						}

						// 1.4. Set the Indices (So the Draw Function knows which Indices you want to use).
						result = d3dDevice->SetIndices(currentTile.GetIndexBuffer());
						if (FAILED(result))
						{
							char buffer[256];
							sprintf(buffer, "Graphics.RenderTileLayers(): Failed to set the indices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
							errorHandler->CreateFatalError(buffer);
							return;
						}

						// 1.5. Set the WorldMatrix
						D3DXMATRIX WorldMatrix;
						WorldMatrix = (currentTile.GetScalingMatrix() * currentTile.GetRotationMatrix()) * currentTile.GetTranslationMatrix();

						result = d3dDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
						if (FAILED(result))
						{
							char buffer[256];
							sprintf(buffer, "Graphics.RenderTileLayers(): Failed to set the WorldMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
							errorHandler->CreateFatalError(buffer);
							return;
						}

						// 1.6. Set the Texture:
						// Only set the Texture if the current used texture is not the same!
						if (usedTileset->GetTexture() != activeTexture)
						{
							result = d3dDevice->SetTexture(0, usedTileset->GetTexture());
							if (FAILED(result))
							{
								char buffer[256];
								sprintf(buffer, "Graphics.RenderTileLayers(): Failed to set the Texture: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
								errorHandler->CreateFatalError(buffer);
								return;
							}

							activeTexture = usedTileset->GetTexture();
						}


						// 1.7. Draw the Primitive
						result = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,			// How to interpret the Data (interpret as single triangles, set of 3 vertices).
																  0,							// Basisindex for the Indices (usually 0)
																  0,							// The smallest Index used
																  4,							// Difference +1 between smallest and highest index used (e.g. Vertexnumber).
																  0,							// The index DirectX beginns to draw (here 0, beginn at first index)
																  2);							// How many primitives have to be drawn
						if (FAILED(result))
						{
							char buffer[256];
							sprintf(buffer, "Graphics.RenderTileLayers(): Failed to draw the IndexedPrimitive: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
							errorHandler->CreateFatalError(buffer);
							return;
						}

						renderedTiles++;
						usedTileset = nullptr;
					}
				}
			}
		}
	}
}
void Graphics::RenderImageLayers(const std::vector<Static_Sprite>& imageLayer)
{
	HRESULT result;

	if (imageLayer.size() > 0)
	{
		for (auto& imageentry : imageLayer)
		{
			// 2.1. Set Vertexbuffer Streamsource (The Stream where the Draw Functions gets its Data).
			result = d3dDevice->SetStreamSource(0,								// StreamNumber 
												  imageentry.GetVertexBuffer(),		// The Vertexbuffer to be bound to the Stream	
												  0,								// Offset in the Vertexbuffer (unused here).
												  sizeof(Vertex));					// the size for one Vertex.
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderImageLayers(): Failed to set the Streamsource: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 2.2. Set the Indices (So the Draw Function knows which Indices you want to use).
			result = d3dDevice->SetIndices(imageentry.GetIndexBuffer());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderImageLayers(): Failed to set the indices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 2.3. Set the WorldMatrix
			D3DXMATRIX WorldMatrix;
			WorldMatrix = (imageentry.GetScalingMatrix() * imageentry.GetRotationMatrix()) * imageentry.GetTranslationMatrix();

			result = d3dDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderImageLayers(): Failed to set the WorldMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 2.4. Set the Texture:
			// Only set the Texture if the current used texture is not the same!
			LPDIRECT3DTEXTURE9 wantedTexture = imageentry.GetTexture();
			if (wantedTexture != activeTexture)
			{
				result = d3dDevice->SetTexture(0, wantedTexture);
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Graphics.RenderImageLayers(): Failed to set the Texture: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}
				activeTexture = wantedTexture;
			}


			// 2.5. Draw the Primitive
			result = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,			// How to interpret the Data (interpret as single triangles, set of 3 vertices).
													   0,							// Basisindex for the Indices (usually 0)
													   0,							// The smallest Index used
													   4,							// Difference +1 between smallest and highest index used (e.g. Vertexnumber).
													   0,							// The index DirectX beginns to draw (here 0, beginn at first index)
													   2);							// How many primitives have to be drawn
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderImageLayers(): Failed to draw the IndexedPrimitive: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
		}
	}
}
void Graphics::RenderObjectLayers(const std::vector<Animated_Sprite*>& objectLayer)
{
	// Data
	HRESULT result;

	if (objectLayer.size() > 0)
	{
		for (auto entry : objectLayer)
		{
			//if (!entry->IsInitialized())
			//{
			//	// 1st: It is not initialized, so you can't draw it!
			//	return;
			//}

			// 2nd: Set Vertexbuffer Streamsource (The Stream where the Draw Functions gets its Data).
			result = d3dDevice->SetStreamSource(0,								// StreamNumber 
												  entry->GetVertexBuffer(),			// The Vertexbuffer to be bound to the Stream	
												  0,								// Offset in the Vertexbuffer (unused here).
												  sizeof(Vertex));					// the size for one Vertex.
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderObjectLayers(): Failed to set the Streamsource: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 3rd: Set the Indices (So the Draw Function knows which Indices you want to use).
			result = d3dDevice->SetIndices(entry->GetIndexBuffer());
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderObjectLayers(): Failed to set the indices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 4th: Set the WorldMatrix
			D3DXMATRIX WorldMatrix;
			WorldMatrix = (entry->GetScalingMatrix() * entry->GetRotationMatrix()) * entry->GetTranslationMatrix();

			result = d3dDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderObjectLayers(): Failed to set the WorldMatrix %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 5th: Set the Texture:
			// Only set the Texture if the current used texture is not the same!
			LPDIRECT3DTEXTURE9 wantedTexture = entry->GetTexture();
			if (wantedTexture != activeTexture)
			{
				result = d3dDevice->SetTexture(0, wantedTexture);
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Graphics.RenderObjectLayers(): Failed to set the Texture: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}
				activeTexture = wantedTexture;
			}


			// 6th: Draw the Primitive
			result = d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,			// How to interpret the Data (interpret as single triangles, set of 3 vertices).
													   0,							// Basisindex for the Indices (usually 0)
													   0,							// The smallest Index used
													   4,							// Difference +1 between smallest and highest index used (e.g. Vertexnumber).
													   0,							// The index DirectX beginns to draw (here 0, beginn at first index)
													   2);							// How many primitives have to be drawn
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Graphics.RenderObjectLayers(): Failed to draw the IndexedPrimitive: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}
		}
	}
}
// Getters
LPDIRECT3DDEVICE9* Graphics::GetDevice()
{
	return &d3dDevice;
}
OrthogonalCamera& Graphics::GetActiveCamera()
{
	return activeCamera;
}
OrthogonalCamera& Graphics::GetDebugCamera()
{
	return debugCamera;
}


// Setters
void Graphics::SetCamera(OrthogonalCamera& camera)
{
	if (camera.IsInitialized())
	{
		// 1st: Set the Member:
		activeCamera.Initialize(camera);
	}
}

// Font Stuff
void Graphics::InitializeFont(const std::string& font, const DWORD& fontSize, const bool& italic, const bool& bold)
{
	this->font.Initialize(d3dDevice, font, fontSize, italic, bold);
}
void Graphics::DrawText(const std::string string, const int& x, const int& y, const D3DCOLOR& color)
{
	FontDraw temp;
	temp.color = color;
	temp.x = x;
	temp.y = y;
	temp.string = string;
	TextDrawQueue.push_back(temp);
}

// Camera Functions:
void Graphics::ToggleDebugCam()
{
	useDebugCam = !useDebugCam;

	// Set the position of the DebugCamera to the current Position 
	// of the normal Camera, if you switch from normal to debug
	if (useDebugCam)
	{
		debugCamera.SetPosition({ activeCamera.GetPos().x, activeCamera.GetPos().y});
	}

}
BOOL Graphics::isDebugCamActive()
{
	return useDebugCam;
}

// Private functions:
void Graphics::SetDeviceStandarts()
{
	HRESULT result;

	// 1st: Set the FVF
	result = d3dDevice->SetFVF(Vertex::FVF);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the FVF: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 2nd: Set the Texture Sampler States
	result = d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture MinFilter: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture MagFilter: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture MIPFilter: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture MIPFilter: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	result = d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture AddressU Sampler: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	result = d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Texture AddressV Sampler: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 3rd: Set the Renderstates:
	result = d3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			// Solid Rendering.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Fillmode Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// No Lighting.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Lighting Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			// No Culling (if you want to rotate sprites,
																				// you NEED to disable this!).
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Culling Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);				// Use Dithering (Color Interpolation).
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Dither Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Alpha Blending States
	result = d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// Use Alpha-Blending
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Alphablending Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingSource Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingDestination Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingDestination Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4.4. Set Texture Stage States for Alpha Blending
	result = d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingDestination Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingDestination Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the AlphablendingDestination Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 5th: Disable DepthTests
	result = d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);				// Don't use Z-Buffer (not needed for 2D Graphics).
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Dither Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// No Writing to Z-Buffer
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Dither Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = d3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);				// Don't use Stencil-Buffer (not needed for 2D Graphics).
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Graphic.Initialize(): Failed to Set the Dither Renderstate: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	return;
}

