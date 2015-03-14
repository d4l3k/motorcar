#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

extern const struct wl_interface motorcar_surface_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&motorcar_surface_interface,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&motorcar_surface_interface,
	NULL,
	NULL,
	NULL,
	&motorcar_surface_interface,
};

static const struct wl_message motorcar_shell_requests[] = {
	{ "get_motorcar_surface", "nouu", types + 8 },
};

WL_EXPORT const struct wl_interface motorcar_shell_interface = {
	"motorcar_shell", 1,
	1, motorcar_shell_requests,
	0, NULL,
};

static const struct wl_message motorcar_surface_requests[] = {
	{ "set_size_3d", "a", types + 0 },
};

static const struct wl_message motorcar_surface_events[] = {
	{ "transform_matrix", "a", types + 0 },
	{ "request_size_3d", "a", types + 0 },
};

WL_EXPORT const struct wl_interface motorcar_surface_interface = {
	"motorcar_surface", 1,
	1, motorcar_surface_requests,
	2, motorcar_surface_events,
};

static const struct wl_message motorcar_viewpoint_events[] = {
	{ "view_matrix", "a", types + 0 },
	{ "projection_matrix", "a", types + 0 },
	{ "view_port", "iiuuiiuu", types + 0 },
};

WL_EXPORT const struct wl_interface motorcar_viewpoint_interface = {
	"motorcar_viewpoint", 1,
	0, NULL,
	3, motorcar_viewpoint_events,
};

static const struct wl_message motorcar_six_dof_pointer_requests[] = {
	{ "release", "", types + 0 },
};

static const struct wl_message motorcar_six_dof_pointer_events[] = {
	{ "enter", "uoaa", types + 12 },
	{ "leave", "uo", types + 16 },
	{ "motion", "uaa", types + 0 },
	{ "button", "uuuu", types + 0 },
};

WL_EXPORT const struct wl_interface motorcar_six_dof_pointer_interface = {
	"motorcar_six_dof_pointer", 1,
	1, motorcar_six_dof_pointer_requests,
	4, motorcar_six_dof_pointer_events,
};

