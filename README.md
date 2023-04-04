# K-Imager

### Project Objective:
The objective of this project is to create an application that generates limited color palette images from full color photos to assist art teachers and students in practicing drawing and painting.

### Benefits:
The application simplifies the process of selecting colors and helps students understand the importance of color choice in art. It encourages creativity by limiting the color palette and provides a fun and interactive way for students to practice their skills.

### User Interface:
The application's interface is intuitive and user-friendly. Users first select an image from their saved files, then select the number of colors they want to reduce the palette to, whether to make the image greyscale, and which processing mode to apply. The application currently supports .bmp, .png, and .jpeg image formats. After these steps, users can hit the "RUN" button and save the resulting limited color palette image.

### Processing Modes:
The processing modes are all based on the k-means algorithm, which analyzes the image and selects the most representative colors. Algorithms specified as "unweighted" do not include duplicate pixel colors in their calculation, while algorithms specified as "++" use a smarter algorithm for selecting colors, reducing the randomness of the result. If users want more randomness, they should not use "++." If an image has a lot of same-colored background, users can try an unweighted algorithm. The result will likely be slightly different every time the "RUN" button is pressed, as the k-means algorithm relies on randomly selecting starting points.

### User Feedback:
User feedback is essential to improving the application. Users are encouraged to report any bugs they encounter or provide feedback on the application's features and usability.

### Conclusion:
This project provides a valuable tool for art teachers and students to improve their skills and explore the creative possibilities of a limited color palette. Users are encouraged to try out the application and provide feedback to help improve and develop new features.
