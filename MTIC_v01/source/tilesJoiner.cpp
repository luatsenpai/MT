#include <cstdio>
#include <cstdlib>
#include <cstring>

    char filename[100];
    int i=0;
    unsigned char tile[1000][256];
    int tsize[1000];
    int allsize=0;
    int farsize[1000];
    long lSize ;
    int pos=0;
    unsigned char *buffer = NULL;
    size_t result;
    char zeros[256];
    char palFilename[16];

    FILE *p = NULL;
    FILE *pout = NULL;

int main(int argc, char *argv[]){

    if(argc == 2){
        p    = fopen(argv[1],"rb");

        sprintf(filename,"output/tiles_%s.bin",argv[1]);
        pout = fopen(filename,"wb");
    }
    else return -1;

    //if( p==NULL || pout == NULL){ return 0;}

        for(i=1; i<=1000; i++){
            char dest[100] = "tiles/";
            strcat(dest,argv[1]);
            sprintf(filename,"/tile%03d.bin",i-1);
            strcat(dest,filename);
            p = fopen(dest,"rb");
            if (p==NULL) break;
                    // obtain file size:
            fseek (p , 0 , SEEK_END);
            lSize = ftell (p);
            rewind (p);

            // allocate memory to contain the whole file:
            buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
            if (buffer == NULL) {fputs ("Memory error",stderr); free(buffer); fclose(p); break;}

            // copy the file into the buffer:
            result = fread (buffer,1,lSize,p);
            if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
            fclose(p);

            for(int j=0; j<lSize; j++){
                tile[i][j] = buffer[j];
            }
            int addZero = 8 - (lSize % 8);
            for(int j=0; j<addZero; j++){
                tile[i][lSize+j] = 0;
            }
            tsize[i] = lSize + addZero;
            allsize += tsize[i];
            farsize[i] = allsize;
            free(buffer);
            //fclose(pout);

        }
        i--;

        tsize[0]=0;
        farsize[0]=0;
        char header[10]={'T','I','L',0,0x10,0,0,0};
        int pc = i; //pointers counter
        int filesize = allsize + pc*4;
        fwrite(header,sizeof(char),8,pout);//write header
        fwrite(&filesize,sizeof(int),1,pout);
        fwrite(&pc,sizeof(int),1,pout);
        for(int i=0; i<pc; i++){
            int buf = pc*4 + farsize[i]+16;
            fwrite(&buf,sizeof(int),1,pout);
        }
        for(int i=1; i<=pc ; i++){
            fwrite(tile[i],sizeof(char),tsize[i],pout);
        }
        fclose(pout);

        free(buffer);
//make a pal with header
        sprintf(filename,"output/map_%s.bin",argv[1]);
        pout = fopen(filename,"wb");

        sprintf(filename,"backgrounds/%s.map",argv[1]);
        p =   fopen(filename,"rb");

        if (p==NULL) return -1;
        // obtain file size:
        fseek (p , 0 , SEEK_END);
        lSize = ftell (p);
        rewind (p);

            // allocate memory to contain the whole file:
        buffer = (unsigned char*) malloc (sizeof(char)*(lSize));
        if (buffer == NULL) {fputs ("Memory error",stderr); free(buffer); fclose(p); return -1;}

            // copy the file into the buffer:
        result = fread (buffer,1,lSize,p);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
        fclose(p);

        bool ChompFormat= false;

        if(strcmp("Chomp_NewForm_BG",argv[1])==0 || strcmp("Chomp_LevelUp_BG",argv[1])==0)
            ChompFormat = true;

        char palHeader[16]={'M','A','P',0,0x90,0,0,0,0,0x0C,0,0,0x20,0,0x18,0};

        sprintf(palFilename,"%s",argv[1]);
        fwrite(palHeader,sizeof(char),16,pout);
        fwrite(palFilename,sizeof(char),16,pout);
        fwrite(zeros,sizeof(char),112,pout);

        for(int i=0; i<lSize-512; i+=2){
            if(ChompFormat){
                fwrite(buffer+i,sizeof(char),1,pout);
                fwrite(zeros,sizeof(char),2,pout);
                fwrite(buffer+i+1,sizeof(char),1,pout);
            } else {
                fwrite(buffer+i,sizeof(char),2,pout);
                fwrite(zeros,sizeof(char),2,pout);
            }

        }
        free(buffer);
        fclose(pout);
    return 0;
}
