#pragma once
#include <d3d11.h>

class SwapChain;

class DeviceContext;

class GraphicsEngine
{
public:
	// Initialize the GraphicsEngine and DX11 Device
	bool init();
	// Release all the resources loaded
	bool release();
	static GraphicsEngine* get();
public:
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
private:
	DeviceContext* m_immediate_device_context;
private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	friend class SwapChain;
};

