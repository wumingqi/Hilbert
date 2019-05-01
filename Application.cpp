#include "Pch.h"
#include "Application.h"

void Application::InitializeResources()
{
	m_deviceResources = new DeviceResources(m_hWnd);
	m_deviceResources->CreateDeviceIndependentResources();
	m_deviceResources->CreateDeviceResources();
	m_d2dContext = m_deviceResources->GetD2DDeviceContext();

	m_gridColor = ColorF(0x666666);
	m_curvColor = ColorF(0xff2244);
	m_d2dContext->CreateSolidColorBrush(m_gridColor, &m_brush);

	m_HilbertN = 2;
	m_gridWidth = 720.f / m_HilbertN;
	GenerateHilbert();
	SetText();
}

void Application::Update()
{
	m_d2dContext = m_deviceResources->GetD2DDeviceContext();
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(ColorF(0xFFFFFF));
	//移动到中间
	UINT hilbertWidth = (UINT)(m_gridWidth * m_HilbertN);
	m_d2dContext->SetTransform(Matrix3x2F::Translation(
		(float)((int)(m_width - hilbertWidth) / 2),
		(float)((int)(m_height - hilbertWidth) / 2)
	));

	//画网格
	int rows = m_HilbertN;
	int cols = m_HilbertN;
	float width = cols * m_gridWidth;
	float height = rows * m_gridWidth;
	m_brush->SetColor(m_gridColor);
	for (int i = 0; i < rows + 1; i++)
	{
		m_d2dContext->DrawLine({ 0,m_gridWidth * i }, { width,m_gridWidth * i }, m_brush.Get(), 2);
	}
	for (int i = 0; i < cols + 1; i++)
	{
		m_d2dContext->DrawLine({ m_gridWidth * i,0 }, { m_gridWidth * i,height }, m_brush.Get(), 2);
	}

	//画曲线
	m_brush->SetColor(m_curvColor);

	for (UINT i = 0; i < m_points.size()-1; i++)
	{
		m_d2dContext->DrawLine(m_points[i], m_points[i + 1], m_brush.Get(), 2);
	}

	m_d2dContext->EndDraw();
	m_deviceResources->GetSwapChain()->Present(1, 0);
}

void Application::GenerateHilbert()
{
	int count = m_HilbertN * m_HilbertN;
	std::vector<Point> pts(count);
	Hilbert::Generate(m_HilbertN, pts.data());

	m_points.resize(count);
	for (int i = 0; i < count; i++)
	{
		m_points[i].x = pts[i].x * m_gridWidth + m_gridWidth / 2;
		m_points[i].y = pts[i].y * m_gridWidth + m_gridWidth / 2;
	}
}
