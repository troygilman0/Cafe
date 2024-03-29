#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util/Types.h"

namespace Geometry {

    static const ui32 MAX_MESH_NUM_VERTICES = 100;
    static const ui32 MAX_MESH_NUM_INDICES = 500;

    struct GeometryMeshData {
        Vector3f vertices[MAX_MESH_NUM_VERTICES];
        ui32 indices[MAX_MESH_NUM_INDICES];
        ui32 numVertices;
        ui32 numIndices;
    };

    struct GeometryMesh {
        ui32 vao;
        ui32 vbo;
        ui32 ibo;
        ui32 numIndices;
    };

    void loadMesh(const GeometryMeshData& meshData, GeometryMesh& result);
    void bindMesh(const GeometryMesh& mesh);
    void unbindMesh();

    // QUAD PRISM
    static const ui32 QUAD_PRISM_MESH_NUM_VERTICES = 8;
    static const ui32 QUAD_PRISM_MESH_NUM_INDICES = 36;
    static const ui32 QUAD_PRISM_MESH_NUM_FACES = 6;
    static const ui32 QUAD_PRISM_MESH_NUM_EDGES = 12;

    struct QuadPrism {
        Vector3f p0;
        Vector3f p1;
        Vector3f p2;
        Vector3f p3;
        Vector3f p4;
        Vector3f p5;
        Vector3f p6;
        Vector3f p7;
    };

    void initWireframeMeshData(QuadPrism& quadPrism, f32 lineThickness, GeometryMeshData& result);
    void initQuadPrismMeshData(const Vector3f& min, const Vector3f& max, GeometryMeshData& result);
    void initQuadPrismMeshDataIndices(GeometryMeshData& result);
    void copyMeshDataWithOffsets(const Vector3f* srcVertices, Vector3f* destVertices, ui32 vertexOffset, const ui32* srcIndices, ui32* destIndices, ui32 indexOffset);

    // PLANE
    struct Plane {
        Vector3f normal;
        f32 distance;
    };

    void normalizePlane(Plane& plane);
    f32 distanceToPlane(const Plane& plane, const Vector3f& point);
    void getPlaneIntersection(const Plane& plane1, const Plane& plane2, const Plane& plane3, Vector3f& result);

    // FRUSTUM
    struct Frustum {
        Plane topPlane;
        Plane bottomPlane;
        Plane rightPlane;
        Plane leftPlane;
        Plane farPlane;
        Plane nearPlane;
        GeometryMesh mesh;
    };

    void createFrustum(const Matrix4f& view, const Matrix4f& projection, Frustum& result);
    void loadFrustumMeshData(const Frustum& frustum, GeometryMeshData& result);
    bool isPointInFrustum(const Frustum& frustum, const Vector3f& point);
    bool isSphereInFrustum(const Frustum& frustum, const Vector3f& center, f32 radius);


    // AABB
    struct AABB {
        Vector3f min;
        Vector3f max;
        GeometryMesh mesh;
    };

    void loadAABBMeshData(const AABB& aabb, GeometryMeshData& result);

}

#endif // GEOMETRY_H