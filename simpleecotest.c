#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>



struct Location{
    char* name;
    int mapsize;
    int : 5, climatezone;
    int : 3, seasonality;
    struct Being* pred;
    struct Being* sub;
    struct Being* herb;
    struct Being* autoph;
    float treecover;

};

struct Being{
    char* name;
    float curpop;
    float ravageK;
    float bodyfoodK;
    float birthdeathRAT;

};

void updatePop(struct Location*);
float updatePopPred(struct Being*);
float updatePopSub(struct Being*);
float updatePopHerb(struct Being*);
float updatePopAutoph(struct Being*);
struct Location* newLocation(char* filename);
void freeLocation(struct Location*);
int* readbeastparams(FILE*, int cursor);


int mainecotest (int argc, char** argv[]){

    int cycles = 100, i=0;
    int numbeings = 4;

    struct Being lion = {"lion", 10.0, 0, 0, 0.5};
    struct Being hyena = {"hyena", 18.4, 0, 0, 0.8};
    struct Being squirrel = {"squirrel", 34.3, 0, 0, 1.2};
    struct Being nuts = {"nuts", 85.0, 0, 0, 1.2153};

    struct Location steppe = {"Uneven Steppes", 2000, 1,\
     0, &lion, &hyena, &squirrel, &nuts, 0.25};

    struct Location fileloc;


    FILE* simpleeco;
    FILE* beastparams;

    int choice=0;
    int* populationhistory = (int*)malloc(sizeof(int)*numbeings*cycles);
    int pophist[4][100];

    char* params[53];
    char* string;
    char mychar;
    int k=0, m=0;
    beastparams = fopen("beastparams.txt", "r+");
 /*   for (k=0;k<4;k++){
        while(fgetchar()!='\n'){
            m++;
        }
        fseek(beastparams, (long)-m, SEEK_CUR);
        fgets(params[k], m, beastparams);
        fseek(beastparams, (long)(m+1), SEEK_CUR);
    }
   */
    printf("Heres the file: %c\n\n", fgetc(beastparams));
     printf("Heres the file: %c\n\n", fgetc(beastparams));
      printf("Heres the file: %c\n\n", fgetc(beastparams));
      fseek(beastparams, (long)3, SEEK_CUR);
       printf("Heres the file: %c\n\n", fgetc(beastparams));
       fseek(beastparams, (long)-10, SEEK_END);
       printf("string: %8s", fgets(string, 8, beastparams));
    fclose(beastparams);
    simpleeco=fopen("simplerECOtest.txt", "a");



    printf("LOCATION!!!! %s\n", steppe.name);
    printf("%s\t%s\t%s\t%s\n", steppe.pred->name, steppe.sub->name, steppe.herb->name, steppe.autoph->name);
    printf("%d\t%d\t%d\t\t%d\n", (int)steppe.pred->curpop, (int)steppe.sub->curpop, (int)steppe.herb->curpop, (int)steppe.autoph->curpop);

    for (i=0; i<cycles;i++){
            updatePop(&steppe);
            pophist[0][i]=steppe.pred->curpop;
            pophist[1][i]=steppe.sub->curpop;
            pophist[2][i]=steppe.herb->curpop;
            pophist[3][i]=steppe.autoph->curpop;
            *(populationhistory + (numbeings*i) + 0)=steppe.pred->curpop;
            *(populationhistory + (numbeings*i) + 1)=steppe.sub->curpop;
            *(populationhistory + (numbeings*i) + 2)=steppe.herb->curpop;
            *(populationhistory + (numbeings*i) + 3)=steppe.autoph->curpop;
            printf("%d\t%d\t%d\t\t%d\n", (int)steppe.pred->curpop, (int)steppe.sub->curpop, (int)steppe.herb->curpop, (int)steppe.autoph->curpop);



    }
    do{
        printf("\n");
        printf("WOW! GREAT RESULTS GENIUS!! PRint tO fILE??\n");
        printf("1. Yes\n");
        printf("2. No and quit\n");
        scanf("%d", &choice);

    }while (choice!=1 && choice!=2);
    if (choice==1){
        fprintf(simpleeco, "LOCATION!!!! %s\n", steppe.name);
        fprintf(simpleeco, "%s\t%s\t%s\t%s\n", steppe.pred->name, steppe.sub->name, steppe.herb->name, steppe.autoph->name);



        for (i=0; i<cycles;i++){
                fprintf(simpleeco, "ARRAY: %d\t%d\t%d\t\t%d\n", pophist[0][i], pophist[1][i], pophist[2][i], pophist[3][i]);
                fprintf(simpleeco, "HEPMM: %d\t%d\t%d\t\t%d\n", *(populationhistory + (numbeings*i) + 0), *(populationhistory + (numbeings*i) + 1), *(populationhistory + (numbeings*i) + 2), *(populationhistory + (numbeings*i) + 3));




        }
    }
    fclose(simpleeco);
    free(populationhistory);
    return 0;
}

void updatePop(struct Location* area){
    ///update bottom-up food chain: (season, trees, weather, cloud/sun) autotroph, herbs, subs, preds
    //AUTOTROPH
    area->autoph->curpop = (float)(area->autoph->curpop)*(area->autoph->birthdeathRAT)/(1.0 + area->treecover);

    //HERBIVORE

    area->herb->curpop = (float)(area->herb->curpop)*(area->herb->birthdeathRAT)*(((area->autoph->curpop) / (area->herb->curpop)) * (((area->herb->curpop) / (area->sub->curpop))));

    //SUBPREDATOR

    area->sub->curpop = (float)(area->sub->curpop)*(area->sub->birthdeathRAT)*(((area->herb->curpop) / (area->sub->curpop)) * (((area->sub->curpop) / (area->pred->curpop)))) ;

    //APEXPREDATOR

    area->pred->curpop = (float)(area->pred->curpop) * (area->pred->birthdeathRAT) * (((area->sub->curpop) / (area->pred->curpop)));

    return;
}

float updatePopPred(struct Being* pred){
    return;
}
float updatePopSub(struct Being* sub){
    return;
}
float updatePopHerb(struct Being* herb){
    return;
}
float updatePopAutoph(struct Being* autoph){
    return;
}

struct Location* newLocation(char* filename){
    struct Location* newLocation = (struct Location*)malloc(sizeof(struct Location));
    FILE* params;
    params = fopen(filename, "r");
    newLocation->name="tundra";
/*
    newLocation->pred->curpop=
    fscanf(params, "%d", &(newLocation->pred->curpop));
    int beast1params[4];
    int i=0;
    while(fgetchar()!=","){
        i++;
    }
    char*params[4];
    fgets(params[0], i, params);
    int j=0;
    beast1params[0];
*/
};
void freeLocation(struct Location* newLocation){
    free(newLocation);
};

