
/* Copyright (c) 2005-2017, Stefan Eilemann <eile@equalizergraphics.com>
 *                          Maxim Makhinya
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EQ_WGL_WINDOW_H
#define EQ_WGL_WINDOW_H

#include <eq/glWindow.h> // base class
#include <eq/os.h>
#include <eq/wgl/types.h>

namespace eq
{
namespace wgl
{
namespace detail
{
class Window;
}

/** The interface defining the minimum functionality for a WGL window. */
class WindowIF : public GLWindow
{
public:
    /** Construct a new WGL window for the given eq::Window. @version 1.7.2 */
    WindowIF(NotifierInterface& parent, const WindowSettings& settings)
        : GLWindow(parent, settings)
    {
    }

    /** Destruct the WGL window. @version 1.0 */
    virtual ~WindowIF() {}
    /** @return the WGL rendering context. @version 1.0 */
    EQ_API virtual HGLRC getWGLContext() const = 0;

    /** @return the Win32 window handle. @version 1.0 */
    EQ_API virtual HWND getWGLWindowHandle() const = 0;

    /**
     * @return the Win32 device context used for the current drawable.
     * @version 1.0
     */
    EQ_API virtual HDC getWGLDC() const = 0;

    /** @return the Win32 affinity device context, if used. @version 1.0 */
    virtual HDC getWGLAffinityDC() { return 0; }
    /** Process a (re)size event. @return true if the event was handled. */
    virtual bool processEvent(EventType type, SizeEvent& event)
    {
        return GLWindow::processEvent(type, event);
    }

    /** Process a mouse event. @return true if the event was handled. */
    virtual bool processEvent(EventType type, PointerEvent& event)
    {
        return GLWindow::processEvent(type, event);
    }

    /** Process a keyboard event. @return true if the event was handled. */
    virtual bool processEvent(EventType type, KeyEvent& event)
    {
        return GLWindow::processEvent(type, event);
    }

    /** Process an axis event. @return true if the event was handled. */
    virtual bool processEvent(AxisEvent& event)
    {
        return GLWindow::processEvent(event);
    }

    /** Process a button event. @return true if the event was handled. */
    virtual bool processEvent(ButtonEvent& event)
    {
        return GLWindow::processEvent(event);
    }

    /** Process a stateless event. @return true if the event was handled. */
    virtual bool processEvent(EventType) { return false; }
};

/** Equalizer default implementation of a WGL window */
class Window : public WindowIF
{
public:
    /** Create a new WGL window for the given eq::Window. @version 1.7.2 */
    EQ_API Window(NotifierInterface& parent, const WindowSettings& settings,
                  Pipe& pipe);

    /** Destruct the WGL window. @version 1.0 */
    EQ_API virtual ~Window();

    /** @name Data Access */
    /**
     * Set the WGL rendering context for this window.
     *
     * This function should only be called from configInit() or
     * configExit().
     * The context has to be set to 0 before it is destroyed.
     *
     * @param context the WGL rendering context.
     * @version 1.0
     */
    EQ_API virtual void setWGLContext(HGLRC context);

    //@{
    /**
     * Set the Win32 window handle for this window.
     *
     * This function should only be called from configInit() or
     * configExit().
     *
     * @param handle the window handle.
     * @version 1.0
     */
    EQ_API virtual void setWGLWindowHandle(HWND handle);

    /** @return the Win32 window handle. @version 1.0 */
    EQ_API virtual HWND getWGLWindowHandle() const;

    /**
     * @return the Win32 device context used for the current drawable.
     * @version 1.0
     */
    EQ_API virtual HDC getWGLDC() const;

    /** @return the WGL rendering context. @version 1.0 */
    EQ_API virtual HGLRC getWGLContext() const;

    /** @return the WGL event handler. @version 1.0 */
    EQ_API const EventHandler* getWGLEventHandler() const;
    //@}

    /** @name WGL/Win32 initialization */
    //@{
    /**
     * Initialize this window for the WGL window system.
     *
     * This method first calls initWGLAffinityDC, then chooses a pixel
     * format with chooseWGLPixelFormat, then creates a drawable using
     * configInitWGLDrawable and finally creates the context using
     * createWGLContext.
     *
     * @return true if the initialization was successful, false otherwise.
     * @version 1.0
     */
    EQ_API virtual bool configInit();

    /** @version 1.0 */
    EQ_API virtual void configExit();

    /**
     * Create, if needed, an affinity device context for this window.
     *
     * @return false on error, true otherwise
     * @version 1.0
     */
    EQ_API virtual bool initWGLAffinityDC();

    /** Destroy the affinity device context. @version 1.0 */
    EQ_API virtual void exitWGLAffinityDC();

    /** @return the affinity device context. @version 1.0 */
    EQ_API virtual HDC getWGLAffinityDC();

    /**
     * Create a device context for the display device of the window.
     *
     * The returned device context has to be released using DeleteDC. The
     * window's error message is set if an error occured.
     *
     * @return the DC, or 0 upon error.
     * @version 1.0
     */
    EQ_API virtual HDC createWGLDisplayDC();

    /**
     * Choose a pixel format based on the window's attributes.
     *
     * Uses the currently set DC (if any) and sets the chosen pixel format
     * on it.
     *
     * @return a pixel format, or 0 if no pixel format was found.
     * @version 1.0
     */
    EQ_API virtual int chooseWGLPixelFormat();

    /**
     * Initialize the window's drawable and bind the WGL context.
     *
     * Sets the window handle on success.
     *
     * @param pixelFormat the window's target pixel format.
     * @return true if the drawable was created, false otherwise.
     * @version 1.0
     */
    EQ_API virtual bool configInitWGLDrawable(int pixelFormat);

    /**
     * Initialize the window with an on-screen Win32 window.
     *
     * Sets the window handle on success.
     *
     * @param pixelFormat the window's target pixel format.
     * @return true if the drawable was created, false otherwise.
     * @version 1.0
     */
    EQ_API virtual bool configInitWGLWindow(int pixelFormat);

    /** Initialize the window for an off-screen FBO */
    EQ_API virtual bool configInitWGLFBO(int pixelFormat);

    /**
     * Create a WGL context.
     *
     * This method does not set the window's WGL context.
     *
     * @return the context, or 0 if context creation failed.
     * @version 1.0
     */
    EQ_API virtual HGLRC createWGLContext();

    /** Destroy the given WGL context. @version 1.0 */
    EQ_API virtual void destroyWGLContext(HGLRC context);

    /**
     * Set up an WGLEventHandler, called by setWGLWindowHandle().
     * @version 1.0
     */
    EQ_API virtual void initEventHandler();

    /**
     * Destroy the WGLEventHandler, called by setWGLWindowHandle().
     * @version 1.0
     */
    EQ_API virtual void exitEventHandler();
    //@}

    /** @name Operations. */
    //@{
    /** @version 1.0 */
    EQ_API virtual void makeCurrent(const bool cache = true) const;

    /** @version 1.10 */
    EQ_API virtual void doneCurrent() const;

    /** @version 1.0 */
    EQ_API virtual void swapBuffers();

    /** Join the WGL_NV_swap_group. @version 1.0 */
    EQ_API virtual void joinNVSwapBarrier(const uint32_t group,
                                          const uint32_t barrier);

    /** Unbind a WGL_NV_swap_barrier. @version 1.0 */
    void leaveNVSwapBarrier();

    EQ_API bool processEvent(EventType type, PointerEvent& event) override;
    EQ_API bool processEvent(EventType type) override;
    //@}

protected:
    /** @return the generic WGL function table for the window's pipe. */
    EQ_API WGLEWContext* wglewGetContext();

private:
    detail::Window* const _impl;

    /** Create an unmapped WGL window. */
    HWND _createWGLWindow(const PixelViewport& pvp);

    /** Use wglChoosePixelFormatARB */
    int _chooseWGLPixelFormatARB(HDC pfDC);

    /** Use ChoosePixelFormat */
    int _chooseWGLPixelFormat(HDC pfDC);

    void _initSwapSync();
    void _resize(const PixelViewport& pvp) override;
};
}
}
#endif // EQ_WGL_WINDOW_H
