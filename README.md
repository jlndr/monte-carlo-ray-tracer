# monte-carlo-ray-tracer


1. Camera send rays through each pixel, one at a time
2. For each ray, calculate intersection. The scene calls each triangle in the scene to calculate if that triangle get hit by the ray
3. The scene returns a reference to closest triangle hit by the ray
