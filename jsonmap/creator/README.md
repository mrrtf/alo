
# A note on contour making

We've investigated a bit the [Boost Geometry library](http://www.boost.org/doc/libs/1_65_1/libs/geometry/doc/html/index.html) features.

At first, the union function seemed promising, but in fact it is creating too many points 
(i.e. it does not simplify the segments enough).

For instance, the union of those two rectangles (x denoting vertices)

    x-----x
    |     |
    x-----x
    |     |
    x-----x
    
gives : 

    x-----x
    |     |
    x     x
    |     |
    x-----x

which is the correct contour, but with too many vertices, as we'd expected : 

    x-----x
    |     |
    |     |
    x-----x
    
The number of vertices being relevant when we realize the example above (of 2 pads) is in reality 
extended to 64 rectangles, for _each_ motif.

So we got back to the idea of porting the contour making algorithm from AliRoot ...
 