void addMovie() {
    Movie *movieEntity;
    movieEntity=(Movie *)malloc(sizeof(Movie));

    printf("Movie ID to add: ");
    scanf("%d",&movieEntity->movieID);
    printf("Title: ");
    scanf(" %s",movieEntity->title);
    printf("Release year: ");
    scanf("%d",&movieEntity->releaseYear);
    printf("How many genre do you want to add: ");
    scanf("%hd",&movieEntity->sizeof_genre);

    movieEntity->genre=(int *)malloc(sizeof(int)*movieEntity->sizeof_genre);

    int i=1;

    while(1)
    {
        movieEntity->title=(char *)malloc(sizeof(char));
        char input[]={getchar()};
        i++;

        if(input[0]=='\n')
            break;
    }

    for(int i=0;i<movieEntity->sizeof_genre;i++)
    {
        char *inputGenre;
        inputGenre=(char *)malloc(sizeof(char));
        printf("Genre %d: ",i);
        scanf(" %s",inputGenre);
        int genreIndex_ByString(char *inputGenre);
    }

    addMovieEntity(movieEntity->movieID, movieEntity->title, movieEntity->releaseYear, movieEntity->genre, movieEntity->sizeof_genre);

    if (addMovieEntity(movieEntity->movieID, movieEntity->title, movieEntity->releaseYear, movieEntity->genre, movieEntity->sizeof_genre)==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else if (addMovieEntity(movieEntity->movieID, movieEntity->title, movieEntity->releaseYear, movieEntity->genre, movieEntity->sizeof_genre)==FAIL_MOVIE_ID_ALREADY_EXISTS)
    {
        printf("Already exist movie");
        return;
    }
    else if (addMovieEntity(movieEntity->movieID, movieEntity->title, movieEntity->releaseYear, movieEntity->genre, movieEntity->sizeof_genre)==FAIL_INVALID_YEAR)
    {
        printf("Invalid year");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}
void removeMovie() {

}
void addTag() {
    Tag *tagEntity;
    tagEntity=(Tag *)malloc(sizeof(Tag));

    printf("User ID to add: ");
    scanf("%d",&tagEntity->userID);
    printf("Movie ID to add: ");
    scanf("%d",&tagEntity->movieID);
    printf("Tag: ");
    scanf(" %s",tagEntity->tag);
    printf("Timestamp: ");
    scanf("%lld",&tagEntity->timestamp);

    addTagEntity(tagEntity->userID, tagEntity->movieID, tagEntity->tag, tagEntity->timestamp);

    if (addTagEntity(tagEntity->userID, tagEntity->movieID, tagEntity->tag, tagEntity->timestamp)==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}
void removeTag() {

}
void addFavourite() {
    Favourite *favouriteEntity;
    favouriteEntity=(Favourite *)malloc(sizeof(Favourite));

    printf("User ID to add: ");
    scanf("%d",&favouriteEntity->userID);
    printf("Movie ID to add: ");
    scanf("%d",&favouriteEntity->movieID);

    addFavouriteEntity(favouriteEntity->userID, favouriteEntity->movieID);

    if (addFavouriteEntity(favouriteEntity->userID, favouriteEntity->movieID)==SUCCESS)
    {
        printf("ADD SUCCESSFULLY!!");
        return;
    }
    else
    {
        printf("Have some problem. Add again");
        return;
    }
}
void searchByUserID() {
    int searchUserID;
    printf("User ID to search: ");
    scanf("%d",&searchUserID);

}
void searchByMovieTitle() {
    char *searchMovieTitle;
    printf("Movie title to search: ");
    scanf("%d",&searchMovieTitle);

}
