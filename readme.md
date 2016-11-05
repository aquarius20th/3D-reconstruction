The project is a 3D reconstruction of multi-view [SPIM][http://www.andor.com/learning-academy/spim-selective-plane-illumination-microscopy] slices

1. First of all, use a blind deconvolution method to deblur the 2D image, which enhance the resolution of the image. 
2. Secondly, the iterative closest point algorithm is used to realize the registration of 3D data point set at different angles. 
3. Then a data fusion method is used to achieve the fusion of 3D image information from all views. 
4. Finally, with the help of VTK toolbox, use a scientific visualization method called volume rendering to realize the visualization of 3D data, ultimately implement the goal of 3D reconstruction.

**some results:**

before deconvolution:

![](http://i.imgur.com/rq8VgJT.jpg)

after deconvolution:

![](http://i.imgur.com/KesxpWq.jpg)

before fusion:

![](http://i.imgur.com/b7ksPvd.jpg)

after fusion:

![](http://i.imgur.com/HJrWUi3.jpg)

after 3D reconstruction:

![](http://i.imgur.com/xWXPZrP.jpg)

![](http://i.imgur.com/vcXBlYv.jpg)

![](http://i.imgur.com/irLOgWR.jpg)

![](http://i.imgur.com/DnCAyDs.jpg)

