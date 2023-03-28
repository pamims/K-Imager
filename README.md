# CSC487_Application

This project is my Senior Project at Wilmington University. I
designed this project to be useful for art teachers and students.

This application processes images into a limited color palette to help
students practice drawing and painting.  I got this idea from an art
project I remember making in highschool where we were specifically
supposed to paint a limited-palette picture from a photo.

The controls should be intuitive.  You first select an image from your
saved files.  Currently, this program supports .gif, .png, and .jpeg
image formats.  Then you can select how many colors you would like to
reduce the palette to, whether the image should be made grayscale, and
which processing mode you would like to apply.

After these steps, you can hit the big RUN button and SAVE the result.

The processing modes are all similar and are based on the k-means
algorithm.  Algorithms specified as "unweighted" do not include duplicate
pixel colors in their calculation. Algorithms specified as '++' use a
smarter algorithm for selecting colors, reducing the randomness of the
result.  If you want more randomness, do not use ++.  If you have an
image with a lot of same-colored background, try an unweighted algorithm.

Regardless of the mode you choose, the result will likely be at least
slightly different every time the run button is pressed because part of
the k-means algorithm relies on randomly selecting starting points.

If you have any comments or feedback or if you find any bugs, please
let me know!
