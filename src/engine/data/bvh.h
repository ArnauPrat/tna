

#ifndef _TNA_BVH_H_
#define _TNA_BVH_H_

#include "../shapes/aabb.h"

namespace tna
{

enum class TnaBVHNodeType
{
  E_INTERNAL,
  E_LEAF,
};

struct TnaBVHNode
{
  TnaBVHNodeType m_type;
  aabb_t        m_aabb;
  union {
    struct {
      void* p_data;
      aabb_t* p_aabb;
    } m_leaf;

    struct {
      TnaBVHNode* p_left;
      TnaBVHNode* p_right;
    } m_internal;
  };
};


/**
 * \brief Creates a BVH
 *
 * \return Returns a pointer to the root node of the BVH
 */
TnaBVHNode*
create_bvh();

/**
 * \brief Destroys a BVH
 *
 * \param root A pointer to the root of the BVH
 */
void
destroy_bvh(TnaBVHNode* root);

/**
 * \brief Initializes a bvh from a set of elements.
 *
 * \param root The root of the BVH
 * \param aabb An array of aabbs
 * \param data An array of void* to the elements of each aabb
 * \param num_elements The number of elements of the array
 */
void
build_bvh(TnaBVHNode* root, 
           aabb_t** aabb, 
           void** data, 
           uint32_t num_elements);

/**
 * \brief Rebuilds a bvh from its current elements.
 *
 * \param root The root of the BVH
 */
void
rebuild_bvh(TnaBVHNode* root);

/**
 * \brief Inserts a set of elements into the BVH. Such an insert can trigger a
 * full BVH rebuild.
 *
 * \param root The root of the BVH
 * \param aabb An array of aabbs
 * \param data An array of void* to the elements of each aabb
 * \param num_elements The number of elements of the array
 */
void
insert_bvh(TnaBVHNode* root, 
           aabb_t** aabb, 
           void** data, 
           uint32_t num_elements);

/**
 * \brief Inserts an element into the BVH. Such an insert can trigger a
 * full BVH rebuild.
 *
 * \param root The root of the BVH
 * \param aabb An array of aabbs
 * \param data An array of void* to the elements of each aabb
 * \param num_elements The number of elements of the array
 */
void
insert_bvh(TnaBVHNode* root, 
           aabb_t* aabb, 
           void* data);

/**
 * \brief Refits the BVH.
 *
 * \param root The root of the BVH
 * \param aabb An array of aabbs
 * \param data An array of void* to the elements of each aabb
 * \param num_elements The number of elements of the array
 */
void
refit_bvh(TnaBVHNode* root);
  
} /* tna */ 

#endif
