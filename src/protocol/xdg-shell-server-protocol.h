/* 
 * Copyright © 2008-2013 Kristian Høgsberg
 * Copyright © 2013      Rafael Antognolli
 * Copyright © 2013      Jasper St. Pierre
 * Copyright © 2010-2013 Intel Corporation
 * 
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * the copyright holders not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied
 * warranty.
 * 
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

#ifndef XDG_SHELL_SERVER_PROTOCOL_H
#define XDG_SHELL_SERVER_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-server.h"

struct wl_client;
struct wl_resource;

struct xdg_shell;
struct xdg_surface;
struct xdg_popup;

extern const struct wl_interface xdg_shell_interface;
extern const struct wl_interface xdg_surface_interface;
extern const struct wl_interface xdg_popup_interface;

#ifndef XDG_SHELL_VERSION_ENUM
#define XDG_SHELL_VERSION_ENUM
/**
 * xdg_shell_version - latest protocol version
 * @XDG_SHELL_VERSION_CURRENT: Always the latest version
 *
 * The 'current' member of this enum gives the version of the protocol.
 * Implementations can compare this to the version they implement using
 * static_assert to ensure the protocol and implementation versions match.
 */
enum xdg_shell_version {
	XDG_SHELL_VERSION_CURRENT = 5,
};
#endif /* XDG_SHELL_VERSION_ENUM */

#ifndef XDG_SHELL_ERROR_ENUM
#define XDG_SHELL_ERROR_ENUM
enum xdg_shell_error {
	XDG_SHELL_ERROR_ROLE = 0,
};
#endif /* XDG_SHELL_ERROR_ENUM */

/**
 * xdg_shell - create desktop-style surfaces
 * @destroy: destroy xdg_shell
 * @use_unstable_version: enable use of this unstable version
 * @get_xdg_surface: create a shell surface from a surface
 * @get_xdg_popup: create a popup for a surface
 * @pong: respond to a ping event
 *
 * xdg_shell allows clients to turn a wl_surface into a "real window"
 * which can be dragged, resized, stacked, and moved around by the user.
 * Everything about this interface is suited towards traditional desktop
 * environments.
 */
struct xdg_shell_interface {
	/**
	 * destroy - destroy xdg_shell
	 *
	 * Destroy this xdg_shell object.
	 *
	 * Destroying a bound xdg_shell object while there are surfaces
	 * still alive with roles from this interface is illegal and will
	 * result in a protocol error. Make sure to destroy all surfaces
	 * before destroying this object.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * use_unstable_version - enable use of this unstable version
	 * @version: (none)
	 *
	 * Negotiate the unstable version of the interface. This
	 * mechanism is in place to ensure client and server agree on the
	 * unstable versions of the protocol that they speak or exit
	 * cleanly if they don't agree. This request will go away once the
	 * xdg-shell protocol is stable.
	 */
	void (*use_unstable_version)(struct wl_client *client,
				     struct wl_resource *resource,
				     int32_t version);
	/**
	 * get_xdg_surface - create a shell surface from a surface
	 * @id: (none)
	 * @surface: (none)
	 *
	 * This creates an xdg_surface for the given surface and gives it
	 * the xdg_surface role. See the documentation of xdg_surface for
	 * more details.
	 */
	void (*get_xdg_surface)(struct wl_client *client,
				struct wl_resource *resource,
				uint32_t id,
				struct wl_resource *surface);
	/**
	 * get_xdg_popup - create a popup for a surface
	 * @id: (none)
	 * @surface: (none)
	 * @parent: (none)
	 * @seat: the wl_seat of the user event
	 * @serial: the serial of the user event
	 * @x: (none)
	 * @y: (none)
	 *
	 * This creates an xdg_popup for the given surface and gives it
	 * the xdg_popup role. See the documentation of xdg_popup for more
	 * details.
	 *
	 * This request must be used in response to some sort of user
	 * action like a button press, key press, or touch down event.
	 */
	void (*get_xdg_popup)(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t id,
			      struct wl_resource *surface,
			      struct wl_resource *parent,
			      struct wl_resource *seat,
			      uint32_t serial,
			      int32_t x,
			      int32_t y);
	/**
	 * pong - respond to a ping event
	 * @serial: serial of the ping event
	 *
	 * A client must respond to a ping event with a pong request or
	 * the client may be deemed unresponsive.
	 */
	void (*pong)(struct wl_client *client,
		     struct wl_resource *resource,
		     uint32_t serial);
};

#define XDG_SHELL_PING	0

#define XDG_SHELL_PING_SINCE_VERSION	1

static inline void
xdg_shell_send_ping(struct wl_resource *resource_, uint32_t serial)
{
	wl_resource_post_event(resource_, XDG_SHELL_PING, serial);
}

#ifndef XDG_SURFACE_RESIZE_EDGE_ENUM
#define XDG_SURFACE_RESIZE_EDGE_ENUM
/**
 * xdg_surface_resize_edge - edge values for resizing
 * @XDG_SURFACE_RESIZE_EDGE_NONE: (none)
 * @XDG_SURFACE_RESIZE_EDGE_TOP: (none)
 * @XDG_SURFACE_RESIZE_EDGE_BOTTOM: (none)
 * @XDG_SURFACE_RESIZE_EDGE_LEFT: (none)
 * @XDG_SURFACE_RESIZE_EDGE_TOP_LEFT: (none)
 * @XDG_SURFACE_RESIZE_EDGE_BOTTOM_LEFT: (none)
 * @XDG_SURFACE_RESIZE_EDGE_RIGHT: (none)
 * @XDG_SURFACE_RESIZE_EDGE_TOP_RIGHT: (none)
 * @XDG_SURFACE_RESIZE_EDGE_BOTTOM_RIGHT: (none)
 *
 * These values are used to indicate which edge of a surface is being
 * dragged in a resize operation. The server may use this information to
 * adapt its behavior, e.g. choose an appropriate cursor image.
 */
enum xdg_surface_resize_edge {
	XDG_SURFACE_RESIZE_EDGE_NONE = 0,
	XDG_SURFACE_RESIZE_EDGE_TOP = 1,
	XDG_SURFACE_RESIZE_EDGE_BOTTOM = 2,
	XDG_SURFACE_RESIZE_EDGE_LEFT = 4,
	XDG_SURFACE_RESIZE_EDGE_TOP_LEFT = 5,
	XDG_SURFACE_RESIZE_EDGE_BOTTOM_LEFT = 6,
	XDG_SURFACE_RESIZE_EDGE_RIGHT = 8,
	XDG_SURFACE_RESIZE_EDGE_TOP_RIGHT = 9,
	XDG_SURFACE_RESIZE_EDGE_BOTTOM_RIGHT = 10,
};
#endif /* XDG_SURFACE_RESIZE_EDGE_ENUM */

#ifndef XDG_SURFACE_STATE_ENUM
#define XDG_SURFACE_STATE_ENUM
/**
 * xdg_surface_state - types of state on the surface
 * @XDG_SURFACE_STATE_MAXIMIZED: the surface is maximized
 * @XDG_SURFACE_STATE_FULLSCREEN: the surface is fullscreen
 * @XDG_SURFACE_STATE_RESIZING: (none)
 * @XDG_SURFACE_STATE_ACTIVATED: (none)
 *
 * The different state values used on the surface. This is designed for
 * state values like maximized, fullscreen. It is paired with the configure
 * event to ensure that both the client and the compositor setting the
 * state can be synchronized.
 *
 * States set in this way are double-buffered. They will get applied on the
 * next commit.
 *
 * Desktop environments may extend this enum by taking up a range of values
 * and documenting the range they chose in this description. They are not
 * required to document the values for the range that they chose. Ideally,
 * any good extensions from a desktop environment should make its way into
 * standardization into this enum.
 *
 * The current reserved ranges are:
 *
 * 0x0000 - 0x0FFF: xdg-shell core values, documented below. 0x1000 -
 * 0x1FFF: GNOME
 */
enum xdg_surface_state {
	XDG_SURFACE_STATE_MAXIMIZED = 1,
	XDG_SURFACE_STATE_FULLSCREEN = 2,
	XDG_SURFACE_STATE_RESIZING = 3,
	XDG_SURFACE_STATE_ACTIVATED = 4,
};
#endif /* XDG_SURFACE_STATE_ENUM */

/**
 * xdg_surface - A desktop window
 * @destroy: Destroy the xdg_surface
 * @set_parent: set the parent of this surface
 * @set_title: set surface title
 * @set_app_id: set application ID
 * @show_window_menu: show the window menu
 * @move: start an interactive move
 * @resize: start an interactive resize
 * @ack_configure: ack a configure event
 * @set_window_geometry: set the new window geometry
 * @set_maximized: (none)
 * @unset_maximized: (none)
 * @set_fullscreen: set the window as fullscreen on a monitor
 * @unset_fullscreen: (none)
 * @set_minimized: set the window as minimized
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide a desktop-style user interface.
 *
 * It provides requests to treat surfaces like windows, allowing to set
 * properties like maximized, fullscreen, minimized, and to move and resize
 * them, and associate metadata like title and app id.
 */
struct xdg_surface_interface {
	/**
	 * destroy - Destroy the xdg_surface
	 *
	 * Unmap and destroy the window. The window will be effectively
	 * hidden from the user's point of view, and all state like
	 * maximization, fullscreen, and so on, will be lost.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * set_parent - set the parent of this surface
	 * @parent: (none)
	 *
	 * Set the "parent" of this surface. This window should be
	 * stacked above a parent. The parent surface must be mapped as
	 * long as this surface is mapped.
	 *
	 * Parent windows should be set on dialogs, toolboxes, or other
	 * "auxilliary" surfaces, so that the parent is raised when the
	 * dialog is raised.
	 */
	void (*set_parent)(struct wl_client *client,
			   struct wl_resource *resource,
			   struct wl_resource *parent);
	/**
	 * set_title - set surface title
	 * @title: (none)
	 *
	 * Set a short title for the surface.
	 *
	 * This string may be used to identify the surface in a task bar,
	 * window list, or other user interface elements provided by the
	 * compositor.
	 *
	 * The string must be encoded in UTF-8.
	 */
	void (*set_title)(struct wl_client *client,
			  struct wl_resource *resource,
			  const char *title);
	/**
	 * set_app_id - set application ID
	 * @app_id: (none)
	 *
	 * Set an application identifier for the surface.
	 *
	 * The app ID identifies the general class of applications to which
	 * the surface belongs. The compositor can use this to group
	 * multiple applications together, or to determine how to launch a
	 * new application.
	 *
	 * See the desktop-entry specification [0] for more details on
	 * application identifiers and how they relate to well-known DBus
	 * names and .desktop files.
	 *
	 * [0] http://standards.freedesktop.org/desktop-entry-spec/
	 */
	void (*set_app_id)(struct wl_client *client,
			   struct wl_resource *resource,
			   const char *app_id);
	/**
	 * show_window_menu - show the window menu
	 * @seat: the wl_seat of the user event
	 * @serial: the serial of the user event
	 * @x: the x position to pop up the window menu at
	 * @y: the y position to pop up the window menu at
	 *
	 * Clients implementing client-side decorations might want to
	 * show a context menu when right-clicking on the decorations,
	 * giving the user a menu that they can use to maximize or minimize
	 * the window.
	 *
	 * This request asks the compositor to pop up such a window menu at
	 * the given position, relative to the local surface coordinates of
	 * the parent surface. There are no guarantees as to what menu
	 * items the window menu contains.
	 *
	 * This request must be used in response to some sort of user
	 * action like a button press, key press, or touch down event.
	 */
	void (*show_window_menu)(struct wl_client *client,
				 struct wl_resource *resource,
				 struct wl_resource *seat,
				 uint32_t serial,
				 int32_t x,
				 int32_t y);
	/**
	 * move - start an interactive move
	 * @seat: the wl_seat of the user event
	 * @serial: the serial of the user event
	 *
	 * Start an interactive, user-driven move of the surface.
	 *
	 * This request must be used in response to some sort of user
	 * action like a button press, key press, or touch down event.
	 *
	 * The server may ignore move requests depending on the state of
	 * the surface (e.g. fullscreen or maximized).
	 */
	void (*move)(struct wl_client *client,
		     struct wl_resource *resource,
		     struct wl_resource *seat,
		     uint32_t serial);
	/**
	 * resize - start an interactive resize
	 * @seat: the wl_seat of the user event
	 * @serial: the serial of the user event
	 * @edges: which edge or corner is being dragged
	 *
	 * Start a user-driven, interactive resize of the surface.
	 *
	 * This request must be used in response to some sort of user
	 * action like a button press, key press, or touch down event.
	 *
	 * The server may ignore resize requests depending on the state of
	 * the surface (e.g. fullscreen or maximized).
	 */
	void (*resize)(struct wl_client *client,
		       struct wl_resource *resource,
		       struct wl_resource *seat,
		       uint32_t serial,
		       uint32_t edges);
	/**
	 * ack_configure - ack a configure event
	 * @serial: the serial from the configure event
	 *
	 * When a configure event is received, if a client commits the
	 * surface in response to the configure event, then the client must
	 * make a ack_configure request before the commit request, passing
	 * along the serial of the configure event.
	 *
	 * The compositor might use this information to move a surface to
	 * the top left only when the client has drawn itself for the
	 * maximized or fullscreen state.
	 *
	 * If the client receives multiple configure events before it can
	 * respond to one, it only has to ack the last configure event.
	 */
	void (*ack_configure)(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t serial);
	/**
	 * set_window_geometry - set the new window geometry
	 * @x: (none)
	 * @y: (none)
	 * @width: (none)
	 * @height: (none)
	 *
	 * The window geometry of a window is its "visible bounds" from
	 * the user's perspective. Client-side decorations often have
	 * invisible portions like drop-shadows which should be ignored for
	 * the purposes of aligning, placing and constraining windows.
	 *
	 * Once the window geometry of the surface is set once, it is not
	 * possible to unset it, and it will remain the same until
	 * set_window_geometry is called again, even if a new subsurface or
	 * buffer is attached.
	 *
	 * If never set, the value is the full bounds of the surface,
	 * including any subsurfaces. This updates dynamically on every
	 * commit. This unset mode is meant for extremely simple clients.
	 *
	 * If responding to a configure event, the window geometry in here
	 * must respect the sizing negotiations specified by the states in
	 * the configure event.
	 *
	 * The width and height must be greater than zero.
	 */
	void (*set_window_geometry)(struct wl_client *client,
				    struct wl_resource *resource,
				    int32_t x,
				    int32_t y,
				    int32_t width,
				    int32_t height);
	/**
	 * set_maximized - (none)
	 */
	void (*set_maximized)(struct wl_client *client,
			      struct wl_resource *resource);
	/**
	 * unset_maximized - (none)
	 */
	void (*unset_maximized)(struct wl_client *client,
				struct wl_resource *resource);
	/**
	 * set_fullscreen - set the window as fullscreen on a monitor
	 * @output: (none)
	 *
	 * Make the surface fullscreen.
	 *
	 * You can specify an output that you would prefer to be
	 * fullscreen. If this value is NULL, it's up to the compositor to
	 * choose which display will be used to map this surface.
	 */
	void (*set_fullscreen)(struct wl_client *client,
			       struct wl_resource *resource,
			       struct wl_resource *output);
	/**
	 * unset_fullscreen - (none)
	 */
	void (*unset_fullscreen)(struct wl_client *client,
				 struct wl_resource *resource);
	/**
	 * set_minimized - set the window as minimized
	 *
	 * Request that the compositor minimize your surface. There is no
	 * way to know if the surface is currently minimized, nor is there
	 * any way to unset minimization on this surface.
	 *
	 * If you are looking to throttle redrawing when minimized, please
	 * instead use the wl_surface.frame event for this, as this will
	 * also work with live previews on windows in Alt-Tab, Expose or
	 * similar compositor features.
	 */
	void (*set_minimized)(struct wl_client *client,
			      struct wl_resource *resource);
};

#define XDG_SURFACE_CONFIGURE	0
#define XDG_SURFACE_CLOSE	1

#define XDG_SURFACE_CONFIGURE_SINCE_VERSION	1
#define XDG_SURFACE_CLOSE_SINCE_VERSION	1

static inline void
xdg_surface_send_configure(struct wl_resource *resource_, int32_t width, int32_t height, struct wl_array *states, uint32_t serial)
{
	wl_resource_post_event(resource_, XDG_SURFACE_CONFIGURE, width, height, states, serial);
}

static inline void
xdg_surface_send_close(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, XDG_SURFACE_CLOSE);
}

#ifndef XDG_POPUP_ERROR_ENUM
#define XDG_POPUP_ERROR_ENUM
/**
 * xdg_popup_error - xdg_popup error values
 * @XDG_POPUP_ERROR_NOT_THE_TOPMOST_POPUP: The client tried to map or
 *	destroy a non-topmost popup
 * @XDG_POPUP_ERROR_INVALID_PARENT: The client specified an invalid
 *	parent surface
 *
 * These errors can be emitted in response to xdg_popup requests.
 */
enum xdg_popup_error {
	XDG_POPUP_ERROR_NOT_THE_TOPMOST_POPUP = 0,
	XDG_POPUP_ERROR_INVALID_PARENT = 1,
};
#endif /* XDG_POPUP_ERROR_ENUM */

/**
 * xdg_popup - short-lived, popup surfaces for menus
 * @destroy: remove xdg_popup interface
 *
 * A popup surface is a short-lived, temporary surface that can be used
 * to implement menus. It takes an explicit grab on the surface that will
 * be dismissed when the user dismisses the popup. This can be done by the
 * user clicking outside the surface, using the keyboard, or even locking
 * the screen through closing the lid or a timeout.
 *
 * When the popup is dismissed, a popup_done event will be sent out, and at
 * the same time the surface will be unmapped. The xdg_popup object is now
 * inert and cannot be reactivated, so clients should destroy it.
 * Explicitly destroying the xdg_popup object will also dismiss the popup
 * and unmap the surface.
 *
 * Clients will receive events for all their surfaces during this grab
 * (which is an "owner-events" grab in X11 parlance). This is done so that
 * users can navigate through submenus and other "nested" popup windows
 * without having to dismiss the topmost popup.
 *
 * Clients that want to dismiss the popup when another surface of their own
 * is clicked should dismiss the popup using the destroy request.
 *
 * The parent surface must have either an xdg_surface or xdg_popup role.
 *
 * Specifying an xdg_popup for the parent means that the popups are nested,
 * with this popup now being the topmost popup. Nested popups must be
 * destroyed in the reverse order they were created in, e.g. the only popup
 * you are allowed to destroy at all times is the topmost one.
 *
 * If there is an existing popup when creating a new popup, the parent must
 * be the current topmost popup.
 *
 * A parent surface must be mapped before the new popup is mapped.
 *
 * When compositors choose to dismiss a popup, they will likely dismiss
 * every nested popup as well.
 *
 * The x and y arguments specify where the top left of the popup should be
 * placed, relative to the local surface coordinates of the parent surface.
 */
struct xdg_popup_interface {
	/**
	 * destroy - remove xdg_popup interface
	 *
	 * This destroys the popup. Explicitly destroying the xdg_popup
	 * object will also dismiss the popup, and unmap the surface.
	 *
	 * If this xdg_popup is not the "topmost" popup, a protocol error
	 * will be sent.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
};

#define XDG_POPUP_POPUP_DONE	0

#define XDG_POPUP_POPUP_DONE_SINCE_VERSION	1

static inline void
xdg_popup_send_popup_done(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, XDG_POPUP_POPUP_DONE);
}

#ifdef  __cplusplus
}
#endif

#endif
