/* ***************************************************************************
 * uwp_renderer.cpp -- UWP display support for LCUI
 *
 * Copyright (C) 2017 by Liu Chao <lc-soft@live.cn>
 *
 * This file is part of the LCUI project, and may only be used, modified,
 * and distributed under the terms of the GPLv2.
 *
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *
 * The LCUI project is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 *
 * You should have received a copy of the GPLv2 along with this file. It is
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/

/* ****************************************************************************
 * uwp_renderer.cpp -- LCUI 的 UWP 版图形输出支持
 *
 * 版权所有 (C) 2017 归属于 刘超 <lc-soft@live.cn>
 *
 * 这个文件是 LCUI 项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和
 * 发布。
 *
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 *
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销
 * 性或特定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在 LICENSE 文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>.
 * ****************************************************************************/

#include "pch.h"
#define LCUI_SURFACE_C
#include "uwp_renderer.h"
#include "Common/DirectXHelper.h"
#include <string.h>

using namespace LCUICore;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;

static struct UWPDisplay {
	LCUI_BOOL is_inited;
	Renderer *renderer;
	LCUI_EventTrigger trigger;
	LCUI_Surface surface;
	LCUI_Graph frame;
	LCUI_Rect rect;
} display = { 0 };

typedef struct LCUI_SurfaceRec_ {
	UWPDisplay *display;
	LCUI_BOOL is_updated;
} LCUI_SurfaceRec;

void UpdateSurfaceSize( void );

Renderer::Renderer( const std::shared_ptr<DX::DeviceResources>& deviceResources ) :
	m_deviceResources( deviceResources )
{
	m_bmp = nullptr;
	m_backBmp = nullptr;
	m_frameSwapable = false;
	m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock( &m_stateBlock );
	display.renderer = this;
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Renderer::Update( DX::StepTimer const& timer )
{

}

void Renderer::Render()
{
	D2D1_RECT_F rect;
	Size logicalSize = m_deviceResources->GetLogicalSize();
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	context->SaveDrawingState( m_stateBlock.Get() );
	context->BeginDraw();
	SwapFrames();
	rect.left = rect.top = 0;
	rect.right = (FLOAT)m_frameSize.width;
	rect.bottom = (FLOAT)m_frameSize.height;
	context->DrawBitmap( m_bmp.Get(), &rect, 1.0f, 
			     D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR );

	// 此处忽略 D2DERR_RECREATE_TARGET。此错误指示该设备
	// 丢失。将在下一次调用 Present 时对其进行处理。
	HRESULT hr = context->EndDraw();
	if( hr != D2DERR_RECREATE_TARGET ) {
		DX::ThrowIfFailed( hr );
	}

	context->RestoreDrawingState( m_stateBlock.Get() );
}

void Renderer::SwapFrames()
{
	D2D1_RECT_F rect;
	if( !m_frameSwapable ) {
		return;
	}
	m_frameSwapable = false;
	rect.left = (FLOAT)display.rect.x;
	rect.top = (FLOAT)display.rect.y;
	rect.right = rect.left + (FLOAT)display.rect.width;
	rect.bottom = rect.top + (FLOAT)display.rect.height;
	auto pos = D2D1::Point2U( (UINT)rect.left, (UINT)rect.top );
	auto readRect = D2D1::RectU( 
		(UINT)rect.left, (UINT)rect.top,
		(UINT)rect.right, (UINT)rect.bottom
	);
	m_backBmp->CopyFromMemory( &readRect, display.frame.bytes,
				   display.frame.bytes_per_row );
	m_bmp->CopyFromBitmap( &pos, m_backBmp.Get(), &readRect );
}

void Renderer::CreateDeviceDependentResources()
{

}

void Renderer::CreateWindowSizeDependentResources()
{
	float dpi = m_deviceResources->GetDpi();
	Size outputSize = m_deviceResources->GetOutputSize();
	auto context = m_deviceResources->GetD2DDeviceContext();
	D2D1_BITMAP_PROPERTIES1 props = {
		{
			DXGI_FORMAT_B8G8R8A8_UNORM, 
			D2D1_ALPHA_MODE_IGNORE
		},
		dpi, dpi, D2D1_BITMAP_OPTIONS_NONE, 0
	};
	m_frameSwapable = true;
	m_frameSize = { (UINT32)outputSize.Width, (UINT32)outputSize.Height };
	Graph_Create( &display.frame, (int)m_frameSize.width, (int)m_frameSize.height );
	Graph_FillRect( &display.frame, RGB( 255, 255, 255 ), NULL, TRUE );
	context->CreateBitmap( m_frameSize, nullptr, 0, &props, &m_bmp );
	context->CreateBitmap( m_frameSize, nullptr, 0, &props, &m_backBmp );
	UpdateSurfaceSize();
}

void Renderer::ReleaseDeviceDependentResources()
{
	
}

static void UpdateSurfaceSize( void )
{
	LCUI_DisplayEventRec dpy_ev;
	if( !display.surface || !display.renderer ) {
		return;
	}
	dpy_ev.type = DET_RESIZE;
	dpy_ev.surface = display.surface;
	dpy_ev.resize.width = (int)display.renderer->m_frameSize.width;
	dpy_ev.resize.height = (int)display.renderer->m_frameSize.height;
	LCUIDisplay_SetSize( dpy_ev.resize.width, dpy_ev.resize.height );
	EventTrigger_Trigger( display.trigger, DET_RESIZE, &dpy_ev );
	LCUIDisplay_InvalidateArea( NULL );
}

static int UWPDisplay_GetWidth( void )
{
	return (int)display.renderer->m_frameSize.width;
}

static int UWPDisplay_GetHeight( void )
{
	return (int)display.renderer->m_frameSize.height;
}

static int UWPDisplay_BindEvent( int event_id, LCUI_EventFunc func,
				 void *data, void( *destroy_data )(void*) )
{
	return EventTrigger_Bind( display.trigger, event_id, func,
				  data, destroy_data );
}

static LCUI_Surface UWPSurface_Create( void )
{
	LCUI_Surface surface = NEW( LCUI_SurfaceRec, 1 );
	surface->is_updated = FALSE;
	surface->display = &display;
	display.surface = surface;
	return surface;
}

static void UWPSurface_Delete( LCUI_Surface surface )
{
	free( surface );
}

static LCUI_BOOL UWPSurface_IsReady( LCUI_Surface surface )
{
	return TRUE;
}

static void UWPSurface_Show( LCUI_Surface surface )
{

}

static void UWPSurface_Hide( LCUI_Surface surface )
{

}

static void UWPSurface_Move( LCUI_Surface surface, int x, int y )
{

}

static void UWPSurface_Resize( LCUI_Surface surface, int width, int height )
{

}

static void UWPSurface_Update( LCUI_Surface surface )
{
	if( !surface->is_updated ) {
		UpdateSurfaceSize();
		surface->is_updated = TRUE;
	}
}

static void UWPSurface_Present( LCUI_Surface surface )
{
	display.renderer->m_frameSwapable = true;
}

static void UWPSurface_SetCaptionW( LCUI_Surface surface, const wchar_t *title )
{

}

static void UWPSurface_SetRenderMode( LCUI_Surface surface, int mode )
{

}

static LCUI_PaintContext UWPSurface_BeginPaint( LCUI_Surface surface, LCUI_Rect *rect )
{
	ASSIGN( paint, LCUI_PaintContext );
	paint->rect = *rect;
	paint->with_alpha = FALSE;
	Graph_Init( &paint->canvas );
	LCUIRect_MergeRect( &display.rect, &display.rect, rect );
	LCUIRect_ValidateArea( &paint->rect, UWPDisplay_GetWidth(), 
			       UWPDisplay_GetHeight() );
	Graph_Quote( &paint->canvas, &display.frame, &paint->rect );
	Graph_FillRect( &paint->canvas, RGB( 255, 255, 255 ), NULL, TRUE );
	return paint;
}

static void UWPSurface_EndPaint( LCUI_Surface surface, LCUI_PaintContext paint )
{
	free( paint );
}

LCUI_DisplayDriver LCUI_CreateUWPDisplay( void )
{
	ASSIGN( driver, LCUI_DisplayDriver );
	strcpy( driver->name, "UWP Display" );
	driver->getWidth = UWPDisplay_GetWidth;
	driver->getHeight = UWPDisplay_GetHeight;
	driver->create = UWPSurface_Create;
	driver->destroy = UWPSurface_Delete;
	driver->isReady = UWPSurface_IsReady;
	driver->show = UWPSurface_Show;
	driver->hide = UWPSurface_Hide;
	driver->move = UWPSurface_Move;
	driver->resize = UWPSurface_Resize;
	driver->update = UWPSurface_Update;
	driver->present = UWPSurface_Present;
	driver->setCaptionW = UWPSurface_SetCaptionW;
	driver->setRenderMode = UWPSurface_SetRenderMode;
	driver->setOpacity = NULL;
	driver->getHandle = NULL;
	driver->beginPaint = UWPSurface_BeginPaint;
	driver->endPaint = UWPSurface_EndPaint;
	driver->bindEvent = UWPDisplay_BindEvent;
	Graph_Init( &display.frame );
	display.frame.color_type = COLOR_TYPE_ARGB;
	display.surface = NULL;
	display.trigger = EventTrigger();
	display.is_inited = TRUE;
	return driver;
}
