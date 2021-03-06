#ifndef SHADER_MAP_H
#define SHADER_MAP_H

#include <unordered_map>
#include <string>

const std::unordered_map<std::string, std::pair<std::string, std::string>> WORLD_SHADERS = {
	{"geometry_shader_static", 				{"vertex/geometry_static.vs", 			"fragment/geometry_static.fs"}},
	{"geometry_shader_static_colored", 		{"vertex/geometry_static_colored.vs", 	"fragment/geometry_static_colored.fs"}},
	{"geometry_shader_animated", 			{"vertex/geometry_animated.vs", 		"fragment/geometry_animated.fs"}},
	{"geometry_shader_animated_colored", 	{"vertex/geometry_animated_colored.vs", "fragment/geometry_animated_colored.fs"}},
	{"primitive_line_shader", 				{"vertex/primitive_line.vs", 			"fragment/primitive_line.fs"}},
	{"point_light_shader", 					{"vertex/instanced_point_light_cube.vs", "fragment/point_light.fs"}},
	{"spot_light_shader", 					{"vertex/instanced_spot_light_cube.vs", "fragment/spot_light.fs"}},
	{"dir_light_shader", 					{"vertex/instanced_fullscreen_quad.vs", "fragment/dir_light.fs"}},
	{"point_light_SSAO_shader", 			{"vertex/instanced_light_cube.vs", 		"fragment/point_light_SSAO.fs"}},
	{"spot_light_SSAO_shader", 				{"vertex/instanced_light_cube.vs", 		"fragment/spot_light_SSAO.fs"}},
	{"dir_light_SSAO_shader", 				{"vertex/instanced_fullscreen_quad.vs", "fragment/dir_light_SSAO.fs"}},
	{"bloom_shader", 						{"vertex/fullscreen_quad.vs", 			"fragment/bloom.fs"}},
	{"vertical_blur_shader", 				{"vertex/fullscreen_quad.vs", 			"fragment/vertical_blur.fs"}},
	{"horizontal_blur_shader", 				{"vertex/fullscreen_quad.vs", 			"fragment/horizontal_blur.fs"}},
	{"HDR_shader", 							{"vertex/fullscreen_quad.vs", 			"fragment/HDR.fs"}},
	{"FXAA_shader", 						{"vertex/fullscreen_quad.vs", 			"fragment/FXAA.fs"}},
	{"SSAO_shader", 						{"vertex/fullscreen_quad.vs", 			"fragment/SSAO.fs"}},
	{"LFST_cull_shader", 					{"vertex/LFST_face_culled_geometry.vs",	"fragment/LFST_face_culled_geometry.fs"}},
	{"LFST_layer_shader", 					{"vertex/fullscreen_quad.vs",			"fragment/LFST_thickness_map.fs"}},
	{"ppe_blend_shader", 					{"vertex/fullscreen_quad.vs", 			"fragment/ppe_blend.fs"}},
	{"mc_density_shader", 					{"vertex/instanced_fullscreen_quad.vs", "fragment/mc_density.fs"}},
};

const std::unordered_map<std::string, std::string> GEOM_SHADERS = {
	{"mc_density_shader", 					"geometry/layered.gs"},
};

#endif




