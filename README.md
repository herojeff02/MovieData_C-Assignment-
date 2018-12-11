<h5>This is a movie management system, written in C.</h5><br>
The program uses four structs(Movie, User, Favourite, Tag) to manage the data.
One of the data files has roughly 95000 lines, and I couldn't use fseek to search from the file immediately, even though it would have been a better solution.<br>
.c and .h files have functions related to the files' name. dataType.h holds the global variables such as movies(Movie struct array). <br>
<br>
Our objective was
1. To fully adopt the given data as a dataset of the previous system.
2. Not to change the given data structure, even while adding a new function.
3. 