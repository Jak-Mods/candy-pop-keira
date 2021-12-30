#pragma once

#include "common/custom_data/Tfrag3Data.h"
#include "common/math/Vector.h"
#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/pipelines/opengl.h"
#include "game/graphics/opengl_renderer/tfrag/tfrag_common.h"
#include "game/graphics/opengl_renderer/tfrag/Tie3.h"

class Tfrag3 {
 public:
  Tfrag3();
  ~Tfrag3();

  void debug_render_all_trees_nolores(const TfragRenderSettings& settings,
                                      SharedRenderState* render_state,
                                      ScopedProfilerNode& prof);

  void render_all_trees(const TfragRenderSettings& settings,
                        SharedRenderState* render_state,
                        ScopedProfilerNode& prof);

  void render_matching_trees(const std::vector<tfrag3::TFragmentTreeKind>& trees,
                             const TfragRenderSettings& settings,
                             SharedRenderState* render_state,
                             ScopedProfilerNode& prof);

  void render_tree(const TfragRenderSettings& settings,
                   SharedRenderState* render_state,
                   ScopedProfilerNode& prof);

  void setup_for_level(const std::vector<tfrag3::TFragmentTreeKind>& tree_kinds,
                       const std::string& level,
                       SharedRenderState* render_state);
  void discard_tree_cache();

  void render_tree_cull_debug(const TfragRenderSettings& settings,
                              SharedRenderState* render_state,
                              ScopedProfilerNode& prof);

  void draw_debug_window();
  struct DebugVertex {
    math::Vector3f position;
    math::Vector4f rgba;
  };

 private:
  struct TreeCache {
    tfrag3::TFragmentTreeKind kind;
    GLuint vertex_buffer = -1;
    GLuint vao;
    u32 vert_count = 0;
    const std::vector<tfrag3::StripDraw>* draws = nullptr;
    const std::vector<tfrag3::TimeOfDayColor>* colors = nullptr;
    const tfrag3::BVH* vis = nullptr;
    SwizzledTimeOfDay tod_cache;

    void reset_stats() {
      rendered_this_frame = false;
      tris_this_frame = 0;
      draws_this_frame = 0;
    }
    bool rendered_this_frame = false;
    int tris_this_frame = 0;
    int draws_this_frame = 0;
    bool allowed = true;
    bool forced = false;
    bool cull_debug = false;
  };

  struct Cache {
    std::vector<u8> vis_temp;
    std::vector<std::pair<int, int>> draw_idx_temp;
    std::vector<u32> index_list;
  } m_cache;

  std::string m_level_name;

  std::vector<GLuint> m_textures;
  std::vector<TreeCache> m_cached_trees;
  GLuint m_time_of_day_texture = -1;
  bool m_has_time_of_day_texture = false;

  std::vector<math::Vector<u8, 4>> m_color_result;

  bool m_has_index_buffer = false;
  GLuint m_index_buffer = -1;

  GLuint m_debug_vao = -1;
  GLuint m_debug_verts = -1;

  // in theory could be up to 4096, I think, but we don't see that many...
  // should be easy to increase (will require a shader change too for indexing)
  static constexpr int TIME_OF_DAY_COLOR_COUNT = 8192;

  static constexpr int DEBUG_TRI_COUNT = 4096;
  std::vector<DebugVertex> m_debug_vert_data;

  bool m_use_fast_time_of_day = true;
};