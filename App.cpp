#include "pch.h"

#include <CLRMain.h>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;

using namespace CLR;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const& applicationView)
    {
        applicationView.Activated({ this, &App::OnActivated });

        CoreApplication::Suspending({ this, &App::OnSuspending });

        CoreApplication::Resuming({ this, &App::OnResuming });
    }

    void Load(hstring const&)
    {
        if (main == nullptr)
        {
            main = winrt::make_self<CLRMain>();
        }
    }

    void Uninitialize()
    {
    }

    void Run()
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();

        CoreDispatcher dispatcher = window.Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
    }

    void SetWindow(CoreWindow const& window)
    {
        Compositor compositor;
        ContainerVisual root = compositor.CreateContainerVisual();
        m_target = compositor.CreateTargetForCurrentView();
        m_target.Root(root);
        m_visuals = root.Children();

        window.PointerPressed({ this, &App::OnPointerPressed });
        window.PointerMoved({ this, &App::OnPointerMoved });

        window.PointerReleased([&](auto && ...)
        {
            m_selected = nullptr;
        });
    }

    void OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
    {
        float2 const point = args.CurrentPoint().Position();

        for (Visual visual : m_visuals)
        {
            float3 const offset = visual.Offset();
            float2 const size = visual.Size();

            if (point.x >= offset.x &&
                point.x < offset.x + size.x &&
                point.y >= offset.y &&
                point.y < offset.y + size.y)
            {
                m_selected = visual;
                m_offset.x = offset.x - point.x;
                m_offset.y = offset.y - point.y;
            }
        }

        if (m_selected)
        {
            m_visuals.Remove(m_selected);
            m_visuals.InsertAtTop(m_selected);
        }
        else
        {
            AddVisual(point);
        }
    }

    void OnPointerMoved(IInspectable const&, PointerEventArgs const& args)
    {
        if (m_selected)
        {
            float2 const point = args.CurrentPoint().Position();

            m_selected.Offset(
                {
                    point.x + m_offset.x,
                    point.y + m_offset.y,
                    0.0f
                });
        }
    }

    void AddVisual(float2 const point)
    {
        Compositor compositor = m_visuals.Compositor();
        SpriteVisual visual = compositor.CreateSpriteVisual();

        static Color colors[] =
        {
            { 0xDC, 0x5B, 0x9B, 0xD5 },
            { 0xDC, 0xED, 0x7D, 0x31 },
            { 0xDC, 0x70, 0xAD, 0x47 },
            { 0xDC, 0xFF, 0xC0, 0x00 }
        };

        static unsigned last = 0;
        unsigned const next = ++last % _countof(colors);
        visual.Brush(compositor.CreateColorBrush(colors[next]));

        float const BlockSize = 100.0f;

        visual.Size(
            {
                BlockSize,
                BlockSize
            });

        visual.Offset(
            {
                point.x - BlockSize / 2.0f,
                point.y - BlockSize / 2.0f,
                0.0f,
            });

        m_visuals.InsertAtTop(visual);

        m_selected = visual;
        m_offset.x = -BlockSize / 2.0f;
        m_offset.y = -BlockSize / 2.0f;
    }

    void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();
    }

    winrt::fire_and_forget OnSuspending(IInspectable const& /* sender */, SuspendingEventArgs const& args)
    {
        //auto lifetime = get_strong();

        //// Save app state asynchronously after requesting a deferral. Holding a deferral
        //// indicates that the application is busy performing suspending operations. Be
        //// aware that a deferral may not be held indefinitely. After about five seconds,
        //// the app will be forced to exit.
        //SuspendingDeferral deferral = args.SuspendingOperation().GetDeferral();

        co_await winrt::resume_background();

        //m_deviceResources->Trim();

        //m_main->Suspend();

        //deferral.Complete();
    }

    void OnResuming(IInspectable const& /* sender */, IInspectable const& /* args */)
    {
        // Restore any data or state that was unloaded on suspend. By default, data
        // and state are persisted when resuming from suspend. Note that this event
        // does not occur if the app was previously terminated.
        //m_main->Resume();
    }

    CompositionTarget m_target{ nullptr };
    VisualCollection m_visuals{ nullptr };
    Visual m_selected{ nullptr };
    float2 m_offset{};

private:
    winrt::com_ptr<CLRMain> main;
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(make<App>());
}
