#ifndef MOTORCAR_SERVER_PROTOCOL_H
#define MOTORCAR_SERVER_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-server.h"

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

/**
 * motorcar_shell - a 3D compositor shell
 * @get_motorcar_surface: create a motorcar surface from a surface
 *
 * An interface to allow a copositor to composite 3D data from multiple
 * clients in a manner that makes it appear to be in the same 3D space.
 * Combined with embedding tradtional wayland surfaces on quads in the same
 * space it provides the framework needed to achieve a seamless mixture of
 * 2D and 3D user interfaces while still giving clients full flexibility in
 * how their content is drawn to 2D
 *
 * It allows clients to associate a motorcar_surface with a basic surface,
 * which both tells the compositor to perform 3D compositing on the client
 * surface, and also provides a mechanism for the compositor to give the
 * client the information it needs to draw its content to 2D correctly.
 */
struct motorcar_shell_interface {
	/**
	 * get_motorcar_surface - create a motorcar surface from a
	 *	surface
	 * @id: the id of the new motorcar surface to be created
	 * @surface: the wl_surface which the motorcar_surface is to be
	 *	associated with
	 * @clipping_mode: the clipping mode to be used by the compositor
	 *	for this surface
	 * @enable_depth_compositing: boolean value indicating whether to
	 *	use depth buffer compositing on this surface
	 *
	 * Create a motorcar surface for an existing surface.
	 *
	 * Only one motorcar surface can be associated with a given
	 * surface.
	 */
	void (*get_motorcar_surface)(struct wl_client *client,
				     struct wl_resource *resource,
				     uint32_t id,
				     struct wl_resource *surface,
				     uint32_t clipping_mode,
				     uint32_t enable_depth_compositing);
};


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
 * @set_size_3d: requests that the client resize the 3D window to the
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
struct motorcar_surface_interface {
	/**
	 * set_size_3d - requests that the client resize the 3D window to
	 *	the given scale
	 * @dimensions: the new size vector
	 *
	 * Sets the new size of the 3D window. If this size is larger
	 * than the size most recently requested by the compositor the
	 * compositor is free to clip the window to a new size, provided
	 * that it immediatly requests the client resize to those
	 * dimensions.
	 *
	 * The size is represented here as a vector of three 32-bit floats,
	 * representing the size of the 3D window along each of the
	 * cardinal axes in the window's local space.
	 */
	void (*set_size_3d)(struct wl_client *client,
			    struct wl_resource *resource,
			    struct wl_array *dimensions);
};

#define MOTORCAR_SURFACE_TRANSFORM_MATRIX	0
#define MOTORCAR_SURFACE_REQUEST_SIZE_3D	1

#define MOTORCAR_SURFACE_TRANSFORM_MATRIX_SINCE_VERSION	1
#define MOTORCAR_SURFACE_REQUEST_SIZE_3D_SINCE_VERSION	1

static inline void
motorcar_surface_send_transform_matrix(struct wl_resource *resource_, struct wl_array *transform)
{
	wl_resource_post_event(resource_, MOTORCAR_SURFACE_TRANSFORM_MATRIX, transform);
}

static inline void
motorcar_surface_send_request_size_3d(struct wl_resource *resource_, struct wl_array *dimensions)
{
	wl_resource_post_event(resource_, MOTORCAR_SURFACE_REQUEST_SIZE_3D, dimensions);
}

#define MOTORCAR_VIEWPOINT_VIEW_MATRIX	0
#define MOTORCAR_VIEWPOINT_PROJECTION_MATRIX	1
#define MOTORCAR_VIEWPOINT_VIEW_PORT	2

#define MOTORCAR_VIEWPOINT_VIEW_MATRIX_SINCE_VERSION	1
#define MOTORCAR_VIEWPOINT_PROJECTION_MATRIX_SINCE_VERSION	1
#define MOTORCAR_VIEWPOINT_VIEW_PORT_SINCE_VERSION	1

static inline void
motorcar_viewpoint_send_view_matrix(struct wl_resource *resource_, struct wl_array *view)
{
	wl_resource_post_event(resource_, MOTORCAR_VIEWPOINT_VIEW_MATRIX, view);
}

static inline void
motorcar_viewpoint_send_projection_matrix(struct wl_resource *resource_, struct wl_array *projection)
{
	wl_resource_post_event(resource_, MOTORCAR_VIEWPOINT_PROJECTION_MATRIX, projection);
}

static inline void
motorcar_viewpoint_send_view_port(struct wl_resource *resource_, int32_t color_x, int32_t color_y, uint32_t color_width, uint32_t color_height, int32_t depth_x, int32_t depth_y, uint32_t depth_width, uint32_t depth_height)
{
	wl_resource_post_event(resource_, MOTORCAR_VIEWPOINT_VIEW_PORT, color_x, color_y, color_width, color_height, depth_x, depth_y, depth_width, depth_height);
}

/**
 * motorcar_six_dof_pointer - six degree of freedom pointer input device
 * @release: release the six_dof pointer object
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
struct motorcar_six_dof_pointer_interface {
	/**
	 * release - release the six_dof pointer object
	 *
	 * 
	 */
	void (*release)(struct wl_client *client,
			struct wl_resource *resource);
};

#define MOTORCAR_SIX_DOF_POINTER_ENTER	0
#define MOTORCAR_SIX_DOF_POINTER_LEAVE	1
#define MOTORCAR_SIX_DOF_POINTER_MOTION	2
#define MOTORCAR_SIX_DOF_POINTER_BUTTON	3

#define MOTORCAR_SIX_DOF_POINTER_ENTER_SINCE_VERSION	1
#define MOTORCAR_SIX_DOF_POINTER_LEAVE_SINCE_VERSION	1
#define MOTORCAR_SIX_DOF_POINTER_MOTION_SINCE_VERSION	1
#define MOTORCAR_SIX_DOF_POINTER_BUTTON_SINCE_VERSION	1

static inline void
motorcar_six_dof_pointer_send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, struct wl_array *position, struct wl_array *orientation)
{
	wl_resource_post_event(resource_, MOTORCAR_SIX_DOF_POINTER_ENTER, serial, surface, position, orientation);
}

static inline void
motorcar_six_dof_pointer_send_leave(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface)
{
	wl_resource_post_event(resource_, MOTORCAR_SIX_DOF_POINTER_LEAVE, serial, surface);
}

static inline void
motorcar_six_dof_pointer_send_motion(struct wl_resource *resource_, uint32_t time, struct wl_array *position, struct wl_array *orientation)
{
	wl_resource_post_event(resource_, MOTORCAR_SIX_DOF_POINTER_MOTION, time, position, orientation);
}

static inline void
motorcar_six_dof_pointer_send_button(struct wl_resource *resource_, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	wl_resource_post_event(resource_, MOTORCAR_SIX_DOF_POINTER_BUTTON, serial, time, button, state);
}

#ifdef  __cplusplus
}
#endif

#endif
