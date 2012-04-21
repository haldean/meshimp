meshimp: Quadric Error Mesh Simplification
=======
Will Brown
---

Run `./meshimp [obj file]` to run the viewer.

In the viewer, use WASD to rotate around two axes, and Q and E to move in and
out.

Use the 1-4 keys on your keyboard to switch between:
1. No shared normals -- each face has a constant normal equal to the cross
   product of two of its edges.
2. Adjacent-face average -- each vertex has a normal equal to the average of the
   normals of the adjacent faces.
3. Area-weighted adjacent-face average -- same as 2., but face normals are
   weighted by the area of their associated face.
4. Angle-weighted adjacent-face average -- same as 2., but face normals are
   weighted by the angle between the two edges that meet at the vertex.

Hit ',' to simplify the mesh and reduce the number of edges by 50%. Hit '.' to
reduce by 25%.

Hit 'g' to toggle drawing edges, 'f' to toggle drawing faces and 'n' to toggle
drawing normals.
