/* ***************************************************************************
 * uwp_app.cpp -- UWP app implementation, responsible for initializing the 
 * related drivers for LCUI.
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
 * uwp_app.cpp -- UWP 应用的实现，负责为 LCUI 初始化相关驱动程序
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
#include "uwp_app.h"
#include <LCUI/platform.h>
#include <LCUI/cursor.h>
#include <LCUI/ime.h>
#include LCUI_APP_H

using namespace LCUICore;

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

static struct UWPAppModule {
	App^ core;
	LCUI::Application *app;
	Direct3DApplicationSource^ source;
	void (*update)(LCUI_Surface);
	void (*present)(LCUI_Surface);
} UWPApp;

static void UWPApp_ProcessEvents( void )
{
	UWPApp.core->ProcessEvents();
}

static int UWPApp_BindSysEvent( int type, LCUI_EventFunc func,
				void *data, void( *destroy_data )(void*) )
{
	return -1;
}

static int UWPApp_UnbindSysEvent( int type, LCUI_EventFunc func )
{
	return -1;
}

static int UWPApp_UnbindSysEvent2( int handler_id )
{
	return -1;
}

static void *UWPApp_GetData( void )
{
	return (void*)UWPApp.core;
}

static LCUI_AppDriver LCUI_CreateUWPAppDriver( App^ app )
{
	ASSIGN( driver, LCUI_AppDriver );
	driver->BindSysEvent = UWPApp_BindSysEvent;
	driver->UnbindSysEvent = UWPApp_UnbindSysEvent;
	driver->UnbindSysEvent2 = UWPApp_UnbindSysEvent2;
	driver->ProcessEvents = UWPApp_ProcessEvents;
	driver->GetData = UWPApp_GetData;
	UWPApp.core = app;
	return driver;
}

static void UWPDisplay_Update( LCUI_Surface surface )
{
	UWPApp.update( surface );
	UWPApp.core->Update();
}

static void UWPDisplay_Present( LCUI_Surface surface )
{
	UWPApp.present( surface );
	UWPApp.core->Present();
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App();
}

App::App() :
	m_windowClosed(false),
	m_windowVisible(true)
{

}

// 创建 IFrameworkView 时调用的第一个方法。
void App::Initialize(CoreApplicationView^ applicationView)
{
	// 注册应用程序生命周期的事件处理程序。此示例包括 Activated，因此我们
	// 可激活 CoreWindow 并开始在窗口上渲染。
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &App::OnResuming);

	// 此时，我们具有访问设备的权限。
	// 我们可创建与设备相关的资源。
	m_deviceResources = std::make_shared<DX::DeviceResources>();
}

// 创建(或重新创建) CoreWindow 对象时调用。
void App::SetWindow( CoreWindow^ window )
{
	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>( this, &App::OnWindowSizeChanged );

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>( this, &App::OnVisibilityChanged );

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>( this, &App::OnWindowClosed );

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::OnDpiChanged );

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::OnOrientationChanged );

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::OnDisplayContentsInvalidated );

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>( this, &App::OnPointerPressed );
	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>( this, &App::OnPointerMoved );
	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>( this, &App::OnPointerReleased );
	window->PointerWheelChanged +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>( this, &App::OnPointerWheelChanged );
	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>( this, &App::OnKeyUp );
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>( this, &App::OnKeyDown );

	m_deviceResources->SetWindow( window );
}

void App::OnPointerPressed(CoreWindow^ window, PointerEventArgs^ args)
{
	m_inputDriver->OnPointerPressed( window, args );
}

void App::OnPointerMoved(CoreWindow^ window, PointerEventArgs^ args)
{
	m_inputDriver->OnPointerMoved( window, args );
}

void App::OnPointerReleased(CoreWindow^ window, PointerEventArgs^ args)
{
	m_inputDriver->OnPointerReleased( window, args );
}

void App::OnPointerWheelChanged(CoreWindow^ window, PointerEventArgs^ args)
{
	m_inputDriver->OnPointerWheelChanged( window, args );
}

void App::OnKeyDown( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args )
{
	m_inputDriver->OnKeyDown( sender, args );
}

void App::OnKeyUp( Windows::UI::Core::CoreWindow^ sender,Windows::UI::Core:: KeyEventArgs^ args )
{
	m_inputDriver->OnKeyUp( sender, args );
}

// 初始化场景资源或加载之前保存的应用程序状态。
void App::Load( Platform::String^ entryPoint )
{
	if( m_main ) {
		return;
	}

	m_displayDriver = LCUI_CreateUWPDisplay();
	m_inputDriver = std::unique_ptr<InputDriver>( new InputDriver );
	m_main = std::unique_ptr<Main>( new Main( m_deviceResources ) );
	m_appDriver = LCUI_CreateUWPAppDriver( this );
	UWPApp.update = m_displayDriver->update;
	UWPApp.present = m_displayDriver->present;
	m_displayDriver->update = UWPDisplay_Update;
	m_displayDriver->present = UWPDisplay_Present;

	LCUI_InitBase();
	LCUI_InitApp( m_appDriver );
	LCUI_InitDisplay( m_displayDriver );
	LCUI_InitIME();
	LCUI_InitCursor();
	m_inputDriver->RegisterIME();
	m_inputDriver->SelectIME();
	UWPApp.app->Load( entryPoint );
}

void App::Present()
{
	if( m_windowVisible ) {
		if( m_main->Render() ) {
			m_deviceResources->Present();
		}
	}
}

void App::Update()
{
	if( m_windowVisible ) {
		m_main->Update();
	}
}

void App::ProcessEvents()
{
	if( m_windowClosed ) {
		LCUI_Quit();
		return;
	}
	auto dispathcer = CoreWindow::GetForCurrentThread()->Dispatcher;
	if( m_windowVisible ) {
		dispathcer->ProcessEvents( CoreProcessEventsOption::ProcessAllIfPresent );
	} else {
		dispathcer->ProcessEvents( CoreProcessEventsOption::ProcessOneAndAllPending );
	}
}

// 将在窗口处于活动状态后调用此方法。
void App::Run()
{
	UWPApp.app->Run();
}

// IFrameworkView 所必需的。
// 终止事件不会导致调用 Uninitialize。如果在应用程序在前台运行时销毁 IFrameworkView
// 类，则将调用该方法。
void App::Uninitialize()
{
}

// 应用程序生命周期事件处理程序。

void App::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() 在 CoreWindow 激活前将不会开始。
	CoreWindow::GetForCurrentThread()->Activate();
}

void App::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// 在请求延期后异步保存应用程序状态。保留延期
	// 表示应用程序正忙于执行挂起操作。
	// 请注意，延期不是无限期的。在大约五秒后，
	// 将强制应用程序退出。
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		m_deviceResources->Trim();

		// 在此处插入代码。

		deferral->Complete();
	});
}

void App::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// 还原在挂起时卸载的任何数据或状态。默认情况下，
	// 在从挂起中恢复时，数据和状态会持续保留。请注意，
	// 如果之前已终止应用程序，则不会发生此事件。

	// 在此处插入代码。
}

// 窗口事件处理程序。

void App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	m_deviceResources->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	m_main->CreateWindowSizeDependentResources();
}

void App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void App::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation 事件处理程序。

void App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// 注意: 在此处检索到的 LogicalDpi 值可能与应用的有效 DPI 不匹配
	// 如果正在针对高分辨率设备对它进行缩放。在 DeviceResources 上设置 DPI 后，
	// 应始终使用 GetDpi 方法进行检索。
	// 有关详细信息，请参阅 DeviceResources.cpp。
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	m_deviceResources->ValidateDevice();
}

void LCUI::Initialize()
{
	UWPApp.source = ref new Direct3DApplicationSource();
}

LCUI::Application::Application()
{

}

void LCUI::Application::Load( Platform::String^ entryPoint )
{

}

void LCUI::Application::Run()
{
	LCUI_Main();
}

void LCUI::Run( LCUI::Application &app )
{
	UWPApp.app = &app;
	CoreApplication::Run( UWPApp.source );
}
