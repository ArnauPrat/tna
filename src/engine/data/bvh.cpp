


#include "bvh.h"
#include <stddef.h>

namespace tna
{

TnaBVHNode*
create_bvh()
{
  TnaBVHNode* bvh = new TnaBVHNode();
  bvh->m_type = TnaBVHNodeType::E_INTERNAL;
  bvh->m_internal.p_left = nullptr;
  bvh->m_internal.p_right = nullptr;
  return bvh;
}

void
destroy_bvh(TnaBVHNode* root)
{
  if(root->m_type == TnaBVHNodeType::E_INTERNAL)
  {
    if(root->m_internal.p_left != nullptr)
    {
      destroy_bvh(root->m_internal.p_left);
      root->m_internal.p_left = nullptr;
    }

    if(root->m_internal.p_right != nullptr)
    {
      destroy_bvh(root->m_internal.p_right);
      root->m_internal.p_right = nullptr;
    }
  }

  delete root;
}


/**
 * \brief Extracts the x component of a vector
 *
 * \param vector The vector to extract the x component from
 *
 * \return  The x component of the vector
 */
float
extract_x(const vector3_t* vector)
{
  return vector->x;
}

/**
 * \brief Extracts the y component of a vector
 *
 * \param vector The vector to extract the y component from
 *
 * \return  The y component of the vector
 */
float
extract_y(const vector3_t* vector)
{
  return vector->y;
}

/**
 * \brief Extracts the z component of a vector
 *
 * \param vector The vector to extract the z component from
 *
 * \return  The z component of the vector
 */
float
extract_z(const vector3_t* vector)
{
  return vector->z;
}

/**
 * \brief Computes the surface are heuristic given a set of centroid elements,
 * their bin maps and the splitting point
 *
 * \param centroids The array of element centroids
 * \param bin_maps The element to bin mapping
 * \param num_elements The number of elements in the array
 * \param split_point The splitting point
 * \param left_aabb The aabb of the left part
 * \param right_aabb The aabb of the right part
 * \param left_count The the number of elements in the left part
 * \param right_count The number of elements in the right part
 *
 * \return Returns the SAH score of such split
 */
float
compute_sah_score(const vector3_t* centroids, 
                  const uint32_t* bin_maps,
                  uint32_t num_elements, 
                  uint32_t split_point, 
                  aabb_t* left_aabb,
                  aabb_t* right_aabb,
                  uint32_t* left_count,
                  uint32_t* right_count)
{
  *left_count = 0;
  *right_count = 0;

  left_aabb->m_min.x = FLT_MAX;
  left_aabb->m_min.y = FLT_MAX;
  left_aabb->m_min.z = FLT_MAX;
  left_aabb->m_max.x = FLT_MIN;
  left_aabb->m_max.y = FLT_MIN;
  left_aabb->m_max.z = FLT_MIN;

  right_aabb->m_min.x = FLT_MAX;
  right_aabb->m_min.y = FLT_MAX;
  right_aabb->m_min.z = FLT_MAX;
  right_aabb->m_max.x = FLT_MIN;
  right_aabb->m_max.y = FLT_MIN;
  right_aabb->m_max.z = FLT_MIN;

  for(uint32_t i = 0; 
      i < num_elements; 
      ++i)
  {
    aabb_t* target_aabb = nullptr;
    if(bin_maps[i] <= split_point)
    {
      target_aabb = left_aabb;
      (*left_count)++;
    }
    else
    {
      target_aabb = right_aabb;
      (*right_count)++;
    }

    const vector3_t* centroid = &centroids[i];
    target_aabb->m_max.x = fmax(target_aabb->m_max.x, centroid->x);
    target_aabb->m_max.y = fmax(target_aabb->m_max.y, centroid->y);
    target_aabb->m_max.z = fmax(target_aabb->m_max.z, centroid->z);

    target_aabb->m_min.x = fmin(target_aabb->m_min.x, centroid->x);
    target_aabb->m_min.y = fmin(target_aabb->m_min.y, centroid->y);
    target_aabb->m_min.z = fmin(target_aabb->m_min.z, centroid->z);
  }
  return 0.0;
}

void
build_bvh(TnaBVHNode* root, 
          aabb_t** aabb, 
          vector3_t* centroids,
          void** data, 
          const uint32_t* index,
          uint32_t num_elements)
{
  assert(num_elements > 0 && "Number of elements needs to be at least 1");

  if(num_elements == 1)
  {
    root->m_type = TnaBVHNodeType::E_LEAF;
    root->m_leaf.p_aabb = aabb[index[0]];
    root->m_leaf.p_data = data[index[0]];
    return;
  }

  // Compute largest extent;
  float min_x = FLT_MAX;
  float max_x = 0.0f;
  float min_y = FLT_MAX;
  float max_y = 0.0f;
  float min_z = FLT_MAX;
  float max_z = 0.0f;

  for(uint32_t i = 0;
      i < num_elements; 
      ++i)
  {
    vector3_t* centroid = &centroids[index[i]];

    max_x = fmax(max_x, centroid->x);
    min_x = fmin(min_x, centroid->x);

    max_y = fmax(max_y, centroid->y);
    min_y = fmin(min_y, centroid->y);

    max_z = fmax(max_z, centroid->z);
    min_z = fmin(min_z, centroid->z);

  }

  float extent_x = fabs(max_x - min_x);
  float extent_y = fabs(max_y - min_y);
  float extent_z = fabs(max_z - min_z);

  float extent_min;
  float extent_max;
  float (*extract_dim)(const vector3_t*) = nullptr;
  if(extent_x > extent_y)
  {
    extract_dim = extract_x;
    extent_min = min_x;
    extent_max = max_x;
    if(extent_x < extent_z)
    {
      extract_dim = extract_z;
      extent_min = min_z;
      extent_max = max_z;
    }
  }
  else
  {
    extract_dim = extract_y;
    extent_min = min_y;
    extent_max = max_y;
    if(extent_y < extent_z)
    {
      extract_dim = extract_z;
      extent_min = min_z;
      extent_max = max_z;
    }
  }

  // computing bin edges
  const uint32_t num_bins = 1024;
  uint32_t* bin_map = new uint32_t[num_elements];
  float bin_span = (extent_max - extent_min) / num_bins;
  for(uint32_t i = 0; i < num_elements; ++i)
  {
    bin_map[i] = (extract_dim(&centroids[index[i]]) - extent_min) / bin_span;
  }

  // search for optimal split point
  uint32_t best_split_point = 0;
  float best_split_score = FLT_MAX;
  aabb_t best_left_aabb;
  aabb_t best_right_aabb;
  uint32_t best_left_count;
  uint32_t best_right_count;
  for(uint32_t i = 0; i < num_bins - 1; ++i)
  {
    aabb_t left_aabb;
    aabb_t right_aabb;
    uint32_t left_count;
    uint32_t right_count;
    float score = compute_sah_score(centroids, 
                                    bin_map,
                                    num_elements,
                                    i,
                                    &left_aabb,
                                    &right_aabb,
                                    &left_count,
                                    &right_count);
    if(score < best_split_score)
    {
      best_split_point = i;
      best_split_score = score;
      best_left_aabb = left_aabb;
      best_right_aabb = right_aabb;
      best_left_count = left_count;
      best_right_count = right_count;
    }
  }

  uint32_t* left_index = nullptr;
  uint32_t* right_index = nullptr;
  if(best_left_count > 0)
  {
    left_index = new uint32_t[best_left_count];
  }

  if(best_right_count > 0)
  {
    right_index = new uint32_t[best_right_count];
  }

  uint32_t left_next = 0;
  uint32_t right_next = 0;
  for(uint32_t i = 0; i < num_elements; ++i)
  {
    if(bin_map[i] <= best_split_point)
    {
      left_index[left_next] = index[i];
      left_next++;
    }
    else
    {
      left_index[right_next] = index[i];
      right_next++;
    }
  }

  if(best_left_count > 0)
  {
    root->m_internal.p_left = create_bvh();
    build_bvh(root->m_internal.p_left,
              aabb, 
              centroids,
              data, 
              left_index,
              best_left_count);
  }

  if(best_right_count > 0)
  {
    root->m_internal.p_right = create_bvh();
    build_bvh(root->m_internal.p_right,
              aabb, 
              centroids,
              data, 
              right_index,
              best_right_count);
  }


  delete [] left_index;
  delete [] right_index;
  delete [] bin_map;

}

void
build_bvh(TnaBVHNode* root, 
          aabb_t** aabb, 
          void** data, 
          uint32_t num_elements)
{

  if(num_elements > 0)
  {

  vector3_t* centroids = new vector3_t[num_elements];
  uint32_t* index = new uint32_t[num_elements];

  for(uint32_t i = 0;
      i < num_elements; 
      ++i)
  {
    vector3_t* centroid = &centroids[i];
    *centroid = vector3_t((aabb[i]->m_max.x - aabb[i]->m_min.x) / 2.0f + aabb[i]->m_min.x,
                           (aabb[i]->m_max.y - aabb[i]->m_min.y) / 2.0f + aabb[i]->m_min.y,
                           (aabb[i]->m_max.z - aabb[i]->m_min.z) / 2.0f + aabb[i]->m_min.z);
    index[i] = i;
  }


  build_bvh(root,
            aabb,
            centroids,
            data,
            index,
            num_elements);

  delete [] index;
  delete [] centroids;
  }

}

void
rebuild_bvh(TnaBVHNode* root)
{
}

void
insert_bvh(TnaBVHNode* root, 
           aabb_t** aabb, 
           void** data, 
           uint32_t num_elements)
{
  
}

void
insert_bvh(TnaBVHNode* root, 
           aabb_t* aabb, 
           void* data)
{
}

void
refit_bvh(TnaBVHNode* root)
{
}

  
} /* tna */ 
