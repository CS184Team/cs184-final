<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_8.jpg" width="800px" />
</center>

## Due Date
Mon Feb 29th, 11:59pm

## Overview
In this project you will explore a subset of the geometric topics covered in lecture. You will tesselate Bezier patches, manipulate half-edge meshes, implement Loop subdivision, and write shaders for your own meshes! When you are finished, you will have a tool that allows you to load and edit basic COLLADA mesh files that are now used by many major modeling packages and real time graphics engines.  For the last part of the assignment, you'll be creating your own COLLADA file using free software like [Blender](http://blender.org)!

### Project Parts
* Part 1: Fun with Bezier Surfaces (20 points)
* Part 2: Average normals for half-edge meshes (10 points)
* Part 3: Edge flip (10 points)
* Part 4: Edge split (10 points)
* Part 5: Upsampling via Loop Subdivision (20 points)
* Part 6: Fun with Shaders (20 points)
* Part 7: Design your own mesh (10+ points)

As with the first assignment, all your deliverables should be documented in the *website/index.html* webpage. Your art competition submission should be saved in the root directory as *competition.png*.

### Quick links

[How to build and submit](http://cs184.eecs.berkeley.edu/cs184_sp16/article/4)

[Half-edge data structure details and implementation](http://cs184.eecs.berkeley.edu/cs184_sp16/article/7)

## Getting set up
You can either [download]() the zipped assignment straight to your computer or clone it from [GitHub]() using the command

```
$ git clone https://github.com/???
```

Please consult this article on [how to build and submit assignments for CS 184](http://cs184.eecs.berkeley.edu/cs184_sp16/article/4).


## Using the GUI

When you have successfully built your code, you will get an executable named `meshedit` in the build directory. The `meshedit` executable takes exactly one argument from the command line. You may load a single COLLADA file by specifying its path. For example, to load the example file *dae/quadball.dae* from your build directory:

```
./meshedit ../dae/quadball.dae
```

After Part 1, you will be able to load Bezier surfaces by running a command such as:

```
./meshedit ../bez/teapot.bez 
```

When you first run the application, you will see a picture of a mesh made of triangles. The starter code that you must modify is drawing this mesh.  The editor already supports some basic functionality, like moving vertices around in space, which you can do by just clicking and dragging on a vertex.  You can also rotate the camera by right-clicking and dragging (or dragging on the background), and zoom in and out using the scroll wheel or multi-touch scrolling on a trackpad. Hitting the spacebar will reset the view.  As you move the cursor around the screen, you'll notice that mesh elements (faces, edges, and vertices) under the cursor get highlighted. Clicking on one of these elements will display some information about the element and its associated data.

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_.jpg" width="600px" />
</center>

In this assignment, you will add additional functionality to the program that allows you to modify the mesh in a variety of ways. Each of these methods is exposed through the viewer. There will be two basic types of operations:

1. Local flip and split operations, which modify the mesh in a small neighborhood around the currently selected mesh element.
2. Loop subdivision, which refines and smooths the entire mesh.

Each operation will be executed with a key press (A table of all the keyboard controls in the `meshedit` application is provided in the appendix).
<center>

| Command                              | Key          |
| ------------------------------------ |:------------:|
| Flip the currently selected edge     | <kbd>F</kbd> |
| Split the currently selected edge    | <kbd>S</kbd> |
| Subdivide the mesh                   | <kbd>U</kbd> |

</center>
Notice that currently, nothing happens when keys are pressed - this is because you haven't yet implemented resampling! Unlike the previous assignment, no reference solution is provided. However, we will provide several examples of correct input/output pairs for each remeshing operation.

Note that each COLLADA file may contain multiple mesh objects; more generally, a COLLADA file describes a __scene graph__ (much like SVG) that is a hierarchical representation of all objects in the scene (meshes, cameras, lights, etc.), as well as their coordinate transformations. Global resampling methods will be run on whichever mesh is currently selected. 

## Getting Acquainted with the Starter Code
Before you start, here is some basic information on the structure of the starter code. Your code for all parts except shading will be contained inside *student_code.cpp*. (For shading, you'll have to edit the *shader/frag* file.) 

For Bezier patches, you'll be filling in member functions of the `BezierPatch` class, declared in *bezierPatch.\**. We have put dummy definitions for all the Bezier patch functions you need to modify inside *student_code.cpp*, where you'll be writing your code. You may additionally need to add member variables to the class definition in *bezierPatch.h*.

For half-edge meshes, you'll be filling in member functions of the `HalfedgeMesh` class, declared in *halfEdgeMesh.\**. We have put dummy definitions for all the half-edge functions you need to modify inside *student_code.cpp*, where you'll be writing your code.

For shaders, you'll be editing the *shader/frag* file.

Before you attempt parts 2-5 (the half-edge sections), you'll want to quickly consult [these lecture slides](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/geometry/slide_059) as a half-edge refresher and then read [this in-depth article](http://cs184.eecs.berkeley.edu/cs184_sp16/article/7) with more detail about the half-edge data structure and its implementation in the starter code.


## Part 1: Fun with Bezier Surfaces

We'll begin with a warmup problem to introduce you to working with triangle meshes before jumping into the half-edge data structure. The goal for this part is to tessellate a Bezier surface into triangles. The input Bezier surface is represented by 16 control points in 3D space. These points are stored as a 4x4 `Vector3D` array called `controlPoints`, which can be accessed from inside the `BezierPatch` class. You need to tessellate the given Bezier surface uniformly on a 8x8 grid in parameter space. You'll want to consult [this slide](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/curves-surfaces/slide_069) for the proper formula to evaluate eahc point, remembering the definition of [Bernstein polynomials](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/curves-surfaces/slide_052) as well.

To implement this, you may need to add some member variables to the `BezierPatch` class (by modifying *bezierPatch.h*). The `BezierPatch::preprocess` function allows you to do some precomputation. For example, you might initialize your own member variables here based on the 16 control points. The `BezierPatch::addTriangle` function (defined inside *bezierPatch.cpp*) helps you add triangles to the output mesh. If your implementation is correct, you will be able to see a teapot by passing `bez/teapot.bez` as the input argument.

These are the functions inside *student_code.cpp* that you will need to modify:

1. `Bezier::preprocess`
2. `Bezier::evaluate`
3. `Bezier::add2mesh`


### Deliverables
* Show a mesh rendering of `bez/teapot.bez` in your writeup. You can run `meshedit` with argument `*.bez` to load and tessellate Bezier surfaces.
* Extra Credit: Implement adaptive Bezier subdivision scheme and show a png image of an adaptively tessellated mesh in your writeup.

## Part 2: Average normals for half-edge meshes

Next, we move on to working with the full half-edge data structure. Again, read [this article](http://cs184.eecs.berkeley.edu/cs184_sp16/article/7) before you start working with the `HalfedgeMesh` class in this and subsequent parts of the assignment. 

In this part, you will implement the `Vertex::normal` function inside *student_code.cpp*. This function returns the area-weighted average normal vector at a vertex. Compute this by summing up the normals of all the triangles touching the vertex, with each normal being multiplied by the area of the triangle that generated it. There is a picture depicting this process in [this slide](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/pipeline/slide_033). Remember to re-normalize your average normal vector to have unit length before returning it! 

Tips:

* Make sure you understand the code given for a `printNeighborPositions` function in the [Half-edge data structure article](http://cs184.eecs.berkeley.edu/cs184_sp16/article/7) before you attempt this part .
* The norm of cross product of two vectors defining the sides of a triangle is equal to twice that triangle's area.

### Deliverables
* Show mesh renderings of `dae/teapot.dae` in your writeup, comparing the default OpenGL shading with and without smoothed normals (use 'Q' to switch between face normals and average vertex normals).


## Part 3: Edge Flip

Now you should be a little more comfortable traversing the half-edge pointers. In this task, you will implement a more substantial method: a local remeshing operation that "flips" an edge, implemented inside the method `HalfedgeMesh::flipEdge` in file *student_code.cpp*.  

More precisely, suppose we have a pair of triangles $(a,b,c)$ and $(c,b,d)$. After flipping the edge $(b,c)$, we should now have triangles $(a,d,c)$ and $(a,b,d)$:

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_1.jpg" width="800px" align="middle"/>
</center>

Your solution should:

 * Ignore requests to flip boundary edges (just return immediately if either neighboring face is a boundary loop).
 * Perform only a constant amount of work -- the cost of flipping a single edge should **not** be proportional to the size of the mesh!
 * Not add or delete any elements.  Since there are the same number of mesh elements before and after the flip, you should only need to reassign pointers.

The biggest challenge in properly implementing this operation (as well as split) is making sure that all the pointers still point to the right place in the modified mesh. An easy recipe for ensuring that all pointers are still valid after any general remeshing operation is:

 1. Draw a picture and/or write down a list of all the elements (vertices, edges, faces, halfedges) that will be needed from the original mesh.
 2. Draw a picture and/or write down a list of all the elements that should appear in the modified mesh.
 3. Allocate any new elements that are needed in the modified mesh, but do not appear in the original mesh (only relevant for the next part).
 4. For every element in the "modified" picture, set **all** of its pointers -- even if they didn't change. For instance, for each halfedge, make sure to set `next`, `twin`, `vertex`, `edge`, and `face` to the correct values in the new (modified) picture. For each vertex, edge, and face, make sure to set its `halfedge` pointer. A convenience method `Halfedge::setNeighbors()` has been created for the purpose of setting all pointers inside a halfedge at once.

The reason for setting all the pointers (and not just the ones that changed) is that it is very easy to miss a pointer, causing your code to fail. Once the code is **working**, you can remove these unnecessary assignments if you wish.

### Deliverables

* Show a screenshot of a mesh before and after some edge flips. Write about your eventful debugging journey, if you experienced one.


## Part 4: Edge Split

This time, you will make a different local modification to the mesh in the neighborhood of an edge, called a __split__. In particular, suppose we have a pair of triangles $(a,b,c)$ and $(c,b,d)$. The edge $(b,c)$ is split by inserting a new vertex m at its midpoint and connecting it to the opposite vertices $a$ and $d$, yielding four triangles:

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_2.jpg" width="800px" align="middle"/>
</center>

This task is a bit tricker than "flip" because there are more pointers to keep track of, and you will have to allocate new mesh elements this time (e.g., two new triangles, three edges, some halfedges...).  Your implementation should:

 * Ignore requests to split boundary edges (just return immediately if either neighboring face is a boundary loop).
 * Assign the position of the new vertex to the midpoint of the original edge, i.e., the average of its two endpoints (see `Vertex::position`).
 * Perform only a constant amount of work -- the cost of splitting a single edge should **not** be proportional to the size of the mesh!
 * Allocate only as many new elements as needed; there should be no "orphaned" elements that are not connected to the rest of the mesh.

To obtain a correct implementation, you might try following the same "recipe" given in the previous task (though clever, clean, and simple alternatives are of course always welcome). To verify that your implementation works correctly, try flipping some edges that you've split, and splitting some edges that you flipped.

### Deliverables

* Show a screenshot of a mesh before and after some edge splits (and maybe also some flips). Write about your epic debugging quest, if you went on one.
* _Extra credit:_ support edge split for edges on the boundary. For this, you will need to carefully read the section about the "virtual boundary face" in the halfedge article. In this case, you will split the edge in half but only split the face that is non-boundary into two. Give screenshot examples if you do this.


## Part 5: Upsampling via Loop Subdivision

Now, we can leverage the previous two parts to make implementing the mesh topology changes in [Loop subdivision](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/geometry/slide_067) very simple! In this task, you will implement the whole Loop subdivision process inside the `MeshResampler::upsample` in *student_code.cpp*.

Loop subdivision is somewhat analogous to upsampling using some interpolation method in image processing: we may have a low-resolution polygon mesh that we wish to upsample for display, simulation, etc.  Simply splitting each polygon into smaller pieces doesn't help, because it does nothing to alleviate blocky silhouettes or chunky features. Instead, we need an upsampling scheme that nicely interpolates or approximates the original data. Polygon meshes are quite a bit trickier than images, however, since our sample points are generally at _irregular_ locations, i.e., they are no longer found at regular intervals on a grid.

Loop subdivision consists of two basic steps:

1. Change the mesh topology: split each triangle into four by connecting edge midpoints (sometimes called "4-1 subdivision").
2. Update vertex positions as a weighted average of neighboring positions.

4-1 subdivision does this to each triangle:

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_3.jpg" width="500px" align="middle"/>
</center>

And the following picture depicts the correct weighting for the new averaged vertex positions:

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_9.jpg" width="500px" align="middle"/>
</center>

Written out, the new position of an old vertex is 

    (1 - n*u) * original_position + u * neighbor_position_sum
    
where `n` is the number of neighboring vertices, `u` is a constant as depicted in the figure above, `original_position` is the vertex's original position, and `neighbor_position_sum` is the sum of all neighboring vertices' positions.
    
The position for a newly created vertex v that splits an edge AB connecting vertices A and B and is flanked by opposite vertices C and D across the two faces connected to AB in the original mesh will be 

    3/8 * (A + B) + 1/8 * (C + D)

If we repeatedly apply these two steps, we will converge to a smoothed approximation of our original mesh.  In this task you will implement Loop subdivision, leveraging the split and flip operations to handle the topology changes.  In particular, you can achieve a 4-1 subdivision by applying the following strategy:

1. Split every edge of the mesh in any order whatsoever.
2. Flip any new edge that touches a new vertex and an old vertex. *Note*: Every original edge will now be represented by 2 edges, you *should not* flip these edges, because they are always already along the boundary of the 4 way divided triangles. In the diagrams below, you should only flip the blue edges that connect an old and new vertex, but you should not flip any of the black new edges.

The following pictures (courtesy Denis Zorin) illustrate this idea:

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_10.jpg" width="800px" />
</center>

#####Implementation Walkthrough#####

For Loop subdivision, we have also provided some additional data members that will make it easy to keep track of the data you need to update the connectivity and vertex positions. In particular:

   * `Vertex::newPosition` can be used as temporary storage for the new position (computed via the weighted average above).  Note that you should _not_ change the value of `Vertex::position` until _all_ the new vertex positions have been computed -- otherwise, you are taking averages of values that have already been averaged!
   * Likewise, `Edge::newPosition` can be used to store the position of the vertices that will ultimately be inserted at edge midpoints.  Again, these values should be computed from the original values (before subdivision), and applied to the new vertices only at the very end. The `Edge::newPosition`value will be used for the position of the vertex that will appear along the old edge after the edge is split. We precompute the position of the new vertex before splitting the edges and allocating the new vertices because it is easier to traverse the simpler original mesh to find the positions for the weighted average that determines the positions of the new vertices.
   * `Vertex::isNew` can be used to flag whether a vertex was part of the original mesh, or is a vertex newly inserted by subdivision (at an edge midpoint).
   * `Edge::isNew` likewise flags whether an edge is a piece of an edge in the original mesh, or is an entirely new edge created during the subdivision step.

Given this setup, we strongly suggest that it will be easiest to implement subdivision according to the following "recipe" (though you are of course welcome to try doing things a different way!). The basic strategy is to _first_ compute the new vertex positions (storing the results in the `newPosition` members of both vertices and edges), and only _then_ update the connectivity. Doing it this way will be much easier, since traversal of the original (coarse) connectivity is much simpler than traversing the new (fine) connectivity. In more detail:

0. Mark all vertices as belonging to the original mesh by setting `Vertex::isNew` to `false` for all vertices in the mesh.
1. Compute updated positions for all vertices in the original mesh using the vertex subdivision rule, and store them in `Vertex::newPosition`.
2. Compute new positions associated with the vertices that will be inserted at edge midpoints, and store them in `Edge::newPosition`.
3. Split every edge in the mesh, being careful about how the loop is written.  In particular, you should make sure to iterate only over edges of the original mesh.  Otherwise, you will keep splitting edges that you just created!
4. Flip any new edge that connects an old and new vertex.
5. Finally, copy the new vertex positions (`Vertex::newPosition`) into the usual vertex positions (`Vertex::position`).

If you made the requested modification to the return value of `HalfedgeMesh::splitEdge()` (see above), then an edge split will now return an iterator to the newly inserted vertex, and the halfedge of this vertex will point along the edge of the original mesh. This iterator is useful because it can be used to (i) flag the vertex returned by the split operation as a new vertex, and (ii) flag each outgoing edge as either being new or part of the original mesh.  (In other words, Step 3 is a great time to set the members `isNew` for vertices and edges created by the split. It is also a good time to copy the `newPosition` field from the edge being split into the `newPosition` field of the newly inserted vertex.)

You might try implementing this algorithm in stages, e.g., _first_ see if you can correctly update the connectivity, _then_ worry about getting the vertex positions right. Some examples below illustrate the correct behavior of the algorithm.

<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_6.jpg" width="800px" align="middle"/>
</center>

__Possible Extra Credit Extensions:__

* _Support surfaces with boundary._ To do this, you will first need to make sure your edge split operation appropriately handles boundary edges. You will also need to use a different weighted average for boundary vertices; see [Boier-Martin et al, "A Survey of Subdivision-Based Tools for Surface Modeling"](http://mrl.nyu.edu/~dzorin/papers/boiermartin2005sbt.pdf) for more information.
* _Implement additional subdivision schemes._ There are many alternatives to Loop subdivision.  Triangle subdivision schemes include Butterfly and [modified Butterfly](http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf) (which are interpolating rather than approximating) and [Sqrt(3)](https://www.graphics.rwth-aachen.de/media/papers/sqrt31.pdf) (which refines the mesh at a "slower" rate); the most popular subdivision scheme for quadrilateral meshes is Catmull-Clark. There are also special subdivision schemes for handling meshes with high degree vertices, e.g., extraordinary vertices, called [polar subdivision](http://www.cise.ufl.edu/research/SurfLab/papers/09bi3c2polar.pdf).

### Deliverables:

* Write some notes and take some screenshots to record your observations of how meshes behave after Loop subdivion. What happens to sharp corners and edges? Can you lessen this effect by pre-splitting some edges? Test this on *cube.dae*. Also, note that the *cube.dae* becomes slightly asymmetric after repeated subdivision steps. Play around with this using flip and split. Can you pre-process the cube with flip and split so it subdivides symmetrically? Document both of these effects on the cube and explain why they occur.
* Write up a report (with pictures) about your extra credit extensions if you attempt them.


## Part 6: Fun with Shaders

For this part, you will implement Phong shading and environment map reflection shading. Take a look at [these slides](http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/pipeline/slide_020) to review the basic concepts and variables used in shading equations. 

The necessary variables for you to compute shader values are already defined in the shader file. In the *meshedit* program, you can press 0-9 to switch between different shading effects. Initially only shader #0 is implemented as reference. Shader #1 and #2 are the two shading effects you will need to implement. Shaders #3-#9 are for extra credit. You can try your own shading there -- be creative!

Tips on environment mapping:

First compute the reflection direction in $(x, y, z)$ coordinates. Then, figure out $(\theta,\ \phi)$ in spherical coordinates as shown in the figure below.

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_6.jpg"/>

Once you have $\theta$ and $\phi$, simply use $(\ \theta/(2\pi),\ \ \phi/\pi\ )$ as texture coordinates to access environment texture.

Reference Images are given below:

####Phong Shading

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_1.jpg"/>

####Environment Map Reflection Shading

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_2.jpg"/>

Here is a list of functions you will need to modify inside *shader/frag*:

1. `shadePhong`
2. `shadeEnvmapReflection`

If you want write any additional shaders to try for extra credit, you will append those functions to the same file.

### Deliverables

* Show renderings of shader #1 and #2 in your writeup.
* *Extra Credit:* Create your own shading using shaders #3-9. Some interesting shading effects include bump mapping, 3D texture mapping, ...

Some examples are here:

####Bump Mapping

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_3.jpg"/>

####3D Texture Mapping

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_4.jpg"/>

<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/5_5.jpg"/>


## Part 7: Design your own mesh!

For this part, we'd like you to design your own COLLADA *.dae* file format mesh using the free program [Blender](http://blender.org). Our suggestion for a baseline starting point is to design a humanoid mesh with a head, two arms, and two legs. We've created a [Blender video tutorial](http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/blender.mp4) to guide you through the basics of making a simple humanoid mesh.

Once you make your mesh, you should load it into *meshedit* and use what you've implemented! Subdivide it to smooth and use your custom shaders from part 6.

Here are some examples of Weilun's mesh-man before and after subdivision and also with the environment map applied:
<center>
<img src="http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/6_11.jpg" width="800px" />
</center>

### Deliverables

* Make sure to include your *my_mesh.dae* mesh file in the *dae/* folder!
* Include many figures in your writeup of your original mesh as well as your mesh after one and two rounds of subdivision. Also include figures of your mesh with custom shaders.
* *Open Ended Extra Credit:* Make a super cool or super detailed mesh! Write fancy shaders and apply them to your mesh. Implement additional geometric operations and demonstrate them on your mesh. The sky is the limit! Describe anything you do beyond the ordinary in detail in your writeup.


## Submission Instructions

As with the first assignment, log in to an instructional machine, navigate to the root directory of your project, and run 

    zip -r hw2.zip .
    submit hw2

This indicates you have succeeded:

    Looking for files to turn in....
    Submitting hw2.zip.
    The files you have submitted are:
        ./hw2.zip 
    Is this correct? [yes/no] yes
    Copying submission of assignment hw2....
    Submission complete.

There are more detailed instructions (including how you can use *scp* to copy your files onto the s349 machines) [here](http://cs184.eecs.berkeley.edu/cs184_sp16/article/4).


## Appendix

### Keyboard Controls

<center>

| Command                              | Key                                                |
| ------------------------------------ |:--------------------------------------------------:|
| Flip the selected edge               | <kbd>F</kbd>                                       |
| Split the selected edge              | <kbd>S</kdb>                                          |
| Upsample the current mesh            | <kbd>U</kdb>                                         |
| Toggle information overlay           | <kbd>I</kdb>                                       |
| Select the next halfedge             | <kbd>N</kbd>                                       |
| Select the twin halfedge             | <kbd>T</kbd>                                       |
| Switch to GLSL shaders               | <kbd>W</kbd>                                       |
| Switch between GLSL shaders          | <kbd>0-9</kbd>                                       |
| Toggle using area-averaged normals   | <kbd>Q</kbd>                                       |
| Recompile shaders                    | <kbd>R</kbd>                                       |
| Reset camera to default position     | <kbd>SPACE</kdb>                                   |
| Edit a vertex position               | (click and drag on vertex)                         |
| Rotate camera                        | (click and drag on background, or right click)     |

</center>
