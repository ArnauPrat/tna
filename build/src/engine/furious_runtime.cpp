


#include <furious/furious.h> 
#include "/home/aprat/projects/personal/furious/build/dist/include/furious/lang/../runtime/data/context.h"
#include "/home/aprat/projects/personal/tna/src/engine/scripts/../components/mesh.h"
#include "/home/aprat/projects/personal/tna/src/engine/scripts/../components/transform.h"
#include "/home/aprat/projects/personal/tna/src/engine/scripts/../components/transform_matrix.h"
#include "/home/aprat/projects/personal/tna/src/engine/scripts/../math/math_tools.h"
#include "/home/aprat/projects/personal/tna/src/scripts/../engine/components/../math/matrix.h"
#include "/home/aprat/projects/personal/tna/src/scripts/../engine/components/../math/vector.h"
#include "/home/aprat/projects/personal/tna/src/scripts/../engine/components/../resources/resources.h"
#include "/home/aprat/projects/personal/tna/src/scripts/../engine/rendering/renderer.h"
#include "/home/aprat/projects/personal/tna/src/scripts/../engine/resources/../rendering/mesh_data.h"
#include "/usr/include/glm/./ext/../detail/type_mat4x4.hpp"
#include "/usr/include/glm/./ext/matrix_float4x4.hpp"
#include "/usr/include/glm/detail/qualifier.hpp"
#include "/usr/include/glm/detail/type_vec3.hpp"



using namespace tna;
using namespace tna;
using namespace tna;
using namespace tna;
struct Rotator 
{
  Rotator(float speed) : 
  m_speed(speed)
  {
  }
  
  void run(furious::Context* context, 
           uint32_t id, 
           Transform* transform) 
  {
    float angle = context->m_dt*m_speed*360.0;
    transform->m_local_rotation.y += angle;
  }
  float m_speed;
};

struct RotatorAroundParent
{
  RotatorAroundParent(float speed) : 
  m_speed(speed)
  {
  }
  
  void run(furious::Context* context, 
           uint32_t id, 
           Transform* transform) 
  {
    float angle = context->m_dt*m_speed*360.0;
    transform->m_global_rotation.y += angle;
  }
  float m_speed;
};

struct RenderMesh 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const Mesh* mesh, 
           const TransformMatrix* transform_matrix) 
  {
    render_mesh(mesh->m_mesh_data, &transform_matrix->m_matrix);
  }
};

struct InitTransformMatrix 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           const Transform* transform) 
  {
    model_matrix->m_matrix = Matrix4(1.0f);
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.x, Vector3(1.0, 0.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.y, Vector3(0.0, 1.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_global_rotation.z, Vector3(0.0, 0.0, 1.0));
    model_matrix->m_matrix = translate(model_matrix->m_matrix, transform->m_position);
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.x, Vector3(1.0, 0.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.y, Vector3(0.0, 1.0, 0.0));
    model_matrix->m_matrix = rotate(model_matrix->m_matrix, transform->m_local_rotation.z, Vector3(0.0, 0.0, 1.0));
    model_matrix->m_matrix = scale(model_matrix->m_matrix, transform->m_scale);
  }
};

struct TransformMatrixHierarchy 
{
  void run(furious::Context* context, 
           uint32_t id, 
           TransformMatrix* model_matrix,
           const TransformMatrix* parent_matrix) 
  {
    model_matrix->m_matrix = parent_matrix->m_matrix * model_matrix->m_matrix;
  }
};

struct SetCamera 
{
  void run(furious::Context* context, 
           uint32_t id, 
           const TransformMatrix* transform_matrix) 
  {
    Matrix4 inv = inverse(transform_matrix->m_matrix);
    set_camera(&inv);
  }
};

namespace furious 
{



// Variable declarations 
TableView<tna::Mesh> table_tna__mesh;
TableView<tna::Transform> table_tna__transform;
TableView<tna::TransformMatrix> table_tna__transformmatrix;
TableView<entity_id_t> ref_table_parent;
BitTable* tagged_MainCamera;
SystemWrapper<Rotator,tna::Transform>* rotator_0;
SystemWrapper<RotatorAroundParent,tna::Transform>* rotatoraroundparent_1;
SystemWrapper<RenderMesh,const tna::Mesh,const tna::TransformMatrix>* rendermesh_2;
SystemWrapper<InitTransformMatrix,tna::TransformMatrix,const tna::Transform>* inittransformmatrix_3;
SystemWrapper<TransformMatrixHierarchy,tna::TransformMatrix,const tna::TransformMatrix>* transformmatrixhierarchy_4;
SystemWrapper<SetCamera,const tna::TransformMatrix>* setcamera_5;



// Variable initializations 
void __furious_init(Database* database)
{
table_tna__mesh  = FURIOUS_FIND_OR_CREATE_TABLE(database, tna::Mesh);
table_tna__transform  = FURIOUS_FIND_OR_CREATE_TABLE(database, tna::Transform);
table_tna__transformmatrix  = FURIOUS_FIND_OR_CREATE_TABLE(database, tna::TransformMatrix);
ref_table_parent  = database->get_references("parent");
tagged_MainCamera = database->get_tagged_entities("MainCamera");
rotator_0 = create_system<Rotator>(1.0);
rotatoraroundparent_1 = create_system<RotatorAroundParent>(1.0);
rendermesh_2 = create_system<RenderMesh>();
inittransformmatrix_3 = create_system<InitTransformMatrix>();
transformmatrixhierarchy_4 = create_system<TransformMatrixHierarchy>();
setcamera_5 = create_system<SetCamera>();
}



void __furious_frame(float delta, Database* database)
{
database->lock();
Context context(delta,database);
//- foreach (2) - "Rotator (1.0)"
//  - tag_filter (1) - has not - "MainCamera" - on_key
//    - scan (0) - "tna::Transform"
{
auto iter_table_tna__transform_0 = table_tna__transform.iterator();
while(iter_table_tna__transform_0.has_next())
{
BlockCluster block_tna__transform_0(iter_table_tna__transform_0.next().get_raw());

const Bitmap* filter = tagged_MainCamera->get_bitmap((&block_tna__transform_0)->m_start);
{
Bitmap negate(TABLE_BLOCK_SIZE);
negate.set_bitmap(filter);
negate.set_negate();
(&block_tna__transform_0)->p_enabled->set_and(&negate);
}
if((&block_tna__transform_0)->p_enabled->num_set() != 0)
{
tna::Transform* data_0 = (tna::Transform*)((&block_tna__transform_0)->m_blocks[0]->p_data);

rotator_0->apply_block(&context,
(&block_tna__transform_0)->m_start,
(&block_tna__transform_0)->p_enabled,
data_0);
}
}

}
//- foreach (6) - "RotatorAroundParent (1.0)"
//  - left_filter_join(5)
//   |- scan (3) - "tna::Transform"
//   |- scan (4) - #REFERENCE "parent"
{
BTree<BlockCluster> hashtable_5;
auto iter_table_tna__transform_3 = table_tna__transform.iterator();
while(iter_table_tna__transform_3.has_next())
{
BlockCluster block_tna__transform_3(iter_table_tna__transform_3.next().get_raw());
hashtable_5.insert_copy((&block_tna__transform_3)->m_start, (&block_tna__transform_3));
}

auto iter_ref_table_parent_4 = ref_table_parent.iterator();
while(iter_ref_table_parent_4.has_next())
{
BlockCluster block_parent_4(iter_ref_table_parent_4.next().get_raw());
BlockCluster* build = hashtable_5.get((&block_parent_4)->m_start);
if(build != nullptr)
{
BlockCluster cluster_5(*build);
cluster_5.filter((&block_parent_4));
if(cluster_5.p_enabled->num_set() != 0)
{
tna::Transform* data_0 = (tna::Transform*)((&cluster_5)->m_blocks[0]->p_data);

rotatoraroundparent_1->apply_block(&context,
(&cluster_5)->m_start,
(&cluster_5)->p_enabled,
data_0);
}
}
}

}
//- foreach (10) - "InitTransformMatrix ()"
//  - join(9)
//   |- scan (7) - "tna::TransformMatrix"
//   |- scan (8) - "tna::Transform"
{
BTree<BlockCluster> hashtable_9;
auto iter_table_tna__transformmatrix_7 = table_tna__transformmatrix.iterator();
while(iter_table_tna__transformmatrix_7.has_next())
{
BlockCluster block_tna__transformmatrix_7(iter_table_tna__transformmatrix_7.next().get_raw());
hashtable_9.insert_copy((&block_tna__transformmatrix_7)->m_start, (&block_tna__transformmatrix_7));
}

auto iter_table_tna__transform_8 = table_tna__transform.iterator();
while(iter_table_tna__transform_8.has_next())
{
BlockCluster block_tna__transform_8(iter_table_tna__transform_8.next().get_raw());
BlockCluster* build = hashtable_9.get((&block_tna__transform_8)->m_start);
if(build != nullptr)
{
BlockCluster cluster_9(*build);
cluster_9.append((&block_tna__transform_8));
if(cluster_9.p_enabled->num_set() != 0)
{
tna::TransformMatrix* data_0 = (tna::TransformMatrix*)((&cluster_9)->m_blocks[0]->p_data);
const tna::Transform* data_1 = (const tna::Transform*)((&cluster_9)->m_blocks[1]->p_data);

inittransformmatrix_3->apply_block(&context,
(&cluster_9)->m_start,
(&cluster_9)->p_enabled,
data_0,
data_1);
}
}
}

}
//- foreach (16) - "TransformMatrixHierarchy ()"
//  - join(15)
//   |- scan (11) - "tna::TransformMatrix"
//   |- cascading_gather (14)
//   |  - scan (13) - #REFERENCE "parent"
//   |  - scan (12) - "tna::TransformMatrix"
{
BTree<BlockCluster> hashtable_15;
auto iter_table_tna__transformmatrix_11 = table_tna__transformmatrix.iterator();
while(iter_table_tna__transformmatrix_11.has_next())
{
BlockCluster block_tna__transformmatrix_11(iter_table_tna__transformmatrix_11.next().get_raw());
hashtable_15.insert_copy((&block_tna__transformmatrix_11)->m_start, (&block_tna__transformmatrix_11));
}

BTree<DynArray<entity_id_t> > ref_parent_groups_14;
auto iter_ref_table_parent_13 = ref_table_parent.iterator();
while(iter_ref_table_parent_13.has_next())
{
BlockCluster block_parent_13(iter_ref_table_parent_13.next().get_raw());
group_references(&ref_parent_groups_14, (&block_parent_13)->m_blocks[0]);
}

BTree<BlockCluster> hashtable_14;
auto iter_table_tna__transformmatrix_12 = table_tna__transformmatrix.iterator();
while(iter_table_tna__transformmatrix_12.has_next())
{
BlockCluster block_tna__transformmatrix_12(iter_table_tna__transformmatrix_12.next().get_raw());
hashtable_14.insert_copy((&block_tna__transformmatrix_12)->m_start, (&block_tna__transformmatrix_12));
}

TableView<tna::TransformMatrix> temp_table_tna__transformmatrix_14 = database->create_temp_table<tna::TransformMatrix>("temp_table_tna__transformmatrix_14");
BitTable bittable1_14;
BitTable bittable2_14;
BitTable* current_frontier_14 = &bittable1_14;
BitTable* next_frontier_14 = &bittable2_14;
find_roots(&ref_parent_groups_14, current_frontier_14);
while(current_frontier_14->size() > 0)
{
temp_table_tna__transformmatrix_14.clear();
auto iter_hashtable_14 = hashtable_14.iterator();
while(iter_hashtable_14.has_next())
{
gather(&ref_parent_groups_14,iter_hashtable_14.next().p_value,current_frontier_14, next_frontier_14,&temp_table_tna__transformmatrix_14);
}
auto iter_temp_table_tna__transformmatrix_14 = temp_table_tna__transformmatrix_14.iterator();
while(iter_temp_table_tna__transformmatrix_14.has_next())
{
BlockCluster cluster_14;
cluster_14.append(iter_temp_table_tna__transformmatrix_14.next().get_raw());
BlockCluster* build = hashtable_15.get((&cluster_14)->m_start);
if(build != nullptr)
{
BlockCluster cluster_15(*build);
cluster_15.append((&cluster_14));
if(cluster_15.p_enabled->num_set() != 0)
{
tna::TransformMatrix* data_0 = (tna::TransformMatrix*)((&cluster_15)->m_blocks[0]->p_data);
const tna::TransformMatrix* data_1 = (const tna::TransformMatrix*)((&cluster_15)->m_blocks[1]->p_data);

transformmatrixhierarchy_4->apply_block(&context,
(&cluster_15)->m_start,
(&cluster_15)->p_enabled,
data_0,
data_1);
}
}
}
BitTable* temp_frontier_14 = current_frontier_14;
current_frontier_14 = next_frontier_14;
next_frontier_14 = temp_frontier_14;
next_frontier_14->clear();
}

}
//- foreach (19) - "SetCamera ()"
//  - tag_filter (18) - has - "MainCamera" - on_key
//    - scan (17) - "tna::TransformMatrix"
{
auto iter_table_tna__transformmatrix_17 = table_tna__transformmatrix.iterator();
while(iter_table_tna__transformmatrix_17.has_next())
{
BlockCluster block_tna__transformmatrix_17(iter_table_tna__transformmatrix_17.next().get_raw());

const Bitmap* filter = tagged_MainCamera->get_bitmap((&block_tna__transformmatrix_17)->m_start);
(&block_tna__transformmatrix_17)->p_enabled->set_and(filter);
if((&block_tna__transformmatrix_17)->p_enabled->num_set() != 0)
{
const tna::TransformMatrix* data_0 = (const tna::TransformMatrix*)((&block_tna__transformmatrix_17)->m_blocks[0]->p_data);

setcamera_5->apply_block(&context,
(&block_tna__transformmatrix_17)->m_start,
(&block_tna__transformmatrix_17)->p_enabled,
data_0);
}
}

}
//- foreach (23) - "RenderMesh ()"
//  - join(22)
//   |- scan (20) - "tna::Mesh"
//   |- scan (21) - "tna::TransformMatrix"
{
BTree<BlockCluster> hashtable_22;
auto iter_table_tna__mesh_20 = table_tna__mesh.iterator();
while(iter_table_tna__mesh_20.has_next())
{
BlockCluster block_tna__mesh_20(iter_table_tna__mesh_20.next().get_raw());
hashtable_22.insert_copy((&block_tna__mesh_20)->m_start, (&block_tna__mesh_20));
}

auto iter_table_tna__transformmatrix_21 = table_tna__transformmatrix.iterator();
while(iter_table_tna__transformmatrix_21.has_next())
{
BlockCluster block_tna__transformmatrix_21(iter_table_tna__transformmatrix_21.next().get_raw());
BlockCluster* build = hashtable_22.get((&block_tna__transformmatrix_21)->m_start);
if(build != nullptr)
{
BlockCluster cluster_22(*build);
cluster_22.append((&block_tna__transformmatrix_21));
if(cluster_22.p_enabled->num_set() != 0)
{
const tna::Mesh* data_0 = (const tna::Mesh*)((&cluster_22)->m_blocks[0]->p_data);
const tna::TransformMatrix* data_1 = (const tna::TransformMatrix*)((&cluster_22)->m_blocks[1]->p_data);

rendermesh_2->apply_block(&context,
(&cluster_22)->m_start,
(&cluster_22)->p_enabled,
data_0,
data_1);
}
}
}

}
database->release();
}
// Variable releases 
void __furious_release()
{
destroy_system(rotator_0);
destroy_system(rotatoraroundparent_1);
destroy_system(rendermesh_2);
destroy_system(inittransformmatrix_3);
destroy_system(transformmatrixhierarchy_4);
destroy_system(setcamera_5);
}
}
