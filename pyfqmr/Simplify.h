/////////////////////////////////////////////
//
// Mesh Simplification Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
//
// https://github.com/sp4cerat/Fast-Quadric-Mesh-S;
// 5/2016: Chris Rorden created minimal version for OSX/Linux/Windows compile

// #include <iostream>
// #include <stddef.h>
// #include <functional>
// #include <sys/stat.h>
// #include <stdbool.h>
#include <string.h>
// #include <ctype.h>
// #include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <float.h> //FLT_EPSILON, DBL_EPSILON

#define loopi(start_l, end_l) for (int i = start_l; i < end_l; ++i)
#define loopi(start_l, end_l) for (int i = start_l; i < end_l; ++i)
#define loopj(start_l, end_l) for (int j = start_l; j < end_l; ++j)
#define loopk(start_l, end_l) for (int k = start_l; k < end_l; ++k)

struct vector3
{
  double x, y, z;
};

struct vec2f {
    double x, y;

    inline vec2f(void) {}

    inline vec2f(double X, double Y) : x(X), y(Y) {}

    inline vec2f operator+(const vec2f &a) const {
        return vec2f(x + a.x, y + a.y);
    }

    inline vec2f operator*(double a) const {
        return vec2f(x * a, y * a);
    }

    inline vec2f operator=(const vec2f a) {
        x = a.x;
        y = a.y;
        return *this;
    }

    inline vec2f operator/(double a) const {
        return vec2f(x / a, y / a);
    }
};

struct vec3f
{
  double x, y, z;

  inline vec3f(void) {}

  inline vec3f(vector3 a)
  {
    x = a.x;
    y = a.y;
    z = a.z;
  }

  inline vec3f(const double X, const double Y, const double Z)
  {
    x = X;
    y = Y;
    z = Z;
  }

  inline vec3f operator+(const vec3f &a) const
  {
    return vec3f(x + a.x, y + a.y, z + a.z);
  }

  inline vec3f operator+=(const vec3f &a) const
  {
    return vec3f(x + a.x, y + a.y, z + a.z);
  }

  inline vec3f operator*(const double a) const
  {
    return vec3f(x * a, y * a, z * a);
  }

  inline vec3f operator*(const vec3f a) const
  {
    return vec3f(x * a.x, y * a.y, z * a.z);
  }

  inline vec3f v3() const
  {
    return vec3f(x, y, z);
  }

  inline vec3f operator=(const vector3 a)
  {
    x = a.x;
    y = a.y;
    z = a.z;
    return *this;
  }

  inline vec3f operator=(const vec3f a)
  {
    x = a.x;
    y = a.y;
    z = a.z;
    return *this;
  }

  inline vec3f operator/(const vec3f a) const
  {
    return vec3f(x / a.x, y / a.y, z / a.z);
  }

  inline vec3f operator-(const vec3f &a) const
  {
    return vec3f(x - a.x, y - a.y, z - a.z);
  }

  inline vec3f operator/(const double a) const
  {
    return vec3f(x / a, y / a, z / a);
  }

  inline double dot(const vec3f &a) const
  {
    return a.x * x + a.y * y + a.z * z;
  }

  inline vec3f cross(const vec3f &a, const vec3f &b)
  {
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;
    return *this;
  }

  inline double length() const
  {
    return (double)sqrt(x * x + y * y + z * z);
  }

  inline vec3f normalize(double desired_length = 1)
  {
    double square = sqrt(x * x + y * y + z * z);
    x /= square;
    y /= square;
    z /= square;
    return *this;
  }

  double random_double_01(double a)
  {
    double rnf = a * 14.434252 + a * 364.2343 + a * 4213.45352 + a * 2341.43255 + a * 254341.43535 + a * 223454341.3523534245 + 23453.423412;
    int rni = ((int)rnf) % 100000;
    return double(rni) / (100000.0f - 1.0f);
  }

  vec3f random01_fxyz()
  {
    x = (double)random_double_01(x);
    y = (double)random_double_01(y);
    z = (double)random_double_01(z);
    return *this;
  }
};

vec3f barycentric(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c)
{
  vec3f v0 = b - a;
  vec3f v1 = c - a;
  vec3f v2 = p - a;
  double d00 = v0.dot(v0);
  double d01 = v0.dot(v1);
  double d11 = v1.dot(v1);
  double d20 = v2.dot(v0);
  double d21 = v2.dot(v1);
  double denom = d00 * d11 - d01 * d01;
  double v = (d11 * d20 - d01 * d21) / denom;
  double w = (d00 * d21 - d01 * d20) / denom;
  double u = 1.0 - v - w;
  return vec3f(u, v, w);
}

vec2f interpolate(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c, const vec2f attrs[3]) {
    vec3f bary = barycentric(p, a, b, c);
    vec2f out = vec2f(0, 0);
    out = out + attrs[0] * bary.x;
    out = out + attrs[1] * bary.y;
    out = out + attrs[2] * bary.z;
    return out;
}

double min(double v1, double v2)
{
  return fmin(v1, v2);
}

class SymetricMatrix
{

public:
  // Constructor
  SymetricMatrix(double c = 0) { loopi(0, 10) m[i] = c; }

  SymetricMatrix(double m11, double m12, double m13, double m14,
                 double m22, double m23, double m24,
                 double m33, double m34,
                 double m44)
  {
    m[0] = m11;
    m[1] = m12;
    m[2] = m13;
    m[3] = m14;
    m[4] = m22;
    m[5] = m23;
    m[6] = m24;
    m[7] = m33;
    m[8] = m34;
    m[9] = m44;
  }

  // Make plane
  SymetricMatrix(double a, double b, double c, double d)
  {
    m[0] = a * a;
    m[1] = a * b;
    m[2] = a * c;
    m[3] = a * d;
    m[4] = b * b;
    m[5] = b * c;
    m[6] = b * d;
    m[7] = c * c;
    m[8] = c * d;
    m[9] = d * d;
  }

  double operator[](int c) const { return m[c]; }

  // Determinant
  double det(int a11, int a12, int a13,
             int a21, int a22, int a23,
             int a31, int a32, int a33)
  {
    double det = m[a11] * m[a22] * m[a33] + m[a13] * m[a21] * m[a32] + m[a12] * m[a23] * m[a31] - m[a13] * m[a22] * m[a31] - m[a11] * m[a23] * m[a32] - m[a12] * m[a21] * m[a33];
    return det;
  }

  const SymetricMatrix operator+(const SymetricMatrix &n) const
  {
    return SymetricMatrix(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3],
                          m[4] + n[4], m[5] + n[5], m[6] + n[6],
                          m[7] + n[7], m[8] + n[8],
                          m[9] + n[9]);
  }

  SymetricMatrix &operator+=(const SymetricMatrix &n)
  {
    m[0] += n[0];
    m[1] += n[1];
    m[2] += n[2];
    m[3] += n[3];
    m[4] += n[4];
    m[5] += n[5];
    m[6] += n[6];
    m[7] += n[7];
    m[8] += n[8];
    m[9] += n[9];
    return *this;
  }

  double m[10];
};
///////////////////////////////////////////

namespace Simplify
{
  // Global Variables & Strctures
  enum Attributes
  {
    NONE,
    NORMAL = 2,
    TEXCOORD = 4,
    COLOR = 8
  };
  struct Triangle
  {
    int v[3];
    double err[4];
    int deleted, dirty, attr;
    vec3f n;
    int material;
  };
  struct Vertex
  {
    vec3f p;
    int tstart, tcount;
    SymetricMatrix q;
    int border;
    vec2f uv;
  };
  struct Ref
  {
    int tid, tvertex;
  };
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;
  std::vector<Ref> refs;
  std::string mtllib;                 //
  std::vector<std::string> materials; //

  // Helper functions
  double vertex_error(SymetricMatrix q, double x, double y, double z);
  double calculate_error(int id_v1, int id_v2, vec3f &p_result, vec2f &uv_result);
  bool flipped(vec3f p, int i0, int i1, Vertex &v0, Vertex &v1, std::vector<int> &deleted);
  void update_triangles(int i0, Vertex &v, std::vector<int> &deleted, int &deleted_triangles);
  void update_mesh(int iteration);
  void compact_mesh();

  //
  // Main simplification function
  //
  // target_count  : target nr. of triangles
  // agressiveness : sharpness to increase the threshold.
  //                 5..8 are good numbers
  //                 more iterations yield higher quality
  //
  void simplify_mesh(int target_count, int update_rate = 5, double agressiveness = 7,
                     bool verbose = false, int max_iterations = 100, double alpha = 0.000000001,
                     int K = 3, bool lossless = false, double threshold_lossless = 0.0001,
                     bool preserve_border = false)
  {
    // init
    loopi(0, triangles.size()) triangles[i].deleted = 0;

    // main iteration loop
    int deleted_triangles = 0;
    std::vector<int> deleted0, deleted1;
    int triangle_count = triangles.size();
    // int iteration = 0;
    // loop(iteration,0,100)
    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
      if (triangle_count - deleted_triangles <= target_count)
        break;

      // update mesh once in a while
      if ((iteration % update_rate == 0) || lossless)
      {
        update_mesh(iteration);
      }

      // clear dirty flag
      loopi(0, triangles.size()) triangles[i].dirty = 0;

      //
      // All triangles with edges below the threshold will be removed
      //
      // The following numbers works well for most models.
      // If it does not, try to adjust the 3 parameters
      //
      double threshold = alpha * pow(double(iteration + K), agressiveness);
      if (lossless)
        threshold = threshold_lossless;

      // target number of triangles reached ? Then break
      if ((verbose) && (iteration % 5 == 0))
      {
        printf("iteration %d - triangles %d threshold %g\n", iteration, triangle_count - deleted_triangles, threshold);
      }

      // remove vertices & mark deleted triangles
      loopi(0, triangles.size())
      {
        Triangle &t = triangles[i];
        if (t.err[3] > threshold)
          continue;
        if (t.deleted)
          continue;
        if (t.dirty)
          continue;

        loopj(0, 3) if (t.err[j] < threshold)
        {
          int i0 = t.v[j];
          Vertex &v0 = vertices[i0];
          int i1 = t.v[(j + 1) % 3];
          Vertex &v1 = vertices[i1];
          // Border check //Added preserve_border method from issue 14
          if (preserve_border)
          {
            if (v0.border || v1.border)
              continue; // should keep border vertices
          }
          else if (v0.border != v1.border)
            continue; // base behaviour

          // Compute vertex to collapse to
          vec3f p;
          vec2f uv;
          calculate_error(i0, i1, p, uv);
          deleted0.resize(v0.tcount); // normals temporarily
          deleted1.resize(v1.tcount); // normals temporarily
          // don't remove if flipped
          if (flipped(p, i0, i1, v0, v1, deleted0))
            continue;
          if (flipped(p, i1, i0, v1, v0, deleted1))
            continue;

          // not flipped, so remove edge
          v0.p = p;
          v0.uv = uv;
          v0.q = v1.q + v0.q;
          int tstart = refs.size();

          update_triangles(i0, v0, deleted0, deleted_triangles);
          update_triangles(i0, v1, deleted1, deleted_triangles);

          int tcount = refs.size() - tstart;

          if (tcount <= v0.tcount)
          {
            // save ram
            if (tcount)
              memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
          }
          else
            // append
            v0.tstart = tstart;

          v0.tcount = tcount;
          break;
        }
        // done?
        if (lossless && (deleted_triangles <= 0))
        {
          break;
        }
        else if (!lossless && (triangle_count - deleted_triangles <= target_count))
        {
          break;
        }

        if (lossless)
          deleted_triangles = 0;
      }
    }
    // clean up mesh
    compact_mesh();
  } // simplify_mesh()

  // Check if a triangle flips when this edge is removed
  bool flipped(vec3f p, int i0, int i1, Vertex &v0, Vertex &v1, std::vector<int> &deleted)
  {
    loopk(0, v0.tcount)
    {
      Triangle &t = triangles[refs[v0.tstart + k].tid];
      if (t.deleted)
        continue;

      int s = refs[v0.tstart + k].tvertex;
      int id1 = t.v[(s + 1) % 3];
      int id2 = t.v[(s + 2) % 3];

      if (id1 == i1 || id2 == i1) // delete ?
      {
        deleted[k] = 1;
        continue;
      }
      vec3f d1 = vertices[id1].p - p;
      d1.normalize();
      vec3f d2 = vertices[id2].p - p;
      d2.normalize();
      if (fabs(d1.dot(d2)) > 0.999)
        return true;
      vec3f n;
      n.cross(d1, d2);
      n.normalize();
      deleted[k] = 0;
      if (n.dot(t.n) < 0.2)
        return true;
    }
    return false;
  }

  // Update triangle connections and edge error after a edge is collapsed
  void update_triangles(int i0, Vertex &v, std::vector<int> &deleted, int &deleted_triangles)
  {
    vec3f p;
    vec2f uv;
    loopk(0, v.tcount)
    {
      Ref &r = refs[v.tstart + k];
      Triangle &t = triangles[r.tid];
      if (t.deleted)
        continue;
      if (deleted[k])
      {
        t.deleted = 1;
        deleted_triangles++;
        continue;
      }
      t.v[r.tvertex] = i0;
      t.dirty = 1;
      t.err[0] = calculate_error(t.v[0], t.v[1], p, uv);
      t.err[1] = calculate_error(t.v[1], t.v[2], p, uv);
      t.err[2] = calculate_error(t.v[2], t.v[0], p, uv);
      t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
      refs.push_back(r);
    }
  }

  // compact triangles, compute edge error and build reference list
  void update_mesh(int iteration)
  {
    if (iteration > 0) // compact triangles
    {
      int dst = 0;
      loopi(0, triangles.size()) if (!triangles[i].deleted)
      {
        triangles[dst++] = triangles[i];
      }
      triangles.resize(dst);
    }

    // Init Reference ID list
    loopi(0, vertices.size())
    {
      vertices[i].tstart = 0;
      vertices[i].tcount = 0;
    }
    loopi(0, triangles.size())
    {
      Triangle &t = triangles[i];
      loopj(0, 3) vertices[t.v[j]].tcount++;
    }
    int tstart = 0;
    loopi(0, vertices.size())
    {
      Vertex &v = vertices[i];
      v.tstart = tstart;
      tstart += v.tcount;
      v.tcount = 0;
    }

    // Write References
    refs.resize(triangles.size() * 3);
    loopi(0, triangles.size())
    {
      Triangle &t = triangles[i];
      loopj(0, 3)
      {
        Vertex &v = vertices[t.v[j]];
        refs[v.tstart + v.tcount].tid = i;
        refs[v.tstart + v.tcount].tvertex = j;
        v.tcount++;
      }
    }

    // Identify boundary : vertices[].border=0,1
    if (iteration == 0)
    {
      std::vector<int> vcount, vids;

      loopi(0, vertices.size())
          vertices[i]
              .border = 0;

      loopi(0, vertices.size())
      {
        Vertex &v = vertices[i];
        vcount.clear();
        vids.clear();
        loopj(0, v.tcount)
        {
          int k = refs[v.tstart + j].tid;
          Triangle &t = triangles[k];
          loopk(0, 3)
          {
            int ofs = 0, id = t.v[k];
            while (ofs < vcount.size())
            {
              if (vids[ofs] == id)
                break;
              ofs++;
            }
            if (ofs == vcount.size())
            {
              vcount.push_back(1);
              vids.push_back(id);
            }
            else
              vcount[ofs]++;
          }
        }
        loopj(0, vcount.size()) if (vcount[j] == 1)
            vertices[vids[j]]
                .border = 1;
      }
    }

    //
    // Init Quadrics by Plane & Edge Errors
    //
    // required at the beginning ( iteration == 0 )
    // recomputing during the simplification is not required,
    // but mostly improves the result for closed meshes
    //
    if (iteration == 0)
    {
      loopi(0, vertices.size())
          vertices[i]
              .q = SymetricMatrix(0.0);

      loopi(0, triangles.size())
      {
        Triangle &t = triangles[i];
        vec3f n, p[3];
        loopj(0, 3) p[j] = vertices[t.v[j]].p;
        n.cross(p[1] - p[0], p[2] - p[0]);
        n.normalize();
        t.n = n;
        loopj(0, 3) vertices[t.v[j]].q =
            vertices[t.v[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
      }
      loopi(0, triangles.size())
      {
        // Calc Edge Error
        Triangle &t = triangles[i];
        vec3f p;
        vec2f uv;
        loopj(0, 3) t.err[j] = calculate_error(t.v[j], t.v[(j + 1) % 3], p, uv);
        t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
      }
    }
  }

  // Finally compact mesh before exiting
  void compact_mesh()
  {
    int dst = 0;
    loopi(0, vertices.size())
    {
      vertices[i].tcount = 0;
    }
    loopi(0, triangles.size()) if (!triangles[i].deleted)
    {
      Triangle &t = triangles[i];
      triangles[dst++] = t;
      loopj(0, 3) vertices[t.v[j]].tcount = 1;
    }
    triangles.resize(dst);
    dst = 0;
    loopi(0, vertices.size()) if (vertices[i].tcount)
    {
      vertices[i].tstart = dst;
      vertices[dst].p = vertices[i].p;
      vertices[dst].uv = vertices[i].uv;
      dst++;
    }
    loopi(0, triangles.size())
    {
      Triangle &t = triangles[i];
      loopj(0, 3) t.v[j] = vertices[t.v[j]].tstart;
    }
    vertices.resize(dst);
  }

  // Error between vertex and Quadric
  double vertex_error(SymetricMatrix q, double x, double y, double z)
  {
    return q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y + 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
  }

  // Error for one edge
  double calculate_error(int id_v1, int id_v2, vec3f &p_result, vec2f &uv_result)
  {
    // compute interpolated vertex
    SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;
    bool border = vertices[id_v1].border & vertices[id_v2].border;
    double error = 0;
    double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);
    if (det != 0 && !border)
    {
      // q_delta is invertible
      p_result.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8)); // vx = A41/det(q_delta)
      p_result.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8));  // vy = A42/det(q_delta)
      p_result.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8)); // vz = A43/det(q_delta)
      error = vertex_error(q, p_result.x, p_result.y, p_result.z);

      // compute interpolated uv
      uv_result = (vertices[id_v1].uv + vertices[id_v2].uv) / 2;
    }
    else
    {
      // det = 0 -> try to find best result
      vec3f p1 = vertices[id_v1].p;
      vec3f p2 = vertices[id_v2].p;
      vec3f p3 = (p1 + p2) / 2;
      vec2f uv1 = vertices[id_v1].uv;
      vec2f uv2 = vertices[id_v2].uv;
      vec2f uv3 = (uv1 + uv2) / 2;
      double error1 = vertex_error(q, p1.x, p1.y, p1.z);
      double error2 = vertex_error(q, p2.x, p2.y, p2.z);
      double error3 = vertex_error(q, p3.x, p3.y, p3.z);
      error = min(error1, min(error2, error3));
      if (error1 == error) {
        p_result = p1;
        uv_result = uv1;
      } else if (error2 == error) {
        p_result = p2;
        uv_result = uv2;
      } else if (error3 == error) {
        p_result = p3;
        uv_result = uv3;
      }
    }

    return error;
  }

  void setMeshFromExt(
      std::vector<std::vector<double>> verts,
      std::vector<std::vector<int>> faces,
      std::vector<std::vector<double>> uvs)
  {
    vertices.clear();
    triangles.clear();

    int N_faces = faces.size();
    int N_vertices = verts.size();
    loopi(0, N_vertices)
    {
      Vertex v;
      v.p.x = verts[i][0];
      v.p.y = verts[i][1];
      v.p.z = verts[i][2];
      v.uv.x = uvs[i][0];
      v.uv.y = uvs[i][1];
      vertices.push_back(v);
    }
    loopi(0, N_faces)
    {
      Triangle t;
      t.v[0] = faces[i][0];
      t.v[1] = faces[i][1];
      t.v[2] = faces[i][2];
      t.attr = 0;
      t.material = -1;
      triangles.push_back(t);
    }
  }

  std::vector<std::vector<double>> getVertices()
  {
    std::vector<std::vector<double>> verts;
    int N_vertices = vertices.size();
    loopi(0, N_vertices)
    {
      std::vector<double> v;
      v.push_back(vertices[i].p.x);
      v.push_back(vertices[i].p.y);
      v.push_back(vertices[i].p.z);
      verts.push_back(v);
    }
    return verts;
  }

  std::vector<std::vector<double>> getUvs()
  {
    std::vector<std::vector<double>> uvs;
    int N_vertices = vertices.size();
    loopi(0, N_vertices)
    {
      std::vector<double> uv;
      uv.push_back(vertices[i].uv.x);
      uv.push_back(vertices[i].uv.y);
      uvs.push_back(uv);
    }
    return uvs;
  }

  std::vector<std::vector<int>> getFaces()
  {
    std::vector<std::vector<int>> faces;
    int N_faces = triangles.size();
    loopi(0, N_faces)
    {
      std::vector<int> t;
      loopj(0, 3)
      {
        t.push_back(triangles[i].v[j]);
      }
      faces.push_back(t);
    }
    return faces;
  }

  std::vector<std::vector<double>> getNormals()
  {
    std::vector<std::vector<double>> normals;
    int N_faces = triangles.size();
    loopi(0, N_faces)
    {
      std::vector<double> n;
      n.push_back(triangles[i].n.x);
      n.push_back(triangles[i].n.y);
      n.push_back(triangles[i].n.z);
      normals.push_back(n);
    }
    return normals;
  }
};
///////////////////////////////////////////