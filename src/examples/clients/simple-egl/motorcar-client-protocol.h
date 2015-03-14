#ifndef MOTORCAR_CLIENT_PROTOCOL_H
#define MOTORCAR_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

struct wl_client;
struct wl_resource;

struct motorcar_shell;
struct motorcar_surface;
struct motorcar_viewpoint;
struct motorcar_six_dof_pointer;

extern const struct wl_interface motorcar_shell_interface;
extern const struct wl_interface motorcar_surface_interface;
extern const struct wl_interface motorcar_viewpoint_interface;
extern const struct wl_interface motorcar_six_dof_pointer_interface;

#define MOTORCAR_SHELL_GET_MOTORCAR_SURFACE	0

static inline void
motorcar_shell_set_user_data(struct motorcar_shell *motorcar_shell, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) motorcar_shell, user_data);
}

static inline void *
motorcar_shell_get_user_data(struct motorcar_shell *motorcar_shell)
{
	return wl_proxy_get_user_data((struct wl_proxy *) motorcar_shell);
}

static inline void
motorcar_shell_destroy(struct motorcar_shell *motorcar_shell)
{
	wl_proxy_destroy((struct wl_proxy *) motorcar_shell);
}

static inline struct motorcar_surface *
motorcar_shell_get_motorcar_surface(struct motorcar_shell *motorcar_shell, struct wl_surface *surface, uint32_t clipping_mode, uint32_t enable_depth_compositing)
{
	struct wl_proxy *id;

	id = wl_proxy_marshal_constructor((struct wl_proxy *) motorcar_shell,
			 MOTORCAR_SHELL_GET_MOTORCAR_SURFACE, &motorcar_surface_interface, NULL, surface, clipping_mode, enable_depth_compositing);

	return (struct motorcar_surface *) id;
}

#ifndef MOTORCAR_SURFACE_CLIPPING_MODE_ENUM
#define MOTORCAR_SURFACE_CLIPPING_MODE_ENUM
/**
 * motorcar_surface_clipping_mode - clipping behavior used by the
 *	compositor for a surface
 * @MOTORCAR_SURFACE_CLIPPING_MODE_CUBOID: Use the cuboid clipping mode
 * @MOTORCAR_SURFACE_CLIPPING_MODE_PORTAL: Use the portal clipping mode
 *
 * The compositor clips the fragments composing the 3D window to lie
 * within its 3D window bounds. The behavior of this clipping mechanism
 * depends on the type of 3D window being clipped. This compositor supports
 * two clippling modes, called cuboid and portal clippling
 *
 * Cuboid clipping treats the 3D window as a box shaped region of the space
 * (defined by the motorcar_surface size_3d) and clips any fragments which
 * do not lie withing this box.
 *
 * Portal clipping treats the 3D window as a 2D opening which connects two
 * disjoint 3D spaces, much like a physical window. This drops any
 * fragments which lie outside the projection of the window opening
 * (defined by the first two dimensions of the motorcar_surface size_3d)
 * and any fragments which lie closer to the viewpoint than the window
 * opening
 */
enum motorcar_surface_clipping_mode {
	MOTORCAR_SURFACE_CLIPPING_MODE_CUBOID = 0,
	MOTORCAR_SURFACE_CLIPPING_MODE_PORTAL = 1,
};
#endif /* MOTORCAR_SURFACE_CLIPPING_MODE_ENUM */

/**
 * motorcar_surface - a 3D, view dependent, depth composited meta-data
 *	surface
 * @transform_matrix: sets the tranformation of the 3D window
 * @request_size_3d: requests that the client resize the 3D window to the
 *	given scale
 *
 * An interface that may be implemented by a wl_surface, for
 * implementations that provide motorcar style depth composited 3D surfaces
 *
 * A motorcar_surface can be created from an exisitng surface, and provides
 * the client with the information needed to draw its content in a way that
 * can be depth composited by a motorcar compliant compositor.
 *
 * A motorcar surface is handled in the compositor as a 3D analog of a
 * traditional window. Rather than the window being a 2 dimensional region
 * of a 2 dimensional interface space, the window represents a 3
 * dimensional region of a 3 dimensional interface space, essentially a 3D
 * box in which the client can draw 3D geometry
 *
 * The 3D window has its own 3D space whose origin is at the center of the
 * window. The window's position refers to the location of this origin in
 * the world space, and its rotation is around this point. The window also
 * has a 3D size, which is defined in its local space. Each component of
 * the size indicates one dimension of the window bounds, so the window
 * extends for one half this distance on each side of the origin in its
 * local space.
 */
struct motorcar_surface_listener {
	/**
	 * transform_matrix - sets the tranformation of the 3D window
	 * @transform: the new transform matrix
	 *
	 * This event is sent to indicate a change in the transform of
	 * the client's 3D window
	 *
	 * This matrix can be though of as the model matrix for the
	 * client's 3D window, which brings vectors from the window local
	 * space into world space.
	 *
	 * It is represented here as a column-major 4x4 matrix of 32 bit
	 * floats, with all values are specified in meters.
	 */
	void (*transform_matrix)(void *data,
				 struct motorcar_surface *motorcar_surface,
				 struct wl_array *transform);
	/**
	 * request_size_3d - requests that the client resize the 3D
	 *	window to the given scale
	 * @dimensions: the requested size vector
	 *
	 * Allows the compositor to request the window take a new size.
	 * Much like the traditional 2D size mechanisms, the compositor
	 * cannot set this explicitly, rather, it can only request that the
	 * client take a certain dimension, and the client then chooses any
	 * size which fits completely within the requested size and sends a
	 * resize event back to the compositor.
	 *
	 * The size is represented here as a vector of three 32-bit floats,
	 * representing the size of the 3D window along each of the
	 * cardinal axes in the window's local space.
	 */
	void (*request_size_3d)(void *data,
				struct motorcar_surface *motorcar_surface,
				struct wl_array *dimensions);
};

static inline int
motorcar_surface_add_listener(struct motorcar_surface *motorcar_surface,
			      const struct motorcar_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) motorcar_surface,
				     (void (**)(void)) listener, data);
}

#define MOTORCAR_SURFACE_SET_SIZE_3D	0

static inline void
motorcar_surface_set_user_data(struct motorcar_surface *motorcar_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) motorcar_surface, user_data);
}

static inline void *
motorcar_surface_get_user_data(struct motorcar_surface *motorcar_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) motorcar_surface);
}

static inline void
motorcar_surface_destroy(struct motorcar_surface *motorcar_surface)
{
	wl_proxy_destroy((struct wl_proxy *) motorcar_surface);
}

static inline void
motorcar_surface_set_size_3d(struct motorcar_surface *motorcar_surface, struct wl_array *dimensions)
{
	wl_proxy_marshal((struct wl_proxy *) motorcar_surface,
			 MOTORCAR_SURFACE_SET_SIZE_3D, dimensions);
}

/**
 * motorcar_viewpoint - represents a single viewpoint in the compositor,
 *	essentially a view and projection matrix
 * @view_matrix: sets the view matrix of this viewpoint for the next
 *	frame
 * @projection_matrix: change the projection matrix of this viewpoint
 * @view_port: sets the view ports of this viewpoint to be used when
 *	rendering into motorcar surface buffers
 *
 * This interface represents a viewpoint (essentially a virtual camera)
 * in the compositor's 3D compositing space, and the events it contains are
 * designed to keep the compositor and client viewpoints synced so that the
 * viewpoints in the client and compositor space stay consistent;
 */
struct motorcar_viewpoint_listener {
	/**
	 * view_matrix - sets the view matrix of this viewpoint for the
	 *	next frame
	 * @view: the view matrix for this frame
	 *
	 * This event is sent (ideally) at the beginning of each frame to
	 * give the client the view matrix it needs to draw its content to
	 * 2D in the same manner as the compositor.
	 *
	 * This matrix is the view matrix, which brings vectors from world
	 * space into view (or camera) space, which is the inverse of the
	 * camera transform.
	 *
	 * It is represented here as a column-major 4x4 matrix of 32 bit
	 * floats, with all values are specified in meters.
	 */
	void (*view_matrix)(void *data,
			    struct motorcar_viewpoint *motorcar_viewpoint,
			    struct wl_array *view);
	/**
	 * projection_matrix - change the projection matrix of this
	 *	viewpoint
	 * @projection: the new projection matrix for this viewpoint
	 *
	 * This event is sent immediately after the viewpoint global is
	 * bound by the client, and is only resent if the projection matrix
	 * for this viewpoint changes in the compoisitor (which would be
	 * unusual but is certainly possible)
	 *
	 * This matrix is in the form of the projection matrices used by
	 * OpenGL, which (with the help of a homogeneous divide done in
	 * hardware) brings vectors from view space into uniform device
	 * coordinate space in which vertices are projected from 3D to 2D.
	 *
	 * It is represented here as a column-major 4x4 matrix of 32 bit
	 * floats, with all values are specified in meters.
	 */
	void (*projection_matrix)(void *data,
				  struct motorcar_viewpoint *motorcar_viewpoint,
				  struct wl_array *projection);
	/**
	 * view_port - sets the view ports of this viewpoint to be used
	 *	when rendering into motorcar surface buffers
	 * @color_x: x position of the color viewport, in pixels
	 * @color_y: y position of the color viewport, in pixels
	 * @color_width: width of the color viewport, in pixels
	 * @color_height: height of the color viewport, in pixels
	 * @depth_x: x position of the depth viewport, in pixels
	 * @depth_y: y position of the depth viewport, in pixels
	 * @depth_width: width of the depth viewport, in pixels
	 * @depth_height: height of the depth viewport, in pixels
	 *
	 * This event tells the client where in its surface buffer it
	 * needs to draw the output associated with this viewpoint, which
	 * allows the compositor to extract the correct output for each
	 * viewpoint. Like the projection matrix, this event is sent
	 * immediately after the client binds this viewpoint global and is
	 * only sent again if the viewport changes on the compositor side.
	 *
	 * Viewport consists of a position and a size in surface local
	 * coordinates. Each viewpoint has two viewports associated with
	 * it, one to hold the color buffer from the client, and one to
	 * hold the depth buffer contents (which is encoded in the color
	 * buffer since EGL does not support a color mode that includes
	 * depth). If an EGL extension was written which creates a format
	 * that includes the depth buffer, then the depth viewport could
	 * hypothetically be ignored when that extension was present.
	 */
	void (*view_port)(void *data,
			  struct motorcar_viewpoint *motorcar_viewpoint,
			  int32_t color_x,
			  int32_t color_y,
			  uint32_t color_width,
			  uint32_t color_height,
			  int32_t depth_x,
			  int32_t depth_y,
			  uint32_t depth_width,
			  uint32_t depth_height);
};

static inline int
motorcar_viewpoint_add_listener(struct motorcar_viewpoint *motorcar_viewpoint,
				const struct motorcar_viewpoint_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) motorcar_viewpoint,
				     (void (**)(void)) listener, data);
}

static inline void
motorcar_viewpoint_set_user_data(struct motorcar_viewpoint *motorcar_viewpoint, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) motorcar_viewpoint, user_data);
}

static inline void *
motorcar_viewpoint_get_user_data(struct motorcar_viewpoint *motorcar_viewpoint)
{
	return wl_proxy_get_user_data((struct wl_proxy *) motorcar_viewpoint);
}

static inline void
motorcar_viewpoint_destroy(struct motorcar_viewpoint *motorcar_viewpoint)
{
	wl_proxy_destroy((struct wl_proxy *) motorcar_viewpoint);
}

/**
 * motorcar_six_dof_pointer - six degree of freedom pointer input device
 * @enter: enter event
 * @leave: leave event
 * @motion: six_dof pointer motion event
 * @button: pointer button event
 *
 * This interface represents a six degree of freedom pointing device
 * which behaves much like a traditional pointing device except that rather
 * then event being associated with a 2D position they are associated with
 * a 3D position and orientation, represented as a 3-vector position and
 * column major 3x3 rotation matrix as packed 32 bit float arrays (all
 * values in meters). Most of the events are the same as the traditional
 * wayaland pointer events other than this.
 *
 * The motorcar_six_dof_pointer interface generates motion, enter and leave
 * events for the surfaces that the six_dof pointer is located over, and
 * button and axis events for button presses, button releases and
 * scrolling.
 */
struct motorcar_six_dof_pointer_listener {
	/**
	 * enter - enter event
	 * @serial: (none)
	 * @surface: (none)
	 * @position: position of pointing device in meters at the time
	 *	of this event, represented as a packed array of three 32 bit
	 *	floats
	 * @orientation: orientation of the pointing device in meters at
	 *	the time of this event, represented as a packed column major 3x3
	 *	rotation matrix of 32 bit floats
	 *
	 * Notification that this seat's six_dof pointer is focused on a
	 * certain surface.
	 */
	void (*enter)(void *data,
		      struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		      uint32_t serial,
		      struct motorcar_surface *surface,
		      struct wl_array *position,
		      struct wl_array *orientation);
	/**
	 * leave - leave event
	 * @serial: (none)
	 * @surface: (none)
	 *
	 * Notification that this seat's six_dof pointer is no longer
	 * focused on a certain surface.
	 *
	 * The leave notification is sent before the enter notification for
	 * the new focus.
	 */
	void (*leave)(void *data,
		      struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		      uint32_t serial,
		      struct motorcar_surface *surface);
	/**
	 * motion - six_dof pointer motion event
	 * @time: timestamp with millisecond granularity
	 * @position: position of pointing device in meters at the time
	 *	of this event, represented as a packed array of three 32 bit
	 *	floats
	 * @orientation: orientation of the pointing device in meters at
	 *	the time of this event, represented as a packed column major 3x3
	 *	rotation matrix of 32 bit floats
	 *
	 * Notification of six_dof pointer location change. The arguments
	 * are the postion and orientation of the six_dof pointer device
	 */
	void (*motion)(void *data,
		       struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		       uint32_t time,
		       struct wl_array *position,
		       struct wl_array *orientation);
	/**
	 * button - pointer button event
	 * @serial: (none)
	 * @time: timestamp with millisecond granularity
	 * @button: (none)
	 * @state: (none)
	 *
	 * Mouse button click and release notifications.
	 *
	 * The location of the click is given by the last motion or enter
	 * event. The time argument is a timestamp with millisecond
	 * granularity, with an undefined base.
	 */
	void (*button)(void *data,
		       struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
		       uint32_t serial,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state);
};

static inline int
motorcar_six_dof_pointer_add_listener(struct motorcar_six_dof_pointer *motorcar_six_dof_pointer,
				      const struct motorcar_six_dof_pointer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) motorcar_six_dof_pointer,
				     (void (**)(void)) listener, data);
}

#define MOTORCAR_SIX_DOF_POINTER_RELEASE	0

static inline void
motorcar_six_dof_pointer_set_user_data(struct motorcar_six_dof_pointer *motorcar_six_dof_pointer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) motorcar_six_dof_pointer, user_data);
}

static inline void *
motorcar_six_dof_pointer_get_user_data(struct motorcar_six_dof_pointer *motorcar_six_dof_pointer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) motorcar_six_dof_pointer);
}

static inline void
motorcar_six_dof_pointer_destroy(struct motorcar_six_dof_pointer *motorcar_six_dof_pointer)
{
	wl_proxy_destroy((struct wl_proxy *) motorcar_six_dof_pointer);
}

static inline void
motorcar_six_dof_pointer_release(struct motorcar_six_dof_pointer *motorcar_six_dof_pointer)
{
	wl_proxy_marshal((struct wl_proxy *) motorcar_six_dof_pointer,
			 MOTORCAR_SIX_DOF_POINTER_RELEASE);

	wl_proxy_destroy((struct wl_proxy *) motorcar_six_dof_pointer);
}

#ifdef  __cplusplus
}
#endif

#endif
